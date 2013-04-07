#ifndef __SCANNER__
#define __SCANNER__

#include <string>
#include <istream>

class Scanner
{
    private:
        std::string delimiter;
        std::string comment;
        std::istream &source;
    public:
        void set_delimiter(std::string s);
        void set_comment(std::string s);
        std::string next();
        int next_int();
        char next_char();
        std::string next_line();
        Scanner(std::string s);
        Scanner(std::istream &is);
};

#endif
