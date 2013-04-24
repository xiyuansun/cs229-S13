#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include "lib/io/aut.h"
#include "lib/board.h"

using namespace std;

int main(int argc, const char* argv[])
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
                vector<string>* sp = split(argv[i + 1], ',');

                if(sp.size() != 2)
                {
                    //TODO: Barf
                }
                
                x_range = new int[2];

                x_range[0] = get_int((*sp)[0]);
                x_range[1] = get_int((*sp)[1]);
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
                vector<string>* sp = split(argv[i + 1], ',')

                if(sp.size() != 2)
                {
                    //TODO: Barf
                }

                y_range = new int[2];

                y_range[0] = get_int((*sp)[0]);
                y_range[1] = get_int((*sp)[1]);
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
                vector<string>* sp = split(argv[i + 1], ',')

                if(sp.size() != 2)
                {
                    //TODO: Barf
                }
                
                x_disp_range = new int[2];

                x_disp_range[0] = get_int((*sp)[0]);
                x_disp_range[1] = get_int((*sp)[1]);
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
                vector<string>* sp = split(argv[i + 1], ',')

                if(sp.size() != 2)
                {
                    //TODO: Barf
                }
                
                y_disp_range = new int[2];

                y_disp_range[0] = get_int((*sp)[0]);
                y_disp_range[1] = get_int((*sp)[1]);
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

    
    AutFile* a = new AutFile(file);
    Board* b = a->get();
    cout << *(b->to_string()) << "\n";
    /*vector<string> vs = a->get();
    
    for(unsigned int i = 0; i < vs.size(); ++i)
    {
        cout << vs[i] << endl;
    }*/

    return 0;
}
