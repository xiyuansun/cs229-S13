#include <cstdlib>
#include "util.h"

std::vector<std::string>* split(std::string s, char c, int n)
{
    unsigned int i = 0;
    unsigned int start = 0;
    std::vector<std::string>* ret = new std::vector<std::string>();
    while(n-- != 0 && i < s.length())
    {
        if(s[i] == c)
        {
            ret->push_back(s.substr(start, i - start));
            start = ++i;
        }
        else
        {
            ++i;
        }
    }

    if(start != i) ret->push_back(s.substr(start));

    return ret;
}

bool is_whitespace(char c, std::string ws)
{
    bool ret = false;
    
    for(unsigned int i = 0; i < ws.length(); ++i)
    {
        if(c == ws[i])
        {
            ret = true;
            break;
        }
    }

    return ret;
}

std::string remove_whitespace(std::string& s, std::string ws)
{
    for(unsigned int i = 0; i < s.length(); ++i)
    {
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
    char* endptr;
    int ret = strtol(s.c_str(), &endptr, 10);

    if(endptr != '\0')
    {
        //TODO: Barf
    }

    return ret;
}
