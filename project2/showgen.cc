#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <stdexcept>

#include "lib/io/aut.h"
#include "lib/board.h"
#include "lib/util.h"

using namespace std;

// Prints usage for the program, then exits with status status.
void print_usage(int status);

int main(int argc, char* argv[])
{
    // Keeps track of the current argument in the argparse loop
    char* cur_arg;

    // Input filename
    string file("");

    // Used to parse input
    AutFile* a;

    // Stores number of generations to run
    int generations = 0;

    // Output type
    string out_type = "terrain";

    // Terrain and Window ranges
    int* x_range = NULL;
    int* y_range = NULL;
    int* x_disp_range = NULL;
    int* y_disp_range = NULL;

    // Input stream
    ifstream* input = NULL;

    try
    {
        // Argparse loop:
        //  1) gets current argument
        //  2) compares against accepted switches
        //  3) takes appropriate action

        for(int i = 1; i < argc; ++i)
        {
            cur_arg = argv[i];

            if(strcmp(cur_arg, "-h") == 0)
            {
                print_usage(0);
            }
            else if(strcmp(cur_arg, "-a") == 0)
            {
                out_type = "aut";
            }
            else if(strcmp(cur_arg, "-g") == 0)
            {
                if(i + 1 < argc)
                {
                    generations = get_int(argv[i + 1]);
                    ++i;
                }
                else
                {
                    throw runtime_error("The -g switch requires an argument.\nUse " + string(argv[0]) + " -h for more details.");
                }
            }
            else if(strcmp(cur_arg, "-tx") == 0)
            {
                if(i + 1 < argc)
                {
                    vector<string>* sp = split(string(argv[i + 1]), ',');

                    if(sp->size() != 2)
                    {
                        delete sp;
                        throw runtime_error("-tx needs a low and a high value.\nUse " + string(argv[0]) + " -h for more details.");
                    }
                
                    x_range = new int[2];

                    x_range[0] = get_int((*sp)[0]);
                    x_range[1] = get_int((*sp)[1]);
                    ++i;

                    delete sp;
                }
                else
                {
                    throw runtime_error("The -tx switch requires an argument.\nUse " + string(argv[0]) + " -h for more details.");
                }
            }
            else if(strcmp(cur_arg, "-ty") == 0)
            {
                if(i + 1 < argc)
                {
                    vector<string>* sp = split(string(argv[i + 1]), ',');

                    if(sp->size() != 2)
                    {
                        delete sp;
                        throw runtime_error("-ty needs a low and a high value.\nUse " + string(argv[0]) + " -h for more details.");
                    }

                    y_range = new int[2];

                    y_range[0] = get_int((*sp)[0]);
                    y_range[1] = get_int((*sp)[1]);
                    ++i;

                    delete sp;
                }
                else
                {
                    throw runtime_error("The -ty switch requires an argument.\nUse " + string(argv[0]) + " -h for more details.");
                }
            }
            else if(strcmp(cur_arg, "-wx") == 0)
            {
                if(i + 1 < argc)
                {
                    vector<string>* sp = split(string(argv[i + 1]), ',');

                    if(sp->size() != 2)
                    {
                        delete sp;
                        throw runtime_error("-wx needs a low and a high value.\nUse " + string(argv[0]) + " -h for more details.");
                    }
                
                    x_disp_range = new int[2];

                    x_disp_range[0] = get_int((*sp)[0]);
                    x_disp_range[1] = get_int((*sp)[1]);
                    ++i;

                    delete sp;
                }
                else
                {
                    throw runtime_error("The -wx switch requires an argument.\nUse " + string(argv[0]) + " -h for more details.");
                }
            }
            else if(strcmp(cur_arg, "-wy") == 0)
            {
                if(i + 1 < argc)
                {
                    vector<string>* sp = split(string(argv[i + 1]), ',');

                    if(sp->size() != 2)
                    {
                        delete sp;
                        throw runtime_error("-wy needs a low and a high value.\nUse " + string(argv[0]) + " -h for more details.");
                    }
                
                    y_disp_range = new int[2];

                    y_disp_range[0] = get_int((*sp)[0]);
                    y_disp_range[1] = get_int((*sp)[1]);
                    ++i;

                    delete sp;
                }
                else
                {
                    throw runtime_error("The -wy switch requires an argument.\nUse " + string(argv[0]) + " -h for more details.");
                }
            }
            else if(file == "")
            {
                file = cur_arg;
            }
            else
            {
                throw runtime_error("Unrecognized switch " + string(cur_arg) + ".");
            }
        }
        
        if(file == "")
        {
            // If no input file was given, us standard input
            input = (ifstream*) &cin;
        }
        else
        {
            // Attempt to open the input file.
            input = new ifstream();
            input->open(file.c_str());

            if(input->fail()) throw runtime_error("Failed attempting to open " + file);
        }

        // Parse the input file and get the board so we can call next_generation()
        a = new AutFile(input, x_range, y_range, x_disp_range, y_disp_range);
        Board* b = a->get();

        while(generations-- > 0)
        {
            b->next_generation();
        }
        
        // Output
        if(out_type == "terrain")
        {
            const string &last_gen = b->to_string();

            cout << last_gen  << "\n";
        }
        else
        {
            a->update();
            cout << a->to_string() << "\n";
        }
        
        // Cleaning up memory.
        delete a;
    
        if(*input != cin)
        {
            delete input;
        }

        return 0;
    }
    catch (exception &e)
    {
        //If we get an exception, we'll end up here.
        cerr << "ERROR: " << e.what() << "\n";
        exit(1);
    }
}

void print_usage(int status)
{
    cout << "Usage: showgen [OPTIONS]... [FILE]\n";
    cout << "Determines and displays the state of each cell at generation n.\n";
    cout << "\nOPTIONS:\n";
    cout << "\t-a\t\tSpecifies output should be in .aut format. Otherwise, output ASCII grid.\n";
    cout << "\t-g N\t\tSpecifies the desired generation number N. Default is N = 0\n";
    cout << "\t-h\t\tShows this help screen.\n";
    cout << "\t-tx L,H\t\tSet the x range of the terrain from L (low) to H (high). This overrides the input file.\n";
    cout << "\t-ty L,H\t\tSet the y range of the terrain from L (low) to H (high). This overrides the input file.\n";
    cout << "\t-wx L,H\t\tSet the x range of the output view from L (low) to H (high). This overrides the input file.\n";
    cout << "\t-wy L,H\t\tSet the y range of the output view from L (low) to H (high). This overrides the input file.\n";

    exit(status);
}
