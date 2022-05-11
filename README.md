# libmingw-stdthread

Standard C++14 threads implementation for MinGW-w64 GCC.

The current implementation is a cleaned up and fixed up subset of the
[`mingw-std-threads`][mingw-std-threads] project.

Requirements and limitations:

- Windows 7 or later (`_WIN32_WINNT` must be defined to `0x0601` or greater).

- No `std::call_once`, `std::future` (but could probably be added relatively
  easily).

- You should still use MinGW GCC configured with POSIX threads support
  (see Background below for details).

- `thread_local` dynamic initialization/destruction does not appear to work.


## Usage

Unlike the upstream/original project, this version does not try to inject any
names into the `std` namespace and then play hide and seek with GCC
developers. And, yes, this unfortunately means that third-party libraries
that include headers like `<thread>` or `<mutex>` cannot be used without
an extra effort.

The typical usage looks like this:

```c++

// Define USE_MINGW_STDTHREAD and _WIN32_WINNT=0x0601 in your buildfile
// to use C++14 threads implementation from libmingw-stdthread.
//
#ifndef USE_MINGW_STDTHREAD
#  include <mutex>
#  include <thread>
#  include <shared_mutex>
#  include <condition_variable>
#else
#  include <libmingw-stdthread/mutex.hxx>
#  include <libmingw-stdthread/thread.hxx>
#  include <libmingw-stdthread/shared_mutex.hxx>
#  include <libmingw-stdthread/condition_variable.hxx>
#endif

#ifndef USE_MINGW_STDTHREAD
  using std::mutex;
  using std::shared_mutex;
  using std::condition_variable;

  using std::unique_lock;
  using std::shared_lock;
  using std::defer_lock;
  using std::adopt_lock;

  using std::thread;
  namespace this_thread = std::this_thread;
#else
  using mingw_stdthread::mutex;
  using mingw_stdthread::shared_mutex;
  using mingw_stdthread::condition_variable;

  using mingw_stdthread::unique_lock;
  using mingw_stdthread::shared_lock;
  using mingw_stdthread::defer_lock;
  using mingw_stdthread::adopt_lock;

  using mingw_stdthread::thread;
  namespace this_thread = mingw_stdthread::this_thread;
#endif

```

## Background

Currently, the only way to get built-in support for C++11/14 threads in MinGW
is to configure GCC with POSIX threads support by means of the `winpthreads`
library. While the result appears to function, based on our exprience with
`build2`, there also appear to be data races or other similar issues.

Specifically, for every few hundred builds on our CI service, `build2` built
with MinGW GCC will crash or lockup a few times, on average. This does not
happen when `build2` is built with MSVC or on any other platform (nor if we
switch to this implementation). And looking at the `winpthreads` source code
makes one wonder how it doesn't happen a lot more often.

Note that the above headers (`<thread>`, `<mutex>`, etc) and the functionality
they provide is not the end of the multi-threading support. Specifically,
there are also the following mechanisms:

- Atomic operations (`<atomic>`).

- Thread-local storage (`thread_local` keyword).

- Exception handling (uses thread local storage).

- Thread-safe function `static` variables (hasn't been tested).

As a result, you should still use MinGW GCC configured with POSIX threads
support to make sure these mechanisms continue to function in a thread-aware
manner. While combining two implementations of threads is not ideal, in
practice the result appears to function reasonably well. The only known
breakage is dynamic initialization/destruction in thread-local storage.


[mingw-std-threads]: https://github.com/meganz/mingw-std-threads
