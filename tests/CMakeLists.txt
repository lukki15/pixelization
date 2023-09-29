if(ENABLE_TESTING)
    file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
    set(TEST_HEADERS "")
    set(TEST_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

    add_executable(${UNIT_TEST_NAME} ${TEST_SOURCES} ${TEST_HEADERS})

    target_link_libraries(
        ${UNIT_TEST_NAME}
        PUBLIC
        ${LIBRARY_NAME}-resize
        ${LIBRARY_NAME}-stb
        ${LIBRARY_NAME}-utils
    )
    target_link_libraries(${UNIT_TEST_NAME} PRIVATE Catch2::Catch2WithMain)


    include(CTest)
    include(Catch)
    catch_discover_tests(${UNIT_TEST_NAME} WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY})

    target_set_warnings(
        TARGET
        ${UNIT_TEST_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})

    if(ENABLE_COVERAGE)
        set(COVERAGE_MAIN "coverage")
        set(COVERAGE_EXCLUDES
            "${PROJECT_SOURCE_DIR}/app/*"
            "${PROJECT_SOURCE_DIR}/cmake/*"
            "${PROJECT_SOURCE_DIR}/docs/*"
            "${PROJECT_SOURCE_DIR}/external/*"
            "${PROJECT_SOURCE_DIR}/tests/*"
            "${PROJECT_SOURCE_DIR}/build/*"
            "/usr/include/*")
        setup_target_for_coverage_lcov(
            NAME
            ${COVERAGE_MAIN}
            EXECUTABLE
            ${UNIT_TEST_NAME}
            DEPENDENCIES
            ${UNIT_TEST_NAME}
            WORKING_DIRECTORY
            ${TEST_WORKING_DIRECTORY}
        )
    endif()
endif()
