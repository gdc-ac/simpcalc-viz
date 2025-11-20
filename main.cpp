#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <vector>
using namespace std;

// We use an enumerator for all token types.
enum class TokenType {
    IDENTIFIER, NUMBER, STRING, ASSIGN, SEMICOLON, COLON, COMMA,
    LEFT_PAREN, RIGHT_PAREN, PLUS, MINUS, MULTIPLY, DIVIDE, RAISE,
    LESS_THAN, EQUAL, GREATER_THAN, LT_EQUAL, GT_EQUAL, NOT_EQUAL,
    PRINT, IF, ELSE, ENDIF, SQRT, AND, OR, NOT, END_OF_FILE, ERROR
};

// We define a token as a struct with two fields: a TokenType and a string containing its lexeme.
struct Token {
    TokenType type;
    string lexeme;
};

// A string function that returns the type of token 
string tokenName(TokenType t) {
    switch(t){
        case TokenType::IDENTIFIER: return "Identifier"; case TokenType::NUMBER: return "Number";
        case TokenType::STRING: return "String"; case TokenType::ASSIGN: return "Assign";
        case TokenType::SEMICOLON: return "Semicolon"; case TokenType::COLON: return "Colon";
        case TokenType::COMMA: return "Comma"; case TokenType::LEFT_PAREN: return "LeftParen";
        case TokenType::RIGHT_PAREN: return "RightParen"; case TokenType::PLUS: return "Plus";
        case TokenType::MINUS: return "Minus"; case TokenType::MULTIPLY: return "Multiply";
        case TokenType::DIVIDE: return "Divide"; case TokenType::RAISE: return "Raise";
        case TokenType::LESS_THAN: return "LessThan"; case TokenType::EQUAL: return "Equal";
        case TokenType::GREATER_THAN: return "GreaterThan"; case TokenType::LT_EQUAL: return "LTEqual";
        case TokenType::GT_EQUAL: return "GTEqual"; case TokenType::NOT_EQUAL: return "NotEqual";
        case TokenType::PRINT: return "Print"; case TokenType::IF: return "If";
        case TokenType::ELSE: return "Else"; case TokenType::ENDIF: return "EndIf";
        case TokenType::SQRT: return "Sqrt"; case TokenType::AND: return "And";
        case TokenType::OR: return "Or"; case TokenType::NOT: return "Not";
        case TokenType::END_OF_FILE: return "EndofFile"; default: return "Error";
    }
}

// We define a scanner module that has different functions for retrieving tokens.
class Scanner {
private:
    string input; // Input string to the scanner
    size_t position = 0; // The start of the string 
    unordered_map<string, TokenType> keywords = {
        {"PRINT", TokenType::PRINT}, {"IF", TokenType::IF}, {"ELSE", TokenType::ELSE},
        {"ENDIF", TokenType::ENDIF}, {"SQRT", TokenType::SQRT}, {"AND", TokenType::AND},
        {"OR", TokenType::OR}, {"NOT", TokenType::NOT}
    };

    // Check if the current character of the string is a letter 
    bool isLetter(char c) { return isalpha(c) || c == '_'; }
    // Check if the current character of the string is a digit
    bool isDigit(char c) { return isdigit(c); }

    // Skip all whitespaces and comments
    void skipWhitespaceAndComments() {
        while (true) {
            while (position < input.size() && isspace(input[position])) position++;
            if (position + 1 < input.size() && input[position] == '/' && input[position+1] == '/') {
                position += 2; // Checking the start of comments by "//"
                while (position < input.size() && input[position] != '\n') position++;
                continue;
            }
            break;
        }
    }

    // Scanning for any identifier tokens
    Token scanIdentifier() {
        size_t start = position++;
        while (position < input.size() && (isLetter(input[position]) || isDigit(input[position]))) position++;
        string word = input.substr(start, position - start);
        if (keywords.count(word)) return {keywords[word], word};
        return {TokenType::IDENTIFIER, word};
    }

    // Scanning for any numbers
    Token scanNumber() {
        size_t start = position;
        while (position < input.size() && isDigit(input[position])) position++;
        // Checking for decimal notation
        if (position < input.size() && input[position] == '.') {
            position++;
            if (position >= input.size() || !isDigit(input[position])) return {TokenType::ERROR, input.substr(start, position-start)};
            while (position < input.size() && isDigit(input[position])) position++;
        }
        // Checking for exponential notation
        if (position < input.size() && (input[position]=='e' || input[position]=='E')) {
            position++;
            if (position < input.size() && (input[position]=='+' || input[position]=='-')) position++;
            if (position >= input.size() || !isDigit(input[position])) return {TokenType::ERROR, input.substr(start, position-start)};
            while (position < input.size() && isDigit(input[position])) position++;
        }
        return {TokenType::NUMBER, input.substr(start, position-start)};
    }

