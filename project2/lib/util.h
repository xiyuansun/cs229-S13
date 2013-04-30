#ifndef __UTIL__
#define __UTIL__

#include <vector>
#include <string>
#include "constants.h"

// Splits a string s at n instances of character c.
// If n is not given, or is negative, s will be
// split as many times as possible before returning.
// s itself will not be modified.
//
// RETURN: A pointer to a vector of strings with at
//         least one item.
std::vector<std::string>* split(std::string s, char c, int n=-1);

// Returns true if c is in the string ws. ws
// defaults to the definition of whitespace
// found in constants.h
bool is_whitespace(char c, std::string ws=WHITESPACE);

// Modifies s, removing every instance any character
// found in ws. ws defaults to the definition of
// whitespace found in constants.h
//
// RETURN: The string s.
std::string remove_whitespace(std::string& s, std::string ws=WHITESPACE);

// Attempts to parse an int from the string s,
// throws an exception if it cannot.
//
// RETURN: integer from string s
// THROWS: std::runtime_error
int get_int(std::string s);

// Finds the maximum of two integers
//
// RETURN: a if a > b, else b.
int inline max(int a, int b)
{
    return (a > b) ? a : b;
}

#endif
