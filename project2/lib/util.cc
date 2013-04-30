#include <cstdlib>
#include <stdexcept>
#include "util.h"

std::vector<std::string>* split(std::string s, char c, int n)
{
    // Keep track of if we need to split
    unsigned int i = 0;
    unsigned int start = 0;

    // Initialize return value
    std::vector<std::string>* ret = new std::vector<std::string>();

    // Loop for n splits, or until we run out of string
    while(n != 0 && i < s.length())
    {
        if(s[i] == c)
        {
            // split
            ret->push_back(s.substr(start, i - start));
            start = ++i;
            --n;
        }
        else
        {
            ++i;
        }
    }
    
    // Pick up any extra string we didn't use.
    if(start != i) ret->push_back(s.substr(start));

    return ret;
}

bool is_whitespace(char c, std::string ws)
{
    // Default return value
    bool ret = false;
    
    for(unsigned int i = 0; i < ws.length(); ++i)
    {
        if(c == ws[i])
        {
            // Whitespace found, return.
            ret = true;
            break;
        }
    }

    return ret;
}

std::string remove_whitespace(std::string& s, std::string ws)
{
    // Loop over s
    for(unsigned int i = 0; i < s.length(); ++i)
    {
        // Remove whitespace
        if(is_whitespace(s[i], ws))
        {
            s.erase(i, 1);
            --i;
        }
    }

    return s;
}

int get_int(std::string s)
{
    // For error checking
    char* endptr;

    // Attempt the parse
    int ret = strtol(s.c_str(), &endptr, 10);

    if(*endptr != '\0')
    {
        // s can not be an integer of base 10
        throw std::runtime_error("Could not convert " + s + " to an integer.");
    }

    return ret;
}
