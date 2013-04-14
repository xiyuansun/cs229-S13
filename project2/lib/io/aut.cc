#include "aut.h"
#include "scanner.h"

#include <fstream>
#include <iostream>

AutFile::AutFile(std::string &in)
{
    parse(in);
}

/*Board const AutFile::get()
{
}

void AutFile::update(Board &b)
{
}*/

std::string const AutFile::to_string()
{
    return "Not implemented.";
}

void AutFile::parse(std::string &s)
{
    //open file. iunno how.
    std::ifstream in;
    in.open(s.c_str());
    Scanner* scanner = new Scanner(&in, ";", '#');
    while(1)
    {
        std::cout << (scanner->next());
    }

}
