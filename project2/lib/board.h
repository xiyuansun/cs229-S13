#ifndef __BOARD__
#define __BOARD__

#include <string>
#include <vector>

class Board
{
    public:
        Board(int x_range[], int y_range[], int x_disp_range[], int y_disp_range[], char* states, unsigned int num_states);
        char get_state_char(unsigned int state);
        void next_generation();
        char get_state(int x, int y);
        unsigned int get_num_states();
        std::string to_string();
        std::string to_aut();
    private:
        unsigned int x_size;
        int x_offset;
        unsigned int x_disp_size;
        int x_disp_offset;
        
        unsigned int y_size;
        int y_offset;
        unsigned int y_disp_size;
        int y_disp_offset;

        string states;
        char* board[];

        void set_state_char(unsigned int state, char c);
};

#endif
