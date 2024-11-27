file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(hunterCGLib SHARED ${SOURCES})

target_include_directories(hunterCGLib PRIVATE ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})

message(STATUS "Linking with CLANG libraries: ${CLANG_LIBS}")
message(STATUS "Linking with LLVM libraries: ${LLVM_LIBS}")

target_link_libraries(hunterCGLib PUBLIC
    ${CLANG_LIBS}
    ${LLVM_LIBS}
    ${SQLite3_LIBRARIES}
)
