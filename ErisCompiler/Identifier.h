/*
 * Defines an Identifier structure to represent an
 * identifier in source code and quickly compare
 * identifiers by unique IDs.
 */
#ifndef ERIS_IDENTIFIER_H
#define ERIS_IDENTIFIER_H

#include "Prelude.h"

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/DenseMapInfo.h>
#include <llvm/Support/raw_ostream.h>

namespace eris {
	struct Identifier {
		llvm::StringRef Text;
		usize           ID;

		explicit Identifier(llvm::StringRef Text);

		Identifier() : ID(0) {}

		inline bool IsNull() const { return ID == 0; }

		bool operator==(const Identifier& RHS) const { return ID == RHS.ID; }
		bool operator!=(const Identifier& RHS) const { return ID != RHS.ID; }

	};
}

namespace llvm {

	// Defining key information for the eris::Identifier so
	// it may be used as a key which relies on the unique
	// ID of the eris::Identifier.
	template<> struct DenseMapInfo<eris::Identifier> {
		static bool isEqual(const eris::Identifier& RHS, const eris::Identifier& LHS) {
			return LHS == RHS;
		}
		static eris::Identifier getTombstoneKey() {
			// Do not remove identifiers from maps.
			return eris::Identifier();
		}
		static eris::Identifier getEmptyKey() {
			return eris::Identifier();
		}
		static usize getHashValue(const eris::Identifier& Val) {
			return Val.ID;
		}
	};
}

#endif // ERIS_IDENTIFIER_H