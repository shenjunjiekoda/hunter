//===- driver.hpp -----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file defines the core driver for the Hunter analyzer tool.
//
//===------------------------------------------------------------------===//

#pragma once

#include "clang/AST/ASTContext.h"
#include "common/util/vfs.hpp"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclGroup.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/LLVM.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include "common/util/log.hpp"

#include <memory>
#include <utility>

namespace hunter {

struct CGContext {
    bool use_color = true;
    bool show_process_sys_function = false;
    llvm::IntrusiveRefCntPtr< llvm::vfs::OverlayFileSystem > overlay_fs;
    const clang::tooling::CompilationDatabase* cdb;
    std::vector< std::string > input_files;

    clang::ASTContext* ast_ctx = nullptr;
    std::string file = "";
};

class HunterASTConsumer : public clang::ASTConsumer {
  public:
    HunterASTConsumer(CGContext& ctx) : m_ctx(ctx) {}

    bool HandleTopLevelDecl(clang::DeclGroupRef decl_group) override;

  private:
    CGContext& m_ctx;
}; // class HunterASTConsumer

class HunterASTConsumerFactory {
  private:
    CGContext& m_ctx;

  public:
    explicit HunterASTConsumerFactory(CGContext& ctx);

    /// \brief Create an AST consumer for the given file.
    [[nodiscard]] std::unique_ptr< clang::ASTConsumer > create_ast_consumer(
        clang::CompilerInstance& ci, llvm::StringRef file);
}; // class HunterASTConsumerFactory

class HunterCGBuilder {
  private:
    CGContext& m_ctx;

  public:
    HunterCGBuilder(CGContext& ctx) : m_ctx(ctx) {}

  public:
    void build();
}; // class HunterDriver

} // namespace hunter
