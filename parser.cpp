#include "parser.hpp"
#include <iostream>
#include <string>


Parser::Parser(Scanner& scanner) : scanner_ref(scanner) {
    input_token = scanner_ref.getToken();
}

void Parser::Parser::match(TokenType expected) {
    if (input_token.type == expected) {
        input_token = scanner_ref.getToken();
        if (input_token.type == END_OF_FILE) {
            valid_end = true;
        }
    } else {
        error("MATCH");
    }
}

void Parser::error(string source){
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
    line_outputs.push_back(statement);
} 

void Parser::valid(TokenType type){
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
    line_outputs.push_back(statement);
}

void Parser::Prg(){
    Blk();
    match(END_OF_FILE);
}

void Parser::Blk(){
    if (input_token.type == IDENTIFIER || input_token.type == PRINT || input_token.type == IF){
        Stm();
        Blk();
    }else{
        return;
    }
}

void Parser::Stm(){
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

void Parser::Argfollow(){
    if (input_token.type == COMMA){
        match(COMMA);
        Arg();
        Argfollow();
    }else{
        return;
    }
}

void Parser::Arg(){
    if (input_token.type == STRING){
        match(STRING);
    }else{
        Exp();
    }
}

void Parser::Iffollow(){
    if (input_token.type == ENDIF){
        match(ENDIF);
        match(SEMICOLON);
    }else if (input_token.type == ELSE){
        match(ELSE);
        Blk();
        match(ENDIF);
        match(SEMICOLON);
    }else{
        error("ENDIF");
    }
}

void Parser::Exp(){
    Trm();
    Trmfollow();
}

void Parser::Trmfollow(){
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

void Parser::Trm(){
    Fac();
    Facfollow();
}

void Parser::Facfollow(){
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

void Parser::Fac(){
    Lit();
    Litfollow();
}

void Parser::Litfollow(){
    if (input_token.type == RAISE){
        match(RAISE);
        Lit();
        Litfollow();
    }else{
        return;
    }
}

void Parser::Lit(){
    if (input_token.type == MINUS){
        match(MINUS);
    }
    Val();
}

void Parser::Val(){
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

void Parser::Cnd(){
    Exp();
    Rel();
    Exp();
}

void Parser::Rel(){
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
