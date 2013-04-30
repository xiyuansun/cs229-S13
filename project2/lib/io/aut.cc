#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "aut.h"
#include "scanner.h"
#include "../util.h"

AutFile::AutFile(std::ifstream* in, int* x_range, int* y_range, int* x_view, int* y_view)
{
    this->x_range = x_range;
    this->y_range = y_range;
    this->x_disp_range = x_view;
    this->y_disp_range = y_view;
    this->states = "~123456789";
    this->name = "Game of Life";
    this->colors = new std::vector<Color>();
    this->parse(in);
}

AutFile::~AutFile()
{
    delete[] this->x_range;
    delete[] this->y_range;
    delete[] this->x_disp_range;
    delete[] this->y_disp_range;
    delete this->b;
    delete this->colors;
}

Board* AutFile::get() const
{
    return this->b;
}

std::string AutFile::get_name() const
{
    return this->name;
}

int AutFile::get_x_low() const
{
    return this->x_disp_range[0];
}

int AutFile::get_x_high() const
{
    return this->x_disp_range[1];
}

int AutFile::get_y_low() const
{
    return this->y_disp_range[0];
}

int AutFile::get_y_high() const
{
    return this->y_disp_range[1];
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
            
            if(y_disp_range == NULL)
            {
                y_disp_range = new int[2];
                y_disp_range[0] = first;
                y_disp_range[1] = second;
            }

            if(y_range == NULL)
            {
                y_range = new int[2];
                y_range[0] = first;
                y_range[1] = second;
            }
        }
        else if(keyword == "Initial" || keyword == "Initial{")
        {
            if(x_range == NULL || y_range == NULL || x_disp_range == NULL || y_disp_range == NULL)
            {
                throw std::runtime_error("The Initial keyword must be the last keyword in the aut file.");
            }

            std::string in_stmnt("");

            while(stmnt->has_next())
            {
                in_stmnt += stmnt->next();
            }

            this->b = new Board(x_range, y_range, x_disp_range, y_disp_range, states, colors);

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
                    
                    delete x_pos;
                }

                delete sub_stmnts;
            }

            delete inner_stmnts;
        }
        else if(keyword == "Name")
        {
            this->name = stmnt->next();
        }
        else if(keyword == "Chars")
        {
            std::string in_stmnt("");
            while(stmnt->has_next())
            {
                in_stmnt += stmnt->next();
            }

            remove_whitespace(in_stmnt);
            std::vector<std::string>* chars = split(in_stmnt, ',');
            
            //TODO: Check size of chars vs maxsize for rules.

            for(unsigned int i = 0; i < chars->size(); ++i)
            {
                this->states[i] = (char) get_int((*chars)[i]);
            }

            delete chars;
        }
        else if(keyword == "Colors")
        {
            std::string in_stmnt("");
            while(stmnt->has_next())
            {
                in_stmnt += stmnt->next();
            }

            remove_whitespace(in_stmnt);

            std::vector<std::string>* clrs = split(in_stmnt, ',');

            //TODO: Check size of clrs % 3 == 0 and size of clrs vs maxsize for rules

            for(unsigned int i = 0; i < clrs->size()/3; ++i)
            {
                Color c;

                std::string r = (*clrs)[i].substr(1);
                std::string g = (*clrs)[i + 1];
                std::string b = (*clrs)[i + 2];

                c.set_red((unsigned char) get_int(r));
                c.set_green((unsigned char) get_int(g));
                c.set_blue((unsigned char) get_int(b.substr(0, b.length()-1)));

                colors->push_back(c);
            }

            delete clrs;
        }
        else
        {
            //Unknown keyword. Display warning.
            std::string k = remove_whitespace(keyword);
            if(k != "") std::cerr << "Unrecognized keyword: " << k << '\n';
        }

        delete stmnt;
    }
    
    delete scanner;
}
