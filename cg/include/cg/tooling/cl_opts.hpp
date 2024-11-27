//===- cl_opts.hpp ----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file defines the command line options for the hunter tool.
//
//===------------------------------------------------------------------===//

#pragma once

#include <clang/Driver/Options.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Option/OptTable.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>

namespace hunter::cg::cl_opts {

using namespace llvm;
using namespace clang::driver;
using namespace clang::tooling;

// NOLINTBEGIN(readability-identifier-naming,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-interfaces-global-init,fuchsia-statically-constructed-objects)

inline cl::OptionCategory hunter_cg_category("hunter cg options");

inline cl::extrahelp common_help_msg(CommonOptionsParser::HelpMessage);
inline cl::extrahelp hunter_help_desc(R"(
Welcome to Hunter!

Hunter is a static analysis tool that checks C/C++ code for vulnerabilities and coding errors.
)");

inline cl::desc desc(StringRef desc) {
    return cl::desc{desc.ltrim()};
}

inline cl::opt< std::string > overlay_file("overlay_file",
                                           desc(R"(
Overlay the vfs described by overlay file on the real fs.
)"),
                                           cl::value_desc("filename"),
                                           cl::cat(hunter_cg_category));

inline cl::opt< bool > use_color("use-color",
                                 desc(R"(
Use colors in output.
)"),
                                 cl::init(true),
                                 cl::cat(hunter_cg_category));

inline cl::opt< bool > show_process_sys_function("show-process-sys-function",
                                                 desc(R"(
Show processing system functions.
)"),
                                                 cl::init(false),
                                                 cl::cat(hunter_cg_category));

// NOLINTEND(readability-identifier-naming,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-interfaces-global-init,fuchsia-statically-constructed-objects)

} // namespace hunter::cg::cl_opts