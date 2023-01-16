#include "ErisContext.h"

namespace eris {
	constexpr usize NUM_KEYWORDS = static_cast<usize>(TokenKind::__KW_END__) -
		                           static_cast<usize>(TokenKind::__KW_START__) + 1;
} 

eris::ErisContext::ErisContext()
	:
	TokenKeywordMap(NUM_KEYWORDS),
	TokenKeywordInvertedMap(NUM_KEYWORDS)
{

}

void eris::ErisContext::Initialize() {
	
	TokenKeywordMap.insert({ "void", TokenKind::KW_VOID });

	for (const auto& [Text, Kind] : TokenKeywordMap) {
		TokenKeywordInvertedMap.insert({ static_cast<u32>(Kind), Text });
	}
}

eris::TokenKind eris::ErisContext::GetKeywordKind(llvm::StringRef Text) const {
	auto Itr = TokenKeywordMap.find(Text);
	if (Itr != TokenKeywordMap.end()) {
		return Itr->second;
	}
	return TokenKind::INVALID;
}
