#include "ErisContext.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/DIBuilder.h>

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

	BinaryOpsPrecedence({
		{ '*', 9 },
		{ '/', 9 },
		{ '%', 9 },

		{ '+', 8 },
		{ '-', 8 },

		{ TokenKind::LT_LT, 7 }, // <<
		{ TokenKind::GT_GT, 7 }, // >>
		
		{ '<'      , 6 },
		{ '>'      , 6 },
		{ TokenKind::LT_EQ, 6 }, // <=
		{ TokenKind::GT_EQ, 6 }, // >=
		
		{ TokenKind::EQ_EQ , 5 }, // ==
		{ TokenKind::EXL_EQ, 5 }, // !=
		
		{ '&', 4 },
		
		{ '^', 3 },
		
		{ '|', 2 },
		
		{ TokenKind::AMP_AMP, 1 }, // &&
		{ TokenKind::BAR_BAR, 1 }, // ||
		
		}),

	LLContext(*new llvm::LLVMContext),
	LLErisModule(*new llvm::Module("Eris Module", LLContext)),

	TokenKeywordMap(NUM_KEYWORDS),
	TokenKeywordInvertedMap(NUM_KEYWORDS)
{
}

eris::ErisContext::~ErisContext() {
	delete &LLErisModule;
	delete &LLContext;
	
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
	
	TokenKeywordMap.insert({ "void"  , TokenKind::KW_VOID   });
	TokenKeywordMap.insert({ "i8"    , TokenKind::KW_I8     });
	TokenKeywordMap.insert({ "i16"   , TokenKind::KW_I16    });
	TokenKeywordMap.insert({ "i32"   , TokenKind::KW_I32    });
	TokenKeywordMap.insert({ "i64"   , TokenKind::KW_I64    });
	TokenKeywordMap.insert({ "u8"    , TokenKind::KW_U8     });
	TokenKeywordMap.insert({ "u16"   , TokenKind::KW_U16    });
	TokenKeywordMap.insert({ "u32"   , TokenKind::KW_U32    });
	TokenKeywordMap.insert({ "u64"   , TokenKind::KW_U64    });
	TokenKeywordMap.insert({ "return", TokenKind::KW_RETURN });

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
