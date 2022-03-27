if (NOT TARGET gtest_main)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.11.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test-andwass_string_view string_view.cpp)
target_link_libraries(test-andwass_string_view gtest_main andwass::string_view)
add_test(NAME test-andwass_string_view COMMAND test-andwass_string_view)