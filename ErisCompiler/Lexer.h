#ifndef ERIS_LEXER_H
#define ERIS_LEXER_H

#include "Source.h"
#include "Tokens.h"

namespace eris {

	class ErisContext;

	class Lexer {
	public:

		explicit Lexer(const ErisContext& Context, const SourceBuf Buffer);

		// Retrieves the next token in the
		// buffer, ignoring comments and whitespace.
		//
		Token NextToken();

	private:
		const ErisContext& Context;
		const c8* CurPtr;
		usize     LineNumber = 1;

		// The next token is either a
		// keyword or an identifier.
		//
		// [a-zA-Z_][a-zA-Z0-9_]*
		Token NextWord();

		// Utility
		// 

		inline llvm::StringRef CreateText(const c8* TokStart) const {
			return llvm::StringRef(TokStart, CurPtr - TokStart);
		}

		inline Token CreateToken(TokenKind Kind, llvm::StringRef Text) const {
			return Token(Kind, SourceLoc{ Text, LineNumber });
		}

		inline Token CreateToken(u8 UTF8Kind, const c8* TokStart) const {
			return Token(static_cast<TokenKind>(UTF8Kind), SourceLoc{ CreateText(TokStart), LineNumber });
		}
	};

}

#endif // ERIS_LEXER_H