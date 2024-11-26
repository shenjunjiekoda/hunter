//===- diagnostic.hpp -------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file defines the diagnostic used by the hunter tool.
//
//===------------------------------------------------------------------===//

#pragma once

#include <clang/Basic/Diagnostic.h>
#include <clang/Frontend/DiagnosticRenderer.h>
#include <clang/Tooling/Core/Diagnostic.h>

#include "common/util/assert.hpp"

namespace hunter {

class HunterContext;

// NOLINTNEXTLINE(altera-struct-pack-align)
struct HunterDiagnostic : clang::tooling::Diagnostic {
    HunterDiagnostic(llvm::StringRef checker,
                     Level diag_level,
                     llvm::StringRef build_dir);
}; // struct HunterDiagnostic

// NOLINTNEXTLINE(altera-struct-pack-align)
struct HunterDiagnosticConsumer : public clang::DiagnosticConsumer {
    explicit HunterDiagnosticConsumer(HunterContext& context);

    void HandleDiagnostic(clang::DiagnosticsEngine::Level diag_level,
                          const clang::Diagnostic& diagnostic) override;

    // Retrieve the diagnostics that were captured.
    std::vector< HunterDiagnostic > take_diags();

  private:
    HunterContext& m_context;
    std::vector< HunterDiagnostic > m_diags;
}; // struct HunterDiagnosticConsumer

class HunterDiagnosticRenderer : public clang::DiagnosticRenderer {
  private:
    HunterDiagnostic& m_diag;

  public:
    HunterDiagnosticRenderer(const clang::LangOptions& lang_opts,
                             clang::DiagnosticOptions* diag_opts,
                             HunterDiagnostic& diag)
        : clang::DiagnosticRenderer(lang_opts, diag_opts), m_diag(diag) {}

  protected:
    void emitDiagnosticMessage(clang::FullSourceLoc loc,
                               clang::PresumedLoc ploc,
                               clang::DiagnosticsEngine::Level level,
                               llvm::StringRef msg,
                               llvm::ArrayRef< clang::CharSourceRange > ranges,
                               clang::DiagOrStoredDiag diag) override;
    void emitDiagnosticLoc(
        clang::FullSourceLoc loc,
        clang::PresumedLoc ploc,
        clang::DiagnosticsEngine::Level level,
        llvm::ArrayRef< clang::CharSourceRange > ranges) override {}

    void emitCodeContext(
        clang::FullSourceLoc loc,
        clang::DiagnosticsEngine::Level level,
        llvm::SmallVectorImpl< clang::CharSourceRange >& ranges,
        clang::ArrayRef< clang::FixItHint > hints) override;

    void emitIncludeLocation(clang::FullSourceLoc loc,
                             clang::PresumedLoc ploc) override {}

    void emitImportLocation(clang::FullSourceLoc loc,
                            clang::PresumedLoc ploc,
                            llvm::StringRef module_name) override {}

    void emitBuildingModuleLocation(clang::FullSourceLoc loc,
                                    clang::PresumedLoc ploc,
                                    llvm::StringRef module_name) override {}

    void endDiagnostic(clang::DiagOrStoredDiag diag,
                       clang::DiagnosticsEngine::Level level) override {
        hunter_assert_msg(!m_diag.Message.Message.empty(),
                          "Message has not been set");
    }

}; // class HunterDiagnosticRenderer

} // namespace hunter