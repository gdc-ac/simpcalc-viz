#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
using namespace std;

enum class TokenType
{
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

struct Token
{
    TokenType type;
    string lexeme;
};

class Scanner
{
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

    bool isLetter(char c)
    {
        return isalpha(c) || c == '_';
    }

    bool isDigit(char c)
    {
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

    void skipWhitespaceAndComments()
    {
        while (true)
        {
            // skip whitespace
            while (position < input.size() && isspace(input[position]))
                position++;

            // skip comments
            if (position + 1 < input.size() && input[position] == '/' && input[position + 1] == '/')
            {
                position += 2;
                while (position < input.size() && input[position] != '\n')
                    position++;
                continue; // check for more whitespace/comments after this line
            }
            break;
        }
    }

    Token scanIdentifier()
    {
        size_t start = position;
        position++; // the first character is already a letter/underscore
        while (position < input.size() && (isLetter(input[position]) || isDigit(input[position])))
            position++;

        string word = input.substr(start, position - start);

        if (keywords.count(word))
            return {keywords[word], word};
        return {TokenType::IDENTIFIER, word};
    }

    Token scanNumber()
    {
        size_t start = position;

        // integer
        while (position < input.size() && isDigit(input[position]))
            position++;

        // floating point
        if (position < input.size() && input[position] == '.')
        {
            position++;
            if (position >= input.size() || !isDigit(input[position]))
            {
                return {TokenType::ERROR, input.substr(start, position - start)};
            } // fractional part
            while (position < input.size() && isDigit(input[position]))
                position++;
        }

        // exponential notation [digits][.digits] [e/E] [+/-/ε] [digits]
        if (position < input.size() && (input[position] == 'e' || input[position] == 'E'))
        {
            position++;
            if (input[position] == '+' || input[position] == '-')
                position++;
            else if (!isDigit(input[position]))
                return {TokenType::ERROR, input.substr(start, position - start)};
            while (position < input.size() && isDigit(input[position]))
                position++;
        }

        return {TokenType::NUMBER, input.substr(start, position - start)};
    }

    Token scanString()
    {
        size_t start = position;
        position++;

        while (position < input.size() && input[position] != '"')
        {
            if (input[position] == '\n')
                break;
            position++;
        }

        if (position >= input.size() || input[position] != '"')
        {
            return {TokenType::ERROR, input.substr(start - 1, position - start + 1)};
        }

        position++;
        string str = input.substr(start, position - start);
        return {TokenType::STRING, str};
    }

    Token scanOperator()
    {
        // multi-character operators
        if (position + 1 < input.size())
        {
            string mco = input.substr(position, 2);
            if (mco == ":=")
            {
                position += 2;
                return {TokenType::ASSIGN, mco};
            }
            if (mco == "**")
            {
                position += 2;
                return {TokenType::RAISE, mco};
            }
            if (mco == "<=")
            {
                position += 2;
                return {TokenType::LT_EQUAL, mco};
            }
            if (mco == ">=")
            {
                position += 2;
                return {TokenType::GT_EQUAL, mco};
            }
            if (mco == "!=")
            {
                position += 2;
                return {TokenType::NOT_EQUAL, mco};
            }
        }

        // single character operators
        char op = input[position++];
        switch (op)
        {
        case ';':
            return {TokenType::SEMICOLON, ";"};
        case ':':
            return {TokenType::COLON, ":"};
        case ',':
            return {TokenType::COMMA, ","};
        case '(':
            return {TokenType::LEFT_PAREN, "("};
        case ')':
            return {TokenType::RIGHT_PAREN, ")"};
        case '+':
            return {TokenType::PLUS, "+"};
        case '-':
            return {TokenType::MINUS, "-"};
        case '*':
            return {TokenType::MULTIPLY, "*"};
        case '/':
            return {TokenType::DIVIDE, "/"};
        case '<':
            return {TokenType::LESS_THAN, "<"};
        case '=':
            return {TokenType::EQUAL, "="};
        case '>':
            return {TokenType::GREATER_THAN, ">"};
        }

        return {TokenType::ERROR, string(0, op)};
    }

public:
    Scanner(const string &src) : input(src) {}

    Token getToken()
    {
        while (true)
        {
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

string tokenName(TokenType t)
{
    switch (t)
    {
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::NUMBER:
        return "NUM";
    case TokenType::STRING:
        return "STRING";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::COLON:
        return "COLON";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::MULTIPLY:
        return "MULTIPLY";
    case TokenType::DIVIDE:
        return "DIVIDE";
    case TokenType::RAISE:
        return "RAISE";
    case TokenType::LESS_THAN:
        return "LESS THAN";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::GREATER_THAN:
        return "GREATER THAN";
    case TokenType::LT_EQUAL:
        return "LTEQUAL";
    case TokenType::GT_EQUAL:
        return "GTEQUAL";
    case TokenType::NOT_EQUAL:
        return "NOTEQUAL";
    case TokenType::PRINT:
        return "PRINT";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::ENDIF:
        return "ENDIF";
    case TokenType::SQRT:
        return "SQRT";
    case TokenType::AND:
        return "AND";
    case TokenType::OR:
        return "OR";
    case TokenType::NOT:
        return "NOT";
    case TokenType::END_OF_FILE:
        return "ENDOFFILE";
    default:
        return "Error reading character ";
    }
}

int main()
{ // testing
    for (const auto &entry : filesystem::directory_iterator("."))
    {
        if (entry.path().extension() == ".txt")
        {
            string inputFile = entry.path().string();
            cout << "Scanning: " << inputFile << endl;

            ifstream file(inputFile);
            if (!file.is_open())
            {
                cerr << "Failed to open file: " << inputFile << endl;
                continue;
            }

            string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            Scanner scanner(code);

            string outputFile = entry.path().stem().string() + "_scan.txt";
            ofstream out(outputFile);
            if (!out.is_open())
            {
                cerr << "Failed to create output file: " << outputFile << endl;
                continue;
            }

            while (true)
            {
                Token t = scanner.getToken();
                out << tokenName(t.type);
                if (t.type != TokenType::END_OF_FILE)
                {
                    if (t.type == TokenType::ERROR)
                    {
                        out << "\"" << t.lexeme << "\"";
                    }
                    else
                    {
                        out << " " << t.lexeme;
                    }
                }

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