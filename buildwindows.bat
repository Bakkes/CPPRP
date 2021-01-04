C:\Python37\python "./scripts/preprocessgameclass.py" "./CPPRP/data/GameClasses.h" json > "./CPPRP/generated/GameClassMacros.h"
C:\Python37\python "./scripts/preprocessgameclass.py" "./CPPRP/data/GameClasses.h" classext > "./CPPRP/generated/ClassExtensions.h"
C:\Python37\python "./scripts/preprocessnetworkdata.py" "./CPPRP/data/NetworkData.h" consume > "./CPPRP/generated/NetworkDataParsersGenerated.h"
C:\Python37\python "./scripts/preprocessnetworkdata.py" "./CPPRP/data/NetworkData.h" json > "./CPPRPJSON/GeneratedSerializeFunctions.h"
MSBuild.exe CPPRP.sln -p:Configuration=JSONBuild -p:Platform=x64