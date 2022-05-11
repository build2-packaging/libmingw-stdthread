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

// Note that while __thread seems to be functioning, at least in the POSIX GCC
// threads configuration, thread_local with dynamic allocation/destruction
// appears to be broken.
//
__thread bool b;
#if 0
thread_local sptr s {nullptr};
#endif

int
main ()
{
  using thread = mingw_stdthread::thread;

  std::vector<thread> ts;
  for (int i = 0; i != 256; ++i)
    ts.push_back (thread([]()
                         {
                           assert (!b);
                           b = true;

#if 0
                           assert (s.p_ == nullptr);
                           s.p_ = new std::string ("POSIX threads are great");
#endif
                         }));

  for (thread& t: ts)
    t.join();
}
