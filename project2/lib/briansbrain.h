#ifndef __BRAIN__
#define __BRAIN__

#include "board.h"

class BriansBrain : public Board
{
    public:
        BriansBrain(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors);

        // Overridden from Board
        void next_generation();

        // Overridden from Board
        unsigned int count_neighbors(int x, int y);
};

#endif
