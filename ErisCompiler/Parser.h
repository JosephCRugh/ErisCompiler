#ifndef ERIS_PARSER_H
#define ERIS_PARSER_H

#include "Lexer.h"
#include "AST.h"

namespace eris {

	class ErisContext;

	class Parser {
	public:

		explicit Parser(ErisContext& Context, const SourceBuf Buffer);

		void Parse(FileUnit* FileUnit);

	private:
		static const usize MAX_SAVED_TOKENS = 8;

		ErisContext& Context;
		Lexer        Lex;
		
		Token CTok;
		usize SavedTokensCount = 0;
		Token SavedTokens[MAX_SAVED_TOKENS];

		//===-------------------------------===//
		// Utilities
		//===-------------------------------===//

		// Retreives the next Token from either
		// the Lexer or the SavedTokens and
		// stores it in CTok.
		//
		void NextToken();
	
		// Looks ahead n tokens and saves the tokens 
		// skipped into SavedTokens.
		Token PeekToken(u32 n);
	};

}

#endif // ERIS_PARSER_H