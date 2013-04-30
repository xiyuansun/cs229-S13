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

void print_usage(int status);

int main(int argc, char* argv[])
{
    char* cur_arg;
    string file("");
    int* x_range = NULL;
    int* y_range = NULL;
    ifstream* input = NULL;

    try
    {
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
    
        input = new ifstream();
        input->open(file.c_str());

        if(input->fail()) throw runtime_error("Failed attempting to open " + file);

        AutFile* a = new AutFile(input, x_range, y_range, x_range, y_range);

        QApplication app(argc, argv);
    
        OptionsDialog* opt = new OptionsDialog(a);
        opt->show();

        return app.exec();
    }
    catch(runtime_error &e)
    {
        cerr << "ERROR: " << e.what() << "\n";
        exit(1);
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
