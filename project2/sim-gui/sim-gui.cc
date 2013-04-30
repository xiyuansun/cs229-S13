#include <QApplication>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>

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

    /*
     * Parse args
     */
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
                    //TODO: Barf
                }
                
                x_range = new int[2];

                x_range[0] = get_int((*sp)[0]);
                x_range[1] = get_int((*sp)[1]);
                ++i;

                delete sp;
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
    
    input = new ifstream();
    input->open(file.c_str());

    AutFile* a = new AutFile(input, x_range, y_range, x_range, y_range);

    QApplication app(argc, argv);
    
    OptionsDialog* opt = new OptionsDialog(a);
    opt->show();

    return app.exec();
}

void print_usage(int status)
{
    return;
}
