from ext.ayon_automator.AyonCiCd import Project
from ext.ayon_automator.AyonCiCd import Cmake

raise ImportError(
    "implement build logger stage implement normal build stage implement test stage"
)
Ayon_Cpp_Tools = Project.Project("AyonCppDevTools")
# "--config",
# "Debug",
# "--clean-first",


def build():
    Cmake.cmake_command(
        Ayon_Cpp_Tools,
        ".",
        "-B",
        "build",
        # "-DAYON_CPPTOOLS_ENABLE_TESTING=true",
        "-DAYON_CPPTOOLS_BUILD_LOGGER=true",
    )
    Cmake.cmake_command(
        Ayon_Cpp_Tools,
        "--build",
        "build",
    )
    Cmake.cmake_command(Ayon_Cpp_Tools, "--install", "build")


build_test_stage = Project.Stage("BuildTest")
build_test_stage.add_funcs(build)
Ayon_Cpp_Tools.add_stage(build_test_stage)

with Ayon_Cpp_Tools as PRJ:
    PRJ.make_project_cli_available()
