#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>
#include <scanner.cpp>

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
using enum TokenType;

class Parser{
    Token input_token;

    void match(TokenType expected){
        if (input_token.type == expected){
            // consume the token, and move to next token
            if (input_token.type == END_OF_FILE){
                valid_end();
            }
        }else{
            error("MATCH");
        }
    }

    void error(string source){
        string statement;
        if (source == "ENDIF"){
            statement = "Incomplete if Statement";
        }else if (source == "REL"){
            statement = "Missing relational operator";
        }else if (source == "STM"){
            statement = "Invalid Statement";
        }else if (source == "MATCH"){
            statement = "Symbol expected";
        }
        // output the statement
    } 

    void valid(TokenType type){
        string statement;
        switch (type){
            case ASSIGN:
                statement = "Assignment Statement Recognized";
                break;
             
            case PRINT:
                statement = "Print Statement Recognized";
                break;
             
            case IF:
                statement = "If Statement Begins";
                break;
             
            case ENDIF:
                statement = "If Statement Ends";
                break;
        }
        // output the statement
    }

    void valid_end(){
        // output "<filename> is a valid SimpCalc program"
    }
    
    void Prg(){
        Blk();
        match(END_OF_FILE);
    }

    void Blk(){
        if (input_token.type == IDENTIFIER || input_token.type == PRINT || input_token.type == IF){
            Stm();
            Blk();
        }else{
            return;
        }
    }

    void Stm(){
        switch (input_token.type){
            case IDENTIFIER:
                match(IDENTIFIER);
                match(ASSIGN);
                Exp();
                match(SEMICOLON);
                valid(ASSIGN);
                break;
            
            case PRINT:
                match(PRINT);
                match(LEFT_PAREN);
                Arg();
                Argfollow();
                match(RIGHT_PAREN);
                match(SEMICOLON);
                valid(PRINT);
                break;

            case IF:
                match(IF);
                Cnd();
                match(COLON);
                Blk();
                Iffollow();
                valid(IF);
                break;
            
            default:
                error("STM");
                break;
        }
    }

    void Argfollow(){
        if (input_token.type == COMMA){
            match(COMMA);
            Arg();
            Argfollow();
        }else{
            return;
        }
    }

    void Arg(){
        if (input_token.type == STRING){
            match(STRING);
        }else{
            Exp();
        }
    }

    void Iffollow(){
        if (input_token.type == ENDIF){
            match(ENDIF);
            match(SEMICOLON);
        }else if (input_token.type == ELSE){
            match(ELSE);
            Blk();
            match(ENDIF);
            match(SEMICOLON);
        }else{
            // error
        }
    }

    void Exp(){
        Trm();
        Trmfollow();
    }

    void Trmfollow(){
        if (input_token.type == PLUS){
            match(PLUS);
            Trm();
            Trmfollow();
        }else if (input_token.type == MINUS){
            match(MINUS);
            Trm();
            Trmfollow();
        }else{
            return;
        }
    }

    void Trm(){
        Fac();
        Facfollow();
    }

    void Facfollow(){
        if (input_token.type == MULTIPLY){
            match(MULTIPLY);
            Fac();
            Facfollow();
        }else if (input_token.type == DIVIDE){
            match(DIVIDE);
            Fac();
            Facfollow();
        }else{
            return;
        }
    }

    void Fac(){
        Lit();
        Litfollow();
    }

    void Litfollow(){
        if (input_token.type == RAISE){
            match(RAISE);
            Lit();
            Litfollow();
        }
    }

    void Lit(){
        if (input_token.type == MINUS){
            match(MINUS);
        }
        Val();
    }

    void Val(){
        switch (input_token.type){
            case IDENTIFIER:
                match(IDENTIFIER);
                break;

            case NUMBER:
                match(NUMBER);
                break;

            case SQRT:
                match(SQRT);
                match(LEFT_PAREN);
                Exp();
                match(RIGHT_PAREN);
                break;

            default:
                match(LEFT_PAREN);
                Exp();
                match(RIGHT_PAREN);
                break;
        }
    }

    void Cnd(){
        Exp();
        Rel();
        Exp();
    }
    
    void Rel(){
        switch(input_token.type){
            case (LESS_THAN):
                match(LESS_THAN);
                break;

            case (EQUAL):
                match(EQUAL);
                break;

            case (GREATER_THAN):
                match(GREATER_THAN);
                break;

            case (GT_EQUAL):
                match(GT_EQUAL);
                break;

            case (NOT_EQUAL):
                match(NOT_EQUAL);
                break;

            case (LT_EQUAL):
                match(LT_EQUAL);
                break;
            
            default:
                error("REL");
        }
    }
};


// int main(int argc, char* argv[]){
//     for (auto const &dir_entry : filesystem::directory_iterator{filesystem::current_path()})
//     {
//         string inputText = dir_entry.path().filename().string();
        
//         if (dir_entry.path().extension() == ".txt" && regex_search(inputText, regex("output_scan")))
//         {
//             string baseName = dir_entry.path().stem().string();
//             cout << baseName << endl;

//             ifstream fin(dir_entry.path());
//             string line, token;
//             int i = 0;
//             vector<string> tokens;
//             while (getline(fin, line)){
//                 istringstream iss(line);
//                 iss >> token;
//                 tokens.push_back(token);
//                 cout << i << " " << token << endl;
//                 i++;
//             }
//         }
//     }

//     // for (auto x :  productions)
//     //     {for (auto y : x) cout << y << " ";
//     //     cout << endl;}
// }