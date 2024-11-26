//===- log.hpp -------------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This header defines some logging utilities.
//
//===------------------------------------------------------------------===//

#pragma once

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>
#include <source_location>

namespace hunter {

inline void print_source_location(
    const char* debug_type,
    const char* function_name,
    bool new_line,
    const std::source_location& location = std::source_location::current()) {
    llvm::outs().changeColor(llvm::raw_ostream::Colors::BLUE);
    llvm::outs() << "[" << debug_type << "][" << function_name << ":"
                 << location.line() << ":" << location.column() << "] ";
    if (new_line) {
        llvm::outs() << '\n';
    }
    llvm::outs().resetColor();
}

#define log_with_type(TYPE, PRINT_SRC, NEW_LINE, X)                      \
    do {                                                                 \
        using namespace llvm;                                            \
        if (::llvm::DebugFlag && isCurrentDebugType(TYPE)) {             \
            if (PRINT_SRC) {                                             \
                hunter::print_source_location(TYPE, __func__, NEW_LINE); \
            }                                                            \
            { X; }                                                       \
        }                                                                \
    } while (false)

#define hunter_log(X) log_with_type(DEBUG_TYPE, true, false, X)
#define hunter_log_nl(X) log_with_type(DEBUG_TYPE, true, true, X)
#define hunter_debug(X) log_with_type(DEBUG_TYPE, false, false, X)

} // namespace hunter
