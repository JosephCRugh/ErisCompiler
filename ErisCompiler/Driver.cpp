
#include <string>

#include "Parser.h"
#include "ErisContext.h"

int main() {

	eris::ErisContext Context;
	Context.Initialize();

	eris::SourceBuf Buffer;
	Buffer.Memory = "void main() {}";
	Buffer.Length = std::strlen(Buffer.Memory);

	eris::Parser Parser(Context, Buffer);
	Parser.Parse(nullptr);

	return 0;
}