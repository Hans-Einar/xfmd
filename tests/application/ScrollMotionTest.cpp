#include "application/scroll/ScrollDynamics.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  ScrollDynamics m;
  int pos=7;
  for(int i=0;i<100;++i)pos=m.advance(1.0/120,10,pos,100);
  CHECK(pos==0);
  for(int i=0;i<100;++i)pos=m.advance(1,10,pos,100);
  CHECK(m.advance(-1,10,pos,100)==10);
  m.reset();pos=0;
  for(int i=0;i<12;++i)pos=m.advance(-1.0/120,10,pos,100);
  CHECK(pos==1);
  CHECK(m.advance(-10000,10,pos,100)==100);
  CHECK(m.advance(1,10,100,100)==90);
}
TEST_MAIN(run)
