#include <libmingw-stdthread/thread.hxx>

#include <string>
#include <vector>

#undef NDEBUG
#include <cassert>

struct sptr
{
  std::string* p_;

  explicit sptr (std::string* p): p_ (p) {}
  ~sptr () {delete p_;}
};

// Note that both thread_local and __thread seem to be functioning, at least
// in the POSIX GCC threads configuration.
//
__thread bool b;
thread_local sptr s {nullptr};

int main ()
{
  using thread = mingw_stdthread::thread;

  std::vector<thread> ts;
  for (int i = 0; i != 256; ++i)
    ts.push_back (thread([]()
                         {
                           assert (!b);
                           b = true;

                           assert (s.p_ == nullptr);
                           s.p_ = new std::string ("POSIX threads are great");
                         }));

  for (thread& t: ts)
    t.join();
}
