#include <mingw-thread.hxx>

#include <vector>

#undef NDEBUG
#include <cassert>

// We could try to fallback to __thread.
//
// @@ Maybe available in later versions?
//
#if 0
#ifndef __cpp_thread_local
# error no thread_local support
#endif

thread_local bool b;
#endif

__thread bool b;

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
