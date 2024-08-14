# Getting Started

## General Info's about the project and Setup.

its build as interface library that organizes into defined namespaces.\
its an header only library but some tools bring there own dependency.\
if an tool brings a dependency its mandatory that this tool can be disabled.

Currently we use Git Submodules but it would be reasonable to implement
dependency's in a CMake file. But it is important that those CMake files live
separated from the main CMake file to allow for scalability without cluttering
the main CMake file.

## Usage

```
add_subdirectory("ayon-cpp-dev-tools")
target_link_libraries(Your_Target_Name AyonCppDevToolsLib)
```

This will make all files under src/lib/ available to your project.

## Developing for this tool set

There are a few things to be aware when Developing code in this repo.

**Ayon_Automator**

we use Ayon_Automator to interact with the tests and the build system. this is
because we want that running those systems is cross platform and easy to
implement into CICD.

**Red-Green_Development**

we run a Red Green Development setup in this repository.

in short for the uninitialized: you should write a test that fails and thereby
proves that the current tool set dose not support your requirement and then you
should start implementing the needed classes and functions to implement the
requested functionality.

All tests are in `tests/` new tests should be implemented in an .hpp file and
included into the main GTest or GBench file. then you can create a stage in the
Build.py to make it available to the Ayon_Automator tool set. Selecting tests
should always be done via test filter and not by building a separate GTest
program for the test alone.
