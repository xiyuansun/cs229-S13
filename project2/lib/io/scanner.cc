#include "scanner.h"
#include <iostream>
#include <cstdlib>

Scanner::Scanner(std::string &s, std::string d=WHITESPACE, char c='#')
{
    source = new std::istringstream(s);
    delimiter = d;
    comment = c;
    hasNext = true;
}

Scanner::Scanner(std::istream *is, std::string d=WHITESPACE, char c='#')
{
    source = is;
    delimiter = d;
    comment = c;
    hasNext = true;
}

void Scanner::set_delimiter(std::string d)
{
    delimiter = d;
}

void Scanner::set_comment(char c)
{
    comment = c;
}

std::string Scanner::next()
{
    std::string ret = "";
    char c = source->get();
    if(this->hasNext)
    {
        while(!is_delimiter(c) && !source->eof())
        {
            if(c == comment)
            {
                next_line();
            }
            else
            {
                ret.push_back(c);
                c = source->get();
            }
        }

        if(source->eof())
        {
            this->hasNext = false;
        }
    }
    else
    {
        //TODO: Barf
        exit(1);
    }
    
    return ret;
}

int Scanner::next_int()
{
}

char Scanner::next_char()
{
    char c = source->get();
    while(c == comment)
    {
        next_line();
        c = source->get();
    }

    return c;
}

std::string Scanner::next_line()
{
    std::string ret = "";
    char c = source->get();
    
    while(c != '\n' && !source->eof())
    {
        ret.push_back(c);
        c = source->get();
    }

    if(source->eof())
    {
        //TODO: Barf
        exit(1);
    }
    
    ret.push_back(c);
    return ret;
}

bool Scanner::is_delimiter(char c)
{
    unsigned int i;
    for(i = 0; i < delimiter.size(); ++i)
    {
        if(c == delimiter[i])
            break;
    }

    return i != delimiter.size();
}
