#ifndef ERIS_TOKENS_H
#define ERIS_TOKENS_H

#include <string>

#include "Source.h"

namespace eris {
	
	class ErisContext;

	enum TokenKind : u16 {
		INVALID = 0,


		// Index starts at 256 to reserve
		// space for UTF-8 characters.
		//
		__UNIQUE_TOKEN_KIND_START__ = 256,

		// === Keyword ===

		__KW_START__,

		KW_I8 = __KW_START__,
		KW_I16,
		KW_I32,
		KW_I64,
		KW_U8,
		KW_U16,
		KW_U32,
		KW_U64,

		KW_VOID,

		KW_RETURN,

		__KW_END__ = KW_RETURN,

		// === Symbols ===

		PLUS_EQ,     // +=
		MINUS_EQ,    // -=
		SLASH_EQ,    // /=
		STAR_EQ,     // *=
		MOD_EQ,      // %=
		AMP_EQ,      // &=
		BAR_EQ,      // |=
		CRT_EQ,      // ^=
		LT_LT,       // <<
		GT_GT,       // >>
		LT_LT_EQ,    // <<=
		GT_GT_EQ,    // >>=
		LT_EQ,       // <=
		GT_EQ,       // >=
		AMP_AMP,     // &&
		BAR_BAR,     // ||
		EQ_EQ,       // ==
		EXL_EQ,      // !=

		// === Extra ===
		
		IDENT,
		INT_LITERAL,

		// End of file
		TK_EOF
	};

	struct Token {
		u16       Kind;
		SourceLoc Loc;
	
		Token() : Kind(0) {}

		Token(u16 Kind, SourceLoc Loc)
			: Kind(Kind), Loc(Loc) {}

		// Checks if the token is of the given kind.
		inline bool Is(u16 Kind) const { return this->Kind == Kind; }

		// Checks if the token is not of the given kind.
		inline bool IsNot(u16 Kind) const { return this->Kind != Kind; }

		static std::string TokenKindToString(const ErisContext& Context, TokenKind Kind);

		// Retrieves a presentation string in the
		// form (kind, 'text').
		std::string GetPresentationString(const ErisContext& Context) const;

	};
}

#endif // ERIS_TOKENS_H