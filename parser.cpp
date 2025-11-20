#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>

using namespace std;

vector<vector<string>> procedures = {
    {"Blk", "ENDOFFILE"},
    {"Stm", "Blk"},
    {"Epsilon"},
    {"IDENTIFIER", "ASSIGN", "Exp"},
    {"IF", "Cnd", "COLON", "Blk", "Iffollow"},
    {"COMMA", "Arg", "Argfollow"},
    {"Epsilon"},
    {"STRING"},
    {"Exp"},
    {"ENDIF", "SEMICOLON"},
    {"ELSE", "Blk", "ENDIF", "SEMICOLON"},
    {"Trm", "Trmfollow"},
    {"PLUS", "Trm", "Trmfollow"},
    {"MINUS", "Trm", "Trmfollow"},
    {"Epsilon"},
    {"Fac", "Facfollow"},
    {"MULTIPLY", "Fac", "Facfollow"},
    {"DIVIDE", "Fac", "Facfollow"},
    {"Epsilon"},
    {"Lit", "Litfollow"},
    {"RAISE", "Lit", "Litfollow"},
    {"Epsilon"},
    {"MINUS", "Val"},
    {"Val"},
    {"IDENTIFIER"},
    {"NUMBER"},
    {"SQRT", "LEFTPAREN", "Exp", "RIGHTPAREN"},
    {"LEFTPAREN", "Exp", "RIGHTPAREN"},
    {"Exp", "Rel", "Exp"},
    {"LESSTHAN"},
    {"EQUAL"},
    {"GREATERTHAN"},
    {"GTEQUAL"},
    {"NOTEQUAL"},
    {"LTEQUAL"},
};

map<string, int> terminal_to_index = {
    {"IDENTIFIER", 0},
    {"NUMBER", 1},
    {"STRING", 2},
    {"ASSIGN", 3},
    {"SEMICOLON", 4},
    {"COLON", 5},
    {"COMMA", 6},
    {"LEFT_PAREN", 7},
    {"RIGHT_PAREN", 8},
    {"PLUS", 9},
    {"MINUS", 10},
    {"MULTIPLY", 11},
    {"DIVIDE", 12},
    {"RAISE", 13},
    {"LESS_THAN", 14},
    {"EQUAL", 15},
    {"GREATER_THAN", 16},
    {"LT_EQUAL", 17},
    {"GT_EQUAL", 18},
    {"NOT_EQUAL", 19},
    {"PRINT", 20},
    {"IF", 21},
    {"ELSE", 22},
    {"ENDIF", 23},
    {"SQRT", 24},
    {"AND", 25},
    {"OR", 26},
    {"NOT", 27},
    {"END_OF_FILE", 28},
    {"ERROR", 29},
};

map<string, int> non_terminal_to_index = {};

void recursive_descent(vector<string> tokens, vector<vector<int>> parse_table, vector<vector<string>> procedures){
    stack<string> token_stack;
    token_stack.push("END_OF_FILE");
    token_stack.push("Blk");
    
    int token_it = 0;
    string current_node;

    while (true){
        current_node = token_stack.top();
        token_stack.pop();

        // check if top of stack is a terminal or a variable
        if (terminal_to_index.find(current_node) != end(terminal_to_index)){
            if (current_node == tokens[token_it]){
                token_it++;
            }else{
                // mismatch
            }
        }else{
            int procedure_index = parse_table[non_terminal_to_index[current_node]][terminal_to_index[tokens[token_it]]];
            for (auto it=procedures[procedure_index].rbegin(); it<procedures[procedure_index].rend(); it++){
                token_stack.push(*it); // push the procedure in the reverse order
            }
        }
    }
}

int main(int argc, char* argv[]){
    for (auto const &dir_entry : filesystem::directory_iterator{filesystem::current_path()})
    {
        string inputText = dir_entry.path().filename().string();
        
        if (dir_entry.path().extension() == ".txt" && regex_search(inputText, regex("output_scan")))
        {
            string baseName = dir_entry.path().stem().string();
            cout << baseName << endl;

            ifstream fin(dir_entry.path());
            string line, token;
            int i = 0;
            vector<string> tokens;
            while (getline(fin, line)){
                istringstream iss(line);
                iss >> token;
                tokens.push_back(token);
                cout << i << " " << token << endl;
                i++;
            }
        }
    }

    for (auto x :  procedures)
        {for (auto y : x) cout << y << " ";
        cout << endl;}
}