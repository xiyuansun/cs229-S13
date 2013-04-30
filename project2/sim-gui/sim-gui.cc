#include <QApplication>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <stdexcept>

#include "widgets/boarddisp.h"
#include "widgets/optionsdialog.h"
#include "../lib/io/aut.h"
#include "../lib/util.h"
#include "../lib/io/scanner.h"
#include "../lib/color.h"

using namespace std;

// Prints usage for the program, then exits with status status
void print_usage(int status);

int main(int argc, char* argv[])
{
    // Current argument of argparse loop
    char* cur_arg;

    // Input file name
    string file("");

    // Values for overriding XRange and YRange in aut file
    int* x_range = NULL;
    int* y_range = NULL;

    // Input stream
    ifstream* input = NULL;

    try
    {
        // Argparse loop
        //  1) get current argument
        //  2) compares against accepted switches
        //  3) takes appropriate action

        for(int i = 1; i < argc; ++i)
        {
            cur_arg = argv[i];

            if(strcmp(cur_arg, "-h") == 0)
            {
                print_usage(0);
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
            else if(file == "")
            {
                file = cur_arg;
            }
            else
            {
                throw runtime_error("Unrecognized switch " + string(cur_arg) + ".");
            }
        }
        
        // If no file was passed in as an argument
        if(file == "")
        {
            throw runtime_error("No file was specifited.\nTry " + string(argv[0]) + " -h for more details.");
        }
        
        // Attempt to open the file
        input = new ifstream();
        input->open(file.c_str());

        if(input->fail()) throw runtime_error("Failed attempting to open " + file);
        
        //Parse file
        AutFile* a = new AutFile(input, x_range, y_range, x_range, y_range);
        
        // Run app
        QApplication app(argc, argv);
        
        // Create and display windows
        OptionsDialog* opt = new OptionsDialog(a);
        opt->show();

        return app.exec();
    }
    catch(runtime_error &e)
    {
        // Show exception and exit
        cerr << "ERROR: " << e.what() << "\n";
        exit(1);
    }
    catch(out_of_range &e)
    {
        cerr << "ERROR: aut file did not specifiy (enough) colors for the simulation.\n";
    }
}

void print_usage(int status)
{
    cout << "USAGE: sim-gui [OPTIONS]... FILE\n";
    cout << "sim-gui provides a graphical interface to interact with .aut files.\n";
    cout << "It reads in one .aut file passed as an argument that specifies the\n";
    cout << "size and initial configuration of the terrain at generation 0, then\n";
    cout << "determines and displays the state of each cell as generations advance.\n";
    cout << "\nOPTIONS:\n";
    cout << "\t-h\t\tDisplays this help screen.\n";
    cout << "\t-tx L,H\t\tSet the x range of the terrain from L (low) to H (high).\n\t\t\tThis overrides the input file.\n";
    cout << "\t-ty L,H\t\tSet the y range of the terrain from L (low) to H (high).\n\t\t\tThis overrides the input file.\n";

    exit(status);
}
