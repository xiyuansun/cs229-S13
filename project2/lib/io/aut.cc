#include "aut.h"
#include "scanner.h"

#include <fstream>
#include <iostream>

AutFile::AutFile(std::string &in)
{
    this->parse(in);
}

std::vector<std::string> const AutFile::get()
{
    return this->statements;
}
/*
void AutFile::update(Board &b)
{
}
*/
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
    while(!(in.eof()))
    {
        this->statements.push_back(scanner->next());
    }

}
