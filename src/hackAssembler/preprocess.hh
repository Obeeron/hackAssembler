#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "misc/symbolTable.hh"

namespace HackAssembler
{
    class Preprocess
    {
    public:
        Preprocess(misc::SymbolTable&);

        // Preprocess srcFilename and put the preprocessed code into ppStream
        // Generate srcFilename.i if genPPFile is set
        void execute(std::string& srcFilename, std::stringstream& ppStream,
                     bool genPPFile);

    private:
        // Write the preprocessed stream to the file <srcName>.i
        void generatePreprocessedFile(std::stringstream& ppStream,
                                      std::string& srcFilename);

        // Preprocess: fill ppStream with the preprocessed source file:
        void preprocess(std::ifstream& srcStream, std::stringstream& ppStream);

        // Remove comments, empty lines and whitespace
        // Load labels into symbol table and remove them from stream
        // Returns true if the preprocessed line is not empty
        bool preprocessLine(const std::string& line, int lineNum,
                            std::stringstream& ppStream);

        // Symbol Table used to store labels and RAM addresses
        misc::SymbolTable& st_;
    };
} // namespace HackAssembler