//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_CORE_THREAD_DETAIL_SCOPEDLOCK_H_INCLUDED
#define BEAST_CORE_THREAD_DETAIL_SCOPEDLOCK_H_INCLUDED

#include "../MutexTraits.h"

namespace detail
{

template <typename Mutex>
class TrackedScopedLock : public Uncopyable
{
public:
    inline explicit TrackedScopedLock (Mutex const& mutex,
        char const* fileName, int lineNumber) noexcept
        : m_mutex (mutex)
        , m_lock_count (0)
    {
        lock (fileName, lineNumber);
    }

    inline ~TrackedScopedLock () noexcept
    {
        if (m_lock_count > 0)
            unlock ();
    }    

    inline void lock (char const* fileName, int lineNumber) noexcept
    {
        ++m_lock_count;
        m_mutex.lock (fileName, lineNumber);
    }

    inline void unlock () noexcept
    {
        m_mutex.unlock ();
        --m_lock_count;
    }

private:
    Mutex const& m_mutex;
    int m_lock_count;
};

//--------------------------------------------------------------------------

template <typename Mutex>
class TrackedScopedTryLock : public Uncopyable
{
public:
    inline explicit TrackedScopedTryLock (Mutex const& mutex,
        char const* fileName, int lineNumber) noexcept
        : m_mutex (mutex)
        , m_lock_count (0)
    {
        try_lock (fileName, lineNumber);
    }

    inline ~TrackedScopedTryLock () noexcept
    {
        if (m_lock_count > 0)
            unlock ();
    }

    inline bool owns_lock () const noexcept
    {
        return m_lock_count > 0;
    }

    inline bool try_lock (char const* fileName, int lineNumber) noexcept
    {
        bool const success = m_mutex.try_lock (fileName, lineNumber);
        if (success)
            ++m_lock_count;
        return success;
    }

    inline void unlock () noexcept
    {
        m_mutex.unlock ();
        --m_lock_count;
    }

private:
    Mutex const& m_mutex;
    int m_lock_count;
};

//--------------------------------------------------------------------------

template <typename Mutex>
class TrackedScopedUnlock : public Uncopyable
{
public:
    inline explicit TrackedScopedUnlock (Mutex const& mutex,
        char const* fileName, int lineNumber) noexcept
        : m_mutex (mutex)
        , m_fileName (fileName)
        , m_lineNumber (lineNumber)
    {
        m_mutex.unlock ();
    }

    inline ~TrackedScopedUnlock () noexcept
    {
        m_mutex.lock (m_fileName, m_lineNumber);
    }

private:
    Mutex const& m_mutex;
    char const* const m_fileName;
    int const m_lineNumber;
};

//--------------------------------------------------------------------------

template <typename Mutex>
class UntrackedScopedLock : public Uncopyable
{
public:
    inline explicit UntrackedScopedLock (Mutex const& mutex,
        char const*, int) noexcept
        : m_mutex (mutex)
        , m_lock_count (0)
    {
        lock ();
    }

    inline ~UntrackedScopedLock () noexcept
    {
        if (m_lock_count > 0)
            unlock ();
    }

    inline void lock () noexcept
    {
        ++m_lock_count;
        m_mutex.lock ();
    }

    inline void lock (char const*, int) noexcept
    {
        lock ();
    }

    inline void unlock () noexcept
    {
        m_mutex.unlock ();
        --m_lock_count;
    }

private:
    Mutex const& m_mutex;
    int m_lock_count;
};

//--------------------------------------------------------------------------

template <typename Mutex>
class UntrackedScopedTryLock : public Uncopyable
{
public:
    inline explicit UntrackedScopedTryLock (Mutex const& mutex,
        char const*, int) noexcept
        : m_mutex (mutex)
        , m_lock_count (0)
    {
        try_lock ();
    }

    inline ~UntrackedScopedTryLock () noexcept
    {
        if (m_lock_count > 0)
        unlock ();
    }

    inline bool owns_lock () const noexcept
    {
        return m_lock_count > 0;
    }

    inline bool try_lock () noexcept
    {
        bool const success = m_mutex.try_lock ();
        if (success)
            ++m_lock_count;
        return success;
    }

    inline bool try_lock (char const*, int) noexcept
    {
        return try_lock ();
    }

    inline void unlock () noexcept
    {
        m_mutex.unlock ();
        --m_lock_count;
    }
  
private:
    Mutex const& m_mutex;
    int m_lock_count;
};

//--------------------------------------------------------------------------

template <typename Mutex>
class UntrackedScopedUnlock : public Uncopyable
{
public:
    UntrackedScopedUnlock (Mutex const& mutex,
        char const*, int) noexcept
        : m_mutex (mutex)
        , m_owns_lock (true)
    {
        MutexTraits <Mutex>::unlock (m_mutex);
        m_owns_lock = false;
    }

    ~UntrackedScopedUnlock () noexcept
    {
        MutexTraits <Mutex>::lock (m_mutex);
        m_owns_lock = true;
    }

private:
    Mutex const& m_mutex;
    bool m_owns_lock;
};

} // namespace detail

#endif
