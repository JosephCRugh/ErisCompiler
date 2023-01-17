#ifndef ERIS_CONTEXT_H
#define ERIS_CONTEXT_H

#include <llvm/ADT/DenseMap.h>

#include "Tokens.h"

namespace llvm {
	class LLVMContext;
	class Module;
}

namespace eris {

	class Type;

	class ErisContext {
	public:

		ErisContext();

		~ErisContext();

		void Initialize();

		// If the text matches one of the keywords then
		// the token kind for the text is returned. Otherwise,
		// a token kind of TokenKind::INVALID is returned.
		TokenKind GetKeywordKind(llvm::StringRef Text) const;

		// Converts a keyword's token kind into its equivalent
		// string.
		llvm::StringRef GetKeywordAsString(TokenKind Kind) const {
			return TokenKeywordInvertedMap.find(static_cast<u32>(Kind))->second;
		}

		//
		// Types
		// --------------
		// Integer Types
		Type* Int8Type;
		Type* Int16Type;
		Type* Int32Type;
		Type* Int64Type;
		Type* UInt8Type;
		Type* UInt16Type;
		Type* UInt32Type;
		Type* UInt64Type;
		// Other
		Type* ErrorType;
		Type* VoidType;

		// ----- LLVM -----
		llvm::LLVMContext& LLContext;
		llvm::Module&      LLErisModule;

	private:
		llvm::DenseMap<llvm::StringRef, TokenKind> TokenKeywordMap;

		// Keyword kind => Text
		//
		// The keyword kind is stored as a 32 bit integer
		// since DenseMaps know about how to store keys
		// of integers but not TokenKind.
		llvm::DenseMap<u32, llvm::StringRef> TokenKeywordInvertedMap;

	};
}

#endif // ERIS_CONTEXT_H