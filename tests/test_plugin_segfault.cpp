#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

std::string quote(const std::filesystem::path& p) {
    return std::string("\"") + p.string() + "\"";
}

TEST(PluginSegfaultTest, CrashProducesStacktraceFile) {
    const char* probe_arg = std::getenv("SEGFAULT_PROBE_PATH");
    const char* plugin_arg = std::getenv("SEGFAULT_PLUGIN_PATH");

    ASSERT_NE(probe_arg, nullptr) << "SEGFAULT_PROBE_PATH not provided";
    ASSERT_NE(plugin_arg, nullptr) << "SEGFAULT_PLUGIN_PATH not provided";

    const std::filesystem::path probe_path = probe_arg;
    const std::filesystem::path plugin_path = plugin_arg;
    const std::filesystem::path stacktrace_path =
        std::filesystem::temp_directory_path() / "plugin_segfault_stacktrace.txt";

    std::error_code ec;
    std::filesystem::remove(stacktrace_path, ec);

    const std::string command =
        quote(probe_path) + " " + quote(plugin_path) + " " + quote(stacktrace_path);

    const int rc = std::system(command.c_str());
    EXPECT_NE(rc, 0) << "Probe should crash with non-zero exit code";

    ASSERT_TRUE(std::filesystem::exists(stacktrace_path))
        << "Stacktrace file was not created";

    std::ifstream input(stacktrace_path);
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("frames="), std::string::npos)
        << "Stacktrace file does not contain frame information";
}

} // namespace