    // Scanning for any strings
    Token scanString() {
        size_t start = position++;
        while (position < input.size() && input[position] != '"') {
            if (input[position]=='\n') break;
            position++;
        }
        if (position >= input.size() || input[position]!='"') return {TokenType::ERROR, input.substr(start-1, position-start+1)};
        position++;
        return {TokenType::STRING, input.substr(start, position-start)};
    }

    // Scanning for any operators
    Token scanOperator() {
        // Scanning for two-character operators
        if (position + 1 < input.size()) {
            string mco = input.substr(position, 2);
            if (mco==":="){ position+=2; return {TokenType::ASSIGN, mco}; }
            if (mco=="**"){ position+=2; return {TokenType::RAISE, mco}; }
            if (mco=="<="){ position+=2; return {TokenType::LT_EQUAL, mco}; }
            if (mco==">="){ position+=2; return {TokenType::GT_EQUAL, mco}; }
        }
        // Scanning for single-character operators
        char op = input[position++];
        switch(op){
            case ';': return {TokenType::SEMICOLON, ";"}; case ':': return {TokenType::COLON, ":"};
            case ',': return {TokenType::COMMA, ","}; case '(': return {TokenType::LEFT_PAREN, "("};
            case ')': return {TokenType::RIGHT_PAREN, ")"}; case '+': return {TokenType::PLUS, "+"};
            case '-': return {TokenType::MINUS, "-"}; case '*': return {TokenType::MULTIPLY, "*"};
            case '/': return {TokenType::DIVIDE, "/"}; case '<': return {TokenType::LESS_THAN, "<"};
            case '=': return {TokenType::EQUAL, "="}; case '>': return {TokenType::GREATER_THAN, ">"};
            case '!':
                // The "!" character is a special case as it is either (1) a NOT_EQUAL token or (2) an error  
                if (position < input.size() && input[position]=='='){ position++; return {TokenType::NOT_EQUAL, "!="}; }
                else return {TokenType::ERROR, "!"};
        }
        return {TokenType::ERROR, string(1, op)};
    }

// Construct
public:
    Scanner(const string &src) : input(src) {}
    
    Token getToken() {
        skipWhitespaceAndComments();
        if (position >= input.size()) return {TokenType::END_OF_FILE, ""};
        char c = input[position];
        if (isLetter(c)) return scanIdentifier();
        if (isDigit(c)) return scanNumber();
        if (c=='"') return scanString();
        return scanOperator();
    }

    vector<Token> scanAll(ofstream &outScan) {
        vector<Token> tokens;
        while (true) {
            Token t = getToken();
            tokens.push_back(t);
            outScan << tokenName(t.type) << " " << t.lexeme << "\n";
            if (t.type == TokenType::END_OF_FILE) break;
        }
        return tokens;
    }
};

class Parser {
private:
    vector<Token> tokens;
    size_t pos = 0;
    ofstream &outParse;
    string filename;
    bool errorFlag = false;

    Token current() { return tokens[pos]; }
    void nextToken() { if(pos < tokens.size()-1) pos++; }

    void match(TokenType expected) {
        if (errorFlag) return;
        if (current().type == expected) nextToken();
        else {
            outParse << "Parse Error: " << tokenName(expected) << " expected.";
            errorFlag = true;
        }
    }

    void Prg() { 
        Blk(); 
        if(!errorFlag){ 
            match(TokenType::END_OF_FILE); 
            if(!errorFlag) 
                outParse << filename << " is a valid SimpCalc program\n"; 
        } 
    }

    void Blk() {
         while(!errorFlag &&(
                current().type==TokenType::IDENTIFIER || 
                current().type==TokenType::PRINT || 
                current().type==TokenType::IF
            )) 
                Stm(); 
    }

    void Stm() {
        if (errorFlag) return;
        switch(current().type){
            case TokenType::IDENTIFIER:
                match(TokenType::IDENTIFIER); 
                match(TokenType::ASSIGN); 
                Exp(); 
                match(TokenType::SEMICOLON);
                if(!errorFlag) 
                    outParse << "Assignment Statement Recognized\n"; 
                break;
            case TokenType::PRINT:
                match(TokenType::PRINT);
                match(TokenType::LEFT_PAREN); 
                Arg();
                Argfollow();
                match(TokenType::RIGHT_PAREN);
                match(TokenType::SEMICOLON);
                if(!errorFlag) 
                    outParse << "Print Statement Recognized\n"; 
                break;
            case TokenType::IF:
                match(TokenType::IF); 
                if(!errorFlag) 
                    outParse << "If Statement Begins\n";
                Cnd();
                match(TokenType::COLON);
                Blk(); 
                Iffollow(); 
                break;
            default: 
                outParse << "Invalid Statement\n"; 
                errorFlag=true; 
                break;
        }
    }

