#include "assembler.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "misc/symbolTable.hh"
#include "misc/util.hh"

namespace HackAssembler
{
    Assembler::Assembler(misc::SymbolTable& st)
        : st_(st)
    {}

    void Assembler::execute(std::stringstream& ppStream, std::string& binName)
    {
        // Create binStream
        std::ofstream binStream(binName);
        if (!binStream.is_open())
            throw std::invalid_argument("Cannot create file: " + binName);

        // Assemble
        assemble(ppStream, binStream);

        binStream.close();
    }

    void Assembler::assemble(std::stringstream& ppStream,
                             std::ofstream& binStream)
    {
        // counter for error reporting
        int lineNum = 0;

        std::string line;
        while (std::getline(ppStream, line))
        {
            // Assemble line
            if (line[0] == '@')
                assembleA(line, binStream, lineNum);
            else
                assembleC(line, binStream, lineNum);

            ++lineNum;
        }
    }

    void Assembler::assembleA(std::string& line, std::ofstream& binStream,
                              int lineNum)
    {
        // Get symbol
        std::string symbol = line.substr(1);

        // If symbol string empty, throw error
        if (symbol.empty())
            throwError("Symbol cannot be empty", lineNum);

        // If address is a number, convert it to binary
        if (misc::isNumber(symbol))
        {
            int dec = std::stoi(symbol);
            std::string bin = decToBin(dec);
            binStream << bin << "\n";
            return;
        }

        // Check symbol format
        // if (!isValidSymbol(symbol))
        //    throwError("Invalid symbol format: " + symbol, lineNum);

        int ramAddr = st_.getSymbolAddr(symbol);
        // If symbol is already in symbol table, get its binary address
        if (ramAddr != -1)
        {
            std::string bin = decToBin(ramAddr);
            binStream << bin << "\n";
        }
        // If symbol is not in symbol table, then declare variable
        else
        {
            // Add Variable
            ramAddr = st_.addVariable(symbol);
            // Convert RAM address to binary
            std::string bin = decToBin(ramAddr);
            binStream << bin << "\n";
        }
    }

    void Assembler::assembleC(std::string& line, std::ofstream& binStream,
                              int lineNum)
    {
        // Get cursor index
        size_t cursorIdx = 0;

        // Get bits for DEST
        std::string destBits = getDestBits(line, cursorIdx, lineNum);
        // Get bits for COMP
        std::string compBits = getCompBits(line, cursorIdx, lineNum);
        // Get bits for JMP
        std::string jmpBits = getJmpBits(line, cursorIdx, lineNum);

        // Create binary instruction
        std::string bin = "111" + compBits + destBits + jmpBits;
        binStream << bin + "\n";
    }

    std::string Assembler::decToBin(int dec)
    {
        std::string binAddr;
        do
        {
            binAddr.push_back('0' + (dec % 2));
            dec /= 2;
        } while (dec != 0);

        binAddr.append(INSTR_LEN - binAddr.length(), '0');
        std::reverse(binAddr.begin(), binAddr.end());

        return binAddr;
    }

    std::string Assembler::getDestBits(std::string& line, size_t& cursorIdx,
                                       int lineNum)
    {
        auto egalPos = line.find('=');
        if (egalPos == std::string::npos)
            return "000";

        std::string destStr = line.substr(0, egalPos);
        cursorIdx = egalPos + 1;

        if (destStr == "A")
            return "100";
        else if (destStr == "D")
            return "010";
        else if (destStr == "M")
            return "001";
        else if (destStr == "AM")
            return "101";
        else if (destStr == "AD")
            return "110";
        else if (destStr == "MD")
            return "011";
        else if (destStr == "AMD")
            return "111";

        throwError("Destination expected on line: " + line, lineNum);
    }

    std::string Assembler::getCompBits(std::string& line, size_t& cursorIdx,
                                       int lineNum)
    {
        if (cursorIdx >= line.length())
            throwError("Expected expression on line: " + line, lineNum);

        auto colonPos = line.find(';');
        if (colonPos == std::string::npos)
            colonPos = line.length();

        std::string compStr = line.substr(cursorIdx, colonPos - cursorIdx);
        cursorIdx = colonPos + 1;

        if (compStr == "0")
            return "0101010";
        if (compStr == "1")
            return "0111111";
        if (compStr == "-1")
            return "0111010";
        if (compStr == "D")
            return "0001100";
        if (compStr == "A")
            return "0110000";
        if (compStr == "M")
            return "1110000";
        if (compStr == "!D")
            return "0001101";
        if (compStr == "!A")
            return "0110001";
        if (compStr == "!M")
            return "1110001";
        if (compStr == "-D")
            return "0001111";
        if (compStr == "-A")
            return "0110011";
        if (compStr == "-M")
            return "1110011";
        if (compStr == "D+1")
            return "0011111";
        if (compStr == "A+1")
            return "0110111";
        if (compStr == "M+1")
            return "1110111";
        if (compStr == "D-1")
            return "0001110";
        if (compStr == "A-1")
            return "0110010";
        if (compStr == "M-1")
            return "1110010";
        if (compStr == "D+A")
            return "0000010";
        if (compStr == "D+M")
            return "1000010";
        if (compStr == "D-A")
            return "0010011";
        if (compStr == "D-M")
            return "1010011";
        if (compStr == "A-D")
            return "0000111";
        if (compStr == "M-D")
            return "1000111";
        if (compStr == "D&A")
            return "0000000";
        if (compStr == "D&M")
            return "1000000";
        if (compStr == "D|A")
            return "0010101";
        if (compStr == "D|M")
            return "1010101";
        throwError("Invalid expression on line: " + line, lineNum);
    }

    std::string Assembler::getJmpBits(std::string& line, size_t& cursorIdx,
                                      int lineNum)
    {
        if (cursorIdx >= line.length())
            return "000";

        std::string jmpStr = line.substr(cursorIdx, line.length() - cursorIdx);

        if (jmpStr == "JGT")
            return "001";
        if (jmpStr == "JEQ")
            return "010";
        if (jmpStr == "JGE")
            return "011";
        if (jmpStr == "JLT")
            return "100";
        if (jmpStr == "JNE")
            return "101";
        if (jmpStr == "JLE")
            return "110";
        if (jmpStr == "JMP")
            return "111";

        throwError("Invalid JMP condition: " + line, lineNum);
    }

    inline void Assembler::throwError(std::string msg, int lineNum)
    {
        std::stringstream ss;
        ss << "Error on line " << lineNum << ": " << msg;
        throw std::invalid_argument(ss.str());
    }

    bool Assembler::isValidSymbol(std::string& symbol)
    {
        // Check if symbol has format: [a-zA-Z_][a-zA-Z0-9_]*
        if (symbol.length() == 0)
            return false;

        if (!isalpha(symbol[0]) && symbol[0] != '_')
            return false;

        for (size_t i = 1; i < symbol.length(); i++)
        {
            if (!isalnum(symbol[i]) && symbol[i] != '_')
                return false;
        }

        return true;
    }
} // namespace HackAssembler