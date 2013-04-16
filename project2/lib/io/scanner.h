#ifndef __SCANNER__
#define __SCANNER__

#include <string>
#include <istream>
#include <sstream>


#define WHITESPACE " \t\n\f"

class Scanner
{
    public:
        Scanner(std::string &s, std::string d=WHITESPACE, char c='#');
        Scanner(std::istream *is, std::string d=WHITESPACE, char c='#');
        void set_delimiter(std::string d);
        void set_comment(char c);
        std::string next();
        bool const has_next();
        int next_int();
        char next_char();
        std::string next_line();
    private:
        std::string delimiter;
        char comment;
        bool hasNext;
        char block;
        std::istream *source;
        bool is_delimiter(char c);
        bool in_str();
        bool in_brace();
};

#endif
