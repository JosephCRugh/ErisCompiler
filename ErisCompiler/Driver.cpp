
#include <string>

#include "Parser.h"
#include "ErisContext.h"

int main() {

	eris::ErisContext Context;
	Context.Initialize();

	eris::SourceBuf Buffer;
	Buffer.Memory = "void main() {}";
	Buffer.Length = std::strlen(Buffer.Memory);

	eris::FileUnit* Unit = new eris::FileUnit;

	eris::Parser Parser(Context, Buffer);
	Parser.Parse(Unit);

	delete Unit;

	return 0;
}