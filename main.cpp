#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "scanner.hpp"
#include "parser.hpp"
using namespace std;

int main(){
    for (const auto &entry : filesystem::directory_iterator(".")){
        if (entry.path().extension() == ".txt")
        {
            string inputFile = entry.path().string();

            ifstream file(inputFile);

            string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            Scanner scanner(code);
            Parser parser(scanner);
            parser.Prg();

            // write output of the scanner
            string scanner_output_file = entry.path().stem().string() + "_scan.txt";
            ofstream scan_out(scanner_output_file);
            vector<string> scanner_output_lines = scanner.line_outputs;
            
            for (auto statement: scanner_output_lines)
                scan_out << statement << endl;

            scan_out.close();
            
            // write output of the parser
            string parse_output_file = entry.path().stem().string() + "_parse.txt";
            ofstream parse_out(parse_output_file);
            vector<string> parse_output_lines = parser.line_outputs;

            for (auto statement: parse_output_lines)
                parse_out << statement << endl;
            
            if (parser.valid_end)
                parse_out << inputFile << " is a valid SimpCalc program" << endl;

            parse_out.close();
        }
    }

    return 0;
}