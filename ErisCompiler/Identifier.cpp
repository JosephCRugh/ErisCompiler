#include "Identifier.h"

#include <llvm/ADT/DenseMap.h>

eris::Identifier::Identifier(llvm::StringRef Text) {
	static llvm::DenseMap<llvm::StringRef, usize> IdentifierCache;
	static usize IdentiferIDCounter = 1;

	auto Itr = IdentifierCache.find(Text);
	if (Itr != IdentifierCache.end()) {
		this->Text = Itr->first;
		ID         = Itr->second;
	} else {
		this->Text = Text;
		ID = IdentiferIDCounter++;
		IdentifierCache.insert({ Text, ID });
	}
}
