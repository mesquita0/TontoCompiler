find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "--c++")

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.h")
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES main.cpp scanner.cpp AST.cpp SymbolTable.cpp)

add_executable(TontoCompiler ${SOURCES} ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
