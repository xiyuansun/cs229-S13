#include "aut.h"
#include "scanner.h"
#include "../util.h"

#include <iostream>
#include <cstdlib>

AutFile::AutFile(std::ifstream* in, int* x_range, int* y_range, int* x_view, int* y_view)
{
    this->x_range = x_range;
    this->y_range = y_range;
    this->x_disp_range = x_view;
    this->y_disp_range = y_view;
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

void AutFile::parse(std::ifstream* in)
{
    std::string n;    
    std::string keyword = "";

    Scanner* scanner = new Scanner(in, ";");
    while(scanner->has_next())
    {
        n = scanner->next();
        Scanner* stmnt = new Scanner(n);
        
        keyword = stmnt->next();

        if(keyword == "Xrange")
        {
            int first = stmnt->next_int();
            int second = stmnt->next_int();
            if(x_range == NULL)
            {
                x_range = new int[2];
                x_range[0] = first;
                x_range[1] = second;
            }

            if(x_disp_range == NULL)
            {
                x_disp_range = new int[2];
                
                x_disp_range[0] = first;
                x_disp_range[1] = second;
            }
        }
        else if(keyword == "Yrange")
        {
            int first = stmnt->next_int();
            int second = stmnt->next_int();
            
            if(y_range == NULL)
            {
                y_disp_range = new int[2];
                y_range[0] = first;
                y_range[1] = second;
            }

            if(y_disp_range == NULL)
            {
                y_disp_range = new int[2];
                y_range[0] = first;
                y_range[1] = second;
            }
        }
        else if(keyword == "Initial" || keyword == "Initial{")
        {
            //TODO: Check for keyword order
            std::string in_stmnt("");

            while(stmnt->has_next())
            {
                in_stmnt += stmnt->next();
            }

            this->b = new Board(x_range, y_range, x_disp_range, y_disp_range, states);

            std::vector<std::string>* inner_stmnts = split(in_stmnt, ';');
            for(unsigned int i = 0; i < inner_stmnts->size(); ++i)
            {
                std::vector<std::string>* sub_stmnts = split((*inner_stmnts)[i], ':');
                
                if(sub_stmnts->size() == 2)
                {
                    int eq = (*sub_stmnts)[0].rfind('=');
                    std::vector<std::string>* x_pos = split((*sub_stmnts)[1], ',');
                    
                    int y = get_int((*sub_stmnts)[0].substr(eq + 1).c_str());

                    for(unsigned int j = 0; j < x_pos->size(); ++j)
                    {
                        int x = get_int((*x_pos)[j].c_str());
                        this->b->set_state(x, y, 1);
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
            std::string k = remove_whitespace(keyword);
            if(k != "") std::cout << "Unrecognized keyword: " << k << '\n';
        }

        delete stmnt;
    }
    
    delete scanner;
}
