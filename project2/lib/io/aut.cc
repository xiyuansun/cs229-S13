#include "aut.h"
#include "scanner.h"
#include "../util.h"

#include <fstream>
#include <iostream>
#include <cstdio>

AutFile::AutFile(std::string &in)
{
    this->x_range = new int[2];
    this->y_range = new int[2];
    this->x_disp_range = new int[2];
    this->y_disp_range = new int[2];
    this->states = "~123456789";
    this->name = "";
    this->parse(in);
}

AutFile::~AutFile()
{
    delete[] this->x_range;
    delete[] this->y_range;
    delete[] this->x_disp_range;
    delete[] this->y_disp_range;
}

Board* AutFile::get() const
{
    return this->b;
}

/*
void AutFile::update(Board &b)
{
}
*/

std::string AutFile::to_string() const
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
            //TODO: Check for keyword order
            std::string in_stmnt("");

            while(stmnt->has_next())
            {
                in_stmnt += stmnt->next();
            }

            std::vector<std::string>* inner_stmnts = split(in_stmnt, ';');
            for(int i = 0; i < inner_stmnts->size(); ++i)
            {
                std::vector<std::string>* sub_stmnts = split((*inner_stmnts)[i], ':');
                
                if(sub_stmnts->size() == 2)
                {
                    int eq = (*sub_stmnts)[0].rfind('=');
                    std::vector<std::string>* x_pos = split((*sub_stmnts)[1], ',')

                    int y = std::stoi((*sub_stmnts)[0].substr(eq + 1));

                    for(int j = 0; j < x_pos.size(); ++j)
                    {
                        int x = std::stoi((*x_pos)[j]);

                    }
                }
            }

            delete inner_stmnts;
        }
        else if(keyword == "Name")
        {
            this->name = stmnt->next();
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
    
    delete scanner;
}
