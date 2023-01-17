#include "IRGen.h"

#include "ErisContext.h"
#include "Types.h"

eris::IRGenerator::IRGenerator(ErisContext& Context)
	: Context(Context),
	  LLContext(Context.LLContext),
	  LLModule(Context.LLErisModule),
   	  Builder(Context.LLContext)
{
}

void eris::IRGenerator::GenFunc(FuncDecl* Func) {
	GenFuncDecl(Func);
	GenFuncBody(Func);
}

void eris::IRGenerator::GenFuncDecl(FuncDecl* Func) {
	if (Func->LLFunction) return; // Already generated.

	llvm::Type* LLRetTy = GenType(Func->RetTy);
	llvm::SmallVector<llvm::Type*, 4> LLParamTypes;

	bool IsVarArgs = false;
	llvm::FunctionType* LLFuncType =
		llvm::FunctionType::get(LLRetTy, LLParamTypes, IsVarArgs);

	std::string LLFuncName = Func->Name.Text.str();

	llvm::Function* LLFunc = llvm::Function::Create(
		LLFuncType,
		llvm::Function::ExternalLinkage,
		LLFuncName,
		LLModule
	);

	Func->LLFunction = LLFunc;
}

void eris::IRGenerator::GenFuncBody(FuncDecl* Func) {

	// Entry block for the function.
	llvm::BasicBlock* LLEntryBlock = llvm::BasicBlock::Create(LLContext, "func.entry", Func->LLFunction);
	Builder.SetInsertPoint(LLEntryBlock);

}

llvm::Type* eris::IRGenerator::GenType(Type* Ty) {
	switch (Ty->GetKind()) {
	case TypeKind::Int8:
	case TypeKind::UnsignedInt8:
		return llvm::Type::getInt8Ty(LLContext);
	case TypeKind::Int16:
	case TypeKind::UnsignedInt16:
		return llvm::Type::getInt16Ty(LLContext);
	case TypeKind::Int32:
	case TypeKind::UnsignedInt32:
		return llvm::Type::getInt32Ty(LLContext);
	case TypeKind::Int64:
	case TypeKind::UnsignedInt64:
		return llvm::Type::getInt64Ty(LLContext);
	case TypeKind::VOID:
		return llvm::Type::getVoidTy(LLContext);
	default:
		assert(!"Failed to implement case for Gentype()");
		return nullptr;
	}
}
