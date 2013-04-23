#include "board.h"
#include <cstdlib>
#include <iostream>

Board::Board(int x_range[], int y_range[], int x_disp_range[], int y_disp_range[], std::string states)
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

    this->board = (char**) malloc(sizeof(char*) * x_size);

    for(unsigned int x = 0; x < x_size; ++x)
    {
        board[x] = (char*) malloc(sizeof(char) * y_size);

        for(unsigned int y = 0; y < y_size; ++y)
        {
            board[x][y] = this->states[0];
        }
    }
}

Board::~Board()
{
    for(unsigned int x = 0; x < x_size; ++x)
    {
        delete[] board[x];
    }
    delete[] board;
}

char Board::get_state_char(unsigned int state)
{
    return this->states[state];
}

void Board::next_generation()
{
}

char Board::get_state(int x, int y)
{
    char ret;
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind < 0 || x_ind >= x_size || y_ind < 0 || y_ind >= y_size)
    {
        ret = this->states[0]; 
    }
    else
    {
        ret = board[x_ind][y_ind];
    }

    return ret;
}

void Board::set_state(int x, int y, unsigned int state)
{
    unsigned int x_ind = x - x_offset;
    unsigned int y_ind = y - y_offset;

    if(x_ind >= 0 && x_ind < x_size && y_ind >= 0 && y_ind < y_size && state < states.length())
    {
        board[x_ind][y_ind] = states[state];
    }
}

unsigned int Board::get_num_states()
{
    return states.length();
}

std::string* Board::to_string()
{
    std::string* ret = new std::string("");

    int x_max = this->x_disp_size + this->x_disp_offset;
    int y_max = this->y_disp_size + this->y_disp_offset;
    
    for(int y = y_max - 1; y >= this->y_disp_offset; --y)
    {
        for(int x = this->x_disp_offset; x < x_max; ++x)
        {
            ret->push_back(this->get_state(x, y));
        }
        
        if(y > this->y_disp_offset)
            ret->push_back('\n');
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
