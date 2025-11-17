#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <filesystem>

using namespace std;

// We define a token as a struct, containing two fields:
// 1. An int ID which determines the actual ID
// 2. A string lexeme containing the lexeme of the token
//
// The token struct has two methods:
// 1. Constructor token(int a, string b) to create the tokens
// 2. A string function getToken(int a) which returns the actual string ID of the token.

struct token
{
public:
    int ID;
    string lexeme;
    token(int a, string b)
    {
        ID = a;
        lexeme = b;
    }
    string getToken(int c)
    {
        switch (c)
        {
        case 1:
            return "ASSIGN  ";
        case 2:
            return "PLUS	";
        case 3:
            return "MINUS	";
        case 4:
            return "NUM     ";
        default:
            return "Lexical Error reading character \"";
        }
    }
};

vector<token> tokens; // We initialize a vector of tokens to store the tokens to be included in the output file.

void generateOutput(string s) // Given the current vector of tokens, write this to the corresponding output file. Close the file and clear the token vector.
{
    string out = "output"; // Using a string "output" for the name of the output file
    string outputName = s; // Taking in the filename to be modified later on

    string newOutput = regex_replace(outputName, regex("^input"), out); // Given a filename, replace "input" with "output"
    ofstream outFile(newOutput);                                        // Make this new string the filename of the output

    for (auto &a : tokens)
    {
        if (a.ID == 0)
        {
            // We define an error as a special token that effectively halts the program.
            outFile << a.getToken(a.ID) << a.lexeme << endl;
            break;
        }
        else
        {
            // For every other token, add it to the file as normal.
            outFile << a.getToken(a.ID) << a.lexeme << endl;
        }
    }

    outFile.close();
    tokens.clear();
}

void getToken(string s1) // This function gets all the tokens in a single string (input sequence)
{
    int counter = 0;  // For iterating over the string
    string tempToken; // A temporary string to store the lexeme. Characters are procedurally added to this string throughout the program.
    
    while (counter < s1.length())
    {
        if (isspace(s1[counter]))
        {
            // Skip any whitespaces, tabs, newlines, etc.
            counter++;
            continue;
        }
        else if (s1[counter] == '=')
        {
            // If the current character is "=", look ahead if the next character is also "="
            // On success, create the ASSIGN token. Otherwise, log the error.
            tempToken += '=';

            if (s1[counter + 1] == '=')
            {
                tempToken += '=';
                tokens.push_back(token(1, tempToken));
                tempToken = "";
                counter++;
            }
            else
            {
                tempToken += "\"";
                tokens.push_back(token(0, tempToken));
            }
        }
        else if (s1[counter] == '+')
        {
            // If the current character is "+", create the PLUS token.
            tokens.push_back(token(2, "+"));
        }
        else if (s1[counter] == '-')
        {
            // If the current character is "-", create the MINUS token.
            tokens.push_back(token(3, "-"));
        }
        else if (isdigit(s1[counter]))
        {
            // If the current character is a digit, start creating the digit token.
            tempToken += s1[counter];

            // Look ahead to the next character. If any non-digit is encountered, finish creating the NUM token.
            if (!isdigit(s1[counter + 1]))
            {
                tokens.push_back(token(4, tempToken));
                tempToken = "";
            }
        }
        else if (!isspace(s1[counter]) && !(s1[counter] == '=') && !(s1[counter] == '+') && !isdigit(s1[counter]))
        {
            // Treat every other character as an error. End the program in this case.
            tempToken += s1[counter];
            tempToken += "\"";
            tokens.push_back(token(0, tempToken));
            break;
        }
        counter++; // Go to the next character
    }
}

int main(int argc, char *argv[])
{
    // string inputName;

    // string inputText = argv[1];      // Filenames are input into the program as the second command line argument, i.e. run the program by inputting "scanner inputN.txt"
    // Get the filename and store it as a string

    for (auto const &dir_entry : filesystem::directory_iterator{filesystem::current_path()})
    {
        string inputText = dir_entry.path().filename().string();

        if (dir_entry.path().extension() == ".txt" && regex_match(inputText, regex("^input(.*)")))
        {
            string charStream; // For storing the character stream
            char c;            // For getting each character of the input
            ifstream inputTokens(inputText);

            while (inputTokens.get(c))
            {
                charStream += c; // Get all text from the input file, and save this into the string charStream
            }

            getToken(charStream);
            // getToken(EOF)
            generateOutput(inputText); // Pass the name of the input file for output generation
            inputTokens.close();
        }
    }

    return 0;
}