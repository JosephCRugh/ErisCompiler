#include "Logger.h"

#include <llvm/Support/raw_ostream.h>

eris::Logger::Logger()
	: OS(llvm::errs()) {
}

void eris::Logger::InternalErrorHeaderPrinting(SourceLoc Loc, const std::function<void()>& Printer) {


	OS << ":" << Loc.LineNumber;
	OS << " Error: ";

	Printer();

	OS << ".";
	
}
