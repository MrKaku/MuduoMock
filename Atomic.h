#pragma once

#include <windows.h>
#include <stdint.h>

template<typename T>
class AtomicIntegerT
{
public:
    AtomicIntegerT()
        : value_(0)
    {
    }

    // uncomment if you need copying and assignment
   
   AtomicIntegerT(const AtomicIntegerT& that)
     : value_(that.get())
   {}
   
   AtomicIntegerT& operator=(const AtomicIntegerT& that)
   {
     getAndSet(that.get());
     return *this;
   }

    T get()
    {
        return AtomicTraits::AtomicAdd((T*)&value_, 0);
    }

    T getAndAdd(T x)
    {
        T tmp = get();
        AtomicTraits::AtomicAdd((T*)&value_, x);
        return tmp;
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        return AtomicTraits::AtomicSet((T*)&value_, newValue);
    }

private:
    struct AtomicTraits;

private:
    volatile T value_;
};

template<> struct AtomicIntegerT<int32_t>::AtomicTraits
{
    static int32_t AtomicAdd(int32_t* t, int32_t x) { return InterlockedExchangeAdd((LONG*)t, (LONG)x); }
    static int32_t AtomicSet(int32_t* t, int32_t x) { return InterlockedExchange((LONG*)t, (LONG)x); }
};

template<> struct AtomicIntegerT<int64_t>::AtomicTraits
{
    static int64_t AtomicAdd(int64_t* t, int64_t x) { return InterlockedExchangeAdd64((LONGLONG*)t, (LONGLONG)x); }
    static int64_t AtomicSet(int64_t* t, int64_t x) { return InterlockedExchange64((LONGLONG*)t, (LONGLONG)x); }
};


typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;
