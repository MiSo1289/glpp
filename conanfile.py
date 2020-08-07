from conans import CMake, CMakeToolchain, ConanFile, tools


class Glpp(ConanFile):
    name = "glpp"
    version = "0.1.0"
    revision_mode = "scm"
    settings = ("os", "compiler", "build_type", "arch")
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "glfw": [True, False],
        "config": [True, False],
        "imgui": [True, False],
        "examples": [True, False],
    }
    default_options = {
        "fPIC": True,
        "glfw": True,
        "config": True,
        "imgui": True,
        "examples": True,
        "glad:gl_version": "4.5",
    }
    generators = "cmake_find_package"
    exports_sources = (
        "src/*",
        "examples/*",
        "CMakeLists.txt",
    )
    requires = (
        "fmt/7.0.1",
        "glad/0.1.33",
        "glm/0.9.9.8",
        "magic_enum/0.6.6",
        "ms-gsl/3.0.1",
    )

    def requirements(self):
        if self.options.glfw:
            self.requires("boost/1.73.0")
            self.requires("glfw/3.3.2")

        if self.options.config:
            self.requires("nlohmann_json/3.9.0")

        if self.options.imgui:
            self.requires("imgui/1.77")

        if self.options.examples:
            self.requires("clara/1.1.5")

    def imports(self):
        # Collect shared libraries
        self.copy("*.so*", dst="lib", keep_path=False)

        if self.options.imgui:
            # Collect ImGui bindings
            self.copy("imgui_impl_glfw.*", dst="bindings", src="res/bindings")
            self.copy("imgui_impl_opengl3.*", dst="bindings", src="res/bindings")

    def toolchain(self):
        tc = CMakeToolchain(self)

        tc.definitions["USING_CONAN"] = True
        tc.definitions["BUILD_GLFW"] = self.options.glfw
        tc.definitions["BUILD_CONFIG"] = self.options.config
        tc.definitions["BUILD_EXAMPLES"] = self.options.examples

        tc.write_toolchain_files()

    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.build()

    def package(self):
        header_excludes = []

        if not self.options.glfw:
            header_excludes.append("glpp/glfw/*")
        if not self.options.config:
            header_excludes.append("glpp/config/*")

        self.copy("*.hpp", dst="include", src="src", excludes=tuple(header_excludes))
        self.copy("*.so*", dst="lib", keep_path=False, symlinks=True)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_id(self):
        # Does not impact the resulting package, exclude from package id
        del self.info.options.examples

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
