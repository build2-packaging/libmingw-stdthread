#include <libmingw-stdthread/thread.hxx>

#include <vector>

#include "throw.hxx"

#undef NDEBUG
#include <cassert>

int
main ()
{
  using thread = mingw_stdthread::thread;

  std::vector<thread> ts;
  for (int i (0); i != 256; ++i)
    ts.push_back (thread([]()
                         {
                           for (int j (0); j != 256; ++j)
                           {
                             try
                             {
                               t ('v');
                               assert (false);
                             }
                             catch (const e& x)
                             {
                               assert (x.data[0] == 'v');
                             }
                           }
                         }));

  for (thread& t: ts)
    t.join();
}
