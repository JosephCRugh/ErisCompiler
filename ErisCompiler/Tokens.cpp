#include "Tokens.h"

#include "ErisContext.h"

std::string eris::Token::TokenKindToString(const ErisContext& Context, TokenKind Kind) {
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
		
		if (Kind >= TokenKind::__KW_START__ && Kind <= TokenKind::__KW_END__) {
			return Context.GetKeywordAsString(Kind).str();
		}
		return "";
	}
}

std::string eris::Token::GetPresentationString(const ErisContext& Context) const {
	return std::string("(") + Token::TokenKindToString(Context, Kind) +
		", '" + Loc.Text.str()  + "')";
}
