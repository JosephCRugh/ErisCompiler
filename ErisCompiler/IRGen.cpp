#include "IRGen.h"

#include "ErisContext.h"
#include "Types.h"


//===-------------------------------===//
// Helper Functions
//===-------------------------------===//

inline llvm::Constant* GetLLInt8(i32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt8Ty(LLContext), value, true);
}
inline llvm::Constant* GetLLUInt8(u32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt8Ty(LLContext), value, false);
}
inline llvm::Constant* GetLLInt16(i32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt16Ty(LLContext), value, true);
}
inline llvm::Constant* GetLLUInt16(u32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt16Ty(LLContext), value, false);
}
inline llvm::Constant* GetLLInt32(i32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt32Ty(LLContext), value, true);
}
inline llvm::Constant* GetLLUInt32(u32 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt32Ty(LLContext), value, false);
}
inline llvm::Constant* GetLLInt64(i64 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt64Ty(LLContext), value, true);
}
inline llvm::Constant* GetLLUInt64(u64 value, llvm::LLVMContext& LLContext) {
	return llvm::ConstantInt::get(
		llvm::IntegerType::getInt64Ty(LLContext), value, false);
}




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

	for (AstNode* Stmt : Func->Stmts) {
		GenNode(Stmt);
	}
}

llvm::Value* eris::IRGenerator::GenNode(AstNode* Node) {
	switch (Node->Kind) {
	case AstKind::RETURN:
		return GenReturn(static_cast<ReturnStmt*>(Node));
	case AstKind::BINARY_OP:
		return GenBinaryOp(static_cast<BinaryOp*>(Node));
	case AstKind::NUMBER_LITERAL:
		return GenNumberLiteral(static_cast<NumberLiteral*>(Node));
	}
}

llvm::Value* eris::IRGenerator::GenRValue(Expr* E) {
	return GenNode(E);
}

llvm::Value* eris::IRGenerator::GenReturn(ReturnStmt* Ret) {
	if (Ret->Value) {
		Builder.CreateRet(GenNode(Ret->Value));	
	} else {
		Builder.CreateRetVoid();
	}
	return nullptr;
}

llvm::Value* eris::IRGenerator::GenBinaryOp(BinaryOp* BinOp) {
	switch (BinOp->Op) {
	case '+': {
		llvm::Value* LLLHS = GenRValue(BinOp->LHS);
		llvm::Value* LLRHS = GenRValue(BinOp->RHS);

		if (BinOp->Ty->IsInt()) {
			return Builder.CreateAdd(LLLHS, LLRHS);
		}
		return Builder.CreateFAdd(LLLHS, LLRHS);
	}
	case '-': {
		llvm::Value* LLLHS = GenRValue(BinOp->LHS);
		llvm::Value* LLRHS = GenRValue(BinOp->RHS);

		if (BinOp->Ty->IsInt()) {
			return Builder.CreateSub(LLLHS, LLRHS);
		}
		return Builder.CreateFSub(LLLHS, LLRHS);
	}
	case '*': {
		llvm::Value* LLLHS = GenRValue(BinOp->LHS);
		llvm::Value* LLRHS = GenRValue(BinOp->RHS);

		if (BinOp->Ty->IsInt()) {
			return Builder.CreateMul(LLLHS, LLRHS);
		}
		return Builder.CreateFMul(LLLHS, LLRHS);
	}
	case '/': {
		llvm::Value* LLLHS = GenRValue(BinOp->LHS);
		llvm::Value* LLRHS = GenRValue(BinOp->RHS);

		if (BinOp->Ty->IsInt()) {
			if (BinOp->Ty->IsSigned()) {
				return Builder.CreateSDiv(LLLHS, LLRHS);
			} else {
				return Builder.CreateUDiv(LLLHS, LLRHS);
			}
		}
		return Builder.CreateFDiv(LLLHS, LLRHS);
	}
	case '%': {
		llvm::Value* LLLHS = GenRValue(BinOp->LHS);
		llvm::Value* LLRHS = GenRValue(BinOp->RHS);

		if (BinOp->Ty->IsSigned()) {
			return Builder.CreateSRem(LLLHS, LLRHS);
		}
		return Builder.CreateURem(LLLHS, LLRHS);
	}
	default:
		assert(!"Unimplemented GenBinaryOP()");
		return nullptr;
	}
}

llvm::Value* eris::IRGenerator::GenNumberLiteral(NumberLiteral* Number) {
	switch (Number->Ty->GetKind()) {
	case TypeKind::Int8:           return GetLLInt8(Number->SignedIntValue, LLContext);
	case TypeKind::Int16:          return GetLLInt16(Number->SignedIntValue, LLContext);
	case TypeKind::Int32:          return GetLLInt32(Number->SignedIntValue, LLContext);
	case TypeKind::Int64:          return GetLLInt64(Number->SignedIntValue, LLContext);
	case TypeKind::UnsignedInt8:   return GetLLUInt8(Number->SignedIntValue, LLContext);
	case TypeKind::UnsignedInt16:  return GetLLUInt16(Number->SignedIntValue, LLContext);
	case TypeKind::UnsignedInt32:  return GetLLUInt32(Number->SignedIntValue, LLContext);
	case TypeKind::UnsignedInt64:  return GetLLUInt64(Number->SignedIntValue, LLContext);
	default:
		assert(!"Unimplemented GenNumberLiteral() case");
		return nullptr;
	}
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
