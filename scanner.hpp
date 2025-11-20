#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>

// We use an enumerator for all token types.
enum class TokenType {
    IDENTIFIER, NUMBER, STRING,
    ASSIGN, SEMICOLON, COLON, COMMA,
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE, RAISE,
    LESS_THAN, EQUAL, GREATER_THAN,
    LT_EQUAL, GT_EQUAL, NOT_EQUAL,
    PRINT, IF, ELSE, ENDIF,
    SQRT, AND, OR, NOT,
    END_OF_FILE, ERROR
};

using enum TokenType;
using namespace std;

// We define a token as a struct with two fields: a TokenType and a string containing its lexeme.
struct Token {
    TokenType type;
    string lexeme;
};

string tokenName(TokenType t);

class Scanner {
public:
    Scanner(const string& src);
    Token getToken();
    vector<string> line_outputs;

private:
    string input;
    size_t position = 0; // position at the start of the string

    unordered_map<string, TokenType> keywords;

    bool isLetter(char c);
    bool isDigit(char c);
    void skipWhitespaceAndComments();
    Token scanIdentifier();
    Token scanNumber();
    Token scanString();
    Token scanOperator();
};

#endif
