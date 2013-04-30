#include "board.h"
#include <cstdlib>
#include <iostream>

Board::Board(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors)
{
    this->x_size = x_range[1] - x_range[0] + 1;
    this->x_offset = x_range[0];
    this->x_disp_size = x_disp_range[1] - x_disp_range[0] + 1;
    this->x_disp_offset = x_disp_range[0];
    
    this->y_size = y_range[1] - y_range[0] + 1;
    this->y_offset = y_range[0];
    this->y_disp_size = y_disp_range[1] - y_disp_range[0] + 1;
    this->y_disp_offset = y_disp_range[0];

    this->states = states;
    this->colors = colors;

    this->board = (int**) malloc(sizeof(int*) * this->x_size);

    for(unsigned int x = 0; x < x_size; ++x)
    {
        this->board[x] = (int*) malloc(sizeof(int) * this->y_size);

        for(unsigned int y = 0; y < this->y_size; ++y)
        {
            this->board[x][y] = 0;
        }
    }
}

Board::~Board()
{
    for(unsigned int x = 0; x < x_size; ++x)
    {
        free(board[x]);
    }
    free(board);
}

char Board::get_state_char(unsigned int state)
{
    return this->states[state];
}

Color Board::get_state_color(unsigned int state)
{
    return (*colors)[state];
}

void Board::next_generation()
{
    int x_max = this->x_offset + this->x_size;
    int y_max = this->y_offset + this->y_size;

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

    for(int x = this->x_offset; x < x_max; ++x)
    {
        for(int y = this->y_offset; y < y_max; ++y)
        {
            unsigned int state = get_state(x, y);
            unsigned int state_mod = get_neighbors(x, y);

            if(state > 0)
            {
                if(state_mod < 2 || state_mod > 3)
                {
                    set_state(x, y, 0);
                }
            }
            else
            {
                if(state_mod == 3) set_state(x, y, 1);
            }
        }
    }
}

unsigned int Board::get_state(int x, int y, bool shift)
{
    unsigned int ret, x_ind, y_ind;
    if(shift)
    {
        x_ind = x - x_offset;
        y_ind = y - y_offset;
    }
    else
    {
        x_ind = x;
        y_ind = y;
    }

    if(x_ind >= x_size || y_ind >= y_size)
    {
        ret = 0; 
    }
    else
    {
        ret = (this->board[x_ind][y_ind]) / 10;
    }

    return ret;
}

unsigned int Board::get_neighbors(int x, int y)
{
    unsigned int ret;
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind >= x_size || y_ind >= y_size)
    {
        ret = 0; 
    }
    else
    {
        ret = (this->board[x_ind][y_ind]) % 10;
    }

    return ret;
}

void Board::set_state(int x, int y, unsigned int state)
{
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind < x_size && y_ind < y_size && state < states.length())
    {
        board[x_ind][y_ind] = state * 10 + count_neighbors(x, y);
    }
}

std::string Board::to_string()
{
    int x_max = this->x_disp_size + this->x_disp_offset;
    int y_max = this->y_disp_size + this->y_disp_offset;

    return this->to_string(this->x_disp_offset, this->y_disp_offset, x_max, y_max);
}

std::string Board::to_string(int start_x, int start_y, int max_x, int max_y)
{
    std::string ret("");

    for(int y = max_y - 1; y >= start_y; --y)
    {
        for(int x = start_x; x < max_x; ++x)
        {
            ret.push_back(get_state_char(get_state(x, y)));
        }
        if(y > start_y)
            ret.push_back('\n');
    }
    return ret;
}

std::string Board::to_aut()
{
    return "Not implemented.";
}

void Board::set_state_char(unsigned int state, char c)
{
    this->states[state] = c;
}

unsigned int Board::count_neighbors(int x, int y)
{
    unsigned int count = 0;
    for(int i = x-1; i <= x+1; ++i)
    {
        for(int j = y-1; j <= y+1; ++j)
        {
            if(i != x || j != y)
            {
                if(get_state(i, j) > 0) ++count;
            }
        }
    }

    return count;
}
