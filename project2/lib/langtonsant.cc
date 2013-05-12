#include "langtonsant.h"

LangtonsAnt::LangtonsAnt(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors) : Board(x_range, y_range, x_disp_range, y_disp_range, states, colors)
{
}

void LangtonsAnt::next_generation()
{
    int x_max = this->x_offset + this->x_size;
    int y_max = this->y_offset + this->y_size;
    
    //flip color and turn ant.
    for(int x = this->x_offset; x < x_max; ++x)
    {
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int state = get_state(x, y);
            if(state == 2) set_state(x, y, 7, false);
            else if(state == 3) set_state(x, y, 8, false);
            else if(state == 4) set_state(x, y, 9, false);
            else if(state == 5) set_state(x, y, 6, false);
            else if(state == 6) set_state(x, y, 5, false);
            else if(state == 7) set_state(x, y, 2, false);
            else if(state == 8) set_state(x, y, 3, false);
            else if(state == 9) set_state(x, y, 4, false);
            set_next_state(x, y, 0);
        }
    }
    // Turn ant and flip color of cell
    // Then add *100 to the cell it is going to be
    // in next
    for(int x = this->x_offset; x < x_max; ++x)
    {
        unsigned int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int y_ind = y - this->y_offset;
            unsigned int next_state = get_state(x, y);
            
            bool clr = check_color(x, y);
            bool other;
            unsigned int other_next_state;
            unsigned int other_state;

            //Determine our state where we're going to move.
            if(next_state == 2 || next_state == 6)
            {
                other = check_color(x, y + 1);
                other_next_state = get_neighbors(x, y + 1);
                other_state = get_state(x, y + 1);

                if(other)
                    next_state = 2;
                else 
                    next_state = 6;

                if(other_next_state > 1 && other_state / 10 == 0) set_collision(x, y + 1);
                else if(other_next_state <= 1) set_next_state(x, y + 1, next_state);
            }
            else if(next_state == 3 || next_state == 7)
            {
                other = check_color(x + 1, y);
                other_next_state = get_neighbors(x + 1, y);
                other_state = get_state(x + 1, y);

                if(other)
                    next_state = 3;
                else 
                    next_state = 7;

                if(other_next_state > 1 && other_state / 10 == 0) set_collision(x + 1, y);
                else if(other_next_state <= 1) set_next_state(x + 1, y, next_state);
            }
            else if(next_state == 4 || next_state == 8)
            {
                other = check_color(x, y - 1);
                other_next_state = get_neighbors(x, y - 1);
                other_state = get_state(x, y - 1);

                if(other)
                    next_state = 4;
                else 
                    next_state = 8;

                if(other_next_state > 1 && other_state / 10 == 0) set_collision(x, y - 1);
                else if(other_next_state <= 1) set_next_state(x, y - 1, next_state);
            }
            else if(next_state == 5 || next_state == 9)
            {
                other = check_color(x - 1, y);
                other_next_state = get_neighbors(x - 1, y);
                other_state = get_state(x - 1, y);

                if(other)
                    next_state = 5;
                else 
                    next_state = 9;

                if(other_next_state > 1 && other_state / 10 == 0) set_collision(x - 1, y);
                else if(other_next_state <= 1) set_next_state(x - 1, y, next_state);
            }
            
            //If we don't already have a next state, set it. This may be overridden.
            unsigned int this_next_state = get_neighbors(x, y);
            if(!clr && this_next_state == 0) this->board[x_ind][y_ind] += 1;
        }
    }

    // Update invading counts
    for(int x = this->x_offset; x < x_max; ++x)
    {
        unsigned int x_ind = x - this->x_offset;
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int y_ind = y - this->y_offset;
            unsigned int state_info = this->board[x_ind][y_ind];
            unsigned int end_state = (check_color(x, y)) ? 0 : 1;

            if(state_info / 100 == 0)
            {
                this->board[x_ind][y_ind] = (state_info % 10) * 10;
            }
            else
            {
                this->board[x_ind][y_ind] = end_state; 
            }
        }
    }
}

bool LangtonsAnt::check_color(int x, int y)
{
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;
    bool ret;

    if(x_ind >= x_size || y_ind >= y_size)
    {
        ret = true;
    }
    else
    {
        unsigned int state = (this->board[x_ind][y_ind] / 10) % 10;
        if(state != 1 && state <= 5) ret = true;
        else ret = false;
    }
    
    return ret;
}

void LangtonsAnt::set_collision(int x, int y)
{
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind < x_size && y_ind < y_size)
    {
        this->board[x_ind][y_ind] += 100;
    }
}

void LangtonsAnt::set_next_state(int x, int y, unsigned int ns)
{
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind < x_size && y_ind < y_size)
    {
        this->board[x_ind][y_ind] -= this->board[x_ind][y_ind] % 10;
        this->board[x_ind][y_ind] += ns;
    }
}
