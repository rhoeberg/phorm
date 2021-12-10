#include <stdio.h>
#include <iostream>

#include "util.h"
#include "util.cpp"
#include "vm_array.h"
#include "vm_array.cpp"

enum TokenType {
	TOKEN_END_OF_STREAM,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_BRACKET_OPEN,
	TOKEN_BRACKET_CLOSE,
	TOKEN_BRACE_OPEN,
	TOKEN_BRACE_CLOSE,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	TOKEN_ASTERIX,
	TOKEN_MINUS,
	TOKEN_DOT,
	TOKEN_COMMA,
	TOKEN_STRING,
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	TOKEN_NUMBER_DECIMAL,

	TOKEN_UNKOWN,
};

struct Tokenizer {
	char *at;
};

struct Token {
	char *at;
	int length;
	TokenType type;
};

struct TokenizedFile {
	VMArray<Token> tokens;
	char *source;
};

// bool IdentifierIsEqual(Token token, char *string)
// {
// 	if(token.type != TOKEN_IDENTIFIER)
// 		return false;

// 	for(int i = 0; i < token.length; i++) {
// 		if(string[i] && string[i] != '\0') {
// 			if(string
// 		}
// 	}
// }

void EatWhitespace(Tokenizer *tokenizer)
{
	while(true) {
		char c = tokenizer->at[0];
		if(c == ' ' ||
		   c == '\t' ||
		   c == '\n' ||
		   c == '\r' ||
		   c == '\v' ||
		   c == '\f') {
			tokenizer->at++;
			continue;
		}
		break;
	}
}


bool IsAlpha(char C)
{
    bool Result = (((C >= 'a') && (C <= 'z')) ||
                   ((C >= 'A') && (C <= 'Z')));

    return(Result);
}

bool IsNumeric(char C)
{
    bool Result = ((C >= '0') && (C <= '9'));
    
    return(Result);
}

Token ParseToken(Tokenizer *tokenizer)
{
	EatWhitespace(tokenizer);

	Token token = {};
	token.length = 1;
	token.at = tokenizer->at;
	switch(tokenizer->at[0]) {
		case '\0': {token.type = TOKEN_END_OF_STREAM; break;}
		case '(': {token.type = TOKEN_PAREN_OPEN; break;}
		case ')': {token.type = TOKEN_PAREN_CLOSE; break;}
		case '[': {token.type = TOKEN_BRACKET_OPEN; break;}
		case ']': {token.type = TOKEN_BRACKET_CLOSE; break;}
		case '{': {token.type = TOKEN_BRACE_OPEN; break;}
		case '}': {token.type = TOKEN_BRACE_CLOSE; break;}
		case ':': {token.type = TOKEN_COLON; break;}
		case ';': {token.type = TOKEN_SEMICOLON; break;}
		case '*': {token.type = TOKEN_ASTERIX; break;}
		case '-': {token.type = TOKEN_MINUS; break;}
		case '.': {token.type = TOKEN_DOT; break;}
		case ',': {token.type = TOKEN_COMMA; break;}
		case '"': {
			token.type = TOKEN_STRING;
			token.at = ++tokenizer->at;
			char *current = token.at;
			while(current[0] &&
				  current[0] != '"') {
				++current;
			}
			token.length = current - token.at;
			++tokenizer->at;
			break;
		}

		default: {
			if(IsAlpha(token.at[0])) {
				// handle identifier
				char *current = token.at;
				token.type = TOKEN_IDENTIFIER;
				while(IsAlpha(current[0]) || IsNumeric(current[0]) ||
					  current[0] == '_') {
					++current;
				}
				token.length = current - token.at;
			}
			else if(IsNumeric(token.at[0])) {
				// handle number
				token.type = TOKEN_NUMBER;
				char *current = token.at;
				while(IsNumeric(current[0]) ||
					  current[0] == '.') {
					++current;
				}
				token.length = current - token.at;
			}
			else {
				token.type = TOKEN_UNKOWN; break;
			}
		}
	}

	tokenizer->at += token.length;

	return token;
}

TokenizedFile TokenizeFile(const char *path)
{
	TokenizedFile result = {};

	char *file = readFile("test.cpp");

	Tokenizer tokenizer = {};
	tokenizer.at = file;



	while(tokenizer.at[0] != 0) {
		Token token = ParseToken(&tokenizer);
		result.tokens.Insert(token);
		// EatWhitespace(&tokenizer);
		// printf("c:%c\n", tokenizer.at[0]);
	}

	return result;
}

void FreeTokenizedFile(TokenizedFile *tokenizedFile)
{
	free(tokenizedFile->source);
}


// int main(int argc, char *argv[])
// {
// 	char *file = readFile("test.cpp");

// 	Tokenizer tokenizer = {};
// 	tokenizer.at = file;

// 	VMArray<Token> tokens;

// 	while(tokenizer.at[0] != 0) {
// 		Token token = ParseToken(&tokenizer);
// 		tokens.Insert(token);
// 		// EatWhitespace(&tokenizer);
// 		// printf("c:%c\n", tokenizer.at[0]);
// 	}

// 	for(int i = 0; i < tokens.Count(); i++) {
// 		printf("TOKEN:%d\t%.*s\n", tokens[i].type, tokens[i].length, tokens[i].at);
// 	}
// }
