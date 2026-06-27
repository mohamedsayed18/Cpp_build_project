from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain


class ChallengeProjectConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = (
        "boost/1.86.0",
        "gtest/1.15.0",
    )
    default_options = {
        "boost/*:shared": True,
        "gtest/*:shared": True,
    }

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = "build/generators"

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.user_presets_path = False
        toolchain.generate()

        deps = CMakeDeps(self)
        deps.generate()

        from conan.tools.env import VirtualRunEnv
        runenv = VirtualRunEnv(self)
        runenv.generate()