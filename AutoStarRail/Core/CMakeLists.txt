add_library(AsrCoreObjects OBJECT)
target_compile_options(AsrCoreObjects PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /MP>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
set_property(TARGET AsrCoreObjects PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(AsrCoreObjects PRIVATE -DASR_BUILD_SHARED)
target_link_libraries(AsrCoreObjects PUBLIC
    Boost::filesystem
    ICU::data
    ICU::i18n
    ICU::uc)

if(ASR_USE_BUNDLED_BOOST)
    target_link_libraries(AsrCoreObjects PUBLIC ${ASR_BUNDLED_BOOST_LIBS})
else()
    # 对于使用msys包管理器安装boost的用户，使用msys的clangd或使用这个语句强制将Boost_INCLUDE_DIRS添加到compile_commands.json
    # target_compile_options(AsrCoreObjects PRIVATE "-I${Boost_INCLUDE_DIRS}")
    target_include_directories(AsrCoreObjects PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(AsrCoreObjects PRIVATE ${Boost_LIBRARIES})
endif()

if(WIN32)
    target_compile_definitions(AsrCoreObjects PRIVATE ASR_PLATFORM="WINDOWS" ASR_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_compile_definitions(AsrCoreObjects PRIVATE ASR_PLATFORM="MACOS")
elseif(UNIX)
    target_compile_definitions(AsrCoreObjects PRIVATE ASR_PLATFORM="LINUX")
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

asr_add_core_component(Utils)
asr_add_core_component(OcvWrapper)
asr_add_core_component(OrtWrapper)
asr_add_core_component(ForeignInterfaceHost)
asr_add_core_component(GlobalSettingsManager)
asr_add_core_component(Logger)
asr_add_core_component(Exceptions)
asr_add_core_component(i18n)
asr_add_core_component(TaskScheduler)

add_library(AsrCore SHARED)
add_dependencies(AsrCore AsrCoreObjects)

add_compile_definitions(ASR_BUILD_SHARED)

# 配置导出库
set(ASR_EXPORT_MACRO_LIST "")
set(ASR_FOREIGN_LANGUAGE_LIB_LIST "")

cmake_policy(SET CMP0078 NEW)

file(GLOB ASR_SWIG_FILES ${CMAKE_SOURCE_DIR}/SWIG/*.i)
set_property(SOURCE ${ASR_SWIG_FILES} PROPERTY CPLUSPLUS ON)
set_source_files_properties(${ASR_SWIG_FILES} PROPERTIES USE_SWIG_DEPENDENCIES TRUE)

if(${EXPORT_CSHARP})
    asr_add_swig_export_library(CSharp AsrCoreCSharpExport "${ASR_SWIG_FILES}")
    target_link_libraries(AsrCoreCSharpExport PUBLIC AsrCore)
    add_dependencies(AsrCoreCSharpExport AsrCore)
    list(APPEND ASR_EXPORT_MACRO_LIST ASR_EXPORT_CSHARP)
endif()

if(${EXPORT_JAVA})
    find_package(JNI REQUIRED)
    asr_add_swig_export_library(Java AsrCoreJavaExport "${ASR_SWIG_FILES}")
    set_property(TARGET AsrCoreJavaExport PROPERTY SWIG_COMPILE_OPTIONS -package org.asr)

    target_link_libraries(AsrCoreObjects PUBLIC JNI::JNI)

    target_link_libraries(AsrCoreJavaExport PUBLIC AsrCore)
    add_dependencies(AsrCoreJavaExport AsrCore)
    list(APPEND ASR_EXPORT_MACRO_LIST ASR_EXPORT_JAVA)
endif()

if(${EXPORT_PYTHON})
    find_package(Python3 COMPONENTS Development REQUIRED)
    asr_add_swig_export_library(Python AsrCorePythonExport "${ASR_SWIG_FILES}")
    set_property(TARGET AsrCorePythonExport PROPERTY SWIG_COMPILE_OPTIONS -threads)

    target_include_directories(AsrCoreObjects PUBLIC ${Python3_INCLUDE_DIRS})
    target_link_libraries(AsrCoreObjects PUBLIC ${Python3_LIBRARIES})

    target_link_libraries(AsrCorePythonExport PUBLIC AsrCore)
    add_dependencies(AsrCorePythonExport AsrCore)
    list(APPEND ASR_EXPORT_MACRO_LIST ASR_EXPORT_PYTHON)
endif()

# FetchContent_Declare(
# Sol2
# URL ${GITHUB_MIRROR_URL}/ThePhD/sol2/archive/refs/tags/v3.3.0.zip
# )
# FetchContent_MakeAvailable(Sol2)
# target_link_libraries(AsrCoreObjects PUBLIC sol2::sol2)
# find_package(Lua 5.4)

# if(NOT ${LUA_FOUND})
# message(STATUS "Downloading and configuring Lua 5.4.6.")
# FetchContent_Declare(
# AsrLua
# URL ${GITHUB_MIRROR_URL}/lua/lua/archive/refs/tags/v5.4.6.zip
# PATCH_COMMAND ${CMAKE_COMMAND} -E copy
# "${CMAKE_SOURCE_DIR}/cmake/CompileLua.cmake" <SOURCE_DIR>/CMakeLists.txt
# )
# FetchContent_MakeAvailable(AsrLua)
# target_link_libraries(AsrCoreObjects PUBLIC Asr::Lua)
# else()
# target_include_directories(AsrCoreObjects PRIVATE ${LUA_INCLUDE_DIR})
# target_link_libraries(AsrCoreObjects PUBLIC ${LUA_LIBRARIES})
# endif()

# list(APPEND ASR_EXPORT_MACRO_LIST ASR_EXPORT_LUA)
target_link_libraries(AsrCoreObjects
    PUBLIC AsrUtils ${ASR_FOREIGN_LANGUAGE_LIB_LIST})
target_compile_definitions(AsrCoreObjects PUBLIC ${ASR_EXPORT_MACRO_LIST})

target_link_libraries(AsrCore PUBLIC AsrCoreObjects)

if(${ASR_BUILD_TEST})
    asr_add_core_test(ForeignInterfaceHost)
    asr_add_core_test(i18n)
endif()
