#include "symbolTable.hh"

#include <utility>

namespace misc
{
    void SymbolTable::init()
    {
        addRamEntry("R0");
        addRamEntry("R1");
        addRamEntry("R2");
        addRamEntry("R3");
        addRamEntry("R4");
        addRamEntry("R5");
        addRamEntry("R6");
        addRamEntry("R7");
        addRamEntry("R8");
        addRamEntry("R9");
        addRamEntry("R10");
        addRamEntry("R11");
        addRamEntry("R12");
        addRamEntry("R13");
        addRamEntry("R14");
        addRamEntry("R15");

        addRamEntry("SCREEN", 16384);
        addRamEntry("KBD", 24576);
        addRamEntry("SP", 0);
        addRamEntry("LCL", 1);
        addRamEntry("ARG", 2);
        addRamEntry("THIS", 3);
        addRamEntry("THAT", 4);
    }

    void SymbolTable::addLabel(std::string label, int lineNum)
    {
        table_[label] = lineNum;
    }

    int SymbolTable::addVariable(std::string symbol)
    {
        if (table_.find(symbol) != table_.end())
            throw std::invalid_argument(
                "Unexpected Error : Symbol already in symbol table: " + symbol);

        addRamEntry(symbol);
        return ramAddr - 1;
    }

    void SymbolTable::addRamEntry(std::string symbol, int addr)
    {
        if (addr == -1)
        {
            table_[symbol] = ramAddr;
            ++ramAddr;
        }
        else
        {
            table_[symbol] = addr;
        }
    }

    int SymbolTable::getSymbolAddr(std::string symbol)
    {
        auto i = table_.find(symbol);
        if (i == table_.end())
            return -1;
        return (*i).second;
    }

    void SymbolTable::print()
    {
        for (auto& i : table_)
            std::cout << i.first << ": " << i.second << std::endl;
    }
} // namespace misc