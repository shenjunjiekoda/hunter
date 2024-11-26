//===- module.hpp -----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file defines the hunter module which contains different analyses
//  and checkers.
//
//===------------------------------------------------------------------===//

#pragma once

#include "analyzer/tooling/options.hpp"

#include <llvm/Support/Registry.h>

namespace hunter {

class HunterFactory;

/// A hunter module groups a number of \c analyses and \c checkers
/// with a unified group name.
class HunterModule {
  public:
    virtual ~HunterModule() {}

    /// \brief register all \c factories belonging to this module.
    virtual void add_to_factory(HunterFactory& factory) = 0;

    /// \brief Gets default options for checkers and analyses defined
    /// in this module.
    virtual HunterOptions get_module_options() { return {}; }
}; // class HunterModule

using HunterModuleRegistry = llvm::Registry< HunterModule >;

} // namespace hunter