#include "IRGen.h"

#include "ErisContext.h"

eris::IRGenerator::IRGenerator(ErisContext& Context)
	: Context(Context),
      Builder(Context.LLContext)
{
}

void eris::IRGenerator::GenFunc(FuncDecl* Func) {
	GenFuncDecl(Func);
	GenFuncBody(Func);
}

void eris::IRGenerator::GenFuncDecl(FuncDecl* Func) {
	if (Func->LLFunction) return; // Already generated.


}

void eris::IRGenerator::GenFuncBody(FuncDecl* Func) {

}

