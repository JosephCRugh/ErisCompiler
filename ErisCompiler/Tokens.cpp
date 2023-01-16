#include "Tokens.h"

std::string eris::Token::TokenKindToString(TokenKind Kind) {
	switch (Kind) {
	case TokenKind::IDENT:  return "ident";
	case TokenKind::TK_EOF: return "eof";
	default:
		if (static_cast<u32>(Kind) < 256) {
			u32 UTF8Kind = static_cast<u32>(Kind);
			if (UTF8Kind >= 33 && UTF8Kind <= 126) // Easily displayable ASCII character.
				return std::string(1, static_cast<c8>(Kind));
			else
				return std::to_string(UTF8Kind);
		}
		return "";
	}
}

std::string eris::Token::GetPresentationString() const {
	return std::string("(") + Token::TokenKindToString(Kind) +
		", '" + Loc.Text.str()  + "')";
}
