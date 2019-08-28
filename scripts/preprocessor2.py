import io
import os
import sys

def parseOption(line):
    substr = line[line.find('(') + 1:line.find(')')]
    return list(map(str.strip, substr.split(",")))


if __name__== "__main__":
    f = open(sys.argv[1], "r")
    s = f.readlines()
    totalStr = ""
    active = False
    classActive = False
    namespace = ""
    code = io.StringIO()
    serializer = io.StringIO()
    serializerSingle = io.StringIO()

    for line in s:
        line = line.strip()
        if line.startswith("//"):
            continue
        if line.startswith("namespace"):
            active = True
            
            namespace = line[line.rfind(' ') + 1:]
        if line.startswith("CLASS"):
            options = parseOption(line)
            if classActive:
                #serializer.write("\twriter.EndObject();\n")
                serializer.write("}\n\n")
            
            classActive = True

            code.write("RegisterClass<{1}::{0}>(\"{1}.{0}\");\n".format(options[0], namespace))
            serializer.write("template<typename Writer>\n")
            serializer.write("inline const void Serialize(Writer& writer, const std::shared_ptr<CPPRP::{1}::{0}>& item)\n".format(options[0], namespace))
            serializer.write("{\n")
            #serializer.write("\tauto item = std::static_pointer_cast<CPPRP::{1}::{0}>(struc);\n".format(options[0], namespace))
            #serializer.write("\twriter.StartObject();\n")
            #serializer.write("\twriter.String(\"actor_id\");\n")
            #serializer.write("\twriter.Uint({0});\n".format(0))
            
            #tempName = options[1]  if "::" not in options[1] else options[1][options[1].rfind(':') + 1:]
            namesp = namespace + "::" if "::" not in options[1] else ""
            serializer.write("\tSerialize<CPPRP::{0}{1}>(writer, item);\n".format(namesp, options[1]))
            
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