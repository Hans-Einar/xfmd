#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <chrono>
#include <thread>
#include <iostream>
int main() {
  Display* d = XOpenDisplay(nullptr);
  if (!d) return 1;
  Window w = XCreateSimpleWindow(d, DefaultRootWindow(d), 100, 100, 500, 400, 0, 0, 0);
  XStoreName(d, w, "XFMD isolated fullscreen probe");
  XMapWindow(d, w); XFlush(d);
  auto state = XInternAtom(d, "_NET_WM_STATE", False);
  auto fullscreen = XInternAtom(d, "_NET_WM_STATE_FULLSCREEN", False);
  auto has = [&] {
    Atom type; int format; unsigned long count, rest; unsigned char* data = nullptr;
    bool result = false;
    if (XGetWindowProperty(d,w,state,0,64,False,XA_ATOM,&type,&format,&count,&rest,&data)==Success && data) {
      auto* atoms = reinterpret_cast<Atom*>(data);
      for (unsigned long i=0;i<count;++i) result |= atoms[i]==fullscreen;
    }
    if (data) XFree(data);
    return result;
  };
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  for (int enabled : {1,0}) {
    XEvent e{}; e.xclient.type=ClientMessage; e.xclient.window=w;
    e.xclient.message_type=state; e.xclient.format=32;
    e.xclient.data.l[0]=enabled; e.xclient.data.l[1]=fullscreen; e.xclient.data.l[3]=1;
    XSendEvent(d,DefaultRootWindow(d),False,SubstructureRedirectMask|SubstructureNotifyMask,&e);
    XFlush(d);
    for(int i=0;i<100 && has()!=bool(enabled);++i)
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    if(has()!=bool(enabled)) { std::cerr<<"Fullscreen state not acknowledged\n"; return 2; }
  }
  XDestroyWindow(d,w); XCloseDisplay(d);
  std::cout<<"Window Maker acknowledged fullscreen enter and exit\n";
}
