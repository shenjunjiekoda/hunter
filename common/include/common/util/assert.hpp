//===- assert.hpp -----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This header defines some assertion macros and unreachable code
//  detection.
//
//===------------------------------------------------------------------===//

#undef hunter_assert
#undef hunter_assert_msg
#undef hunter_unreachable

#ifdef NDEBUG

#    define hunter_assert(expr) static_cast< void >(0)

#    define hunter_assert_msg(expr, msg) static_cast< void >(0)

#    if __has_builtin(__builtin_unreachable)
#        define hunter_unreachable(msg) __builtin_unreachable()
#    elif defined(_MSC_VER)
#        define hunter_unreachable(msg) __assume(false)
#    else
#        include <cstdlib>
#        define hunter_unreachable(msg) abort()
#    endif

#else

#    include <cassert>

#    define hunter_assert(expr) assert(expr)
#    define hunter_assert_msg(expr, msg) assert((expr) && (msg))
#    define hunter_unreachable(msg) assert(false && (msg))

#endif
