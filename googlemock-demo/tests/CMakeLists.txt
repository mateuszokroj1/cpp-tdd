set(PROJECT_TESTS "tests-${PROJECT_ID}")
set(PROJECT_TESTS "tests-${PROJECT_ID}" PARENT_SCOPE)
message(STATUS "PROJECT_TESTS is: " ${PROJECT_TESTS})

####################
# GTest
find_package(GTest)

if (NOT GTest_FOUND)
    message(STATUS "GTest package not found! Trying to download it...")

    include(FetchContent)

    FetchContent_Declare(
        googletest 
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON)
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

####################
# Sources & headers
file(GLOB SRC_FILES *.cpp *.c *.cxx)
file(GLOB SRC_HEADERS *.h *.hpp *.hxx)

add_executable(${PROJECT_TESTS} ${SRC_FILES} ${SRC_HEADERS})
target_link_libraries(${PROJECT_TESTS} ${PROJECT_LIB} GTest::gmock_main)

####################
# GTest discover tests
include(GoogleTest)
gtest_discover_tests(${PROJECT_TESTS})