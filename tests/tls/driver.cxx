#include <libmingw-stdthread/thread.hxx>

#include <vector>

#undef NDEBUG
#include <cassert>

// Note that both thread_local and __thread seem to be functioning in both
// posix and win32 GCC threads configurations.
//
thread_local bool b;
//__thread bool b;

int main ()
{
  using thread = mingw_stdthread::thread;

  std::vector<thread> ts;
  for (int i = 0; i != 256; ++i)
    ts.push_back (thread([]()
                         {
                           assert (!b);
                           b = true;
                         }));

  for (thread& t: ts)
    t.join();
}
