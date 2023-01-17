/*
 * Defines structures for the abstract
 * syntax tree of a .eris file.
 */
#ifndef ERIS_AST_H
#define ERIS_AST_H

#include <llvm/ADT/DenseMap.h>

#include "Identifier.h"
#include "Logger.h"

namespace llvm {
	class Function;
}

namespace eris {

	class Type;
	struct FuncDecl;

	// Base AST node which contains
	// all the information AST information
	// for a given file.
	//
	struct FileUnit {

		~FileUnit();

		Logger Log;

		llvm::DenseMap<Identifier, FuncDecl*> GlobalFuncs;
	};

	struct Decl {
		Identifier Name;
	};

	struct FuncDecl : public Decl {
		Type* RetTy;

		llvm::Function* LLFunction = nullptr;
	};

}

#endif // ERIS_AST_H