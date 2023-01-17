#include "Parser.h"

#include <llvm/Support/raw_ostream.h>

#include "ErisContext.h"
#include "Types.h"

eris::Parser::Parser(ErisContext& Context, const SourceBuf Buffer, FileUnit* Unit)
	: Context(Context), Lex(Context, Buffer), Unit(Unit), Log(Unit->Log) {
}

void eris::Parser::Parse() {
	NextToken(); // Prime the parser.

	FuncDecl* Func = ParseFuncDecl(ParseType());
	// TODO: Will want to check here for function redeclaration.
	Unit->Funcs.insert({ Func->Name, Func });

}

eris::Type* eris::Parser::ParseType() {
	eris::Type* Ty = nullptr;
	switch (CTok.Kind) {
	case TokenKind::KW_I8:   Ty = Context.Int8Type;   NextToken(); break;
	case TokenKind::KW_I16:  Ty = Context.Int16Type;  NextToken(); break;
	case TokenKind::KW_I32:  Ty = Context.Int32Type;  NextToken(); break;
	case TokenKind::KW_I64:  Ty = Context.Int64Type;  NextToken(); break;
	case TokenKind::KW_U8:   Ty = Context.UInt8Type;  NextToken(); break;
	case TokenKind::KW_U16:  Ty = Context.UInt16Type; NextToken(); break;
	case TokenKind::KW_U32:  Ty = Context.UInt32Type; NextToken(); break;
	case TokenKind::KW_U64:  Ty = Context.UInt64Type; NextToken(); break;
	case TokenKind::KW_VOID: Ty = Context.VoidType;   NextToken(); break;
	default:
		Error(CTok, "Expected valid type");
		return Context.ErrorType;
	}

	return Ty;
}

eris::FuncDecl* eris::Parser::ParseFuncDecl(Type* RetTy) {

	FuncDecl* Func = new FuncDecl;
	Func->RetTy = RetTy;

	Func->Name = ParseIdentifier("Expected an identifier for function declaration");
	Match('(');
	Match(')');
	Match('{');
	Match('}');

	return Func;
}

//===-------------------------------===//
// Utilities
//===-------------------------------===//

void eris::Parser::NextToken() {
	PrevToken = CTok;
	if (SavedTokensCount) {
		CTok = SavedTokens[0];
		std::rotate(SavedTokens, SavedTokens + 1, SavedTokens + SavedTokensCount);
		--SavedTokensCount;
	} else {
		CTok = Lex.NextToken();
	}
}

eris::Identifier eris::Parser::ParseIdentifier(const c8* ErrorMessage) {
	if (CTok.IsNot(TokenKind::IDENT)) {
		// TODO: Report error!
		return Identifier(); // Returning a null identifier.
	}
	llvm::StringRef Text = CTok.Loc.Text;
	NextToken(); // Consuming ident token.
	return Identifier(Text);
}

eris::Token eris::Parser::PeekToken(u32 n) {
	assert(n != 0 && "Cannot peek zero tokens");
	assert(n < MAX_SAVED_TOKENS && "Cannot peek more than the maximum peek token amount");
	for (u32 i = 0; i < SavedTokensCount; i++) {
		SavedTokens[SavedTokensCount++] = Lex.NextToken();
	}
	return SavedTokens[SavedTokensCount - 1];
}

void eris::Parser::Match(TokenKind Kind, const c8* Purpose) {
	if (CTok.Is(Kind)) {
		NextToken(); // Consuming the matched token.
		return;
	}
	Error(PrevToken, "Expected '%s'%s%s", Token::TokenKindToString(Context, Kind),
		Purpose ? " " : "", Purpose);
}
