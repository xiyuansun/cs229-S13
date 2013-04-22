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
        Board* get() const;
        //void update(Board &b);
        std::string to_string() const;
    private:
        int* x_range;
        int* y_range;
        int* x_disp_range;
        int* y_disp_range;
        std::string states;
        std::string name;
        void parse(std::string &s);
        std::vector<std::string> statements;
        Board* b;
};
#endif
