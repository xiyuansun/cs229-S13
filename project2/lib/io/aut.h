#ifndef __AUT__
#define __AUT__

#include "../board.h"
#include "../color.h"
#include <string>
#include <vector>
#include <fstream>

// Stores information about
// and parses aut files.
class AutFile
{
    public:
        AutFile(std::ifstream* in, int* x_range, int* y_range, int* x_view, int* y_view);
        ~AutFile();

        // Get the board object the
        // input file parsed to.
        //
        // RETURN: Board object representing
        //         the terrain from the aut
        Board* get() const { return this->b; }
        
        // Gets the name as defined in the
        // input file.
        //
        // RETURN: A string containing the
        //         value defined by the Name
        //         keyword in the input file.
        std::string get_name() const { return this->name; }

        // Gets the low bound of the x display range
        // as defined by values passed into the
        // constructor, or by the input file
        //
        // RETURN: An integer representing the
        //         low x display bound.
        int get_x_low() const { return this->x_disp_range[0]; }

        // Gets the high bound of the x display range
        // as defined by values passed into the
        // constructor, or by the input file
        //
        // RETURN: An integer representing the
        //         high x display bound.
        int get_x_high() const { return this->x_disp_range[1]; }

        // Gets the low y bound of the display range
        // as defined by values passed into the
        // constructor, or by the input file
        //
        // RETURN: An integer representing the
        //         low y display bound.
        int get_y_low() const { return this->y_disp_range[0]; }

        // Gets the high y bound of the display range
        // as defined by values passed into the
        // constructor, or by the input file
        //
        // RETURN: An integer representing the
        //         high y display bound.
        int get_y_high() const { return this->y_disp_range[1]; }

        // Gets updates the string representation
        // of the aut file by pulling current
        // state information from the board
        // object.
        void update() { statements.push_back(this->b->to_aut()); }

        // Provides a string representation of the
        // aut file, aka the contents of the aut
        // file.
        //
        // RETURN: The contents of the aut file.
        std::string to_string() const;

    private:
        // Terrain ranges
        int* x_range;
        int* y_range;

        // Display ranges
        int* x_disp_range;
        int* y_disp_range;

        // State characters
        std::string states;

        // Name of the simulation
        std::string name;

        // Parses the input file,
        // Keeping track of
        // unrecognized keywords.
        void parse(std::ifstream* in);

        // Keep track of the statements
        // for forward compatibility
        std::vector<std::string> statements;

        // Terrain representation of
        // the aut file.
        Board* b;

        // Colors parsed from the aut file
        std::vector<Color>* colors;
};
#endif
