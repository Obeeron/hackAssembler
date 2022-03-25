#include "preprocess.hh"

#include <fstream>
#include <iostream>
#include <string>

#include "misc/symbolTable.hh"
#include "preprocess.hh"

namespace HackAssembler
{
    Preprocess::Preprocess(misc::SymbolTable& st)
        : st_(st)
    {}

    void Preprocess::execute(std::string& srcFilename,
                             std::stringstream& ppStream, bool genPPFile)
    {
        // Open source file
        std::ifstream srcStream(srcFilename);
        if (!srcStream.is_open())
            throw std::invalid_argument("Cannot open file: " + srcFilename);

        // Preprocess file and put is into ss;
        preprocess(srcStream, ppStream);

        // If genPPFile is set, write ppStream to a file
        if (genPPFile)
            generatePreprocessedFile(ppStream, srcFilename);

        // close source file
        srcStream.close();
    }

    void Preprocess::preprocess(std::ifstream& srcStream,
                                std::stringstream& ppStream)
    {
        std::string line;
        int lineNum = 0;
        while (std::getline(srcStream, line))
        {
            if (preprocessLine(line, lineNum, ppStream))
                ++lineNum;
        }
    }

    bool Preprocess::preprocessLine(const std::string& line, int lineNum,
                                    std::stringstream& ppStream)
    {
        // Preprocessed line
        std::string ppLine;

        // Remove white spaces
        for (auto c : line)
        {
            if (c != ' ' && c != '\t' && c != '\r')
                ppLine.push_back(c);
        }

        // Remove comment
        auto pos = ppLine.find("//");
        if (pos != std::string::npos)
            ppLine.erase(pos);

        // Remove empty lines
        if (ppLine.empty() || ppLine == "\n")
            return false;

        // Add labels to the symbol table
        if (ppLine[0] == '(')
        {
            // If no matching ')' found, throw exception
            auto pos = ppLine.find(')');
            if (pos == std::string::npos)
                throw std::invalid_argument("Missing ')' in line: " + line);
            st_.addLabel(ppLine.substr(1, pos - 1), lineNum);
            return false;
        }
        // Add line to the preprocessed stream
        ppStream << ppLine << std::endl;

        return true;
    }

    void Preprocess::generatePreprocessedFile(std::stringstream& ppStream,
                                              std::string& srcFilename)
    {
        std::ofstream os;

        // Get source file name without extension
        std::string srcName =
            srcFilename.substr(0, srcFilename.find_last_of('.'));

        // Default preprocessed file is <srcName>.i
        std::string ppFilename = srcName + ".i";

        // Create preprocessed file
        os.open(ppFilename);
        if (!os.is_open())
            throw std::invalid_argument("Cannot create file: " + ppFilename);

        os << ppStream.str();
        os.close();
    }

    // void preprocess(const std::string& source)
    // {
    //     std::ifstream is(source);
    //     if (!is.is_open())
    //         throw std::invalid_argument("Cannot open " + source);

    //     std::string ppFilename = source + ".i";
    //     std::ofstream os(ppFilename);
    //     if (!os.is_open())
    //         throw std::invalid_argument("Cannot create intermediary file "
    //                                     + ppFilename);

    //     int pureLineIdx = 0;
    //     std::string line;
    //     while (std::getline(is, line))
    //     {
    //         std::string ppLine;

    //         // Create preprocessed line without white spaces
    //         for (auto c : line)
    //         {
    //             if (c != ' ' && c != '\r' && c != '\t')
    //                 ppLine.push_back(c);
    //         }

    //         // If line is empty or comment, skip it
    //         if (ppLine.length() == 0
    //             || (ppLine.length() >= 2 && ppLine[0] == '/'
    //                 && ppLine[1] == '/'))
    //             continue;

    //         // If label definition

    //         if (ppLine.length() >= 3 && ppLine[0] == '('
    //             && ppLine[ppLine.length() - 1] == ')')
    //         {
    //             // Add label to Symbol Table and skip line
    //             std::string labelStr = ppLine.substr(1, ppLine.length() - 2);
    //             st_.addLabel(labelStr, pureLineIdx);
    //             continue;
    //         }

    //         // Add preprocessed line to preprocessed file
    //         os << ppLine << '\n';

    //         pureLineIdx++;
    //     }
    // }
} // namespace HackAssembler