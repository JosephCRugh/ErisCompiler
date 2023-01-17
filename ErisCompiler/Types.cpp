#include "Types.h"

bool eris::Type::Is(Type* Ty) const {
	return Ty->Kind == Kind;
}