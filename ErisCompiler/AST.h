/*
 * Defines structures for the abstract
 * syntax tree of a .eris file.
 */
#ifndef ERIS_AST_H
#define ERIS_AST_H

#include <llvm/ADT/DenseMap.h>

#include "Identifier.h"
#include "Logger.h"

namespace eris {
	
	struct FuncDecl;

	// Base AST node which contains
	// all the information AST information
	// for a given file.
	//
	struct FileUnit {

		~FileUnit();

		Logger Log;

		llvm::DenseMap<Identifier, FuncDecl*> Funcs;
	};

	struct Decl {
		Identifier Name;
	};

	struct FuncDecl : public Decl {

	};

}

#endif // ERIS_AST_H