    void Argfollow(){ 
        if(!errorFlag && current().type==TokenType::COMMA){ 
            match(TokenType::COMMA); 
            Arg(); 
            Argfollow(); 
        } 
    }
    
    void Arg(){ 
        if(!errorFlag){ 
            if(current().type==TokenType::STRING) 
                match(TokenType::STRING); 
            else Exp(); 
        } 
    }

    void Iffollow(){
        if(errorFlag) return;
        if(current().type==TokenType::ENDIF){ 
            match(TokenType::ENDIF); 
            match(TokenType::SEMICOLON); 
            if(!errorFlag)
                outParse << "If Statement Ends\n"; 
        } else if(current().type==TokenType::ELSE){ 
            match(TokenType::ELSE); 
            Blk(); 
            match(TokenType::ENDIF); 
            match(TokenType::SEMICOLON); 
            if(!errorFlag) 
                outParse << "If Statement Ends\n"; 
        }
    }

    void Exp(){ 
        if(!errorFlag){
            Trm(); 
            Trmfollow(); 
        } 
    }

    void Trmfollow(){ 
        if(!errorFlag){ 
            if(current().type==TokenType::PLUS){ 
                match(TokenType::PLUS); 
                Trm(); 
                Trmfollow(); 
            } else if(current().type==TokenType::MINUS){ 
                match(TokenType::MINUS); 
                Trm(); 
                Trmfollow(); 
            } 
        } 
    }

    void Trm(){ 
        if(!errorFlag){ 
            Fac(); 
            Facfollow(); 
        } 
    }

    void Facfollow(){ 
        if(!errorFlag){ 
            if(current().type==TokenType::MULTIPLY){ 
                match(TokenType::MULTIPLY); 
                Fac(); 
                Facfollow(); 
            } else if(current().type==TokenType::DIVIDE){ 
                match(TokenType::DIVIDE);
                Fac(); 
                Facfollow(); 
            } 
        } 
    }
    
    void Fac(){ 
        if(!errorFlag){ 
            Lit(); 
            Litfollow(); 
        } 
    }
    
    void Litfollow(){ 
        if(!errorFlag && current().type==TokenType::RAISE){ 
            match(TokenType::RAISE); 
            Lit(); 
            Litfollow(); 
        } 
    }

    void Lit(){ if(!errorFlag){ 
        if(current().type==TokenType::MINUS) 
            match(TokenType::MINUS); 
            Val(); 
        } 
    }

    void Val(){
        if(errorFlag) return;
        switch(current().type){
            case TokenType::IDENTIFIER: match(TokenType::IDENTIFIER); break;
            case TokenType::NUMBER: match(TokenType::NUMBER); break;
            case TokenType::SQRT: match(TokenType::SQRT); match(TokenType::LEFT_PAREN); Exp(); match(TokenType::RIGHT_PAREN); break;
            default: match(TokenType::LEFT_PAREN); Exp(); match(TokenType::RIGHT_PAREN); break;
        }
    }

    void Cnd(){ 
        if(!errorFlag){ 
            Exp(); 
            Rel(); 
            Exp(); 
        } 
    }
    
    void Rel(){
        if(errorFlag) return;
        switch(current().type){
            case TokenType::LESS_THAN: match(TokenType::LESS_THAN); break;
            case TokenType::EQUAL: match(TokenType::EQUAL); break;
            case TokenType::GREATER_THAN: match(TokenType::GREATER_THAN); break;
            case TokenType::GT_EQUAL: match(TokenType::GT_EQUAL); break;
            case TokenType::NOT_EQUAL: match(TokenType::NOT_EQUAL); break;
            case TokenType::LT_EQUAL: match(TokenType::LT_EQUAL); break;
            default: outParse << "Missing relational operator\n"; errorFlag=true; break;
        }
    }

public:
    Parser(const vector<Token>& tks, ofstream &parseOut, string fname) 
        : tokens(tks), outParse(parseOut), filename(fname) {}
    void parse(){ Prg(); }
};

int main() {
    for(const auto &entry : filesystem::directory_iterator(".")) {
        if(entry.path().extension() == ".txt") {
            string inputFile = entry.path().string();
            ifstream file(inputFile);
            if(!file.is_open()) continue;

            string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            string stem = entry.path().stem().string();

            size_t lastUnderscore = stem.find_last_of('_');
            string number = stem.substr(lastUnderscore + 1);

            ofstream outScan("sample_output_scan_" + number + ".txt");
            ofstream outParse("sample_output_parse_" + number + ".txt");

            if(!outScan.is_open() || !outParse.is_open()) continue;

            Scanner scanner(code);
            vector<Token> tokens = scanner.scanAll(outScan); // scan everything first

            Parser parser(tokens, outParse, entry.path().filename().string());
            parser.parse();

            outScan.close();
            outParse.close();
        }
    }
    return 0;
}
