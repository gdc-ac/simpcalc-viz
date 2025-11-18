#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
using namespace std;

enum class TokenType {
    IDENTIFIER,
    NUMBER,
    STRING,
    ASSIGN,
    SEMICOLON,
    COLON,
    COMMA,
    LEFT_PAREN,
    RIGHT_PAREN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    RAISE,
    LESS_THAN,
    EQUAL,
    GREATER_THAN,
    LT_EQUAL,
    GT_EQUAL,
    NOT_EQUAL,
    PRINT,
    IF,
    ELSE,
    ENDIF,
    SQRT,
    AND,
    OR,
    NOT,
    END_OF_FILE,
    ERROR
};

struct Token {
    TokenType type;
    string lexeme;
};

class Scanner {
private:
    string input;
    size_t position = 0;
    unordered_map<string, TokenType> keywords = {
        {"PRINT", TokenType::PRINT},
        {"IF", TokenType::IF},
        {"ELSE", TokenType::ELSE},
        {"ENDIF", TokenType::ENDIF},
        {"SQRT", TokenType::SQRT},
        {"AND", TokenType::AND},
        {"OR", TokenType::OR},
        {"NOT", TokenType::NOT},
    };

    bool isLetter(char c) {
        return isalpha(c) || c == '_';
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    // void skipWhitespace() {
    //     while (position < input.size() && isspace(input[position]))
    //     position++;
    // }

    // void skipComment() {
    //     if (position + 1 < input.size() && input[position] == '/' && input[position+1] =='/') {
    //         position += 2;
    //         while (position < input.size() && input[position] != '\n')
    //             position++;
    //     }
    // }

    void skipWhitespaceAndComments() {
        while (true) {
            // skip whitespace
            while (position < input.size() && isspace(input[position]))
                position++;

            // skip comments
            if (position + 1 < input.size() && input[position] == '/' && input[position+1] == '/') {
                position += 2;
                while (position < input.size() && input[position] != '\n')
                    position++;
                continue; // check for more whitespace/comments after this line
            }
            break;
        }
    }

    Token scanIdentifier() {
        size_t start = position;
        position++; // the first character is already a letter/underscore
        while (position < input.size() && (isLetter(input[position]) || isDigit(input[position])))
            position++;
        
        string word = input.substr(start, position - start);

        if (keywords.count(word))
            return {keywords[word], word};
        return {TokenType::IDENTIFIER, word};
    }

    Token scanNumber() {
        size_t start = position;

        // integer
        while (position < input.size() && isDigit(input[position]))
            position++;

        // floating point
        if (position < input.size() && input[position] == '.') {
            position++;
            if (position >= input.size() || !isDigit(input[position])) {
                return {TokenType::ERROR, input.substr(start, position - start)};
            } // fractional part
            while (position < input.size() && isDigit(input[position]))
                position++;
        }

        // exponential notation [digits][.digits] [e/E] [+/-/ε] [digits]
        if (position < input.size() && (input[position] == 'e' || input[position] == 'E')) {
            position++;
            if (input[position] == '+' || input[position] == '-')
                position++;
            while (position < input.size() && isDigit(input[position]))
                position++;
        }

        return {TokenType::NUMBER, input.substr(start, position - start)};
    }

    Token scanString() {
        size_t start = position;
        position++;

        while (position < input.size() && input[position] != '"') {
            if (input[position] == '\n')
                break;
            position++;
        }

        if (position >= input.size() || input[position] != '"') {
            return {TokenType::ERROR, input.substr(start - 1, position - start + 1)};
        }

        position++;
        string str = input.substr(start, position - start);
        return {TokenType::STRING, str};
    }

    Token scanOperator() {
        // multi-character operators
        if (position + 1 < input.size()) {
            string mco = input.substr(position, 2);
            if (mco == ":=") {
                position += 2;
                return {TokenType::ASSIGN, mco};
            }
            if (mco == "**") {
                position += 2;
                return {TokenType::RAISE, mco};
            }
            if (mco == "<=") {
                position += 2;
                return {TokenType::LT_EQUAL, mco};
            }
            if (mco == ">=") {
                position += 2;
                return {TokenType::GT_EQUAL, mco};
            }
            if (mco == "!=") {
                position += 2;
                return {TokenType::NOT_EQUAL, mco};
            }
        }

        // single character operators
        char op = input[position++];
        switch (op) {
            case ';': return {TokenType::SEMICOLON, ";"};
            case ':': return {TokenType::COLON, ":"};
            case ',': return {TokenType::COMMA, ","};
            case '(': return {TokenType::LEFT_PAREN, "("};
            case ')': return {TokenType::RIGHT_PAREN, ")"};
            case '+': return {TokenType::PLUS, "+"};
            case '-': return {TokenType::MINUS, "-"};
            case '*': return {TokenType::MULTIPLY, "*"};
            case '/': return {TokenType::DIVIDE, "/"};
            case '<': return {TokenType::LESS_THAN, "<"};
            case '=': return {TokenType::EQUAL, "="};
            case '>': return {TokenType::GREATER_THAN, ">"};
        }

        return {TokenType::ERROR, string(1, op)};

    }

public:

    Scanner(const string& src) : input(src) {}

    Token getToken() {
        while (true) {         
            skipWhitespaceAndComments();

            if (position >= input.size())
                return {TokenType::END_OF_FILE, ""};
            
            char c = input[position];

            if (isLetter(c))
                return scanIdentifier();

            if (isDigit(c))
                return scanNumber();

            if (c == '"')
                return scanString();

            return scanOperator();

        }
    }
 
};

string tokenName(TokenType t) {
    switch (t) {
        case TokenType::IDENTIFIER: return "Identifier";
        case TokenType::NUMBER: return "Number";
        case TokenType::STRING: return "String";
        case TokenType::ASSIGN: return "Assign";
        case TokenType::SEMICOLON: return "Semicolon";
        case TokenType::COLON: return "Colon";
        case TokenType::COMMA: return "Comma";
        case TokenType::LEFT_PAREN: return "LeftParen";
        case TokenType::RIGHT_PAREN: return "RightParen";
        case TokenType::PLUS: return "Plus";
        case TokenType::MINUS: return "Minus";
        case TokenType::MULTIPLY: return "Multiply";
        case TokenType::DIVIDE: return "Divide";
        case TokenType::RAISE: return "Raise";
        case TokenType::LESS_THAN: return "LessThan";
        case TokenType::EQUAL: return "Equal";
        case TokenType::GREATER_THAN: return "GreaterThan";
        case TokenType::LT_EQUAL: return "LTEqual";
        case TokenType::GT_EQUAL: return "GTEqual";
        case TokenType::NOT_EQUAL: return "NotEqual";
        case TokenType::PRINT: return "Print";
        case TokenType::IF: return "If";
        case TokenType::ELSE: return "Else";
        case TokenType::ENDIF: return "Endif";
        case TokenType::SQRT: return "Sqrt";
        case TokenType::AND: return "And";
        case TokenType::OR: return "Or";
        case TokenType::NOT: return "Not";
        case TokenType::END_OF_FILE: return "EndofFile";
        default: return "Error";
    }
}

int main() { //testing
    for (const auto& entry : filesystem::directory_iterator(".")) {
        if (entry.path().extension() == ".txt") {
            string inputFile = entry.path().string();
            cout << "Scanning: " << inputFile << endl;

            ifstream file(inputFile);
            if (!file.is_open()) {
                cerr << "Failed to open file: " << inputFile << endl;
                continue;
            }

            string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            Scanner scanner(code);

            string outputFile = entry.path().stem().string() + "_scan.txt";
            ofstream out(outputFile);
            if (!out.is_open()) {
                cerr << "Failed to create output file: " << outputFile << endl;
                continue;
            }

            while(true) {
                Token t = scanner.getToken();
                out << tokenName(t.type);
                if (t.type != TokenType::END_OF_FILE)
                    out << " " << t.lexeme;
                out << "\n";

                if (t.type == TokenType::END_OF_FILE)
                    break;

            }

            out.close();
            cout << "Output written to: " << outputFile << endl;

        }
    }

   return 0;

}