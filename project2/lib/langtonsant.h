#ifndef __ANT__
#define __ANT__

#include "board.h"

class LangtonsAnt : public Board
{
    public:
        LangtonsAnt(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors);

        // Overridden from Board
        void next_generation();

    private:
        // Returns true if cell is white,
        // false if cell is black.
        bool check_color(int x, int y);

        //Sets the marker for a collision at location
        void set_collision(int x, int y);

        //Sets the marker for the next state at location
        void set_next_state(int x, int y, unsigned int ns);
};

#endif
