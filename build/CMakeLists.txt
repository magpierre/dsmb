cmake_minimum_required(VERSION 3.11)

# This ensures that things like gnu++11 get passed correctly
#if(NOT DEFINED CMAKE_CXX_STANDARD)
#  set(CMAKE_CXX_STANDARD 17)
#endif()

if(NOT DEFINED TARGET)
  set(TARGET dsmb)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set the project name
project(${TARGET})

list(APPEND CMAKE_MODULE_PATH "/opt/homebrew/opt/apache-arrow/lib/cmake/arrow/")


    execute_process(
        COMMAND brew --prefix OpenSSL 
        RESULT_VARIABLE BREW_OPENSSL
        OUTPUT_VARIABLE BREW_OPENSSL_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_OPENSSL EQUAL 0 AND EXISTS "${BREW_OPENSSL_PREFIX}")
        message(STATUS "Found OpenSSL keg installed by Homebrew at ${BREW_OPENSSL_PREFIX}")
        set(OPENSSL_ROOT_DIR "${BREW_OPENSSL_PREFIX}/")
        set(OPENSSL_INCLUDE_DIR "${BREW_OPENSSL_PREFIX}/include")
        set(OPENSSL_LIBRARIES "${BREW_OPENSSL_PREFIX}/lib")
        set(OPENSSL_CRYPTO_LIBRARY "${BREW_OPENSSL_PREFIX}/lib/libcrypto.dylib")
    endif()


find_library(DeltaSharingClient required)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost 1.79.0 REQUIRED COMPONENTS Program_options)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)


add_executable(${TARGET} ../src/main.cpp;../src/Scroller.cpp;../src/functions.cpp)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v3.0.0 
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_compile_features(${TARGET} PRIVATE cxx_std_17)
target_include_directories(${TARGET} PRIVATE src)
target_include_directories(${TARGET} PRIVATE /usr/local/include/)


target_link_libraries(${TARGET}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
  PRIVATE Boost::program_options 
  PRIVATE nlohmann_json::nlohmann_json 
  PRIVATE /usr/local/lib/libDeltaSharingClient.dylib 
)
