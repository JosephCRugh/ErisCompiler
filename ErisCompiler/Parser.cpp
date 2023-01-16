#include "Parser.h"

#include <llvm/Support/raw_ostream.h>

#include "ErisContext.h"

eris::Parser::Parser(ErisContext& Context, const SourceBuf Buffer)
	: Context(Context), Lex(Context, Buffer) {
}

void eris::Parser::Parse(FileUnit* Unit) {
	this->Unit = Unit;

	NextToken(); // Prime the parser.

	FuncDecl* Func = ParseFuncDecl();
	// TODO: Will want to check here for function redeclaration.
	Unit->Funcs.insert({ Func->Name, Func });

}

void eris::Parser::ParseType() {
	NextToken(); // TODO: Actually implement.
}

eris::FuncDecl* eris::Parser::ParseFuncDecl() {

	ParseType();

	FuncDecl* Func = new FuncDecl;
	Func->Name = ParseIdentifier("Expected an identifier for function declaration");
	Match('(');
	Match(')');
	Match('{');
	Match('}');

	return Func;
}

//===-------------------------------===//
// Utilities
//===-------------------------------===//

void eris::Parser::NextToken() {
	if (SavedTokensCount) {
		CTok = SavedTokens[0];
		std::rotate(SavedTokens, SavedTokens + 1, SavedTokens + SavedTokensCount);
		--SavedTokensCount;
	} else {
		CTok = Lex.NextToken();
	}
}

eris::Identifier eris::Parser::ParseIdentifier(const c8* ErrorMessage) {
	if (CTok.IsNot(TokenKind::IDENT)) {
		// TODO: Report error!
		return Identifier(); // Returning a null identifier.
	}
	llvm::StringRef Text = CTok.Loc.Text;
	NextToken(); // Consuming ident token.
	return Identifier(Text);
}

eris::Token eris::Parser::PeekToken(u32 n) {
	assert(n != 0 && "Cannot peek zero tokens");
	assert(n < MAX_SAVED_TOKENS && "Cannot peek more than the maximum peek token amount");
	for (u32 i = 0; i < SavedTokensCount; i++) {
		SavedTokens[SavedTokensCount++] = Lex.NextToken();
	}
	return SavedTokens[SavedTokensCount - 1];
}

void eris::Parser::Match(TokenKind Kind, const c8* Purpose) {
	if (CTok.Is(Kind)) {
		NextToken(); // Consuming the matched token.
		return;
	}
	// TODO: report an error
}
