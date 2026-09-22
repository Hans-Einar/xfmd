#include "WindowEndpoint.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <stdexcept>
namespace xfmd {
namespace {
bool identity(const std::string& s) { return !s.empty() && s.size() <= 64 && s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") == std::string::npos; }
sockaddr_un address(const std::string& path) {
  sockaddr_un a{}; a.sun_family = AF_UNIX;
  if (path.size() >= sizeof(a.sun_path)) throw std::runtime_error("IPC path too long");
  std::memcpy(a.sun_path, path.c_str(), path.size()+1); return a;
}
void privateDirectory(const std::string& path) {
  if (::mkdir(path.c_str(), 0700) && errno != EEXIST) throw std::runtime_error("Cannot create private runtime directory");
  struct stat s{};
  if (lstat(path.c_str(), &s) || !S_ISDIR(s.st_mode) || s.st_uid != getuid() || (s.st_mode & 077))
    throw std::runtime_error("Runtime directory must be owned by this user and mode 0700");
}
}
std::string WindowEndpoint::runtimeDirectory() {
  const char* xdg = std::getenv("XDG_RUNTIME_DIR");
  std::string base = xdg ? xdg : "/tmp/xfmd-runtime-" + std::to_string(getuid());
  privateDirectory(base); auto dir = base + "/xfmd"; privateDirectory(dir); return dir;
}
WindowEndpoint::WindowEndpoint(const std::string& id) : window(id) {
  if (!identity(id)) throw std::runtime_error("Invalid window ID");
  socketPath = runtimeDirectory() + "/" + id + ".sock";
  auto a = address(socketPath);
  listener = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (listener < 0) throw std::runtime_error("Cannot create window endpoint");
  if (bind(listener, reinterpret_cast<sockaddr*>(&a), sizeof(a)) || listen(listener, 16)) {
    close(listener); listener=-1; throw std::runtime_error("Window ID is already registered or unavailable");
  }
}
WindowEndpoint::~WindowEndpoint() {
  for (auto c : clients) close(c.fd);
  if (listener >= 0) { close(listener); unlink(socketPath.c_str()); }
}
std::string WindowEndpoint::handle(const std::string& message) {
  if (message.empty() || message.size()>32768 || message.back()!='\n' || message.find('\0')!=std::string::npos) return "ERROR\tinvalid packet\n";
  auto line=message.substr(0,message.size()-1);
  if (line.find('\n')!=std::string::npos || line.find('\r')!=std::string::npos) return "ERROR\tinvalid packet\n";
  std::vector<std::string> f; std::istringstream in(line); std::string field;
  while (std::getline(in,field,'\t')) f.push_back(field);
  if (f.size()==3 && f[0]=="XFMD1" && f[1]=="INFO" && f[2]==window) return "OK\t"+(info?info():"")+"\n";
  if (f.size()!=8 || f[0]!="XFMD1" || f[1]!="OPEN" || f[2]!=window || !identity(f[3]) || (f[5]!="main" && f[5]!="navigation") || f[6].empty() || f[6][0]!='/' || f[7]!="end") return "ERROR\tinvalid target or request\n";
  unsigned long long seq=0;
  try { if(f[4].empty() || f[4].find_first_not_of("0123456789")!=std::string::npos) throw std::runtime_error("sequence"); seq=std::stoull(f[4]); } catch(...) { return "ERROR\tinvalid sequence\n"; }
  auto key=f[3]+":"+f[5];
  if (sequences.size()>=256 && !sequences.count(key)) return "ERROR\tclient limit\n";
  if (seq<=sequences[key]) return "ERROR\tstale request\n";
  sequences[key]=seq;
  if (!open || !open(f[5],f[6])) return "ERROR\tdocument not opened\n";
  return "OK\t"+f[4]+"\n";
}
void WindowEndpoint::poll() {
  for (int i=0;i<16;i++) {
    int fd=accept4(listener,nullptr,nullptr,SOCK_NONBLOCK|SOCK_CLOEXEC); if(fd<0) break;
    ucred cred{}; socklen_t length=sizeof(cred);
    if (clients.size()>=16 || getsockopt(fd,SOL_SOCKET,SO_PEERCRED,&cred,&length) || cred.uid!=getuid()) { close(fd); continue; }
    clients.push_back({fd,std::chrono::steady_clock::now()+std::chrono::seconds(2)});
  }
  for (auto it=clients.begin();it!=clients.end();) {
    char buffer[32769]; auto n=recv(it->fd,buffer,sizeof(buffer),MSG_DONTWAIT|MSG_TRUNC);
    if(n<0 && (errno==EAGAIN || errno==EWOULDBLOCK) && std::chrono::steady_clock::now()<it->deadline) { ++it;continue; }
    std::string response="ERROR\tpacket size or timeout\n";
    if(n>0 && n<=32768) { try { response=handle(std::string(buffer,n)); } catch(const std::exception& e) { response="ERROR\tdocument failure\n"; } }
    send(it->fd,response.data(),response.size(),MSG_NOSIGNAL);close(it->fd);it=clients.erase(it);
  }
}
std::string WindowEndpoint::request(const std::string& id,const std::string& message) {
  if(!identity(id) || message.size()>32768) throw std::runtime_error("Invalid window request");
  auto a=address(runtimeDirectory()+"/"+id+".sock");
  int fd=socket(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0);if(fd<0)throw std::runtime_error("IPC socket failed");
  struct Guard{int fd;~Guard(){close(fd);}} guard{fd};
  if(connect(fd,reinterpret_cast<sockaddr*>(&a),sizeof(a)))throw std::runtime_error("Target window is closed or unavailable");
  if(send(fd,message.data(),message.size(),MSG_NOSIGNAL)!=static_cast<ssize_t>(message.size()))throw std::runtime_error("IPC send failed");
  pollfd p{fd,POLLIN,0};if(::poll(&p,1,5000)<=0)throw std::runtime_error("Window acknowledgement timed out");
  char b[32769];auto n=recv(fd,b,sizeof(b),MSG_TRUNC);if(n<=0||n>32768)throw std::runtime_error("Invalid window response");return std::string(b,n);
}
}
