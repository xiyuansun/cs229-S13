#include "scanner.h"
#include <iostream>
#include <cstdlib>

Scanner::Scanner(std::string &s, std::string d, char c)
{
    source = new std::istringstream(s);
    delimiter = d;
    comment = c;
    hasNext = true;
    block = 0;
    del = true;
}

Scanner::Scanner(std::istream *is, std::string d, char c)
{
    source = is;
    delimiter = d;
    comment = c;
    hasNext = true;
    block = 0;
    del = false;
}

Scanner::~Scanner()
{
    if(del) delete source;
}

void Scanner::set_delimiter(std::string d)
{
    delimiter = d;
}

void Scanner::set_comment(char c)
{
    comment = c;
}

bool const Scanner::has_next()
{
    return hasNext;
}

std::string Scanner::next()
{
    std::string ret = "";
    char c = source->get();
    if(this->hasNext)
    {
        while(is_delimiter(c) && !source->eof())
        {
            c = source->get();
        }

        while((!is_delimiter(c) || in_brace()) && !source->eof())
        {
            if(!in_str() && !in_brace() && (c == '"' || c == '{'))
            {
                block = c;
            }
            else if((in_str() && c == '"') || (in_brace() && c == '}'))
            {
                block = '\0';
            }

            if(c == comment && !in_str())
            {
                next_line();
            }
            else
            {
                ret.push_back(c);
            }

            c = source->get();
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
    int ret = 0;
    (*source) >> ret;

    if(source->fail())
    {
        //TODO: Barf
        exit(1);
    }

    return ret;
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

bool Scanner::in_str()
{
    return block == '"';
}

bool Scanner::in_brace()
{
    return block == '{';
}
