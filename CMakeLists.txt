cmake_minimum_required(VERSION 3.15.5)

###### Project informations ######
project(AGHPresenter VERSION 1.0)

###### Conan package manager configuration ######
execute_process(COMMAND "python" "-m" "pip" "install" "conan")

include(cmake-includes/cmake-conan/conan.cmake)

conan_add_remote(NAME vectrom
                 INDEX 0
                 URL https://artifactory.plociennik.net/artifactory/api/conan/conan
                 VERIFY_SSL True)     

conan_add_remote(NAME bincrafters 
                 INDEX 1
                 URL https://api.bintray.com/conan/bincrafters/public-conan
                 VERIFY_SSL True)     

conan_cmake_run(CONANFILE conanfile.txt
                BUILD missing
                CONFIGURATION_TYPES "Release;Debug"
                BASIC_SETUP NO_OUTPUT_DIRS CMAKE_TARGETS KEEP_RPATHS UPDATE
                )

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_BINARY_DIR}")
list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_BINARY_DIR}")

###### Qt configuration ######
find_package(qt REQUIRED CONFIG)
find_package(poppler REQUIRED CONFIG)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core)

###### Build configuration ######
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX and NOT APPLE)
    target_compile_options(${MODULE_NAME} PRIVATE -fPIC)
endif()

###### Add sources ######
add_subdirectory("src")

###### Visual Studio project configuration ######
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

###### CPack configuration ######
if(WIN32)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>/" DESTINATION "." FILES_MATCHING PATTERN "*.exe")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>/" DESTINATION "." FILES_MATCHING PATTERN "*.dll")
elseif(APPLE)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>/" DESTINATION "." USE_SOURCE_PERMISSIONS)
endif()


set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR "${AGHPresenter_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${AGHPresenter_VERSION_MINOR}")


include(CPack)