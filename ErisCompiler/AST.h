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
	struct AstNode;
	struct FuncDecl;
	
	using ScopeStmts = llvm::SmallVector<AstNode*, 8>;

	enum class AstKind {
		
		ERROR,

		FUNC_DECL,
		RETURN,

		NUMBER_LITERAL

	};

	// Base AST node which contains
	// all the information AST information
	// for a given file.
	//
	struct FileUnit {

		~FileUnit();

		Logger Log;

		llvm::DenseMap<Identifier, FuncDecl*> GlobalFuncs;
	};

	struct AstNode {
		SourceLoc Loc;
		AstKind   Kind;

		AstNode(AstKind Kind) : Kind(Kind) {}

		inline bool Is(AstKind Kind) { return this->Kind == Kind; }
		inline bool IsNot(AstKind Kind) { return this->Kind != Kind; }

	};

	struct ErrorNode : AstNode {
		ErrorNode() :
			AstNode(AstKind::ERROR) {}
	};

	struct Decl : AstNode {
		Identifier Name;
	
		Decl(AstKind Kind) : AstNode(Kind) {}
	};

	struct FuncDecl : Decl {
		Type* RetTy;

		llvm::Function* LLFunction = nullptr;

		ScopeStmts Stmts;
	
		FuncDecl() : Decl(AstKind::FUNC_DECL) {}
	};

	struct ReturnStmt : AstNode {
		AstNode* Value = nullptr;

		ReturnStmt() : AstNode(AstKind::RETURN) {}
	};

	//===-------------------------------===//
	// Expressions
	//===-------------------------------===//

	struct Expr : AstNode {
		// Typically set during type checking.
		Type* Ty;

		Expr(AstKind Kind) : AstNode(Kind) {}
	};

	struct NumberLiteral : Expr {

		// The AstKind discriminates the union.
		union {
			i64 SignedIntValue;
			u64 UnsignedIntValue;
			double FloatValue;
		};

		NumberLiteral()
			: Expr(AstKind::NUMBER_LITERAL) {}
	};
}

#endif // ERIS_AST_H