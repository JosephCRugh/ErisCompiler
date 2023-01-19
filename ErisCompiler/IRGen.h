#ifndef ERIS_IR_GEN_H
#define ERIS_IR_GEN_H

#include <llvm/IR/IRBuilder.h>

#include "AST.h"

namespace eris {
	
	class ErisContext;

	class IRGenerator {
	public:

		explicit IRGenerator(ErisContext& Context);

		void GenFunc(FuncDecl* Func);

	private:
		ErisContext&       Context;
		llvm::LLVMContext& LLContext;
		llvm::Module&      LLModule;
		llvm::IRBuilder<>  Builder;

		void GenFuncDecl(FuncDecl* Func);
		void GenFuncBody(FuncDecl* Func);

		llvm::Value* GenNode(AstNode* Node);
		llvm::Value* GenReturn(ReturnStmt* Ret);
		llvm::Value* GenNumberLiteral(NumberLiteral* Number);

		llvm::Type* GenType(Type* Ty);

	};
}

#endif // ERIS_IR_GEN_H