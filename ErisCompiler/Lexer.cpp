#include "Lexer.h"

#include "ErisContext.h"

eris::Lexer::Lexer(const ErisContext& Context, Logger& Log, const SourceBuf Buffer)
	: Context(Context), Log(Log), CurPtr(Buffer.Memory) {
}

eris::Token eris::Lexer::NextToken() {

	// When skipping tokens this label is used
	// to jumps back and start over.
restartLex:

	const c8* TokStart = CurPtr;

	switch (*CurPtr++) {
	// Whitespace
	case ' ':
	case '\t':
	case '\v':
	case '\f':
		goto restartLex;
	case '\n':
		++LineNumber;
		goto restartLex;
	case '\r':
		++LineNumber;
		if (*CurPtr == '\n') {
			++CurPtr;
		}
		goto restartLex;
	case 'a': case 'b': case 'c': case 'd': case 'e':
	case 'f': case 'g': case 'h': case 'i': case 'j':
	case 'k': case 'l': case 'm': case 'n': case 'o':
	case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y':
	case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E':
	case 'F': case 'G': case 'H': case 'I': case 'J':
	case 'K': case 'L': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y':
	case 'Z':
	case '_':
		return NextWord();
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return NextNumber();
	case '(': return CreateToken('(', TokStart);
	case ')': return CreateToken(')', TokStart);
	case '{': return CreateToken('{', TokStart);
	case '}': return CreateToken('}', TokStart);
	case '+': return CreateToken('+', TokStart);
	case '-': return CreateToken('-', TokStart);
	case '*': return CreateToken('*', TokStart);
	case '/': return CreateToken('/', TokStart);
	case '%': return CreateToken('%', TokStart);
	case '\0':

		return CreateToken(TokenKind::TK_EOF, TokStart);
	default:
		Error(TokStart, "Unknown character: '%s'", CreateText(TokStart));
		goto restartLex;
	}
}

eris::Token eris::Lexer::NextWord() {

	const c8* TokStart = CurPtr - 1; // -1 since initial switch table consumes one character.

	while (std::isalpha(*CurPtr) || std::isdigit(*CurPtr) || *CurPtr == '_') {
		++CurPtr;
	}

	llvm::StringRef Text = CreateText(TokStart);

	// Checking if it is a keyword.
	TokenKind KeywordKind = Context.GetKeywordKind(Text);
	if (KeywordKind != TokenKind::INVALID) {
		return CreateToken(KeywordKind, CreateText(TokStart));
	}

	// Not a keyword, so it is an identifier.
	return CreateToken(TokenKind::IDENT, Text);
}

eris::Token eris::Lexer::NextNumber() {

	const c8* TokStart = CurPtr - 1;  // -1 since initial switch table consumes one character.
	
	// Leading whole digits [0-9']+
	while (std::isdigit(*CurPtr) || *CurPtr == '\'') {
		++CurPtr;
	}

	return CreateToken(TokenKind::INT_LITERAL, TokStart);
}
