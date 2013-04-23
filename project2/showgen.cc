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
            }
        }
        else if(strcmp(cur_arg, "-tx") == 0)
        {
            if(i + 1 < argc)
            {
            }
            else
            {
            }
        }
        else if(strcmp(cur_arg, "-ty") == 0)
        {
            if(i + 1 < argc)
            {
            }
            else
            {
            }
        }
        else if(strcmp(cur_arg, "-wx") == 0)
        {
            if(i + 1 < argc)
            {
            }
            else
            {
            }
        }
        else if(strcmp(cur_arg, "-wy") == 0)
        {
            if(i + 1 < argc)
            {
            }
            else
            {
            }
        }
        else if(file == "")
        {
            file = cur_arg;
        }
        else
        {
        }
    }

    file("file.aut");
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
