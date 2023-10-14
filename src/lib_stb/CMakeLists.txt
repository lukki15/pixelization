# Sources and Headers
file(GLOB LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

# MyLib Library
add_library(${LIBRARY_NAME}-stb STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(
    ${LIBRARY_NAME}-stb
    PUBLIC ${LIBRARY_INCLUDES}
    PRIVATE ${STB_INCLUDE_DIRS}
)
target_link_libraries(
    ${LIBRARY_NAME}-stb
    PRIVATE
        cxxopts::cxxopts # TODO remove
)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${LIBRARY_NAME}-stb
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${LIBRARY_NAME}-stb
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${LIBRARY_NAME}-stb)
endif()
