#ifndef __AUT__
#define __AUT__

#include "../board.h"
#include <string>
#include <vector>

class AutFile
{
    public:
        AutFile(std::string &in);
        ~AutFile();
        Board const get();
        //void update(Board &b);
        std::string const to_string();
    private:
        int x_range[];
        int y_range[];
        int x_disp_range[];
        int y_disp_range[];
        std::string states;
        std::string name;
        void parse(std::string &s);
        std::vector<std::string> statements;
        Board* b;
};
#endif
