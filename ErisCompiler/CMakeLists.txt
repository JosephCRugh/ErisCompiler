cmake_minimum_required (VERSION 3.8)

#importing LLVM
find_package(LLVM REQUIRED CONFIG)

# Including LLVM source
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})


# Add source to this project's executable.
add_executable (ErisCompiler   "Driver.cpp" "Lexer.h" "Source.h" "Prelude.h" "Lexer.cpp" "Tokens.h" "Tokens.cpp")

#LLVM Components provide access to the underlying libraries without having
# to specify the libraries being used.
llvm_map_components_to_libnames(LLVM_Libs
  Analysis
  Core
  ExecutionEngine
  InstCombine
  Object
  OrcJIT
  RuntimeDyld
  Support
  TransformUtils
  native
  )

#Linking the LLVM Libraries
target_link_libraries(ErisCompiler ${LLVM_Libs})