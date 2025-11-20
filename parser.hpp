#ifndef PARSER_H
#define PARSER_H

#include "scanner.hpp"
#include <vector>
#include <string>

class Parser {
public:
    Parser(Scanner& scanner);

    void Prg();
    vector<string> line_outputs;
    bool valid_end = false;

private:
    Scanner& scanner_ref;
    Token input_token;

    void match(TokenType expected);
    void error(string source);
    void valid(TokenType type);

    void Blk();
    void Stm();
    void Arg();
    void Argfollow();
    void Iffollow();
    void Exp();
    void Trm();
    void Trmfollow();
    void Fac();
    void Facfollow();
    void Lit();
    void Litfollow();
    void Val();
    void Cnd();
    void Rel();
};

#endif
