//===- llvm.cpp -------------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This header implements some llvm related utilities.
//
//===------------------------------------------------------------------===//

#include "common/util/llvm.hpp"

#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>

#define DEBUG_TYPE "llvm-util"

namespace hunter::llvm_util {

namespace {

std::string extract_llvm_version(std::string input) {
    std::regex re("\\d+");
    std::smatch match;

    if (std::regex_search(input, match, re)) {
        return match.str();
    }
    hunter_log_nl(llvm::WithColor::note()
                  << "failed to extract version from input: " << input);
    return input;
}

} // anonymous namespace

std::string get_clang_include_dir() {
    return std::string("-I" LLVM_LIBRARY_DIRS "/clang/") +
           extract_llvm_version(LLVM_PACKAGE_VERSION) + "/include/";
}

} // namespace hunter::llvm_util