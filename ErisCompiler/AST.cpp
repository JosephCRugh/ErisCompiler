#include "AST.h"

eris::FileUnit::~FileUnit() {
	for (auto [Ident, Func] : Funcs) {
		delete Func;
	}
}
