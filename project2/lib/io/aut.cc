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
    std::ifstream in;
    std::string n;
    
    //should probably be class variables =D
    int x_low;
    int x_high;
    int y_low;
    int y_high;
    std::string name = "";
    std::string chars = "";
    //somehow store colors.
    
    std::string keyword = "";

    in.open(s.c_str());
    Scanner* scanner = new Scanner(&in, ";");
    while(scanner->has_next())
    {
        n = scanner->next();
        Scanner* stmnt = new Scanner(n);
        
        keyword = stmnt->next();

        if(keyword == "Xrange")
        {
            x_low = stmnt->next_int();
            x_high = stmnt->next_int();
        }
        else if(keyword == "Yrange")
        {
            y_low = stmnt->next_int();
            y_high = stmnt->next_int();
        }
        else if(keyword == "Initial" || keyword == "Initial{")
        {
            //Parse!
        }
        else if(keyword == "Name")
        {
        }
        else if(keyword == "Chars")
        {
        }
        else if(keyword == "Colors")
        {
        }
        else
        {
            //Unknown keyword. Display warning.
            std::cout << "Unrecognized keyword: " << keyword << '\n';
        }
    }

    std::cout << "Xrange: " << x_low << ", " << x_high << '\n';
    std::cout << "Yrange: " << y_low << ", " << y_high << '\n';
}
