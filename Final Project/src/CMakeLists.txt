add_library(Compiler exceptions.cpp regex_parser.cpp nfa.cpp dfa.cpp parser_generator.cpp lexer_generator.cpp)
target_include_directories(Compiler PRIVATE ${FINAL_PROJECT_SOURCE_DIR}/include/)

