#ifndef __BOARD__
#define __BOARD__

#include <string>
#include <vector>
#include "color.h"

// Used to store and keep track of the terrain.
class Board
{
    public:
        Board(int* x_range, int* y_range, int* x_disp_range, int* y_disp_range, std::string states, std::vector<Color>* colors);
        ~Board();

        // Gets the character associate with state state.
        //
        // RETURN: Display character for state state.
        char get_state_char(unsigned int state) { return states.at(state); }

        // Gets the color associated with state state.
        //
        // RETURN: Display color for state state.
        Color get_state_color(unsigned int state) { return colors->at(state); }

        // Calculates the next generation according
        // to Conway's Game of Life ruleset.
        void next_generation();

        // Gets the state of the board at (x, y).
        // Shifts the coordinates from the real
        // board range to array-appropriate ranges
        // if shift is true. Does not do so if shift
        // is false.
        //
        // RETURN: An integer representing the state at (x, y)
        unsigned int get_state(int x, int y, bool shift=true);
        
        // Counts the number of live neighbors
        // for the cell at (x, y). x and y are
        // shifted from real board values to
        // array-appropriate values. May be
        // inaccurate while board is updating
        // with next_generation().
        //
        // RETURN: the number of live neighbors
        //         for the cell at (x, y)
        unsigned int count_neighbors(int x, int y);

        // Sets the state of the cell at (x, y)
        // to state state. x and y are shifted
        // from real board values to array-
        // appropriate values.
        void set_state(int x, int y, unsigned int state);

        // Returns a string representing the
        // board over the whole display range.
        //
        // RETURN: A string representing
        //         the board as terrain.
        std::string to_string();

        // Returns a string representing the
        // board from start_x to max_x on
        // the x-axis and max_y to start_y
        // on the y-axis (top to bottom).
        //
        // RETURN: A string representing
        //         the board as terrain.
        std::string to_string(int start_x, int start_y, int max_x, int max_y);

        // Turns the current state of
        // the board in to an Initial
        // statement, as found in an
        // aut file.
        //
        // RETURN: The current state
        //         of the board as an
        //         Initial statement.
        std::string to_aut();

    private:
        // Keeps track of the x terrain
        unsigned int x_size;
        int x_offset;

        // Keeps track of the x display
        unsigned int x_disp_size;
        int x_disp_offset;
        
        // Keeps track of the y terrain
        unsigned int y_size;
        int y_offset;

        // Keeps track of the y display
        unsigned int y_disp_size;
        int y_disp_offset;
        
        // Keeps track of states
        std::string states;
        
        // Keeps track of colors
        std::vector<Color>* colors;

        // Keeps track of cell locations
        // and values on the board. The
        // ones place is the number of
        // live neighbors, and the tens
        // place is the cell's current
        // state.
        int** board;

        // Returns the number of live neighbors
        // for the cell at (x, y). May be inaccurate,
        // but faster than repeated callse to count_neighbors
        //
        // RETURN: an integer representing the number of
        //         live neighbors for the cell at (x, y)
        unsigned int get_neighbors(int x, int y);
};

#endif
