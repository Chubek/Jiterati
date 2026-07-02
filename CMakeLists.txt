cmake_minimum_required(VERSION 3.16)
project(Jiterati VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(JITERATI_BUILD_TESTS "Build Jiterati tests" ON)
option(JITERATI_BUILD_EXAMPLES "Build Jiterati examples" ON)
option(JITERATI_BUILD_LUA "Build Lua/Sol2 bindings" OFF)
option(JITERATI_BUILD_DOCS "Enable the documentation target" ON)

# Public headers
set(JITERATI_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Jiterati.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Jiterati-BE.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Jiterati-Macro.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Jiterati-Pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Jiterati-Plugin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DSLtk.hpp
)

# Backend sources
set(JITERATI_BACKEND_SOURCES
    src/JIT.cpp
    BE/Metadata.cpp
    BE/AMD64/AMD64.cpp
    BE/AMD64/Backend.cpp
    BE/AMD64/ISel.cpp
    BE/AMD64/RegAlloc.cpp
    BE/AMD64/Rewrite.cpp
    BE/AMD64/Peephole.cpp
    BE/AMD64/Emit.cpp
    BE/AMD64/IRCompiler.cpp
    BE/AArch64/AArch64.cpp
    BE/AArch64/ISel.cpp
    BE/AArch64/RegAlloc.cpp
    BE/AArch64/Rewrite.cpp
    BE/AArch64/Peephole.cpp
    BE/AArch64/Emit.cpp
    BE/AArch64/IRCompiler.cpp
    BE/RV64/RV64.cpp
    BE/RV64/ISel.cpp
    BE/RV64/RegAlloc.cpp
    BE/RV64/Rewrite.cpp
    BE/RV64/Peephole.cpp
    BE/RV64/Emit.cpp
    BE/RV64/IRCompiler.cpp
    BE/WASM/WASM.cpp
    BE/WASM/ISel.cpp
    BE/WASM/RegAlloc.cpp
    BE/WASM/Rewrite.cpp
    BE/WASM/Peephole.cpp
    BE/WASM/Emit.cpp
    BE/WASM/IRCompiler.cpp
    IR/AST.cpp
    IR/TAC.cpp
    IR/Terse.cpp
    IR/Analyze.cpp
    IR/Rewrite.cpp
    IR/Peephole.cpp
    IR/Parse.cpp
)

# Pass library sources
set(JITERATI_PASS_SOURCES
    Passlib/CFGAnalysis.cpp
    Passlib/DominatorAnalysis.cpp
    Passlib/LivenessAnalysis.cpp
    Passlib/ConstantFolding.cpp
    Passlib/ConstantPropagation.cpp
    Passlib/DeadCodeRemoval.cpp
    Passlib/StrengthReduction.cpp
    Passlib/PeepholeOptimization.cpp
)

# Plugin sources
set(JITERATI_PLUGIN_SOURCES
    Plugins/BURSISel.cpp
    Plugins/MaxMunchISel.cpp
    Plugins/ListISched.cpp
)

# DSL / JBL / JPL
set(JITERATI_DSL_SOURCES
    src/Jiterati-JBL.cpp
    src/Jiterati-JPL.cpp
)

# Lua bindings
set(JITERATI_LUA_SOURCES
    Lua/JiteratiBinding.cpp
)

# Core library: backends + passes + plugins + DSLs.
add_library(jiterati STATIC
    ${JITERATI_BACKEND_SOURCES}
    ${JITERATI_PASS_SOURCES}
    ${JITERATI_PLUGIN_SOURCES}
    ${JITERATI_DSL_SOURCES}
)

target_include_directories(jiterati PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

if(JITERATI_BUILD_LUA)
    find_package(Lua REQUIRED)
    if(LUA_VERSION_STRING VERSION_GREATER_EQUAL "5.5")
        message(FATAL_ERROR "JITERATI_BUILD_LUA requires Lua 5.1 through 5.4 because sol2 does not support Lua ${LUA_VERSION_STRING}")
    endif()
    find_package(sol2 QUIET)
    if(NOT sol2_FOUND)
        message(STATUS "sol2 not found via find_package; using bundled header path if present")
        target_include_directories(jiterati PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol2/include)
    endif()
    target_sources(jiterati PRIVATE ${JITERATI_LUA_SOURCES})
    target_link_libraries(jiterati PUBLIC ${LUA_LIBRARIES})
    target_include_directories(jiterati PUBLIC ${LUA_INCLUDE_DIR})
    target_compile_definitions(jiterati PUBLIC JITERATI_HAS_LUA=1)
endif()

if(JITERATI_BUILD_EXAMPLES)
    add_executable(jiterati_square examples/square.cpp)
    target_link_libraries(jiterati_square PRIVATE jiterati)

    add_executable(jiterati_abs examples/abs.cpp)
    target_link_libraries(jiterati_abs PRIVATE jiterati)

    file(GLOB JITERATI_API_EXAMPLES CONFIGURE_DEPENDS examples/API/*.cpp)
    foreach(example_source IN LISTS JITERATI_API_EXAMPLES)
        get_filename_component(example_name ${example_source} NAME_WE)
        set(example_target jiterati_api_${example_name})
        add_executable(${example_target} ${example_source})
        target_link_libraries(${example_target} PRIVATE jiterati)
    endforeach()
endif()

add_executable(jiterati-cli src/jiterati-cli.cpp)
target_link_libraries(jiterati-cli PRIVATE jiterati)

if(JITERATI_BUILD_TESTS)
    enable_testing()

    add_executable(jiterati_test_core tests/test_core.cpp)
    target_link_libraries(jiterati_test_core PRIVATE jiterati)
    add_test(NAME core COMMAND jiterati_test_core)

    add_executable(jiterati_test_passes tests/test_passes.cpp)
    target_link_libraries(jiterati_test_passes PRIVATE jiterati)
    add_test(NAME passes COMMAND jiterati_test_passes)

    add_executable(jiterati_test_jbl tests/test_jbl.cpp)
    target_link_libraries(jiterati_test_jbl PRIVATE jiterati)
    add_test(NAME jbl COMMAND jiterati_test_jbl)

    add_executable(jiterati_test_plugins tests/test_plugins.cpp)
    target_link_libraries(jiterati_test_plugins PRIVATE jiterati)
    add_test(NAME plugins COMMAND jiterati_test_plugins)

    add_executable(jiterati_test_backend_metadata tests/test_backend_metadata.cpp)
    target_link_libraries(jiterati_test_backend_metadata PRIVATE jiterati)
    add_test(NAME backend_metadata COMMAND jiterati_test_backend_metadata)

    if(JITERATI_BUILD_LUA)
        add_executable(jiterati_test_lua tests/test_lua.cpp)
        target_link_libraries(jiterati_test_lua PRIVATE jiterati)
        add_test(NAME lua COMMAND jiterati_test_lua)
    endif()

    add_test(NAME cli_help COMMAND jiterati-cli help)
    add_test(NAME cli_parse COMMAND jiterati-cli parse ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/valid.terse)
    add_test(NAME cli_validate COMMAND jiterati-cli validate ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/valid.terse)
    add_test(NAME cli_format COMMAND jiterati-cli format ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/valid.terse)
    add_test(NAME cli_compile COMMAND jiterati-cli compile --target amd64 --emit asm ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/valid.terse)
    add_test(NAME cli_package_pack COMMAND jiterati-cli package --pack ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/package/package.sh --out ${CMAKE_CURRENT_BINARY_DIR}/cli-fixture.jpkg)
    add_test(NAME cli_package_validate COMMAND jiterati-cli package --validate ${CMAKE_CURRENT_BINARY_DIR}/cli-fixture.jpkg)
    add_test(NAME cli_package_manifest COMMAND jiterati-cli package --view-manifest ${CMAKE_CURRENT_BINARY_DIR}/cli-fixture.jpkg)
    add_test(NAME cli_package_list_plugins COMMAND jiterati-cli package --list-plugins ${CMAKE_CURRENT_BINARY_DIR}/cli-fixture.jpkg)
    add_test(NAME cli_package_malformed COMMAND jiterati-cli package --validate ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/malformed/missing-name.jpkg)
    add_test(NAME cli_package_pack_rejects_undeclared_file COMMAND jiterati-cli package --pack ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/package-extra/package.sh --out ${CMAKE_CURRENT_BINARY_DIR}/bad-extra.jpkg)
    add_test(NAME cli_package_install COMMAND ${CMAKE_COMMAND} -E env JITERATI_HOME=${CMAKE_CURRENT_BINARY_DIR}/jiterati-home $<TARGET_FILE:jiterati-cli> package --install ${CMAKE_CURRENT_BINARY_DIR}/cli-fixture.jpkg)
    add_test(NAME cli_package_verify COMMAND ${CMAKE_COMMAND} -E env JITERATI_HOME=${CMAKE_CURRENT_BINARY_DIR}/jiterati-home $<TARGET_FILE:jiterati-cli> package --verify cli-fixture)
    add_test(NAME cli_package_remove COMMAND ${CMAKE_COMMAND} -E env JITERATI_HOME=${CMAKE_CURRENT_BINARY_DIR}/jiterati-home $<TARGET_FILE:jiterati-cli> package --remove cli-fixture)
    set_tests_properties(cli_package_validate cli_package_manifest cli_package_list_plugins PROPERTIES DEPENDS cli_package_pack)
    set_tests_properties(cli_package_malformed cli_package_pack_rejects_undeclared_file PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(cli_package_install PROPERTIES DEPENDS cli_package_pack)
    set_tests_properties(cli_package_verify PROPERTIES DEPENDS cli_package_install)
    set_tests_properties(cli_package_remove PROPERTIES DEPENDS cli_package_verify)

    file(GLOB JITERATI_IR_EXAMPLES CONFIGURE_DEPENDS examples/IR/*.ir)
    foreach(example_ir IN LISTS JITERATI_IR_EXAMPLES)
        get_filename_component(example_name ${example_ir} NAME_WE)
        add_test(NAME ir_example_${example_name} COMMAND jiterati-cli validate ${example_ir})
    endforeach()
endif()

if(JITERATI_BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Install rules
install(FILES ${JITERATI_PUBLIC_HEADERS} DESTINATION include/jiterati)
install(TARGETS jiterati ARCHIVE DESTINATION lib)
install(TARGETS jiterati-cli RUNTIME DESTINATION bin)
