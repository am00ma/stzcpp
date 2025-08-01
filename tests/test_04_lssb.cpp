#include "arena.h"
#include "doctest.h"
#include "types.h"

/* ---------------------------------------------------------------------------
 * List, Slice, Str, Buf
 * ------------------------------------------------------------------------- */

template <typename T> struct List {

    union {
        T*       buf = 0;
        const T* cbuf;
    };
    int len = 0;

    // /home/x/hub/repos/study/cpp/stzcpp/tests/test_lssb.cpp:37:34:
    // error: constructor for 'Str' must explicitly initialize the base class 'List<char>' which does not have a
    //        default constructor
    //    37 |     template <isize N> constexpr Str(const char (&s)[N])
    //  NOTE: Cannot initialize child without default
    List() = default;

    List(char* buf_, isize len_)
    {
        buf = buf_;
        len = len_;
    }

    template <isize N> constexpr List(T (&s)[N])
    {
        buf = s;
        len = N;
    }

    template <isize N> constexpr List(const T (&s)[N])
    {
        cbuf = s;
        len  = N;
    }

    // https://en.cppreference.com/w/cpp/language/this.html
    // The type of `this` in a member function of class `X` is `X*` (pointer to `X`).
    List<T>  operator[]() { return *this; };
    List<T>& operator[](bool, bool, bool) { return *this; };
    T*       operator[](isize i) { return &buf[i]; };                          // No checks
    List<T>  operator[](isize i, isize j) { return List<T>(&buf[i], j - i); }; // No checks

    // Equality is necessary for Tests
    bool operator==(List<T> s)
    {
        if (len != s.len) { return false; }
        if ((len == 0) && (buf == s.buf)) { return true; }
        RANGE(i, len)
        {
            if (buf[i] != *s[i]) return false;
        }
        return true;
    }

    // 1. Non-const lvalue reference to type 'List<int>' cannot bind to a temporary of type 'List<int> *'
    // [lvalue_reference_bind_to_temporary]
    // List<T>& operator[](bool ref) { return this; };
};

typedef struct Str : List<char> {

    template <isize N> constexpr Str(char (&s)[N])
    {
        buf = s;
        len = N - 1;
    }

    template <isize N> constexpr Str(const char (&s)[N])
    {
        cbuf = s;
        len  = N - 1;
    }

    Str(List<char> l)
    {
        buf = l.buf;
        len = l.len;
    }

} Str;

template <typename T> struct Slice : List<T> {

    int cap;

    //  NOTE: Cannot initialize child without default
    Slice() = default;

    Slice(char* buf_, int cap_)
    {
        List<T>::buf = buf_;
        List<T>::len = 0;
        cap          = cap_;
    };

    Slice(List<T> l)
    {
        List<T>::buf = l.buf;
        List<T>::len = l.len;
        cap          = l.len;
    };

    Slice<T> operator+=(T val)
    {
        if (List<T>::len + 1 <= cap)
        {
            List<T>::buf[List<T>::len] = val;
            List<T>::len++;
        }
        return *this;
    };

    Slice<T> operator+=(List<T> val)
    {
        RANGE(i, val.len)
        {
            if (List<T>::len + 1 <= cap)
            {
                List<T>::buf[List<T>::len] = *val[i];
                List<T>::len++;
            }
        }
        return List<T>(List<T>::buf, List<T>::len);
    };
};

typedef struct Buf : Slice<char> {

    Buf(Slice<char> b)
    {
        buf = b.buf;
        len = b.len;
        cap = b.cap;
    };

    Buf(Str l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    };

    Buf(List<char> l)
    {
        buf = l.buf;
        len = l.len;
        cap = l.len;
    };

    Buf(char* buf_, int cap_)
    {
        buf = buf_;
        len = 0;
        cap = cap_;
    };

    template <isize N> constexpr Buf(char (&s)[N])
    {
        List<char>::buf = s;
        List<char>::len = 0;
        cap             = N;
    }

    // Override List<char>::operator+=
    Buf operator+=(Str val)
    {
        RANGE(i, val.len)
        {
            if (List<char>::len + 1 <= cap)
            {
                List<char>::buf[List<char>::len] = *val[i];
                List<char>::len++;
            }
        }
        return *this;
    };

    // Shrinks arena
    // NOTE: provided no new objects after declaration of Buf
    Str Final(Arena* a)
    {
        a->beg -= cap - List<char>::len;
        cap     = List<char>::len; // So nothing can be added later
        return List<char>(List<char>::buf, List<char>::len);
    };

} Buf;

typedef List<int>   Lint;
typedef List<char>  Lchar;
typedef Slice<int>  Sint;
typedef Slice<char> Schar;

#define pstr(s)  (int)s.len, s.buf
#define ppstr(s) (int)s->len, s->buf

/* ---------------------------------------------------------------------------
 * List, Slice, Str, Buf
 * ------------------------------------------------------------------------- */

