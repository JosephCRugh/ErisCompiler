#include "AST.h"

eris::FileUnit::~FileUnit() {
	for (auto [Name, Func] : GlobalFuncs) {
		delete Func;
	}
}

eris::FuncDecl::~FuncDecl() {
	for (AstNode* Stmt : Stmts) {
		delete Stmt;
	}
}

eris::BinaryOp::~BinaryOp() {
	delete LHS;
	delete RHS;
}
