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

// #include "analyzer/core/analysis_manager.hpp"
#include "analyzer/tooling/diagnostic.hpp"
// #include "analyzer/tooling/factory.hpp"
#include "analyzer/tooling/driver.hpp"
#include "analyzer/tooling/module.hpp"
#include "analyzer/tooling/reporter.hpp"
#include "common/util/llvm.hpp"
#include "common/util/vfs.hpp"

#include <clang/Tooling/Tooling.h>

#define DEBUG_TYPE "Hunter"

LLVM_INSTANTIATE_REGISTRY(hunter::HunterModuleRegistry); // NOLINT

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
    explicit HunterActionFactory(HunterContext& ctx
                                 // ,
                                 // std::unique_ptr< analyzer::AnalysisManager >
                                 // external_analysis_manager = nullptr,
                                 // std::unique_ptr< analyzer::CheckerManager >
                                 // external_checker_manager = nullptr
                                 )
        : m_ast_factory(ctx
                        // ,
                        // std::move(external_analysis_manager),
                        // std::move(external_checker_manager)
          ) {}
    std::unique_ptr< clang::FrontendAction > create() override {
        return std::make_unique< HunterAction >(&m_ast_factory);
    }

  private:
    HunterASTConsumerFactory m_ast_factory;
}; // class HunterActionFactory

} // anonymous namespace

HunterASTConsumerFactory::HunterASTConsumerFactory(
    HunterContext& ctx
    // ,
    // std::unique_ptr< dfa::AnalysisManager > external_analysis_manager,
    // std::unique_ptr< dfa::CheckerManager > external_checker_manager
    )
    : m_ctx(ctx) {
    // if (external_analysis_manager != nullptr) {
    //     m_analysis_manager = std::move(external_analysis_manager);
    // } else {
    //     m_analysis_manager = std::make_unique< dfa::AnalysisManager >(m_ctx);
    // }
    // if (external_checker_manager != nullptr) {
    //     m_checker_manager = std::move(external_checker_manager);
    // } else {
    //     m_checker_manager =
    //         std::make_unique< dfa::CheckerManager >(m_ctx,
    //         *m_analysis_manager);
    // }

    // m_factory = std::make_unique< HunterFactory >(*m_analysis_manager,
    //                                               *m_checker_manager);
    for (auto entry : HunterModuleRegistry::entries()) {
        // entry.instantiate()->add_to_factory(*m_factory);
    }
}

std::unique_ptr< clang::ASTConsumer > HunterASTConsumerFactory::
    create_ast_consumer(clang::CompilerInstance& ci, llvm::StringRef file) {
    auto& source_mgr = ci.getSourceManager();
    auto& file_mgr = source_mgr.getFileManager();
    m_ctx.set_current_file(file);
    m_ctx.set_current_ast_context(&ci.getASTContext());
    // m_analysis_manager->set_ast_context(ci.getASTContext());

    auto working_dir =
        file_mgr.getVirtualFileSystem().getCurrentWorkingDirectory();
    if (working_dir) {
        m_ctx.set_current_build_dir(working_dir.get());
    }

    // for (const auto& [id, _] : get_enabled_checks()) {
    //     m_checker_manager->add_required_checker(id);
    // }
    // auto checkers = m_factory->create_checkers(*m_checker_manager, &m_ctx);
    // m_checker_manager->add_all_required_analyses_by_checker_dependencies();

    // for (const auto& [id, _] : get_directly_enabled_analyses()) {
    //     hunter_log(llvm::outs() << "add required by directly enabled: "
    //                             << dfa::get_analysis_name_by_id(id) <<
    //                             "\n";);
    //     m_analysis_manager->add_required_analysis(id);
    // }

    // for (const auto& [id, _] : get_enabled_core_analyses()) {
    //     hunter_log(llvm::outs() << "add required by core enabled: "
    //                             << dfa::get_analysis_name_by_id(id) <<
    //                             "\n";);
    //     m_analysis_manager->add_required_analysis(id);
    // }

    // m_analysis_manager->compute_all_required_analyses_by_dependencies();
    // auto analyses = m_factory->create_analyses(*m_analysis_manager, &m_ctx);
    // m_analysis_manager->compute_full_order_analyses_after_registry();

    return std::make_unique< HunterASTConsumer >(m_ctx
                                                 // ,
                                                 //  *m_analysis_manager,
                                                 //  *m_checker_manager,
                                                 //  std::move(checkers),
                                                 //  std::move(analyses)
    );
}

