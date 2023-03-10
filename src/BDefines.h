/* date = March 3rd 2023 9:33 pm */

#ifndef _B_DEFINES_H
#define _B_DEFINES_H

//
// NOTE(evan): Compilers
//
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

// TODO(evan): More compilers!
#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#undef COMPILER_LLVM
#define COMPILER_LLVM
#endif
#endif

#define COBJMACROS
#include <windows.h>
#include <intrin.h>
#include <stdint.h>

#define global static
#define internal static

#define Kibibytes(Value) (Value*1024)
#define Mebibytes(Value) (Kibibytes(Value)*1024LL)
#define Gibibytes(Value) (Mebibytes(Value)*1024LL)
#define Tebibytes(Value) (Gibibytes(Value)*1024LL)

#define Pi32 3.14159265359f
#define DEG_TO_RAD (Pi32 / 180)
#define EPSILON 1.19209290E-07F

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

#if BLIB_DEBUG
#define Assert(X) { if(!(X)) __debugbreak(); }
#else
#define Assert(X)
#endif

#define STR2(X) #X
#define STR(X) STR2(X)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t  s64;

typedef s32 b32;
#define true 1
#define false 0

typedef float f32;
typedef double f64;

#endif //_B_DEFINES_H
