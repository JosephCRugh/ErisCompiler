#include "AST.h"

eris::FileUnit::~FileUnit() {
	for (auto [Name, Func] : GlobalFuncs) {
		delete Func;
	}
}
