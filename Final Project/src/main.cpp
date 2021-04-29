#include "lexer/lexer_generator.hh"
#include "parser/parser_generator.hh"

#include <iostream>
#include <fstream>

void generate_lexer()
{
    std::cout << "Enter filename with regular expressions for lexer: " << std::endl;
    std::string filename;
    std::cin >> filename;
    //std::string filename = "C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\build\\test_grammar2.txt";
    std::ifstream fin(filename.c_str());
    while(!fin.is_open())
    {
        std::cout << "Error opening file. Enter new filename or type Q to quit: ";
        std::cin >> filename;
        if(filename == "Q")
            return;
        fin.close();
        fin.open(filename.c_str());
    }
    final_project::lexer::generate_lexer(filename);
}

void generate_parser_tables()
{
    std::cout << "Enter filename with context free grammar for parser: " << std::endl;
    std::string filename;
    std::cin >> filename;
    //std::string filename = "C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\build\\test_grammar2.txt";
    std::ifstream fin(filename.c_str());
    while(!fin.is_open())
    {
        std::cout << "Error opening file. Enter new filename or type Q to quit: ";
        std::cin >> filename;
        if(filename == "Q")
            return;
        fin.close();
        fin.open(filename.c_str());
    }
    final_project::parser::parser_generator pg(fin);
    auto tables = pg.create_parse_tables();
    std::ofstream fout("Parser Table.txt");
    final_project::parser::parser_generator::print_tables(fout, tables.first, tables.second);
}

int main(int argc, char** argv)
{
    std::cout << "Select option:\n[1] Generate Lexer\n[2] Generate Parser Tables" << std::endl;
    std::string option;
    std::cin >> option; 
    if (option == "2")
        generate_parser_tables();
    else
        generate_lexer();
}