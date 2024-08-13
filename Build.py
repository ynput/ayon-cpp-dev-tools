from logging import log
import os
import shutil
from ext.ayon_automator.AyonCiCd import Project, Cmake, GTest, GBench


PRJ_ROOT_PATH = os.path.dirname(__file__)
# raise ImportError(
#     "implement build logger stage implement normal build stage implement test stage"
# )
Ayon_Cpp_Tools = Project.Project("AyonCppDevTools")
Ayon_Cpp_Tools.setVar("enable_testing", "False")
Ayon_Cpp_Tools.setVar("enable_benching", "False")

setup_build_dev = Project.Stage("SetupDevBuild")
setup_build_dev.add_funcs(
    Project.Func(
        "Set Build type to Debug",
        Ayon_Cpp_Tools.setVar,
        "conf_type",
        "Debug",
    ),
)
Ayon_Cpp_Tools.add_stage(setup_build_dev)

setup_build_release = Project.Stage("SetupReleaseBuild")
setup_build_release.add_funcs(
    Project.Func(
        "Set Build type to Release",
        Ayon_Cpp_Tools.setVar,
        "conf_type",
        "Release",
    ),
)
Ayon_Cpp_Tools.add_stage(setup_build_release)

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

enable_build_logger = Project.Stage("EnalbeBuildLogger")
enable_build_logger.add_funcs(
    Project.Func("Set Enable Logger", Ayon_Cpp_Tools.setVar, "enable_logger", "True")
)
Ayon_Cpp_Tools.add_stage(enable_build_logger)

build_stage = Project.Stage("BuildLogger")
build_stage.add_funcs(
    Project.Func(
        "Cmake-Configure",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        ".",
        "-B",
        "build",
        lambda: f"-D CMAKE_BUILD_TYPE={Ayon_Cpp_Tools.getVar('conf_type')}",
        lambda: f"-DAYON_CPPTOOLS_ENABLE_TESTING={Ayon_Cpp_Tools.getVar('enable_testing')}",
        lambda: f"-DAYON_CPPTOOLS_ENABLE_BENCH={Ayon_Cpp_Tools.getVar('enable_benching')}",
        lambda: f"-DAYON_CPPTOOLS_BUILD_LOGGER={Ayon_Cpp_Tools.getVar('enable_build_logger')}",
    ),
    Project.Func(
        "Cmake-Build",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        "--build",
        "build",
        "--config",
        lambda: Ayon_Cpp_Tools.getVar("conf_type"),
    ),
    Project.Func(
        "Cmake-Install",
        Cmake.cmake_command,
        Ayon_Cpp_Tools,
        "--install",
        "build",
        "--config",
        lambda: Ayon_Cpp_Tools.getVar("conf_type"),
    ),
)
Ayon_Cpp_Tools.add_stage(build_stage)

logger_run_test = Project.Stage("TestLogger")
logger_run_test.add_funcs(
    Project.Func(
        "Run GTest",
        GTest.run_google_test,
        os.path.join(PRJ_ROOT_PATH, "bin", "AyonCppToolsTest"),
        os.path.join(PRJ_ROOT_PATH, "build", "GTest.xml"),
        Ayon_Cpp_Tools,
        "--gtest_filter=AyonLogger*",
    )
)
logger_run_test.addArtefactFoulder(
    os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogTest.json")
)
Ayon_Cpp_Tools.add_stage(logger_run_test)

logger_run_bench = Project.Stage("BenchLogger")
logger_run_bench.add_funcs(
    Project.Func(
        "Run GBench",
        GBench.run_google_benchmark,
        os.path.join(PRJ_ROOT_PATH, "bin", "AyonCppToolsBench"),
        os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogBench.json"),
        "ns",
        "--benchmark_filter=BM_AyonLogger*",
    )
)
logger_run_bench.addArtefactFoulder(
    os.path.join(PRJ_ROOT_PATH, "tests", "out", "LogTest.json")
)
Ayon_Cpp_Tools.add_stage(logger_run_bench)

clean_up_test_results = Project.Stage("Clean_Testing")
clean_up_test_results.add_funcs(
    Project.Func(
        "rm tests/out",
        shutil.rmtree,
        os.path.join(
            PRJ_ROOT_PATH,
            "tests",
            "out",
        ),
    )
)

Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuild",
    clean_stage,
    setup_build_dev,
    setup_enable_testing,
    build_stage,
)

Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuildTestsLogger",
    clean_stage,
    setup_build_dev,
    enable_build_logger,
    setup_enable_testing,
    build_stage,
    logger_run_test,
    clean_up_test_results,
)

Ayon_Cpp_Tools.creat_stage_group(
    "BuildTestsLogger",
    setup_build_dev,
    enable_build_logger,
    setup_enable_testing,
    build_stage,
    logger_run_test,
    clean_up_test_results,
)

Ayon_Cpp_Tools.creat_stage_group(
    "CleanBuildBenchLogger",
    clean_stage,
    setup_build_dev,
    enable_build_logger,
    setup_enable_bench,
    build_stage,
    logger_run_bench,
    clean_up_test_results,
)

Ayon_Cpp_Tools.creat_stage_group(
    "BuildBenchLogger",
    setup_build_dev,
    enable_build_logger,
    setup_enable_bench,
    build_stage,
    logger_run_bench,
    clean_up_test_results,
)

with Ayon_Cpp_Tools as PRJ:
    PRJ.make_project_cli_available()
