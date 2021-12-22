from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class Glpp(ConanFile):
    name = "glpp"
    version = "0.2.0"
    description = "OpenGL/GLFW C++ wrapper with ImGui integration"
    homepage = "https://github.com/MiSo1289/glpp"
    url = "https://github.com/MiSo1289/glpp"
    license = "MIT"
    revision_mode = "scm"
    settings = ("os", "compiler", "build_type", "arch")
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_glfw": [True, False],
        "with_config": [True, False],
        "with_imgui": [True, False],
        "with_examples": [True, False],
    }
    default_options = {
        "fPIC": True,
        "with_glfw": True,
        "with_config": True,
        "with_imgui": True,
        "with_examples": True,
        "glad:gl_version": "4.5",
    }
    exports_sources = (
        "src/*",
        "examples/*",
        "CMakeLists.txt",
    )
    requires = (
        "fmt/7.1.3",
        "glad/0.1.33",
        "glm/0.9.9.8",
        "magic_enum/0.7.3",
        "ms-gsl/3.0.1",
        "opengl/system",
    )

    def requirements(self):
        if self.options.with_glfw:
            self.requires("boost/1.77.0")
            self.requires("glfw/3.3.2")

        if self.options.with_config:
            self.requires("nlohmann_json/3.9.1")

        if self.options.with_imgui:
            self.requires("imgui/1.77")
    
        if self.options.with_examples:
            self.requires("lyra/1.5.1")

    def imports(self):
        self.copy("*.so*", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

        if self.options.with_imgui:
            self.copy("imgui_impl_glfw.*", dst="bindings", src="res/bindings")
            self.copy("imgui_impl_opengl3.*", dst="bindings", src="res/bindings")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["BUILD_EXAMPLES"] = self.options.with_examples
        tc.variables["BUILD_CONFIG"] = self.options.with_config
        tc.variables["BUILD_GLFW"] = self.options.with_glfw
        tc.variables["BUILD_IMGUI"] = self.options.with_imgui
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_id(self):
        del self.info.options.with_examples

    def package_info(self):
        self.cpp_info.components["core"].libs = ["glpp_core"]
        self.cpp_info.components["core"].requires = [
            "fmt::fmt",
            "glad::glad",
            "glm::glm",
            "opengl::opengl",
            "magic_enum::magic_enum",
            "ms-gsl::ms-gsl",
        ]
        
        if self.options.with_glfw:
            self.cpp_info.components["glfw"].libs = ["glpp_glfw"]
            self.cpp_info.components["glfw"].requires = ["boost::boost", "glfw::glfw", "core"]

        if self.options.with_config:
            self.cpp_info.components["config"].libs = ["glpp_config"]
            self.cpp_info.components["config"].requires = ["nlohmann_json::nlohmann_json", "core"]

        if self.options.with_imgui:
            self.cpp_info.components["imgui"].libs = ["glpp_imgui"]
            self.cpp_info.components["imgui"].requires = ["imgui::imgui", "core", "glfw"]

        if self.options.with_examples:
            self.cpp_info.components["examples"].requires = ["lyra::lyra"]
