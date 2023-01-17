
#include <string>

#include "Parser.h"
#include "ErisContext.h"
#include "IRGen.h"

int main() {

	eris::ErisContext Context;
	Context.Initialize();

	eris::SourceBuf Buffer;
	Buffer.Memory = "void main() {}";
	Buffer.Length = std::strlen(Buffer.Memory);

	eris::FileUnit* Unit = new eris::FileUnit;

	eris::Parser Parser(Context, Buffer, Unit);
	Parser.Parse();

	eris::IRGenerator IRGenr(Context);
	for (auto [Name, Func] : Unit->GlobalFuncs) {
		IRGenr.GenFunc(Func);
	}

	delete Unit;

	return 0;
}