#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "scanner.h"

// Make Scanner to parse string
Scanner::Scanner(std::string &s, std::string d, char c)
{
    source = new std::istringstream(s);
    delimiter = d;
    comment = c;
    hasNext = true;
    block = 0;

    // Made a new istringstream. Must delete it later.
    del = true;
}

// Make Scanner to parse input stream.
Scanner::Scanner(std::istream *is, std::string d, char c)
{
    source = is;
    delimiter = d;
    comment = c;
    hasNext = true;
    block = 0;

    // Didn't make new stream, don't delete.
    del = false;
}

Scanner::~Scanner()
{
    if(del) delete source;
}

std::string Scanner::next()
{
    std::string ret = "";
    char c = source->get();

    // If this call is legal
    //  1) Find the next non-delimiter character
    //  2) Loop until we hit the end or we find
    //     a delimiter character while we aren't
    //     in a {} block.
    //      a) set up a new block if necessary
    //      b) else exit block if necessary
    //      c) else, if comment, ignore
    //      d) else safe to push to string.
    //  3) Get next character, jump to 2
    //  4) If we've hit the end of the stream,
    //     calls to next() will no longer be legal.
    if(this->hasNext)
    {
        while(is_delimiter(c) && !source->eof())
        {
            c = source->get();
        }

        while((!is_delimiter(c) || in_brace() || in_str()) && !source->eof())
        {
            if(!in_str() && !in_brace() && (c == '"' || c == '{'))
            {
                block = c;
            }
            else if((in_str() && c == '"') || (in_brace() && c == '}'))
            {
                block = '\0';
            }
            else if(c == comment && !in_str())
            {
                next_line();
            }
            else
            {
                ret.push_back(c);
            }

            if(c == '"') ret.push_back(c);

            c = source->get();
        }
        
        if(source->eof())
        {
            this->hasNext = false;
        }
    }
    else
    {
       throw std::runtime_error("Scanner has no next."); 
    }
    
    return ret;
}

int Scanner::next_int()
{
    // Attempt to pull integer from source
    int ret = 0;
    (*source) >> ret;
    
    // Check for failure
    if(source->fail())
    {
        throw std::runtime_error("Next token is not an integer.");
    }

    return ret;
}

char Scanner::next_char()
{
    // Pull next non-comment character
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
    // Get the next full line, or get to EOF
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
    //Loop over delimiter to check for c
    unsigned int i;
    for(i = 0; i < delimiter.size(); ++i)
    {
        if(c == delimiter[i])
            break;
    }

    return i != delimiter.size();
}
