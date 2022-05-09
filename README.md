# libmingw-stdthreads

Standard C++14 threads implementation for MinGW GCC.

The current implementation is a cleaned up subset of the
[`mingw-std-threads`][mingw-std-threads] project.

- Windows 7 or later (`_WIN32_WINNT` must be defined to `0x0601` or greater).

- Only x86-64.

- No std::call_once, std::future.

[mingw-std-threads]: https://github.com/meganz/mingw-std-threads
