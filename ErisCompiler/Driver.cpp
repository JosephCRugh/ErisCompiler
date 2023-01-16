
#include <llvm/Support/raw_ostream.h>

#include <string>

#include "Lexer.h"

int main() {

	eris::SourceBuf Buffer;
	Buffer.Memory = "void main() {}";
	Buffer.Length = std::strlen(Buffer.Memory);

	eris::Lexer Lex(Buffer);

	eris::Token Tok;
	while ((Tok = Lex.NextToken()).IsNot(eris::TokenKind::TK_EOF)) {
		llvm::outs() << Tok.GetPresentationString() << '\n';
	}

	return 0;
}