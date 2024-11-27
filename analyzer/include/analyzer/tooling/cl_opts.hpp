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

namespace hunter::analyzer::cl_opts {

using namespace llvm;
using namespace clang::driver;
using namespace clang::tooling;

// NOLINTBEGIN(readability-identifier-naming,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-interfaces-global-init,fuchsia-statically-constructed-objects)

inline cl::OptionCategory hunter_analyzer_category("hunter analyzer options");

inline cl::extrahelp common_help_msg(CommonOptionsParser::HelpMessage);
inline cl::extrahelp hunter_analyzer_help_desc(R"(
Welcome to Hunter!

Hunter is a static analysis tool that checks C/C++ code for vulnerabilities and coding errors.
)");

inline cl::desc desc(StringRef desc) {
    return cl::desc{desc.ltrim()};
}

inline cl::opt< std::string > analyses("analyses",
                                       desc(R"(
use `*` for all analyses enabled, and `-` prefix to disable 
analyses. You can provide a comma-separated glob list of 
checker names.
)"),
                                       cl::init("-*"),
                                       cl::cat(hunter_analyzer_category));

inline cl::opt< std::string > checkers("checkers",
                                       desc(R"(
use `*` for all checkers enabled, and `-` prefix to disable 
checkers. You can provide a comma-separated glob list of 
checker names.
)"),
                                       cl::init("-*"),
                                       cl::cat(hunter_analyzer_category));

inline cl::opt< bool > list_checkers("list-checkers",
                                     desc(R"(
list enabled checkers and exit program. Use with
-checkers=* to list all available checkers.
)"),
                                     cl::init(false),
                                     cl::cat(hunter_analyzer_category));

inline cl::opt< bool > list_analyses("list-analyses",
                                     desc(R"(
list enabled analyses and exit program. Use with
-analyses=* to list all available analyses.
)"),
                                     cl::init(false),
                                     cl::cat(hunter_analyzer_category));

inline cl::opt< std::string > overlay_file("overlay_file",
                                           desc(R"(
Overlay the vfs described by overlay file on the real fs.
)"),
                                           cl::value_desc("filename"),
                                           cl::cat(hunter_analyzer_category));

inline cl::opt< bool > use_color("use-color",
                                 desc(R"(
Use colors in output.
)"),
                                 cl::init(true),
                                 cl::cat(hunter_analyzer_category));

inline cl::opt< bool > try_fix("fix",
                               desc(R"(
Try to apply suggested fixes.
)"),
                               cl::init(false),
                               cl::cat(hunter_analyzer_category));

inline cl::opt< bool > view_cfg("view-cfg",
                                desc(R"(
View the control flow graph of the analyzed function.
)"),
                                cl::init(false),
                                cl::cat(hunter_analyzer_category));

inline cl::opt< bool > dump_cfg("dump-cfg",
                                desc(R"(
Dump the control flow graph of the analyzed function.
)"),
                                cl::init(false),
                                cl::cat(hunter_analyzer_category));

inline cl::list< std::string > XcArgs(
    "Xc",
    cl::desc("Pass the following argument to the analyzer options"),
    cl::ZeroOrMore,
    cl::cat(hunter_analyzer_category));

inline cl::OptionCategory hunter_analyzer_engine_category(
    "hunter analyzer engine options");

inline cl::opt< int > widening_delay(
    "widening-delay",
    cl::init(1),
    cl::desc("delay count of iterations before widening"),
    cl::cat(hunter_analyzer_engine_category));

inline cl::opt< int > max_widening_iterations(
    "max-widening-iterations",
    cl::desc("maximum number of widening iterations"),
    cl::init(5),
    cl::cat(hunter_analyzer_engine_category));

inline cl::opt< int > max_narrowing_iterations(
    "max-narrowing-iterations",
    cl::desc("maximum number of narrowing iterations"),
    cl::init(5),
    cl::cat(hunter_analyzer_engine_category));

inline cl::opt< bool > analyze_with_threshold(
    "analyze-with-threshold",
    cl::desc("if true, do widening and narrowing with threshold"),
    cl::init(false),
    cl::cat(hunter_analyzer_engine_category));

// NOLINTEND(readability-identifier-naming,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-interfaces-global-init,fuchsia-statically-constructed-objects)

} // namespace hunter::analyzer::cl_opts