//===- llvm.hpp -------------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This header defines some llvm related utilities.
//
//===------------------------------------------------------------------===//

#pragma once

#include <regex>
#include <string>

#include "common/util/log.hpp"

namespace hunter::llvm_util {

std::string get_clang_include_dir();

} // namespace hunter::llvm_util