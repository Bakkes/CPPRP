import io
import os
import sys

def parseOption(line):
    substr = line[line.find('(') + 1:line.find(')')]
    return list(map(str.strip, substr.split(",")))

class Struct:
    name = "ERR"
    parent = None
    namespace = ""
    fields = []

    def __init__(self):
        self.name = "ERR"
        self.parent = None
        self.namespace = ""
        self.fields = []

def parseStruct(text):
    #line 0 must be struct header
    struct = Struct()
    structText = text[0].strip()
    spl = structText.split(' ')
    struct.name = spl[1]
    if ":" in structText:
        struct.parent = spl[4]
    for line in text[1:]:
        line = line.strip()
        linesplit = line.split(' ')
        if line.startswith("FIELD") or len(line) < 2 or len(linesplit) < 2 or line.startswith("//") or line.startswith("virtual ~"):
            continue
        typename = line[0:line.rfind(" ")]
        fieldname = line[line.rfind(" ") + 1:-1]
        struct.fields.append((typename, fieldname))
    #print(len(text))
    #print(len(struct.fields))
    return struct

def generateParserCode(struct):
    code = io.StringIO()
    code.write("RegisterClass<{0}::{1}>(\"{0}.{1}\");\n".format(struct.namespace, struct.name))

    for field in struct.fields:
        code.write("RegisterField(\"{0}.{1}:{2}\", [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) {{ std::static_pointer_cast<CPPRP::{0}::{1}>(struc)->{2} = Consume<{3}>(br); }});\n".format(struct.namespace, struct.name, field[1], field[0]))#0namespace, 1 classname, 2 propname, 3 typename
    return (code.getvalue())

if __name__== "__main__":
    f = open("C:/Users/Bakkes/Documents/repos/CPPRP/CPPRP/GameClasses.h", "r")
    s = f.readlines()
    totalStr = ""
    active = False
    classActive = False
    namespace = ""
    code = io.StringIO()
    serializer = io.StringIO()
    serializerSingle = io.StringIO()

    iterator = s.__iter__()
    currentNamespace = ""
    structs = []
    for line in iterator:
        line = line.strip()
        
        if line.startswith("namespace"):
            currentNamespace = line.split(' ')[1]
        if line.startswith("struct"):
            fullStructText = []
            while "}" not in line:
                fullStructText.append(line)
                line = next(iterator).strip()
            
            struct = parseStruct(fullStructText)
            struct.namespace = currentNamespace
            if struct.parent is not None and "::" not in struct.parent:
                struct.parent = struct.namespace + "::" + struct.parent
            structs.append(struct)
            
    code = io.StringIO()
    code.writelines(["template<typename T>\n", "constexpr inline static T Initializor()\n", "{\n"])
    for struct in structs:
        if struct.name is not "Object":
            code.write(generateParserCode(struct))
    code.writelines(["return 0;\n", "}\n"])
    print(code.getvalue())
    sys.exit(0)
    for line in s:
        line = line.strip()
        if line.startswith("//"):
            continue
        if line.startswith("namespace"):
            active = True
            
            namespace = line[line.rfind(' ') + 1:]
        if line.startswith("struct"):
            #options = parseOption(line)
            if classActive:
                #serializer.write("\twriter.EndObject();\n")
                serializer.write("}\n\n")
            
            spl = line.split(" ")
            className = spl[1]
            parentClassName = ""
            if line.find(":"):
                parentClassName = spl[4]

            classActive = True

            code.write("RegisterClass<{1}::{0}>(\"{1}.{0}\");\n".format(className, namespace))
            serializer.write("template<typename Writer>\n")
            serializer.write("inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::{1}::{0}>& item)\n".format(className, namespace))
            serializer.write("{\n")
            #serializer.write("\tauto item = std::static_pointer_cast<CPPRP::{1}::{0}>(struc);\n".format(options[0], namespace))
            #serializer.write("\twriter.StartObject();\n")
            #serializer.write("\twriter.String(\"actor_id\");\n")
            #serializer.write("\twriter.Uint({0});\n".format(0))
            
            #tempName = options[1]  if "::" not in options[1] else options[1][options[1].rfind(':') + 1:]
            namesp = namespace + "::" if "::" not in options[1] else ""
            serializer.write("\tSerialize<CPPRP::{0}{1}>(writer, item);\n".format(namesp, parentClassName))
            
            #print(options)
        if line.startswith("FIELD"):
            options = parseOption(line)
            #std::static_pointer_cast<Engine::Actor>(struc)->
            #<{4}::{0}, {2}>
            code.write("RegisterField({3}, [](std::shared_ptr<Engine::Actor>& struc, CPPBitReader<BitReaderType>& br) {{ std::static_pointer_cast<CPPRP::{4}::{0}>(struc)->{1} = Consume<{2}>(br); }});\n".format(options[0], options[1], options[2], options[3], namespace))
            serializer.write("\twriter.String(\"{0}\");\n".format(options[1]))
            serializer.write("\tSerialize<{0}>(writer, item->{1});\n".format(options[2].replace("struct ", "CPPRP::"), options[1]))
            serializerSingle.write("RegisterJSONSerializer({0}, ".format(options[3]))
            serializerSingle.write("[](auto& writer, std::shared_ptr<CPPRP::Engine::Actor>& struc) {\n")
            serializerSingle.write("\t\twriter.String(\"{0}\");\n".format(options[1]))
            serializerSingle.write("\t\tSerialize<{2}>(std::static_pointer_cast<CPPRP::{4}::{0}>(struc)->{1});\n".format(options[0].replace("struct ", "CPPRP::"), options[1], options[2].replace("struct ", "CPPRP::"), options[3], namespace))
            serializerSingle.write("\t});\n")
            #serializer.write("\twriter.EndObject();\n")
            #print(options)

    #serializer.write("\twriter.EndObject();\n")
    serializer.write("}\n")

    #print(code.getvalue())
    print(serializer.getvalue())
    #print(serializerSingle.getvalue())