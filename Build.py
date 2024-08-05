from ext.ayon_automator.AyonCiCd import Project


Ayon_Cpp_Tools = Project.Project("AyonCppDevTools")

build_test_stage = Project.Stage("BuildTest")
build_test_stage.add_funcs(print)
