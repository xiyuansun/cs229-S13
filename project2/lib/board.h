#ifndef __BOARD__
#define __BOARD__

#include <string>
#include <vector>

class Board
{
    public:
        Board(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states);
        ~Board();
        char get_state_char(unsigned int state);
        void next_generation();
        unsigned int get_state(int x, int y);
        unsigned int get_neighbors(int x, int y);
        void set_state(int x, int y, unsigned int state);
        std::string to_string();
        std::string to_string(int start_x, int start_y, int max_x, int max_y);
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

        std::string states;
        int** board;

        void set_state_char(unsigned int state, char c);
        unsigned int count_neighbors(int x, int y);
};

#endif
