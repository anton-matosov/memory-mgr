from conan import ConanFile
from conan.tools.files import copy
from conan.tools.build import check_min_cppstd
from pathlib import Path

class MemoryMgr(ConanFile):
  name = "memory-mgr"
  version = "1.4.0"
  package_type = "header-library"
  # No settings/options are necessary, this is header only
  no_copy_source = True
  exports_sources = "memory-mgr/*"

  generators = "CMakeToolchain"

  def requirements(self):
    self.requires("boost/1.85.0", options={
      "header_only": True,
    })
    # Use conan only for library dependencies.
    # Use conda for all tool packages instead, this allows to install compiler as well.
    # self.tool_requires("cmake/[>=3.30]")
    # self.tool_requires("ninja/[>=1.12]")

  # def validate(self):
  #   check_min_cppstd(self, 14, gnu_extensions=False)

  def package_info(self):
    # For header-only packages, libdirs and bindirs are not used
    # so it's necessary to set those as empty.
    self.cpp_info.bindirs = []
    self.cpp_info.libdirs = []

  def package(self):
    copy(self, "*.[h,hpp]", Path(self.source_folder) / "memory-mgr", self.package_folder)  # Adjust src path as necessary
