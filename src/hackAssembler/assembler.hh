#pragma once

#include <string>

#include "misc/symbolTable.hh"

#define INSTR_LEN 16

#include <iostream>

namespace HackAssembler
{
    class Assembler
    {
    public:
        Assembler(misc::SymbolTable& st);

        // Translate a Hack pure program to binary representation
        void execute(std::stringstream& ppStream, std::string& binName);

    private:
        // Translate a Hack pure program to binary representation
        void assemble(std::stringstream& ppStream, std::ofstream& binStream);

        // Assemble an A instruction : @address
        void assembleA(std::string& line, std::ofstream& binStream,
                       int lineNum);
        // Assemble a C instruction : [DEST=]COMP[;JMP]
        void assembleC(std::string& line, std::ofstream& binStream,
                       int lineNum);

        // Compute bits for DEST
        std::string getDestBits(std::string& line, size_t& cursorIdx,
                                int lineNum);
        // Compute bits for COMP
        std::string getCompBits(std::string& line, size_t& cursorIdx,
                                int lineNum);
        // Compute bits for JMP
        std::string getJmpBits(std::string& line, size_t& cursorIdx,
                               int lineNum);

        // Check if a symbol has format [a-zA-Z_$][a-zA-Z_$0-9]*
        bool isValidSymbol(std::string& symbol);

        // Return string binary representation from int
        std::string decToBin(int dec);

        // Throw error message
        [[noreturn]] inline void throwError(std::string msg, int lineNum);

        // Symbol Table used to store labels and RAM addresses
        misc::SymbolTable& st_;
    };

} // namespace HackAssembler