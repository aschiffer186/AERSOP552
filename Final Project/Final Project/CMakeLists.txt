cmake_minimum_required(VERSION 2.8)
project(FINAL_PROJECT)

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED True)
add_compile_options(-std=c++11)

add_subdirectory(src)

add_subdirectory(tests)

add_executable(Final_Project src/main)
target_include_directories(Final_Project PRIVATE ${FINAL_PROJECT_SOURCE_DIR}/include/)
target_link_libraries(Final_Project PRIVATE Compiler)

set (CMAKE_INSTALL_PREFIX "../executable")
install(FILES sample_grammar1.txt DESTINATION "bin")
install(FILES sample_grammar2.txt DESTINATION "bin")
install(FILES sample_regex1.txt DESTINATION "bin")
install(FILES sample_regex2.txt DESTINATION "bin")
install(FILES sample_regex3.txt DESTINATION "bin")
install(FILES lexer_skeleton.cpp DESTINATION "bin")
install(FILES lexer_skeleton.hh DESTINATION "bin")
install(FILES test_lexer.cpp DESTINATION "bin")
install(TARGETS Final_Project DESTINATION "bin")
