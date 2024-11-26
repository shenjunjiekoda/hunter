#include "analyzer/tooling/options.hpp"
#include "common/util/assert.hpp"

#include <llvm/Support/Process.h>

namespace hunter {

const char* option_src_to_string(OptionSource source) {
    switch (source) {
        case OptionSource::Default:
            return "Default";
        case OptionSource::CommandLine:
            return "CommandLine";
        case OptionSource::ConfigFile:
            return "ConfigFile";
    }
    hunter_unreachable("invalid option source"); // NOLINT
}

HunterOptionsDefaultProvider::HunterOptionsDefaultProvider() {
    if (auto user_opt = llvm::sys::Process::GetEnv("USER")) {
        options.user = *user_opt;
    } else if (auto user_opt = llvm::sys::Process::GetEnv("USERNAME")) {
        options.user = *user_opt;
    }

    set_default_options();
}

OptionSource HunterOptionsDefaultProvider::get_checker_option_source(
    [[maybe_unused]] const std::string& option) const {
    return OptionSource::Default;
}

void HunterOptionsDefaultProvider::set_default_options() {
    // Set default checker options here.
}

void HunterOptionsDefaultProvider::set_checker_option(const std::string& option,
                                                      CheckerOptVal value) {
    options.check_opts[option] = value;
}

HunterOptions HunterOptionsDefaultProvider::get_options_for(
    [[maybe_unused]] const std::string& file) const {
    return options;
}

OptionSource HunterOptionsCommandLineProvider::get_checker_option_source(
    const std::string& option) const {
    if (m_cmd_override_opts.contains(option)) {
        return OptionSource::CommandLine;
    }
    return OptionSource::Default;
}

void HunterOptionsCommandLineProvider::set_checker_option(
    const std::string& option, CheckerOptVal value) {
    options.check_opts[option] = value;
    m_cmd_override_opts.insert(option);
}

HunterOptionsConfigFileProvider::HunterOptionsConfigFileProvider(
    [[maybe_unused]] std::string config_file) { // NOLINT
    // TODO(config): Implement this.
}

} // namespace hunter