#pragma once

#include <iostream>
#include <map>

namespace misc
{
    class SymbolTable
    {
    public:
        SymbolTable() = default;

        // Add a symbol to the symbol table
        void addLabel(std::string label, int lineNum);

        // Add a variable to the symbol table and increment the RAM address
        // Throw an exception if the variable is already in the symbol table
        // Returns the address of the variable
        int addVariable(std::string symbol);

        // Return the address of the symbol
        // return -1 if the symbol is not in the symbol table
        int getSymbolAddr(std::string symbol);

        // print the symbol table
        void print();

        // Init Symbol Table by filling it with default keywords
        void init();

    private:
        void addRamEntry(std::string symbol, int value = -1);

        std::map<std::string, int> table_;
        int ramAddr = 0;
    };
} // namespace misc