#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
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
                    //TODO: Barf
                }
                
                x_range = new int[2];

                x_range[0] = get_int((*sp)[0]);
                x_range[1] = get_int((*sp)[1]);
                ++i;

                delete sp;
            }
            else
            {
                //TODO: Barf
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
                    //TODO: Barf
                }

                y_range = new int[2];

                y_range[0] = get_int((*sp)[0]);
                y_range[1] = get_int((*sp)[1]);
                ++i;

                delete sp;
            }
            else
            {
                //TODO: Barf
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
                    //TODO: Barf
                }
                
                x_disp_range = new int[2];

                x_disp_range[0] = get_int((*sp)[0]);
                x_disp_range[1] = get_int((*sp)[1]);
                ++i;

                delete sp;
            }
            else
            {
                //TODO: Barf
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
                    //TODO: Barf
                }
                
                y_disp_range = new int[2];

                y_disp_range[0] = get_int((*sp)[0]);
                y_disp_range[1] = get_int((*sp)[1]);
                ++i;

                delete sp;
            }
            else
            {
                //TODO: Barf
            }
        }
        else if(file == "")
        {
            file = cur_arg;
        }
        else
        {
            //TODO: Barf
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

void print_usage(int status)
{
    cout << "This is where usage goes.\n";
    exit(status);
}
