#include "briansbrain.h"

BriansBrain::BriansBrain(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors) : Board(x_range, y_range, x_disp_range, y_disp_range, states, colors)
{
}

void BriansBrain::next_generation()
{
    int x_max = this->x_offset + this->x_size;
    int y_max = this->y_offset + this->y_size;

    // Update neighbor count
    for(int x = this->x_offset; x < x_max; ++x)
    {
        int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            int y_ind = y - this->y_offset;
            unsigned int state_mod = get_neighbors(x, y);
            unsigned int neighbors = count_neighbors(x, y);

            this->board[x_ind][y_ind] -= state_mod;
            this->board[x_ind][y_ind] += neighbors;
        }
    }

    // Updade cell based on neighbor count
    for(int x = this->x_offset; x < x_max; ++x)
    {
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int state = get_state(x, y);
            unsigned int state_mod = get_neighbors(x, y);

            if(state == 0 && state_mod == 2)
            {
                // Turn on
                set_state(x, y, 2);
            }
            else if(state == 1)
            {
                // Dead
                set_state(x, y, 0);
            }
            else if(state == 2)
            {
                // Dying
                set_state(x, y, 1);
            }
        }
    }
}

unsigned int BriansBrain::count_neighbors(int x, int y)
{
    unsigned int count = 0;
    for(int i = x-1; i <= x+1; ++i)
    {
        for(int j = y-1; j <= y+1; ++j)
        {
            if(i != x || j != y)
            {
                // Only care about counting "On" cells
                if(get_state(i, j) == 2) ++count;
            }
        }
    }

    return count;
}
