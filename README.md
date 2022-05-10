# libmingw-stdthread

Standard C++14 threads implementation for MinGW-w64 GCC.

The current implementation is a cleaned/fixed up subset of the
[`mingw-std-threads`][mingw-std-threads] project.

- Windows 7 or later (`_WIN32_WINNT` must be defined to `0x0601` or greater).

- No `std::call_once`, `std::future` (but could probably be added relatively
  easily).

[mingw-std-threads]: https://github.com/meganz/mingw-std-threads
