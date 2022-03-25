#include <fstream>
#include <iostream>

#include "hackAssembler/assembler.hh"
#include "hackAssembler/preprocess.hh"
#include "misc/symbolTable.hh"

void usage(std::string binName)
{
    std::cout << "usage: " << binName << " <sourceFile> [-o <outputFile>] [-p]"
              << std::endl;
}

// Get optional args
void getOpts(int argc, char* argv[], std::string& src, std::string& out,
             bool& genPPFile)
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(1);
    }

    src = argv[1];
    // Get src file name without extension
    std::string srcName = src.substr(0, src.find_last_of('.'));
    // Default output file is <srcName>.hack without asm extension
    out = srcName + ".hack";
    genPPFile = false;

    for (int i = 2; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-o")
        {
            if (i + 1 < argc)
                out = argv[i++ + 1];
            else
            {
                std::cout << "Missing output file name" << std::endl;
                usage(argv[0]);
                exit(1);
            }
        }
        else if (std::string(argv[i]) == "-p")
            genPPFile = true;
        else
        {
            std::cout << "Unknown option: " << argv[i] << std::endl;
            usage(argv[0]);
            exit(1);
        }
    }
}

int main(int argc, char* argv[])
{
    std::string srcFile;
    std::string binName = "res.hack";
    bool genPPFile = false;

    getOpts(argc, argv, srcFile, binName, genPPFile);

    // Init Symbol Table
    misc::SymbolTable st;
    st.init();

    std::stringstream ppStream;
    // Preprocess source file
    HackAssembler::Preprocess pp(st);
    pp.execute(srcFile, ppStream, genPPFile);

    // Assemble preprocessed file and store in binary file <binName>
    HackAssembler::Assembler assembler(st);
    assembler.execute(ppStream, binName);

    return 0;
}