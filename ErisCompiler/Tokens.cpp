#include "Tokens.h"

#include "ErisContext.h"

std::string eris::Token::TokenKindToString(const ErisContext& Context, TokenKind Kind) {
	switch (Kind) {
	case TokenKind::IDENT:       return "ident";
	case TokenKind::INT_LITERAL: return "int-literal";
	case TokenKind::TK_EOF:      return "eof";
	case TokenKind::PLUS_EQ:     return "+=";
	case TokenKind::MINUS_EQ:    return "-=";
	case TokenKind::SLASH_EQ:    return "/=";
	case TokenKind::STAR_EQ:     return "*=";
	case TokenKind::MOD_EQ:	     return "%=";
	case TokenKind::AMP_EQ:	     return "&=";
	case TokenKind::BAR_EQ:	     return "|=";
	case TokenKind::CRT_EQ:	     return "^=";
	case TokenKind::LT_LT:	     return "<<";
	case TokenKind::GT_GT:	     return ">>";
	case TokenKind::LT_LT_EQ:    return "<<=";
	case TokenKind::GT_GT_EQ:    return ">>=";
	case TokenKind::LT_EQ:       return "<=";
	case TokenKind::GT_EQ:       return ">=";
	case TokenKind::AMP_AMP:     return "&&";
	case TokenKind::BAR_BAR:     return "||";
	case TokenKind::EQ_EQ:       return "==";
	case TokenKind::EXL_EQ:      return "!=";
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
	return std::string("(") + Token::TokenKindToString(Context, static_cast<TokenKind>(Kind)) +
		", '" + Loc.Text.str()  + "')";
}
