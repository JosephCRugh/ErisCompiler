#ifndef ERIS_PARSER_H
#define ERIS_PARSER_H

#include "Lexer.h"
#include "AST.h"

namespace eris {

	class ErisContext;

	class Parser {
	public:

		explicit Parser(ErisContext& Context, const SourceBuf Buffer);

		void Parse(FileUnit* Unit);

	private:
		static const usize MAX_SAVED_TOKENS = 8;

		ErisContext& Context;
		Lexer        Lex;
		
		FileUnit* Unit;

		Token CTok;
		usize SavedTokensCount = 0;
		Token SavedTokens[MAX_SAVED_TOKENS];

		void ParseType();

		FuncDecl* ParseFuncDecl();

		Identifier ParseIdentifier(const c8* ErrorMessage);

		//===-------------------------------===//
		// Utilities
		//===-------------------------------===//

		// Retreives the next Token from either
		// the Lexer or the SavedTokens and
		// stores it in CTok.
		void NextToken();
	
		// Looks ahead n tokens and saves the tokens 
		// skipped into SavedTokens.
		Token PeekToken(u32 n);

		// if the current token matches the TokenKind
		// then it is consumed, otherwise an error is
		// generated.
		void Match(TokenKind Kind, const c8* Purpose = nullptr);
		inline void Match(u8 UTF8Kind, const c8* Purpose = nullptr) {
			return Match(static_cast<TokenKind>(UTF8Kind), Purpose);
		}
	};

}

#endif // ERIS_PARSER_H