int main()
{

    BufArena(perm, abuf, 1024);

    TEST_SUITE("List Str Slice Buf")
    {

        TEST_CASE("Initialization: List")
        {
            // Needs (int[]) casting
            Lint li = (int[]){1, 2, 3};
            TEqualInt(li.len, 3);

            // Needs Lint() casting
            li = Lint({1, 2, 3});
            TEqualInt(li.len, 3);

            // Works right away
            Lchar lc = "hello";
            TEqualInt(lc.len, 6);
        }

        TEST_CASE("Initialization: Str")
        {
            // Different init for Str (inherits Lchar)
            Str s = "hello";
            TEqualInt(s.len, 5);
        }

        TEST_CASE("Initialization: Slice")
        {
            // Sint si = (int[]){1, 2, 3};
            //
            // Not intended usage, so not allowed
            // Slices must be initialized using arena
            // They are meant for memory management of Lists

            // This is allowed though :|
            Sint si = Lint({1, 2, 3});
            TEqualInt(si.len, 3);
            TEqualInt(si.cap, 3);
        }

        TEST_CASE("Initialization: Buf")
        {
            // Schar si = "hello";
            //
            // Not intended usage, so not allowed
            // Slices must be initialized using arena
            // They are meant for memory management of Lists

            // This is allowed though :|
            Schar si = Lchar("hello");
            TEqualInt(si.len, 6);
            TEqualInt(si.cap, 6);

            // Not allowed: 1. No viable conversion from 'const char[6]' to 'Buf' [typecheck_nonviable_condition]
            // Buf b = "hello";

            // !! Not found: 1. No viable conversion from 'Str' to 'Buf' [typecheck_nonviable_condition]
            // Had to define explicitly
            Buf b = Str("hello");
            TEqualInt(b.len, 5);
            TEqualInt(b.cap, 5);

            // Actual initialization needs 'arena'
            char abuf[16] = {};

            // Now we get proper behaviour (len=0, cap=16)
            Buf b1 = {abuf, 16};
            TEqualInt(b1.len, 0);
            TEqualInt(b1.cap, 16);
        }

        TEST_CASE("Operator: List indexing")
        {
            Lint a1 = Lint({1, 2, 3})[]; // returns Lint  (returns *this)
            TEqualInt(a1.len, 3);

            Lint a2 = Lint({1, 2, 3})[true, true, true]; // returns Lint& (returns *this) - Why does it accept Lint?
            TEqualInt(a2.len, 3);
        }

        TEST_CASE("Operator: Str indexing")
        {
            // Not possible without providing constructor `Str(List<char>)`
            // 1. No viable conversion from 'List<char>' to 'Str' [typecheck_nonviable_condition]
            Str a1 = Str("hello")[3, 5];
            TEqualStr(a1, Str("lo"));

            // Intended use
            Str a2 = "hello hi how are you";

            // Subscriptable, by reference
            Str a3 = a2[0, 5];
            TEqualInt(a3.len, 5);
            TEqualStr(a3, Str("hello"));

            // Zero indexing, so 'h' from 'hi' (7th char) has idx: 6
            Str a4 = a2[6, 8];
            TEqualInt(a4.len, 2);
            TEqualStr(a4, Str("hi"));
        }

        TEST_CASE("Operator: Slice indexing")
        {
            // operator[] returns Sint (not Lint)  (returns *this)
            Sint a1 = Lint({1, 2, 3})[];
            TEqualInt(a1.len, 3);

            // operator[bool] returns Sint& ? (not Lint&) (returns *this) - Why does it accept Lint?
            Sint a2 = Lint({1, 2, 3})[true, true, true];
            TEqualInt(a2.len, 3);
        }

        TEST_CASE("Operator: Slice append, extend")
        {
            char abuf[128] = {};

            Schar s1 = {abuf, 128};

            // Append
            RANGE(i, 6) { s1 += i + 65; };

            TEqualInt(s1.len, 6);
            TEqualInt(s1.cap, 128);
            TEqualStr(s1, Str("ABCDEF"));

            // Temporarily extend, so changes are not reflected to s1
            Schar s2 = s1;

            // Extend
            s2 += "hello"; // Treated as Lchar, not Str
            TEqualInt(s2.len, 12);
            TEqualInt(s2.cap, 128);
            TEqualStr(s2, Str("ABCDEFhello\0")); // Includes null char
            TEqualStr(s1, Str("ABCDEF"));        // Remains the same

            // Extend, indended behaviour for Buf (resuming from s1)
            s1 += Str("hello");
            TEqualInt(s1.len, 11);
            TEqualInt(s1.cap, 128);
            TEqualStr(s1, Str("ABCDEFhello"));
        }

        TEST_CASE("Operator: Buf indexing")
        {
            // operator[] returns Buf (not Lint)  (returns *this)
            // 1. No viable conversion from 'List<char>' to 'Buf' [typecheck_nonviable_condition]
            // Buf a1 = Str("hello")[];

            // So need to cast List<char> to Str then to Buf
            Buf a1 = Str(Str("hello")[]);
            TEqualInt(a1.len, 5);

            // After defining constructor from List<char>
            Buf a2 = Str("hello")[];
            TEqualInt(a1.len, 5);

            // Not possible, and not intended
            // Buf a3 = "hello";

            // Expected use
            char  abuf[128] = {};
            char* pbuf      = abuf;

            // Initialize from buffer on stack/heap
            Buf a3 = {abuf};
            Buf a4 = {pbuf, 128};

            // After redefining special case for Str, this works
            a3 += "hello";
            TEqualInt(a3.len, 5);

            a3 += " hi";
            TEqualInt(a3.len, 8);

            a3 += Str(" how")[0, 1];
            TEqualInt(a3.len, 9);

            TEqualStr(a3, Str("hello hi "));
        }

        //
    }

    TEST_RESULTS();
}
