#ifndef __SCANNER__
#define __SCANNER__

#include <string>
#include <istream>
#include <sstream>
#include "../constants.h"

// Provides generic input stream parsing
class Scanner
{
    public:
        Scanner(std::string &s, std::string d=WHITESPACE, char c='#');
        Scanner(std::istream *is, std::string d=WHITESPACE, char c='#');
        ~Scanner();

        // Sets the delimiter to d.
        void set_delimiter(std::string d) { delimiter = d; }

        // Set the comment character to c
        void set_comment(char c) { comment = c; }

        // Returns if the scanner has
        // a next token or not. If
        // there are no more tokens
        // left and a call to next()
        // has not been made, will
        // still return true.
        //
        // RETURN: True if a call to
        //         next() is legal.
        bool has_next() const { return hasNext; }

        // Parses out the next token in the
        // input stream and returns it. The
        // next token does not include comments,
        // as defined by the comment character,
        // and will pull in an entire string
        // enclosed in double quotes, as well
        // as a block enclosed by {}. Breaks on
        // delimiter and returns the string built
        // from the input following these rules.
        //
        // RETURN: The next token available in
        //         the input stream.
        // THROWS: runtime_error if the call to
        //         next is not legal.
        std::string next();

        // Attempts to parse an integer from
        // the input stream.
        //
        // RETURN: The integer parsed from the
        //         input stream.
        // THROWS: runtime_error if the attempt
        //         fails.
        int next_int();

        // Gets the next character that is not
        // part of a comment block.
        //
        // RETURN: The next character in the
        //         input stream.
        char next_char();

        // Gets the next line available to
        // the input stream and returns it.
        //
        // RETURN: The next line available
        //         to the input stream
        std::string next_line();
    private:
        // Stores the characters on which to
        // break to the next token.
        std::string delimiter;

        // Stores the character that
        // indicates a comment in the
        // input stream.
        char comment;

        // Flag for if a call to next()
        // is legal. Set by next() when
        // it runs out of tokens to parse.
        bool hasNext;

        // Block that we are currently in.
        char block;

        // Wether or not to delete the 
        // input stream upon deconstruction
        bool del;

        // Input stream
        std::istream *source;

        // Returns true if c is in the
        // delimiter string.
        bool is_delimiter(char c);

        // Returns true if we are currently parsing
        // a string.
        bool in_str() const { return block == '"'; }

        // Returns true if we are currently parsing
        // a {} block.
        bool in_brace() const { return block == '{'; }
};

#endif
