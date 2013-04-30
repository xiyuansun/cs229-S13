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

void print_usage(int status);

int main(int argc, char* argv[])
{
    char* cur_arg;
    string file("");
    AutFile* a;
    int generations = 0;
    string out_type = "terrain";
    int* x_range = NULL;
    int* y_range = NULL;
    int* x_disp_range = NULL;
    int* y_disp_range = NULL;
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
                    //TODO: Barf
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
                throw runtime_error("Unrecognized argument " + string(cur_arg) + ".");
            }
        }

        if(file == "")
        {
            input = (ifstream*) &cin;
        }
        else
        {
            input = new ifstream();
            input->open(file.c_str());

            if(input->fail()) throw runtime_error("Failed attempting to open " + file);
        }

        a = new AutFile(input, x_range, y_range, x_disp_range, y_disp_range);
        Board* b = a->get();

        while(generations-- > 0)
        {
            b->next_generation();
        }
    
        const string &last_gen = b->to_string();

        cout << last_gen  << "\n";

        delete a;
    
        if(*input != cin)
        {
            delete input;
        }

        return 0;
    }
    catch (exception &e)
    {
        cout << "ERROR: " << e.what() << "\n";
        exit(1);
    }
}

void print_usage(int status)
{
    cout << "This is where usage goes.\n";
    exit(status);
}
