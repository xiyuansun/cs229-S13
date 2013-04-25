#ifndef __UTIL__
#define __UTIL__

#include <vector>
#include <string>
#include "constants.h"

std::vector<std::string>* split(std::string s, char c, int n=-1);
bool is_whitespace(char c, std::string ws=WHITESPACE);
std::string remove_whitespace(std::string& s, std::string ws=WHITESPACE);
int get_int(std::string s);

#endif
