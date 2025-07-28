#include "doctest.h"
#include "types.h"

// Constant struct
typedef struct A {
    i32 a = 4;
    i32 b = 5;
} A;

// Child constant struct
typedef struct B : A {
    i32 c = 6;

    B(i32 a_, i32 b_)
    {
        a = a_;
        b = b_;
    }
} B;

// Generic struct, with list as well
template <typename T> struct G {
    T a = 4;
    T b = 5;
};

// Child generic struct
template <typename T> struct F : G<T> {
    T c = 6;

    F(T a_, T b_)
    {
        // This does not work even with `using a = typename G<T>::a;`
        // a = a_;
        // b = b_;

        G<T>::a = a_;
        G<T>::b = b_;
    }
};

// Template, but specified
typedef struct H : G<char> {
    char c = 6;

    H(char a_, char b_)
    {
        a = a_;
        b = b_;
    }
} H;

int main()
{
    TEST_SUITE("Inheritence")
    {

        TEST_CASE("Sizes: Constant struct")
        {
            TEqual(sizeof(A), (isize)8, "%ld");

            // No new members => 8 bytes
            typedef struct B0 : A {
                B0(i32 a_, i32 b_)
                {
                    a = a_;
                    b = b_;
                }
            } B0;
            TEqual(sizeof(B0), (isize)8, "%ld");

            // 1 new members => 12 bytes
            typedef struct B1 : A {
                i32 c;
                B1(i32 a_, i32 b_, i32 c_)
                {
                    a = a_;
                    b = b_;
                    c = c_;
                }
            } B1;
            TEqual(sizeof(B1), (isize)12, "%ld");

            // Renaming members => 16 bytes
            // Why != 8 ??
            //   Orig fields are: `A::a`, `A::b`
            //   a, b: new members (`B::a`, `B::b`)
            //   Total: 8 + 8 (A::, B::)
            typedef struct B2 : A {
                i32 a;
                i32 b;
                B2(i32 a_, i32 b_)
                {
                    a = a_;
                    b = b_;
                }
            } B2;
            TEqual(sizeof(B2), (isize)16, "%ld");

            // Resizing members => 16 + 8 bytes
            // Why != 16 ??
            //   Orig fields are: `A::a`, `A::b`
            //   a, b: new members (`B::a`, `B::b`)
            //   Total: 8 + 16 (A::, B::)
            typedef struct B3 : A {
                i64 a;
                i64 b;
            } B3;
            TEqual(sizeof(B3), (isize)24, "%ld");
        }

        TEST_CASE("Sizes: Templated struct")
        {
            TEqual(sizeof(G<char>), (isize)2, "%ld");
            TEqual(sizeof(G<u16>), (isize)4, "%ld");
            TEqual(sizeof(G<u32>), (isize)8, "%ld");

            TEqual(sizeof(F<char>), (isize)3, "%ld");
            TEqual(sizeof(F<u16>), (isize)6, "%ld");
            TEqual(sizeof(F<u32>), (isize)12, "%ld");

            TEqual(sizeof(H), (isize)3, "%ld");
        }
    }

    return 0;
}
