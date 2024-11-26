//===- options.hpp ----------------------------------------------------===//
//
// Copyright (c) 2024 Junjie Shen
//
// see https://github.com/shenjunjiekoda/hunter/blob/main/LICENSE for
// license information.
//
//===------------------------------------------------------------------===//
//
//  This file defines the hunter options used by the hunter library and
//  command-line tools.
//
//===------------------------------------------------------------------===//

#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>
#include <variant>

#include "analyzer/core/analyzer_options.hpp"

namespace hunter {

enum class OptionSource { Default, CommandLine, ConfigFile };

const char* option_src_to_string(OptionSource source);

using CheckerOptVal = std::variant< bool, std::string, int >;
using Extensions = std::set< std::string >;

constexpr unsigned HunterOptionsAlignment = 128U;
constexpr unsigned HunterOptionsProviderAlignment = 64U;
constexpr unsigned HunterOptionsProviderBigAlignment = 128U;

/// \brief Hunter options.
struct alignas(HunterOptionsAlignment) HunterOptions {
    /// \brief Checkers filter.
    std::string checkers;

    /// \brief Analyses filter.
    std::string analyses;

    /// \brief Header file extensions.
    Extensions header_extensions = {"h", "hh", "hpp", "hxx"};

    /// \brief Implematation file extensions.
    Extensions impl_extensions = {"c", "cc", "cpp", "cxx"};

    /// \brief checker specific options
    std::map< std::string, CheckerOptVal > check_opts;

    /// \brief the user using hunter
    std::string user = "unknown";

    /// \brief use color in output
    bool use_color = false;

    /// \brief view control flow graph
    bool view_cfg = false;

    /// \brief dump control flow graph
    bool dump_cfg = false;

    /// \brief analyzer options
    analyzer::AnalyzerOptions analyzer_opts;

}; // struct HunterOptions

struct HunterOptionsProvider {
    virtual ~HunterOptionsProvider() = default;

    [[nodiscard]] virtual OptionSource get_checker_option_source(
        const std::string& option) const = 0;

    [[nodiscard]] virtual HunterOptions get_options_for(
        const std::string& file) const = 0;

    virtual void set_checker_option(const std::string& option,
                                    CheckerOptVal value) = 0;
}; // struct HunterOptionsProvider

struct alignas(HunterOptionsProviderBigAlignment)
    HunterOptionsDefaultProvider // NOLINT(altera-struct-pack-align)
    : HunterOptionsProvider {
  public:
    HunterOptions options;

  public:
    HunterOptionsDefaultProvider();

    [[nodiscard]] OptionSource get_checker_option_source(
        const std::string& option) const override;

    [[nodiscard]] HunterOptions get_options_for(
        const std::string& file) const override;

    void set_checker_option(const std::string& option,
                            CheckerOptVal value) override;

  protected:
    void set_default_options();

}; // struct HunterOptionsDefaultProvider

struct alignas(HunterOptionsProviderBigAlignment)
    HunterOptionsCommandLineProvider // NOLINT(altera-struct-pack-align)
    : HunterOptionsDefaultProvider {
  protected:
    std::set< std::string > m_cmd_override_opts;

  public:
    HunterOptionsCommandLineProvider() = default;

    [[nodiscard]] OptionSource get_checker_option_source(
        const std::string& option) const override;

    void set_checker_option(const std::string& option,
                            CheckerOptVal value) override;

}; // class HunterOptionsCommandLineProvider

// TODO(config-file): add config file support
class HunterOptionsConfigFileProvider
    : public HunterOptionsCommandLineProvider {
  public:
    explicit HunterOptionsConfigFileProvider(std::string config_file);
};

} // namespace hunter