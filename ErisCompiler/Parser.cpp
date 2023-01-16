#include "Parser.h"

#include <llvm/Support/raw_ostream.h>

#include "ErisContext.h"

eris::Parser::Parser(ErisContext& Context, const SourceBuf Buffer)
	: Context(Context), Lex(Context, Buffer) {
}

void eris::Parser::Parse(FileUnit* FileUnit) {
	NextToken(); // Prime the parser.

	while (CTok.IsNot(TokenKind::TK_EOF)) {
		llvm::outs() << CTok.GetPresentationString(Context) << '\n';
		NextToken();
	}
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

eris::Token eris::Parser::PeekToken(u32 n) {
	assert(n != 0 && "Cannot peek zero tokens");
	assert(n < MAX_SAVED_TOKENS && "Cannot peek more than the maximum peek token amount");
	for (u32 i = 0; i < SavedTokensCount; i++) {
		SavedTokens[SavedTokensCount++] = Lex.NextToken();
	}
	return SavedTokens[SavedTokensCount - 1];
}
