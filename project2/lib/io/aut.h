#ifndef __AUT__
#define __AUT__

#include "../board.h"
#include <string>
#include <vector>
#include <fstream>

class AutFile
{
    public:
        AutFile(std::ifstream* in, int* x_range, int* y_range, int* x_view, int* y_view);
        ~AutFile();
        Board* get() const;
        std::string get_name() const;
        int get_x_low() const;
        int get_x_high() const;
        int get_y_low() const;
        int get_y_high() const;
        //void update(Board &b);
        std::string to_string() const;
    private:
        int* x_range;
        int* y_range;
        int* x_disp_range;
        int* y_disp_range;
        std::string states;
        std::string name;
        void parse(std::ifstream* in);
        std::vector<std::string> statements;
        Board* b;
};
#endif
