/// \file mingw.shared_mutex.h
/// \brief Standard-compliant shared_mutex for MinGW
///
/// (c) 2017 by Nathaniel J. McClatchey, Athens OH, United States
/// \author Nathaniel J. McClatchey
///
/// \copyright Simplified (2-clause) BSD License.
///
/// \note This file may become part of the mingw-w64 runtime package. If/when
/// this happens, the appropriate license will be added, i.e. this code will
/// become dual-licensed, and the current BSD 2-clause license will stay.
/// \note Target Windows version is determined by WINVER, which is determined in
/// <windows.h> from _WIN32_WINNT, which can itself be set by the user.

#ifndef MINGW_SHARED_MUTEX_H_
#define MINGW_SHARED_MUTEX_H_

#if !defined(__cplusplus) || (__cplusplus < 201402L)
#  error C++14 compiler required
#endif

#if !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0601
#  error _WIN32_WINNT should be 0x0601 (Windows 7) or greater
#endif

#include <cassert>
//  For descriptive errors.
#include <system_error>
//  For timing in shared_timed_mutex.
#include <chrono>
#include <limits>

#include <shared_mutex> // shared_lock

//  For defer_lock_t, adopt_lock_t, and try_to_lock_t
#include "mingw-mutex.hxx"

#include <synchapi.h>

namespace mingw_stdthread
{
  using std::shared_lock;

  class condition_variable_any;

  // Slim Reader-Writer (SRW)-based implementation that requires Windows 7.
  //
  class shared_mutex : mutex
  {
    friend class condition_variable_any;
  public:
    using mutex::native_handle_type;
    using mutex::lock;
    using mutex::unlock;
    using mutex::native_handle;

    void lock_shared (void)
    {
      AcquireSRWLockShared(native_handle());
    }

    void unlock_shared (void)
    {
      ReleaseSRWLockShared(native_handle());
    }

    bool try_lock_shared (void)
    {
      return TryAcquireSRWLockShared(native_handle()) != 0;
    }

    using mutex::try_lock;
  };

  class shared_timed_mutex : shared_mutex
  {
    typedef shared_mutex Base;
  public:
    using Base::lock;
    using Base::try_lock;
    using Base::unlock;
    using Base::lock_shared;
    using Base::try_lock_shared;
    using Base::unlock_shared;

    template< class Clock, class Duration >
    bool try_lock_until ( const std::chrono::time_point<Clock,Duration>& cutoff )
    {
      do
      {
        if (try_lock())
          return true;
      }
      while (std::chrono::steady_clock::now() < cutoff);
      return false;
    }

    template< class Rep, class Period >
    bool try_lock_for (const std::chrono::duration<Rep,Period>& rel_time)
    {
      return try_lock_until(std::chrono::steady_clock::now() + rel_time);
    }

    template< class Clock, class Duration >
    bool try_lock_shared_until ( const std::chrono::time_point<Clock,Duration>& cutoff )
    {
      do
      {
        if (try_lock_shared())
          return true;
      }
      while (std::chrono::steady_clock::now() < cutoff);
      return false;
    }

    template< class Rep, class Period >
    bool try_lock_shared_for (const std::chrono::duration<Rep,Period>& rel_time)
    {
      return try_lock_shared_until(std::chrono::steady_clock::now() + rel_time);
    }
  };
}

#endif // MINGW_SHARED_MUTEX_H_
