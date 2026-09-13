#include "application/scroll/ScrollDynamics.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  ScrollDynamics m;
  int pos = 7;
  for (int i = 0; i < 100; ++i)
    pos = m.advance(1.0 / 120, 10, pos, 100);
  CHECK(pos == 0);
  for (int i = 0; i < 100; ++i)
    pos = m.advance(1, 10, pos, 100);
  CHECK(m.advance(-1, 10, pos, 100) == 10);
  m.reset();
  pos = 0;
  for (int i = 0; i < 12; ++i)
    pos = m.advance(-1.0 / 120, 10, pos, 100);
  CHECK(pos == 1);
  CHECK(m.advance(-10000, 10, pos, 100) == 100);
  CHECK(m.advance(1, 10, 100, 100) == 90);
  ScrollProfile plain{1, false, .5, 3}, fast{1, true, 1, 3};
  ScrollDynamics a, b;
  int p = 500, q = 500;
  for (unsigned t = 100; t < 180; t += 8) {
    p = a.advance({-1, t}, plain, 1, p, 10000);
    q = b.advance({-1, t}, fast, 1, q, 10000);
  }
  CHECK(q > p && q <= 530);
  CHECK(b.advance({1, 181}, fast, 1, q, 10000) == q - 1);  // Reversal resets gain.
  CHECK(b.advance({-1, 500}, fast, 1, q, 10000) == q + 1); // Idle resets gain.
  CHECK(b.advance({-1, 501, ScrollAxis::Vertical, ScrollOrigin::Sync}, fast, 1, q, 10000) == q);
  a.reset();
  b.reset();
  p = q = 0;
  for (unsigned t = 100; t < 180; t += 8) {
    p = a.advance({-.1, t}, plain, 10, p, 10000);
    q = b.advance({-.1, t}, ScrollProfile{2, false, .5, 3}, 10, q, 10000);
  }
  CHECK(q == 2 * p);
}
TEST_MAIN(run)
