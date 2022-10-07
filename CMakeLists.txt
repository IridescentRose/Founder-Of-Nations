cmake_minimum_required(VERSION 3.17)
project(PSPVille)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    add_compile_definitions(BUILD_PLAT=0)
elseif(PSP)
    add_compile_definitions(BUILD_PLAT=2)
elseif(VITA)
    add_compile_definitions(BUILD_PLAT=3)
else()
    add_compile_definitions(BUILD_PLAT=1)
endif()

add_subdirectory(ext/Stardust-Celeste Stardust)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE INC_FILES include/*.hpp)

add_executable(PSPVille ${SRC_FILES} ${INC_FILES})
target_include_directories(PSPVille PUBLIC ext/Stardust-Celeste/ext/glm/glm/)
target_link_libraries(PSPVille SC-Entry Stardust-Celeste yaml-cpp)

if(WIN32)
    target_link_libraries(PSPVille ws2_32.lib)
else()
    target_compile_options(PSPVille PRIVATE -g)
endif()

if(PSP)
    create_pbp_file(
        TARGET PSPVille
        TITLE PSPVille
#        ICON_PATH TODO
#        BACKGROUND_PATH TODO
        BUILD_PRX 1
    )
endif()