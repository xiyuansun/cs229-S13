#include "langtonsant.h"

LangtonsAnt::LangtonsAnt(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors) : Board(x_range, y_range, x_disp_range, y_disp_range, states, colors)
{
}

void LangtonsAnt::next_generation()
{
    /*int x_max = this->x_offset + this->x_size;
    int y_max = this->y_offset + this->y_size;
   
    // Turn ant and flip color of cell
    // Then add *100 to the cell it is going to be
    // in next
    for(int x = this->x_offset; x < x_max; ++x)
    {
        unsigned int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int y_ind = y - this->y_offset;
            unsigned int state = get_state(x, y);

            //Turned state
            unsigned int next_state = 0
            
            bool clr = check_color(x, y);
            bool other;
            unsigned int other_next_state;
            unsigned int other_state;

            //Turn the ant
            if(state == 2) next_state = 3;
            else if(state == 3) next_state = 4;
            else if(state == 4) next_state = 5;
            else if(state == 5) next_state = 2;
            else if(state == 6) next_state = 9;
            else if(state == 7) next_state = 6;
            else if(state == 8) next_state = 7;
            else if(state == 9) next_state = 8;

            if(next_state == 2 || next_state == 6)
            {
                other = check_color(x, y + 1);
                other_next_state = get_neighbors(x, y + 1);
                other_state = get_state(x, y + 1);

                if(other)
                    next_state = 2;
                else 
                    next_state = 6;

                if(other_next_state > 0 && other_state / 10 == 0) set_collision(x, y + 1);
                else if(other_next_state == 0) set_next_state(x, y + 1, next_state);
            }
            else if(next_state == 3 || next_state = 7)
            {
                other = check_color(x + 1, y);
                other_next_state = get_neighbors(x + 1, y);
                other_state = get_state(x + 1, y);

                if(other)
                    next_state = 3;
                else 
                    next_state = 7;

                if(other_next_state > 0 && other_state / 10 == 0) this->board[x_ind][y_ind] += 100;
                else if(other_next_state == 0) this->board[x_ind + 1][y_ind] += next_state;
            }
            else if(next_state == 4 || next_state = 8)
            {
                other = check_color(x, y - 1);
                other_next_state = get_neighbors(x, y - 1);
                other_state = get_state(x, y - 1);

                if(other)
                    next_state = 4;
                else 
                    next_state = 8;

                if(other_next_state > 0 && other_state / 10 == 0) this->board[x_ind][y_ind] += 100;
                else if(other_next_state == 0) this->board[x_ind][y_ind - 1] += next_state;
            }
            else if(next_state == 5 || next_state = 9)
            {
                other = check_color(x - 1, y);
                other_next_state = get_neighbors(x - 1, y);
                other_state = get_state(x - 1, y);

                if(other)
                    next_state = 5;
                else 
                    next_state = 9;

                if(other_next_state > 0 && other_state / 10 == 0) this->board[x_ind][y_ind] += 100;
                else if(other_next_state == 0) this->board[x_ind - 1][y_ind] += next_state;
            }
            
            unsigned int this_next_state = get_neighbors(x, y);
            if(!clr && this_next_state == 0) this->board[x_ind][y_ind] += 1;
        }
    }

    // Update invading counts
    for(int x = this->x_offset; x < x_max; ++x)
    {
        int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            y_ind = y - this->y_offset;
            unsigned int state_mod = get_neighbors(x, y);
            unsigned int neighbors = count_neighbors(x, y);

            this->board[x_ind][y_ind] -= state_mod;
            this->board[x_ind][y_ind] += neighbors;
        }
    }

    // Update cell based on invading count
    for(int x = this->x_offset; x < x_max; ++x)
    {
        int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            int y_ind = y - this->y_offset;
            
            if(
        }
    }*/
}

unsigned int LangtonsAnt::count_neighbors(int x, int y)
{
    /* IEEE random number. */
    return 4;
}
