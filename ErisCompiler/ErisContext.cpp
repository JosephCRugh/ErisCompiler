#include "ErisContext.h"

#include "Types.h"

namespace eris {
	constexpr usize NUM_KEYWORDS = static_cast<usize>(TokenKind::__KW_END__) -
		                           static_cast<usize>(TokenKind::__KW_START__) + 1;
} 

eris::ErisContext::ErisContext()
	:
	Int8Type(new Type(TypeKind::Int8)),
	Int16Type(new Type(TypeKind::Int16)),
	Int32Type(new Type(TypeKind::Int32)),
	Int64Type(new Type(TypeKind::Int64)),
	UInt8Type(new Type(TypeKind::UnsignedInt8)),
	UInt16Type(new Type(TypeKind::UnsignedInt16)),
	UInt32Type(new Type(TypeKind::UnsignedInt32)),
	UInt64Type(new Type(TypeKind::UnsignedInt64)),
	ErrorType(new Type(TypeKind::ERROR)),
	VoidType(new Type(TypeKind::VOID)),

	TokenKeywordMap(NUM_KEYWORDS),
	TokenKeywordInvertedMap(NUM_KEYWORDS)
{
}

eris::ErisContext::~ErisContext() {
	delete Int8Type;
	delete Int16Type;
	delete Int32Type;
	delete Int64Type;
	delete UInt8Type;
	delete UInt16Type;
	delete UInt32Type;
	delete UInt64Type;
	delete ErrorType;
	delete VoidType;
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
