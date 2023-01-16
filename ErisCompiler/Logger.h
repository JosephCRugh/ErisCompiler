/*
 * Defines a logger class for logging
 * errors and other information about
 * the state of the compilation.
 */
#ifndef ERIS_LOGGER_H
#define ERIS_LOGGER_H

#include <llvm/Support/raw_ostream.h>

#include <functional>

#include "Source.h"

namespace eris {
	class Logger {
	public:

		Logger();

		void BeginError(SourceLoc Loc, const c8* MsgHeader) {
			InternalErrorHeaderPrinting(Loc, [this, MsgHeader]() { OS << MsgHeader; });
		}

		template<typename... TArgs>
		void BeginError(SourceLoc Loc, const c8* Fmt, TArgs&&... Args) {
			InternalErrorHeaderPrinting(Loc, [&]() {
				ForwardFmt(OS, Fmt, std::forward<TArgs>(Args)...);
			});
		}

		void EndError() {}

	private:
		llvm::raw_ostream& OS;
	
		void InternalErrorHeaderPrinting(SourceLoc Loc, const std::function<void()>& Printer);
		
		template<typename T>
		static void ForwardFmt(llvm::raw_ostream& OS, const c8* Fmt, T&& Val) {
			for (; *Fmt != '\0'; Fmt++) {
				if (*Fmt == '%' && *(Fmt + 1) == 's') {
					OS << std::forward<T>(Val);
					++Fmt;
					continue;
				}
				OS << *Fmt;
			}
		}

		template<typename T, typename... TArgs>
		static void ForwardFmt(llvm::raw_ostream& OS, const c8* Fmt, T&& Val, TArgs&&... Args) {
			for (; Fmt != '\0'; Fmt++) {
				if (*Fmt == '%' && *(Fmt + 1) == 's') {
					OS << std::forward<T>(Val);
					ForwardFmt(OS, Fmt + 2, std::forward<TArgs>(Args)...);
					return;
				}
				OS << *Fmt;
			}
		}
	};
}

#endif // ERIS_LOGGER_H