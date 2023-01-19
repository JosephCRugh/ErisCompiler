#ifndef ERIS_LEXER_H
#define ERIS_LEXER_H

#include "Logger.h"
#include "Source.h"
#include "Tokens.h"

namespace eris {

	class ErisContext;

	class Lexer {
	public:

		explicit Lexer(const ErisContext& Context, Logger& Log, const SourceBuf Buffer);

		// Retrieves the next token in the
		// buffer, ignoring comments and whitespace.
		//
		Token NextToken();

	private:
		const ErisContext& Context;
		Logger&            Log;

		const c8* CurPtr;
		usize     LineNumber = 1;

		// The next token is either a
		// keyword or an identifier.
		//
		// [a-zA-Z_][a-zA-Z0-9_]*
		Token NextWord();

		// The next token is a number.
		// 
		// [0-9']+
		Token NextNumber();

		// Utility
		// 

		inline llvm::StringRef CreateText(const c8* TokStart) const {
			return llvm::StringRef(TokStart, CurPtr - TokStart);
		}

		inline Token CreateToken(u16 Kind, llvm::StringRef Text) const {
			return Token(Kind, SourceLoc{ Text, LineNumber });
		}

		void Error(SourceLoc Loc, const c8* Msg) {
			Log.BeginError(Loc, Msg);
			Log.EndError();
		}

		template<typename... TArgs>
		void Error(const c8* CharPos, const c8* Fmt, TArgs&&... Args) {
			Log.BeginError(SourceLoc{ llvm::StringRef(CharPos, 1), LineNumber },
				Fmt, std::forward<TArgs>(Args)...);
			Log.EndError();
		}
	};

}

#endif // ERIS_LEXER_H