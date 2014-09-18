import sys
sys.path.insert(0, '../hiaux/hibuilder/')

from hibuilder import HiBuilder

env = Environment()

b = HiBuilder("punktd", "clang++")

hicore_path = "../"
hiaux_path = "../"
hithird_path = "../"

b.addPath(hicore_path)
b.addPath(hiaux_path)

b.connectLib(hicore_path+"geber/cpp-client/")
b.addPath(hicore_path+"geber/")
b.connectLib(hicore_path+"geber/")

b.addPath(hicore_path+"zeit/cpp-client/")
b.connectLib(hicore_path+"zeit/cpp-client/")

b.connectLib("Targeter/")
b.addPath("Targeter/")

b.addPath("Formatters/")
b.addCppGlob("Formatters/*.cpp")

b.connectLib("VisitorsStorage/")
b.addPath("VisitorsStorage/")

b.connectLib("TargeterCookieOnly/")
b.addPath("TargeterCookieOnly/")

b.addPath("../hashd/cpp-client/")

b.addPath("./")

b.addPath(hiaux_path+"hiaux/structs/")
b.connectLib(hiaux_path+"hiaux/loadconf/")
b.connectLib(hiaux_path+"hiaux/network/")
b.connectLib(hiaux_path+"hiaux/crypt/")

b.addPath(hithird_path+"thirdparty/http-parser/")
b.connectLib(hithird_path+"thirdparty/http-parser/")

b.addCCFlags(["-Wno-return-type"])
b.addCppGlob("*.cpp")
b.addCppGlob("*.pb.cc")


b.addLinkFlags(["-lprotobuf", "-ljansson", "-lpq", 
    "-L/usr/local/lib/"])

b.build(Program)
