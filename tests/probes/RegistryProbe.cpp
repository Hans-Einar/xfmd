#include <fx.h>
#include <filesystem>
#include <fstream>
#include <iostream>
int main() {
  FX::FXRegistry r("xfmd","xfmd");
  r.writeStringEntry("WorkPaths","path0","/tmp");
  r.writeStringEntry("Future","unknown","preserve");
  r.writeRealEntry("Scroll","speed",1.5);
  if(!r.write()) return 1;
  FX::FXRegistry loaded("xfmd","xfmd");
  if(!loaded.read() || loaded.readRealEntry("Scroll","speed",0)!=1.5 ||
     FX::FXString(loaded.readStringEntry("WorkPaths","path0",""))!="/tmp" ||
     FX::FXString(loaded.readStringEntry("Future","unknown",""))!="preserve") return 2;
  auto path=std::filesystem::path(FX::FXSystem::getHomeDirectory().text())/".foxrc"/"xfmd"/"xfmd";
  std::filesystem::remove(path);
  std::filesystem::create_directory(path);
  std::ofstream(path/"blocker")<<"keep";
  loaded.writeRealEntry("Scroll","speed",2);
  if(loaded.write())return 3;
  std::cout<<"Registry roundtrip, unknown/WorkPaths preservation and rename failure passed\n";
}
