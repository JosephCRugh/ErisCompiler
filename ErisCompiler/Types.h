#ifndef ERIS_TYPES_H
#define ERIS_TYPES_H

namespace eris {

	enum class TypeKind {
		
		// Integers
		Int8,
		Int16,
		Int32,
		Int64,

		UnsignedInt8,
		UnsignedInt16,
		UnsignedInt32,
		UnsignedInt64,

		// Other
		ERROR,
		VOID,
		
	};

	class Type {
	public:

		Type(TypeKind Kind)
			: Kind(Kind) {}

		TypeKind GetKind() const { return Kind; }

		bool Is(Type* Ty) const;
		inline bool IsNot(Type* Ty) const { return !Is(Ty); }

	private:
		TypeKind Kind;
	};
}

#endif // ERIS_TYPES_H