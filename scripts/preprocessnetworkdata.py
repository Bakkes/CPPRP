import io
import code
import os
import sys
printJson = False
printConsume = False

def parseOption(line):
    substr = line[line.find('(') + 1:line.find(')')]
    return list(map(str.strip, substr.split(",")))

class CodeToGenerate:
    
    
    def __init__(self):
        self.struct_name = "error"
        self.consume = True
        self.to_string = True
        self.consumeIO = io.StringIO()
        self.tostringIO = io.StringIO()
        self.jsonIO = io.StringIO()

def parseStruct(lines):
    
    code = CodeToGenerate()
    code.jsonIO.write("writer.StartObject();\n")
    field_index = 0
    
    parseFields = False
    iterator = iter(lines)
    for line in iterator:
        if line.strip().startswith("//"):
            #print(line)
            continue
        if line.startswith("__ParserAttribute__") and not parseFields:
            options = parseOption(line)
            if options[0].lower() == "consume":
                code.consume = options[1].lower() == "true" or options[1] == "1"
            if options[0].lower() == "tostring":
                code.to_string = options[1].lower() == "true" or options[1] == "1"
        elif line.startswith("struct"):
            code.struct_name = line.split(' ')[1]
        elif line.startswith("{"):
            parseFields = True
        elif line.startswith("}"): #we're done
            break
        elif parseFields:
            line = line.strip()
            if len(line) < 3:
                continue
            engineversion = -1
            licenseeversion = -1
            maxbits = 0
            numbits = 0
            callconsume = False
            calltostring = False
            while line.startswith("__ParserAttribute__"):
                options = parseOption(line)
                if options[0].lower() == "maxbits":
                    maxbits = int(options[1])
                if options[0].lower() == "numbits":
                    numbits = int(options[1])
                if options[0].lower() == "engineversion":
                    engineversion = int(options[1])
                if options[0].lower() == "licenseeversion":
                    licenseeversion = int(options[1])
                if options[0].lower() == "callconsume":
                    callconsume = options[1].lower() == "true" or options[1] == "1"
                if options[0].lower() == "calltostring":
                    calltostring = options[1].lower() == "true" or options[1] == "1"
                    #print(calltostring)
                #print("{0} = {1}".format(options[0], options[1]))
                
                line = next(iterator).strip()
                #print("next")
                while len(line) < 2:
                    line = next(iterator).strip()
                    #print("next2")
                    #print(line)
                
            data = [x.replace(';', '') for x in line.strip().split(" ")];
            #print(data)
            if len(data) < 2:
                continue
            
            setVersionReq = True
            if engineversion >= 0 and licenseeversion >= 0:
                code.consumeIO.write("if(reader.engineVersion >= {0} \n".format(engineversion))
                code.consumeIO.write("\t&& reader.licenseeVersion >= {0}) {{ \n".format(licenseeversion))
            elif engineversion >= 0:
                code.consumeIO.write("if(reader.engineVersion >= {0}) {{ \n".format(engineversion))
            elif licenseeversion >= 0:
                code.consumeIO.write("if(reader.licenseeVersion >= {0}) {{ \n".format(licenseeversion))
            else:
                setVersionReq = False
                
            if setVersionReq:
                code.consumeIO.write("\t")
            if callconsume:
                if data[0].lower().startswith("std::vector"):
                    vector_type = data[0][data[0].find('<') + 1:data[0].rfind('>')]
                    code.consumeIO.write("item.{0} = ConsumeVector<{1}>(reader);\n".format(data[1], vector_type))
                else:
                    code.consumeIO.write("item.{0} = Consume<{1}>(reader);\n".format(data[1], data[0]))
            elif maxbits > 0:
                code.consumeIO.write("item.{0} = reader.readBitsMax<{1}>({2});\n".format(data[1], data[0], maxbits))
            else:
                code.consumeIO.write("item.{0} = reader.read<{1}>({2});\n".format(data[1], data[0], "" if numbits == 0 else numbits))
                
            if data[0].lower().startswith("std::vector"):
                    vector_type = data[0][data[0].find('<') + 1:data[0].find('>')]
            #if calltostring:
               # print("AAAAAAAAAAA")
            code.tostringIO.write("ss {1}<< \"{0} = \" << ToString(item.{0});\n".format(data[1], "" if field_index == 0 else "<< \",\\n \" "))
           # else:
                #print("BBBBBBBB")
                #code.tostringIO.write("ss {1}<< \"{0} = \" << item.{0};\n".format(data[1], "" if field_index == 0 else "<< \",\\n \" "))
            
            if setVersionReq:
                code.consumeIO.write("} else {\n")
                code.consumeIO.write("\titem.{0} = 0;\n".format(data[1]))
                code.consumeIO.write("}\n")
            code.jsonIO.write("writer.Key(\"{0}\");\n".format(data[1]))
            code.jsonIO.write("Serialize(writer, item.{1});\n".format(data[0], data[1]))
            
            
            field_index += 1
            
    code.jsonIO.write("writer.EndObject();\n")
    return code


