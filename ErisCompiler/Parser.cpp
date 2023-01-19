#include "Parser.h"

#include <llvm/Support/raw_ostream.h>
#include <stack>

#include "ErisContext.h"
#include "Types.h"

namespace eris {
	template<typename N>
	static inline N* NewNode(Token STok) {
		N* Node = new N;
		Node->Loc = STok.Loc;
		return Node;
	}
}

eris::Parser::Parser(ErisContext& Context, const SourceBuf Buffer, FileUnit* Unit)
	: Context(Context), Lex(Context, Unit->Log, Buffer), Unit(Unit), Log(Unit->Log) {
}

void eris::Parser::Parse() {
	NextToken(); // Prime the parser.

	FuncDecl* Func = ParseFuncDecl(ParseType());
	// TODO: Will want to check here for function redeclaration.
	Unit->GlobalFuncs.insert({ Func->Name, Func });

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
	Func->Stmts.push_back(ParseStmt());

	Match('}');

	return Func;
}

//===-------------------------------===//
// Statements
//===-------------------------------===//

eris::AstNode* eris::Parser::ParseStmt() {
	AstNode* Stmt = ParseReturn();
	Match(';');
	return Stmt;
}

eris::ReturnStmt* eris::Parser::ParseReturn() {
	
	ReturnStmt* Ret = NewNode<ReturnStmt>(CTok);
	NextToken(); // Consuming 'return' token.

	if (CTok.IsNot(';')) {
		Ret->Value = ParseExpr();
	}

	return Ret;
}

//===-------------------------------===//
// Expressions
//===-------------------------------===//

eris::Expr* eris::Parser::ParseExpr() {
	return ParseBinaryExpr();
}

eris::Expr* eris::Parser::ParseBinaryExpr() {
	Expr* LHS = ParsePrimaryExpr();

	// Since some operations have to be delayed
	// because of order of operations a stack
	// is formed keeping a backlog of those operations
	// that need to be processed later
	struct StackUnit {
		Token Op;
		Expr* E;
	};
	std::stack<StackUnit> OpStack;

	auto NewBinaryOp = [](Token OpTok, Expr* LHS, Expr* RHS) {
		BinaryOp* BinOp = NewNode<BinaryOp>(OpTok);
		BinOp->Op  = static_cast<usize>(OpTok.Kind);
		BinOp->LHS = LHS;
		BinOp->RHS = RHS;
		BinOp->Ty = LHS->Ty; // TODO: REMOVE (and replace with semantic checking)
		return BinOp;
	};

	Token Op = CTok;
	Token NextOp;
	llvm::DenseMap<u16, u32>::iterator OpItr;
	while ((OpItr = Context.BinaryOpsPrecedence.find(Op.Kind))
		     != Context.BinaryOpsPrecedence.end()) {
		NextToken(); // Consuming the operator

		llvm::DenseMap<u16, u32>::iterator NextOpItr;
		Expr* RHS = ParsePrimaryExpr();
		NextOp = CTok;
		bool MoreOperators = (NextOpItr = Context.BinaryOpsPrecedence.find(NextOp.Kind))
			                    != Context.BinaryOpsPrecedence.end();
		if (MoreOperators && NextOpItr->second > OpItr->second) {
			// Delaying the operation until later since the next operator has a
			// higher precedence.
			StackUnit Unit = StackUnit{ Op, LHS };
			OpStack.push(Unit);
			LHS = RHS;
			Op  = NextOp;
		} else {
			LHS = NewBinaryOp(Op, LHS, RHS);

			while (!OpStack.empty()) {
				RHS = LHS;
				StackUnit Unit = OpStack.top();
				// Still possible to have the right side have higher precedence.
				if (MoreOperators &&
					Context.BinaryOpsPrecedence[NextOp.Kind] > Context.BinaryOpsPrecedence[Unit.Op.Kind]) {
					LHS = RHS;
					Op = NextOp;
					break;
				}

				OpStack.pop();
				LHS = Unit.E;

				// Apply the binary operator!
				LHS = NewBinaryOp(Unit.Op, LHS, RHS);
			}
			Op = CTok;
		}
	}

	return LHS;
}

eris::Expr* eris::Parser::ParsePrimaryExpr() {
	switch (CTok.Kind) {
	// ---- Literals ----
	case TokenKind::INT_LITERAL: return ParseIntLiteral();
	// ---- Other ----
	default:
		Error(CTok, "Expected an expression");
		ErrorNode* Err = NewNode<ErrorNode>(CTok);
		NextToken(); // Shouldn't be needed but helps prevent endless looping
		SkipRecovery();
		return Err;
	}
}

eris::NumberLiteral* eris::Parser::ParseIntLiteral() {

	llvm::StringRef Text = CTok.Loc.Text;

	// TODO: Check for overflow
	usize Idx = 0;
	u64 IntValue = 0;
	while (Idx < Text.size()) {
		c8 C = Text[Idx];
		if (C == '\'') continue;
		if (!std::isdigit(C)) break;
		++Idx;

		IntValue  = IntValue * 10 + ((u64)C - '0');
	}

	NumberLiteral* Number = NewNode<NumberLiteral>(CTok);

	bool Unsigned = false;
	if (IntValue <= std::numeric_limits<i32>::max()) {
		Number->Ty = Context.Int32Type;
	} else if (IntValue <= std::numeric_limits<i64>::max()) {
		Number->Ty = Context.Int64Type;
	} else {
		Number->Ty = Context.UInt64Type;
		Unsigned = true;
	}

	if (Unsigned) {
		Number->UnsignedIntValue = IntValue;
	} else {
		Number->SignedIntValue = IntValue;
	}

	NextToken();
	return Number;
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

void eris::Parser::SkipRecovery() {
	while (true) {
		switch (CTok.Kind) {
		// statement keywords
		case TokenKind::KW_RETURN:
		// Other
		case TokenKind::TK_EOF:
		case static_cast<TokenKind>(';'):
			return;
		default:
			// Skip and continue
			NextToken();
			break;
		}
	}
}