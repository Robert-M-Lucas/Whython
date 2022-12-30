cmake_minimum_required(VERSION 3.23)
project(PLang3)

set(CMAKE_CXX_STANDARD 23)

# set(CMAKE_<LANG>_FLAGS -static-libgcc -static-libstdc++)
set(CMAKE_EXE_LINKER_FLAGS "-static")
# target_link_libraries(cmake-build-release "-static")

add_executable(PLang3 main.cpp Compilation/MemoryManager.cpp Compilation/MemoryManager.h Compilation/BlockHandler.cpp Compilation/BlockHandler.h Blocks/BlockController.cpp Blocks/BlockController.h Preprocessing/LexicalAnalyser.cpp Preprocessing/LexicalAnalyser.h Preprocessing/LexicalResult.h References/ReferenceHandler.cpp References/ReferenceHandler.h References/Reference.h Blocks/WhileBlock.cpp Blocks/WhileBlock.h Compilation/LineHandler.cpp Compilation/LineHandler.h Types/AbstractType.h Types/IntType.cpp Types/IntType.h Instructions/WriteInstruction.h InternalTypes/AddressType.h Util/Common.h Instructions/OutInstruction.h Util/Conversions.cpp Util/Conversions.h Instructions/GotoInstruction.h Instructions/CopyInstruction.h Compilation/Compile.h Execution/Execute.h Util/Common.cpp Instructions/AddInstruction.h Blocks/LoopBlock.cpp Blocks/LoopBlock.h Types/BoolType.cpp Types/BoolType.h Instructions/EqualsInstruction.h Instructions/GotoIfInstruction.h Blocks/IfBlock.cpp Blocks/IfBlock.h Blocks/IfBlock.h Util/BoolTools.cpp Util/BoolTools.h Util/TypeGetter.cpp Util/TypeGetter.h Types/CharType.cpp Types/CharType.h Instructions/SubtractInstruction.h)
