#ifndef ERIS_PARSER_H
#define ERIS_PARSER_H

#include "Lexer.h"
#include "AST.h"

namespace eris {

	class ErisContext;
	class Type;

	class Parser {
	public:

		explicit Parser(ErisContext& Context, const SourceBuf Buffer, FileUnit* Unit);

		void Parse();

	private:
		static const usize MAX_SAVED_TOKENS = 8;

		ErisContext& Context;
		Lexer        Lex;
		FileUnit*    Unit;
		Logger&      Log;

		Token PrevToken;
		Token CTok;
		usize SavedTokensCount = 0;
		Token SavedTokens[MAX_SAVED_TOKENS];

		Type* ParseType();

		FuncDecl* ParseFuncDecl(Type* RetTy);

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

		void Error(Token Tok, const c8* Msg) {
			Log.BeginError(Tok.Loc, Msg);
			Log.EndError();
		}

		template<typename... TArgs>
		void Error(Token Tok, const c8* Fmt, TArgs&&... Args) {
			Log.BeginError(Tok.Loc, Fmt, std::forward<TArgs>(Args)...);
			Log.EndError();
		}

		void Error(SourceLoc Loc, const c8* Msg) {
			Log.BeginError(Loc, Msg);
			Log.EndError();
		}

		template<typename... TArgs>
		void Error(SourceLoc Loc, const c8* Fmt, TArgs&&... Args) {
			Log.BeginError(Loc, Fmt, std::forward<TArgs>(Args)...);
			Log.EndError();
		}
	};

}

#endif // ERIS_PARSER_H