std::vector< std::pair< analyzer::CheckerID, llvm::StringRef > >
HunterASTConsumerFactory::get_enabled_checks() const {
    std::vector< std::pair< analyzer::CheckerID, llvm::StringRef > >
        enabled_checks;
    // for (const auto& [checker, registry] : m_factory->checker_registries()) {
    //     if (m_ctx.is_check_enabled(checker.second)) {
    //         enabled_checks.emplace_back(checker);
    //     }
    // }
    return enabled_checks;
}

std::vector< std::pair< analyzer::AnalysisID, llvm::StringRef > >
HunterASTConsumerFactory::get_directly_enabled_analyses() const {
    std::vector< std::pair< analyzer::AnalysisID, llvm::StringRef > >
        enabled_analyses;
    // for (const auto& [analysis, registry] : m_factory->analysis_registries())
    // {
    //     if (m_ctx.is_analysis_directly_enabled(analysis.second)) {
    //         enabled_analyses.emplace_back(analysis);
    //     }
    // }
    return enabled_analyses;
}

std::vector< std::pair< analyzer::AnalysisID, llvm::StringRef > >
HunterASTConsumerFactory::get_enabled_core_analyses() const {
    std::vector< std::pair< analyzer::AnalysisID, llvm::StringRef > >
        enabled_analyses;
    // for (const auto& [analysis, registry] : m_factory->analysis_registries())
    // { if (m_ctx.is_core_analysis_enabled(analysis.second)) {
    // enabled_analyses.emplace_back(analysis);
    // }
    // }
    return enabled_analyses;
}

std::vector< HunterDiagnostic > HunterDriver::run() {
    using namespace clang;
    using namespace clang::tooling;
    ClangTool clang_tool(m_cdb,
                         m_input_files,
                         std::make_shared< PCHContainerOperations >(),
                         m_base_fs);

    clang::tooling::CommandLineArguments clang_include{
        llvm_util::get_clang_include_dir()};
    clang_tool.appendArgumentsAdjuster(
        clang::tooling::
            getInsertArgumentAdjuster(clang_include,
                                      clang::tooling::ArgumentInsertPosition::
                                          END));

    HunterDiagnosticConsumer diag_consumer(m_ctx);
    DiagnosticsEngine diag_engine(new DiagnosticIDs(),
                                  new DiagnosticOptions(),
                                  &diag_consumer,
                                  false);
    m_ctx.set_diagnostic_engine(&diag_engine);
    clang_tool.setDiagnosticConsumer(&diag_consumer);

    // auto analysis_manager = std::make_unique< analyzer::AnalysisManager
    // >(m_ctx); auto checker_manager =
    //     std::make_unique< analyzer::CheckerManager >(m_ctx,
    //     *analysis_manager);

    // HunterActionFactory action_factory(m_ctx,
    //                                    std::move(analysis_manager),
    //                                    std::move(checker_manager));
    // clang_tool.run(&action_factory);
    return diag_consumer.take_diags();
}

void HunterDriver::handle_diagnostics(
    const std::vector< HunterDiagnostic >& diagnostics, bool try_fix) {
    const FixKind fix = try_fix ? FixKind::FixIt : FixKind::None;

    DiagnosticReporter reporter(fix, m_ctx, std::move(m_base_fs));

    auto& vfs =
        reporter.get_source_manager().getFileManager().getVirtualFileSystem();
    auto origin_cwd = vfs.getCurrentWorkingDirectory();
    hunter_assert_msg(origin_cwd, "failed to get current working directory");

    for (const auto& diagnostic : diagnostics) {
        if (!diagnostic.BuildDirectory.empty()) {
            (void)vfs.setCurrentWorkingDirectory(diagnostic.BuildDirectory);
        }
        reporter.report(diagnostic);
        (void)vfs.setCurrentWorkingDirectory(*origin_cwd);
    }
    reporter.apply_fixes();
}

} // namespace hunter