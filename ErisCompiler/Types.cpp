#include "Types.h"

bool eris::Type::Is(Type* Ty) const {
	return Ty->Kind == Kind;
}

bool eris::Type::IsInt() const {
	switch (GetKind()) {
	case TypeKind::Int8:
	case TypeKind::Int16:
	case TypeKind::Int32:
	case TypeKind::Int64:
	case TypeKind::UnsignedInt8:
	case TypeKind::UnsignedInt16:
	case TypeKind::UnsignedInt32:
	case TypeKind::UnsignedInt64:
		return true;
	default:
		return false;
	};
}

bool eris::Type::IsSigned() const {
	switch (GetKind()) {
	case TypeKind::Int8:
	case TypeKind::Int16:
	case TypeKind::Int32:
	case TypeKind::Int64:
		return true;
	default:
		return false;
	};
}