def generateClasses(code):
    #f = open(sys.argv[2], "a")
    
    consumeFunc = io.StringIO()
    consumeFunc.write("template<>\n")
    consumeFunc.write("inline const {0} Consume(CPPBitReader<BitReaderType>& reader) \n".format(code.struct_name))
    consumeFunc.write("{\n")
    
    consumeFunc.write("\t{0} item;\n".format(code.struct_name))
    [consumeFunc.write("\t{0}\n".format(x)) for x in code.consumeIO.getvalue().splitlines()]
    consumeFunc.write("\treturn item;\n")
    consumeFunc.write("}\n\n")
    
    
    tostringFunc = io.StringIO()
    tostringFunc.write("template<>\n")
    tostringFunc.write("inline const std::string ToString(const {0}& item) \n".format(code.struct_name))
    tostringFunc.write("{\n")
    tostringFunc.write("\tstd::stringstream ss;\n".format(code.struct_name))
    [tostringFunc.write("\t{0}\n".format(x)) for x in code.tostringIO.getvalue().splitlines()]
    tostringFunc.write("\treturn ss.str();\n")
    tostringFunc.write("}\n\n")
    
    jsonFunc = io.StringIO()
    jsonFunc.write("template<typename Writer>\n")
    jsonFunc.write("inline const void Serialize(Writer& writer, const {0}& item)\n".format(code.struct_name))
    jsonFunc.write("{\n")
    [jsonFunc.write("\t{0}\n".format(x)) for x in code.jsonIO.getvalue().splitlines()]
    jsonFunc.write("}\n\n")
    global printJson, printConsume
    if printJson:
        print(jsonFunc.getvalue())
    if printConsume and code.consume:
        print(consumeFunc.getvalue())
    #if code.consume:
    #    print(consumeFunc.getvalue())
    #if code.consume:
    #    f.write(consumeFunc.getvalue())
   # if code.to_string:
    #    f.write(tostringFunc.getvalue())
    #f.write(jsonFunc.getvalue())
    
#if os.path.exists(sys.argv[2]):
#    os.remove(sys.argv[2])
#f = open(sys.argv[2], "a")
#f.write('#pragma once\n#include "CPPBitReader.h"\n#include "NetworkData.h"\n#include <sstream>\nnamespace CPPRP {\n')
    
if __name__== "__main__":
    #print("??")
    f = open(sys.argv[1], "r")
    s = f.readlines()

    printJson = sys.argv[2] == "json"
    printConsume = sys.argv[2] == "consume"
    totalStr = ""
    active = False
    for line in s:
        line = line.strip()
        if line.startswith("//") or line.startswith("virtual ~"):
            continue
        if line.startswith("struct") or line.startswith("__ParserAttribute__"):
            active = True
        if active:
            totalStr += line + "\n"
        if "}" in line:
            code = parseStruct(totalStr.splitlines())
            if(code.struct_name.strip().lower() != "error"):
                #print(code.struct_name.strip().lower())
                generateClasses(code)
            totalStr = ""
            active = False

#f = open(sys.argv[2], "a")
#f.write('\n};')