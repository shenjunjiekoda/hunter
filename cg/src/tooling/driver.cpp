//===- hunter.cpp -----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file implements the core driver for the Hunter tool.
//
//===------------------------------------------------------------------===//

#include "cg/tooling/driver.hpp"
#include "common/util/llvm.hpp"
#include "common/util/log.hpp"
#include "common/util/vfs.hpp"

#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Support/raw_ostream.h>

#define DEBUG_TYPE "cg-driver"

STATISTIC(NumSystemFunctions, "The # of times system functions are processed");
STATISTIC(NumImplicitFunctions,
          "The # of times implicit functions are processed");
STATISTIC(NumTotalFunctions, "The # of times total functions are processed");

namespace hunter {

namespace {

class HunterAction : public clang::ASTFrontendAction {
  public:
    explicit HunterAction(HunterASTConsumerFactory* ast_factory)
        : m_ast_factory(ast_factory) {}
    std::unique_ptr< clang::ASTConsumer > CreateASTConsumer(
        clang::CompilerInstance& ci, llvm::StringRef file) override {
        return m_ast_factory->create_ast_consumer(ci, file);
    }

  private:
    HunterASTConsumerFactory* m_ast_factory;
}; // class HunterAction

class HunterActionFactory : public clang::tooling::FrontendActionFactory {
  public:
    explicit HunterActionFactory(CGContext& ctx) : m_ast_factory(ctx) {}
    std::unique_ptr< clang::FrontendAction > create() override {
        return std::make_unique< HunterAction >(&m_ast_factory);
    }

  private:
    HunterASTConsumerFactory m_ast_factory;
}; // class HunterActionFactory

} // anonymous namespace

HunterASTConsumerFactory::HunterASTConsumerFactory(CGContext& ctx)
    : m_ctx(ctx) {}

std::unique_ptr< clang::ASTConsumer > HunterASTConsumerFactory::
    create_ast_consumer(clang::CompilerInstance& ci, llvm::StringRef file) {
    auto& source_mgr = ci.getSourceManager();
    auto& file_mgr = source_mgr.getFileManager();
    m_ctx.file = file;
    m_ctx.ast_ctx = &ci.getASTContext();

    return std::make_unique< HunterASTConsumer >(m_ctx);
}

void HunterCGBuilder::build() {
    using namespace clang;
    using namespace clang::tooling;
    ClangTool clang_tool(*m_ctx.cdb,
                         m_ctx.input_files,
                         std::make_shared< PCHContainerOperations >(),
                         m_ctx.overlay_fs);
    clang::tooling::CommandLineArguments clang_include{
        llvm_util::get_clang_include_dir()};
    hunter_log_nl(llvm::outs()
                  << "clang include dir: " << clang_include[0] << "\n");
    clang_tool.appendArgumentsAdjuster(
        clang::tooling::
            getInsertArgumentAdjuster(clang_include,
                                      clang::tooling::ArgumentInsertPosition::
                                          END));

    HunterActionFactory action_factory(m_ctx);
    clang_tool.run(&action_factory);
}

bool HunterASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef decl_group) {
    for (auto* decl : decl_group) {
        auto* function = llvm::dyn_cast_or_null< clang::FunctionDecl >(decl);
        if (function == nullptr || !function->hasBody()) {
            continue;
        }
        auto is_system_function =
            m_ctx.ast_ctx->getSourceManager().isInSystemHeader(
                decl->getLocation());
        if (!is_system_function || m_ctx.show_process_sys_function) {
            llvm::outs() << "[*] Processing function: ";
        }

        if (is_system_function) {
            NumSystemFunctions++;
        }
        if (function->isImplicit()) {
            NumImplicitFunctions++;
        }
        NumTotalFunctions++;

        if (m_ctx.use_color) {
            if (is_system_function) {
                if (m_ctx.show_process_sys_function) {
                    llvm::outs().changeColor(llvm::raw_ostream::Colors::RED);
                }
            } else {
                llvm::outs().changeColor(llvm::raw_ostream::Colors::GREEN);
            }
        }
        if (!is_system_function || m_ctx.show_process_sys_function) {
            function->printName(llvm::outs());
            llvm::outs().resetColor();
            if (function->isImplicit()) {
                llvm::outs() << " (implicit)";
            }
            llvm::outs() << "\n";
        }
    }

    return true;
}
} // namespace hunter