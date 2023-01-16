/*
 * Definitions for the source code of .eris
 * files and how to process them.
 */
#ifndef ERIS_SOURCE_H
#define ERIS_SOURCE_H

#include <llvm/ADT/StringRef.h>

#include "Prelude.h"

namespace eris {
	
	// Source buffer which points
	// to the source code of a .eris
	// file.
	//
	struct SourceBuf {
		c8*   Memory;
		usize Length;
	};

	// Identifiers a location within the
	// source buffer of a .eris file.
	// 
	// The use of an llvm::StringRef allows
	// for pointing to a character location in the
	// buffer along with the length of the text
	// being of a token.
	//
	struct SourceLoc {
		llvm::StringRef Text;
		usize           LineNumber;
	};
}

#endif // ERIS_SOURCE_H