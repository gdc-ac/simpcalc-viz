#include "scanner.hpp"
#include <cctype>

// The scanner object takes in a string input, through which tokens are detected.
Scanner::Scanner(const string &src) : input(src) {
    keywords = {
        {"PRINT", PRINT},
        {"IF", IF},
        {"ELSE", ELSE},
        {"ENDIF", ENDIF},
        {"SQRT", SQRT},
        {"AND", AND},
        {"OR", OR},
        {"NOT", NOT},
    };
}

// Check if the current character being looked at is a digit 
bool Scanner::isLetter(char c)
{
    return isalpha(c) || c == '_';
}

// Check if the current character being looked at is a digit 
bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// Skip over any whitespaces and comments
void Scanner::skipWhitespaceAndComments()
{
    while (true)
    {
        // Skip whitespace if detected
        while (position < input.size() && isspace(input[position]))
            position++;

        // Skip comments, if the "//" signifier is detected
        if (position + 1 < input.size() && input[position] == '/' && input[position + 1] == '/')
        {
            position += 2;
            while (position < input.size() && input[position] != '\n')
                position++;
            continue; // Check for more whitespaces/comments after this line
        }
        break;
    }
}

// Scan for any identifiers
Token Scanner::scanIdentifier()
{
    size_t start = position;
    position++; // the first character is already a letter/underscore
    while (position < input.size() && (isLetter(input[position]) || isDigit(input[position])))
        position++;

    string word = input.substr(start, position - start);

    if (keywords.count(word))
        return {keywords[word], word};
    return {IDENTIFIER, word}; // if successful, create an IDENTIFIER token 
}

// Scan for any numbers
Token Scanner::scanNumber()
{
    size_t start = position;

    // For integers 
    while (position < input.size() && isDigit(input[position]))
        position++;

    // For floating points
    if (position < input.size() && input[position] == '.')
    {
        position++;
        if (position >= input.size() || !isDigit(input[position]))
        {
            return {ERROR, input.substr(start, position - start)};
        } // For the fractional part
        while (position < input.size() && isDigit(input[position]))
            position++;
    }

    // For exponential notation [digits][.digits] [e/E] [+/-/ε] [digits]
    if (position < input.size() && (input[position] == 'e' || input[position] == 'E'))
    {
        position++;

        if (input[position] == '+' || input[position] == '-')
            position++;
        if (!isDigit(input[position]))
        {
            position++;
            return {ERROR, input.substr(start, position - start - 1)};
        }
        while (position < input.size() && isDigit(input[position]))
            position++;
    }

    return {NUMBER, input.substr(start, position - start)};
}

// Scan for any strings enclosed in quotation marks
Token Scanner::scanString()
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
        return {ERROR, input.substr(start - 1, position - start + 1)};
    }

    position++;
    string str = input.substr(start, position - start);
    return {STRING, str};
}

// Scan for any operators
Token Scanner::scanOperator()
{
    // For two-character operators
    if (position + 1 < input.size())
    {
        string mco = input.substr(position, 2);
        if (mco == ":=")
        {
            position += 2;
            return {ASSIGN, mco};
        }
        if (mco == "**")
        {
            position += 2;
            return {RAISE, mco};
        }
        if (mco == "<=")
        {
            position += 2;
            return {LT_EQUAL, mco};
        }
        if (mco == ">=")
        {
            position += 2;
            return {GT_EQUAL, mco};
        }
    }

    // For single-character operators
    char op = input[position++];
    switch (op)
    {
    case ';':
        return {SEMICOLON, ";"};
    case ':':
        return {COLON, ":"};
    case ',':
        return {COMMA, ","};
    case '(':
        return {LEFT_PAREN, "("};
    case ')':
        return {RIGHT_PAREN, ")"};
    case '+':
        return {PLUS, "+"};
    case '-':
        return {MINUS, "-"};
    case '*':
        return {MULTIPLY, "*"};
    case '/':
        return {DIVIDE, "/"};
    case '<':
        return {LESS_THAN, "<"};
    case '=':
        return {EQUAL, "="};
    case '>':
        return {GREATER_THAN, ">"};
    case '!': // The exclamation point is a special case as it can either be NOT_EQUAL or not valid at all.
        char next = input[position++];
        switch (next)
        {
        case '=':
            return {NOT_EQUAL, "!="};
        default:
            return {ERROR, string(1, op)};
        }
    }

    return {ERROR, string(0, op)};
}

// We define our Scanner constructor to take in a string as input

// Gets a token. Whitespaces and comments are skipped over. According to the current character being checked, scan for its corresponding type of token.
Token Scanner::getToken() {
    Token returned_token;
    while (true) {
        skipWhitespaceAndComments();
        if (position >= input.size()) {
            returned_token = {END_OF_FILE, ""};
            break;
        }
        char c = input[position];
        if (isLetter(c)) returned_token = scanIdentifier();
        if (isDigit(c)) returned_token = scanNumber();
        if (c == '"') returned_token = scanString();
        returned_token = scanOperator();
    }

    string line_output = tokenName(returned_token.type);
    if (returned_token.type != END_OF_FILE) {
        if (returned_token.type == ERROR)
            line_output += "\"" + returned_token.lexeme + "\"";
        else
            line_output += " " + returned_token.lexeme;
    }
    line_outputs.push_back(line_output);

    return returned_token;
}

// A function that returns a certain token type string
string tokenName(TokenType t){
    switch (t){
        case IDENTIFIER:
            return "Identifier";
        case NUMBER:
            return "Num";
        case STRING:
            return "String";
        case ASSIGN:
            return "Assign";
        case SEMICOLON:
            return "Semicolon";
        case COLON:
            return "Colon";
        case COMMA:
            return "Comma";
        case LEFT_PAREN:
            return "LeftParen";
        case RIGHT_PAREN:
            return "RightParen";
        case PLUS:
            return "Plus";
        case MINUS:
            return "Minus";
        case MULTIPLY:
            return "Multiply";
        case DIVIDE:
            return "Divide";
        case RAISE:
            return "Raise";
        case LESS_THAN:
            return "LessThan";
        case EQUAL:
            return "Equal";
        case GREATER_THAN:
            return "GreaterThan";
        case LT_EQUAL:
            return "LTEqual";
        case GT_EQUAL:
            return "GTEqual";
        case NOT_EQUAL:
            return "NotEqual";
        case PRINT:
            return "Print";
        case IF:
            return "If";
        case ELSE:
            return "Else";
        case ENDIF:
            return "EndIf";
        case SQRT:
            return "Sqrt";
        case AND:
            return "And";
        case OR:
            return "Or";
        case NOT:
            return "Not";
        case END_OF_FILE:
            return "Endoffile";
        default:
            return "Error ";
    }
}
