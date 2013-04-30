#ifndef __SCANNER__
#define __SCANNER__

#include <string>
#include <istream>
#include <sstream>
#include "../constants.h"

class Scanner
{
    public:
        Scanner(std::string &s, std::string d=WHITESPACE, char c='#');
        Scanner(std::istream *is, std::string d=WHITESPACE, char c='#');
        ~Scanner();
        void set_delimiter(std::string d);
        void set_comment(char c);
        std::string next();
        bool has_next() const;
        int next_int();
        char next_char();
        std::string next_line();
    private:
        std::string delimiter;
        char comment;
        bool hasNext;
        char block;
        bool del;
        std::istream *source;
        bool is_delimiter(char c);
        bool in_str();
        bool in_brace();
};

#endif
