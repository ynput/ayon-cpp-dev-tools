import os
import shutil
from ext.ayon_automator.AyonCiCd import Project, Cmake, GTest, GBench


PRJ_ROOT_PATH = os.path.dirname(__file__)

Ayon_Cpp_Tools = Project.Project("AyonCppDevTools")
Ayon_Cpp_Tools.setVar("enable_testing", "False")
Ayon_Cpp_Tools.setVar("enable_benching", "False")

Ayon_Cpp_Tools.add_cmd_arg("--EnableLogger", action="store_true")

Ayon_Cpp_Tools.add_cmd_arg(
    "--BuildType",
    action="store_true",
    default="Release",
    help="set the CMake build type as a string {Debug, Release}",
)

cmd = Ayon_Cpp_Tools.setup_prj()


setup_enable_testing = Project.Stage("EnableTesting")
setup_enable_testing.add_funcs(
    Project.Func(
        "Set Build type to Debug",
        Ayon_Cpp_Tools.setVar,
        "enable_testing",
        "True",
    ),
)
Ayon_Cpp_Tools.add_stage(setup_enable_testing)

setup_enable_bench = Project.Stage("EnableBenching")
setup_enable_bench.add_funcs(
    Project.Func(
        "Set Build type to Debug",
        Ayon_Cpp_Tools.setVar,
        "enable_benching",
        "True",
    ),
)
Ayon_Cpp_Tools.add_stage(setup_enable_bench)


clean_stage = Project.Stage("CleanPrj")
clean_stage.add_funcs(
    Project.Func(
        "rm build foulder",
        shutil.rmtree,
        os.path.join(os.path.dirname(__file__), "build"),
        ignore_errors=True,
    ),
    Project.Func(
        "rm bin foulder",
        shutil.rmtree,
        os.path.join(os.path.dirname(__file__), "bin"),
        ignore_errors=True,
    ),
)
Ayon_Cpp_Tools.add_stage(clean_stage)


build_stage = Project.Stage("BuildLogger")
build_stage.add_funcs(
    Project.Func(
        "Cmake-Configure",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        None,
        ".",
        "-B",
        "build",
        lambda: f"-D CMAKE_BUILD_TYPE={cmd.BuildType}",
        lambda: f"-DAYON_CPPTOOLS_ENABLE_TESTING={Ayon_Cpp_Tools.getVar('enable_testing')}",
        lambda: f"-DAYON_CPPTOOLS_ENABLE_BENCH={Ayon_Cpp_Tools.getVar('enable_benching')}",
        lambda: ("-DAYON_CPPTOOLS_BUILD_LOGGER=1" if cmd.EnableLogger else ""),
        "-DAYON_INSTRUMENT=1",
    ),
    Project.Func(
        "Cmake-Build",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        None,
        "--build",
        "build",
        "--config",
        lambda: cmd.BuildType,
    ),
    Project.Func(
        "Cmake-Install",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        None,
        "--install",
        "build",
        "--config",
        lambda: cmd.BuildType,
    ),
)
Ayon_Cpp_Tools.add_stage(build_stage)


def dell_tests_out():
    test_out_f = os.path.join(
        PRJ_ROOT_PATH,
        "tests",
        "out",
    )
    if os.path.exists(test_out_f):
        shutil.rmtree(test_out_f)


g_test = Project.Stage("GTest")
g_test.add_funcs(
    Project.Func(
        "Run GTest",
        GTest.run_google_test,
        os.path.join(PRJ_ROOT_PATH, "bin", "AyonCppToolsTest"),
        os.path.join(PRJ_ROOT_PATH, "build", "GTest.xml"),
        Ayon_Cpp_Tools,
        None,
    ),
    Project.Func(
        "Delet Tests Out Foulder",
        dell_tests_out,
    ),
)
g_test.addArtefactFolder(os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogTest.json"))
Ayon_Cpp_Tools.add_stage(g_test)

g_bench = Project.Stage("GBench")
g_bench.add_funcs(
    Project.Func(
        "Run GBench",
        GBench.run_google_benchmark,
        os.path.join(PRJ_ROOT_PATH, "bin", "AyonCppToolsBench"),
        os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogBench.json"),
        "ns",
        None,
    ),
    Project.Func(
        "Delet Tests Out Foulder",
        dell_tests_out,
    ),
)
g_bench.addArtefactFolder(os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogTest.json"))
Ayon_Cpp_Tools.add_stage(g_bench)


Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuild",
    clean_stage,
    build_stage,
)

Ayon_Cpp_Tools.creat_stage_group(
    "BuildTest",
    setup_enable_testing,
    build_stage,
    g_test,
)

Ayon_Cpp_Tools.creat_stage_group(
    "BuildBench",
    setup_enable_testing,
    build_stage,
    g_bench,
)

Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuildTest",
    clean_stage,
    setup_enable_testing,
    build_stage,
    g_test,
)

Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuildBench",
    clean_stage,
    setup_enable_testing,
    build_stage,
    g_bench,
)


with Ayon_Cpp_Tools as PRJ:
    PRJ.make_project_cli_available()
