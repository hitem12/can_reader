from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMake


class NetlearnConan(ConanFile):
    name = "can_reader"
    version = "0.1.0"
    description = "CAN reader"
    author = "Tomasz Pniewski-Przygodzki <t.pniewski.przygodzki@gmail.com>"
    license = "MIT"
    url = "https://github.com/hitem12/can_reader"
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }
    
    # Build requirements for Conan 2.0
    tool_requires = []
    
    # Runtime dependencies
    requires = (
        "cli11/2.6.0",
        "libpcap/1.10.5",
        "spdlog/1.17.0",
    )
    
    # Build/Test dependencies
    test_requires = (
        "gtest/1.17.0",
    )
    
    # Generators for CMake integration
    generators = "CMakeDeps"
    
    def config_options(self):
        """Configure options based on OS"""
        pass

    def configure(self):
        """Configure build settings"""
        pass

    def layout(self):
        """Define folder layout"""
        cmake_layout(self)
        bt = str(self.settings.build_type).lower()
        platform = self.conf.get("user.can_reader:platform", default="host")
        suffix = "" if platform == "host" else f"-{platform}"
        self.folders.build      = f"cmake-build-{bt}"
        self.folders.generators = f"cmake-build-{bt}"
    def generate(self):
        """Generate build files"""

        tc = CMakeToolchain(self)
        
        # C++23 configuration
        tc.variables["CMAKE_CXX_STANDARD"] = "17"
        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        tc.variables["CMAKE_CXX_EXTENSIONS"] = "OFF"
        
        # Optimization flags for Release builds
        if self.settings.build_type == "Release":
            tc.variables["CMAKE_CXX_FLAGS_RELEASE"] = "-O3 -DNDEBUG"
        
        # Enable testing support
        tc.variables["ENABLE_TESTING"] = "ON"
        
        tc.generate()

def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()