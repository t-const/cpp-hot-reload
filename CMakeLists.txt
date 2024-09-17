cmake_minimum_required(VERSION 3.10)
project(HotReload VERSION 1.0 LANGUAGES CXX)
SET(COMPONENT_NAME ${PROJECT_NAME})

MESSAGE(STATUS "Creating project ${PROJECT_NAME} using component name ${COMPONENT_NAME}")

include(cmake/BuildSetup.cmake)
include(cmake/TargetFunctions.cmake)

# Permit better projects organization in generated solution file.
# Will produce "CMakePredefinedTargets" and "CTestDashboardTargets" folder in generated solution
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Setup compiler options and build folders
SetupBuildSettings()

# Create the main application library
set(libraryName ${PROJECT_NAME}-lib)

CreateLibrary(
   ${libraryName}
   true
   "${CMAKE_CURRENT_SOURCE_DIR}/include/"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)
target_link_libraries(
   ${libraryName}
   PRIVATE
   dl
)

set(testLibrary HotReloadTest)
CreateLibrary(
   ${testLibrary}
   true
   "${CMAKE_CURRENT_SOURCE_DIR}/HotReloadTest/"
   "${CMAKE_CURRENT_SOURCE_DIR}/HotReloadTest/"
)

# Manage project dependencies
target_link_libraries(
    ${testLibrary}
    PUBLIC
    ${libraryName}
)

# Create executable
set(app ${PROJECT_NAME})

CreateExecutable(
   ${app}
   "${CMAKE_CURRENT_SOURCE_DIR}/app/src/"
   "${CMAKE_CURRENT_SOURCE_DIR}/app/include/"
   )
target_link_libraries(${app} PRIVATE dl)
target_compile_definitions(${app} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Manage project dependencies
target_link_libraries(
    ${app}
    PRIVATE
        ${libraryName}
)

# TODO make a target function to ease that process?
target_include_directories(
   ${app}
   PRIVATE
   "${CMAKE_CURRENT_SOURCE_DIR}/HotReloadTest")

# Add the tests subdirectory
add_subdirectory(tests)