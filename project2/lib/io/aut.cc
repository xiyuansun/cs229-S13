#include "aut.h"
#include "scanner.h"

#include <fstream>
#include <iostream>

AutFile::AutFile(std::string &in)
{
    this->x_range = new int[2];
    this->y_range = new int[2];
    this->x_disp_range = new int[2];
    this->y_disp_range = new int[2];
    this->states("~123456789");
    this->name("");
    this->parse(in);
}

AutFile::~AutFile()
{
    delete[] this->x_range;
    delete[] this->y_range;
    delete[] this->x_disp_range;
    delete[] this->y_disp_range;

}

Board const AutFile::get()
{
    return this->b;
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
            x_range[0] = stmnt->next_int();
            x_range[1] = stmnt->next_int();
        }
        else if(keyword == "Yrange")
        {
            y_range[0] = stmnt->next_int();
            y_range[1] = stmnt->next_int();
        }
        else if(keyword == "Initial" || keyword == "Initial{")
        {
            
        }
        else if(keyword == "Name")
        {
            this->name(stmnt->next());
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

        delete stmnt;
    }

    std::cout << "Xrange: " << x_low << ", " << x_high << '\n';
    std::cout << "Yrange: " << y_low << ", " << y_high << '\n';
}
