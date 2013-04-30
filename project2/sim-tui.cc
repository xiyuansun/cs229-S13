#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <stdexcept>

#include "lib/board.h"
#include "lib/io/aut.h"
#include "lib/util.h"

using namespace std;

void print_usage(int status);
WINDOW* make_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW* local_win);
void print_gen(WINDOW* local_win, Board* b, int start_y, int start_x, int height, int width);
void disp_x_scroll(WINDOW* local_win, int y_pos, int start, int scroll_size, int width);
void disp_y_scroll(WINDOW* local_win, int x_pos, int start, int scroll_size, int height);

int main(int argc, char* argv[])
{
    //Display variables
    WINDOW* board_win;
    int start_x, start_y, width, height;
    int ch;
    double x_shown, y_shown;
    int max_x, min_y, disp_start_x, disp_start_y;
    bool enable_x_scroll, enable_y_scroll;
    int x_scroll_size, y_scroll_size, x_scroll_start, y_scroll_start;

    //generation state variables
    int generation = 0;
    int delay = 100;
    bool paused = true;
    bool done = false;
    bool step = false;
    bool update = false;

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

        if(file == "")
        {
            throw runtime_error("No file was specifited.\nTry " + string(argv[0]) + " -h for more details.");
        }

        input = new ifstream();
        input->open(file.c_str());
    
        if(input->fail()) throw runtime_error("Failed attempting to open " + file);

        AutFile* a = new AutFile(input, x_range, y_range, x_range, y_range);
        Board* b = a->get();

        initscr();
        raw();
        curs_set(0);
        keypad(stdscr, TRUE);
        timeout(delay);
        noecho();
    
        height = LINES - 3;
        width = COLS;
        start_y = 2;
        start_x = 0;
        refresh();
        board_win = make_win(height, width, start_y, start_x);
   
        string board_name = a->get_name();

        mvwprintw(stdscr, 0, COLS/2 - board_name.length()/2, board_name.c_str());
        mvwprintw(stdscr, 1, 0, "Delay: %5d (+/-)", delay);
        mvwprintw(stdscr, 1, COLS - 16, "Generation %5d", generation);
        mvwprintw(stdscr, LINES - 1, 0, "(Q)UIT  (P)LAY/PAUSE  (S)TEP  Arrows: scroll");
    
        x_shown = (a->get_x_high() - a->get_x_low()) / (width - 2.0);
        y_shown = (a->get_y_high() - a->get_y_low()) / (height - 2.0);
        max_x = a->get_x_high() - (width - 2);
        min_y = a->get_y_low() + (height - 2);
        disp_start_x = a->get_x_low();
        disp_start_y = a->get_y_high();
        x_scroll_size = max((int) (1/x_shown * (width - 4)), 1);
        y_scroll_size = max((int) (1/y_shown * (height - 4)), 1);
        y_scroll_start = 2;
        x_scroll_start = 2;


        enable_x_scroll = x_shown >= 1;
        enable_y_scroll = y_shown >= 1;

        print_gen(board_win, b, disp_start_y, disp_start_x, height - 1, width - 2);

        if(enable_x_scroll)
        {
            disp_x_scroll(board_win, height - 1, x_scroll_start, x_scroll_size, width);
        }

        if(enable_y_scroll)
        {
            disp_y_scroll(board_win, width - 1, y_scroll_start, y_scroll_size, height);
        }

        while(!done)
        {
            step = false;
            update = false;
            ch = getch();

            switch(ch)
            {
                case KEY_LEFT:
                    if(enable_x_scroll && disp_start_x > a->get_x_low())
                    {
                        --disp_start_x;
                        x_scroll_start = (int) ((1.0 * disp_start_x - a->get_x_low())/(max_x - a->get_x_low()) * (width - 4 - x_scroll_size) + 2);
                        disp_x_scroll(board_win, height - 1, x_scroll_start, x_scroll_size, width);
                        update = true;
                    }
                    break;
                case KEY_RIGHT:
                    if(enable_x_scroll && disp_start_x < max_x)
                    {
                        ++disp_start_x;
                        x_scroll_start = (int) ((1.0 * disp_start_x - a->get_x_low())/(max_x - a->get_x_low()) * (width - 4 - x_scroll_size) + 2);
                        disp_x_scroll(board_win, height - 1, x_scroll_start, x_scroll_size, width);
                        update = true;
                    }
                    break;
                case KEY_UP:
                    if(enable_y_scroll && disp_start_y < a->get_y_high())
                    {
                        ++disp_start_y;
                        y_scroll_start = (int) ((1.0 * a->get_y_high() - disp_start_y)/(a->get_y_high() - min_y) * (height - 4 - y_scroll_size) + 2);
                        disp_y_scroll(board_win, width - 1, y_scroll_start, y_scroll_size, height);
                        update = true;
                    }
                    break;
                case KEY_DOWN:
                    if(enable_y_scroll && disp_start_y > min_y)
                    {
                        --disp_start_y;
                        y_scroll_start = (int) ((1.0 * a->get_y_high() - disp_start_y)/(a->get_y_high() - min_y) * (height - 4 - y_scroll_size) + 2);
                        disp_y_scroll(board_win, width - 1, y_scroll_start, y_scroll_size, height);
                        update = true;
                    }
                    break;
                case 'q':
                case 'Q':
                    paused = true;
                    done = true;
                    break;
                case 'p':
                case 'P':
                    paused = !paused;
                    break;
                case 's':
                case 'S':
                    step = true;
                    break;
                case '+':
                    ++delay;
                    timeout(delay);
                    mvwprintw(stdscr, 1, 0, "Delay: %5d (+/-)", delay);
                    break;
                case '-':
                    --delay;
                    if(delay <= 0) delay = 1;
                    timeout(delay);
                    mvwprintw(stdscr, 1, 0, "Delay: %5d (+/-)", delay);
                    break;
                default:
                    break;
            }

            if(!paused || step)
            {
                b->next_generation();
                update = true;
                ++generation;
                mvwprintw(stdscr, 1, COLS - 16, "Generation %5d", generation);
            }

            if(update)
            {
                print_gen(board_win, b, disp_start_y, disp_start_x, height - 1, width - 2);
            }

            wrefresh(board_win);
        }
    
        destroy_win(board_win);
        endwin();
        delwin(stdscr);

        if(*input != cin)
        {
            delete input;
        }

        delete a;
        delete[] x_range;
        delete[] y_range;

        return 0;
    }
    catch(runtime_error &e)
    {
        endwin();
        delwin(stdscr);
        cerr << "ERROR: " << e.what() << "\n";
        exit(1);
    }
}

void print_usage(int status)
{
    cout << "sim-tui reads in a single .aut file passed as an argument, specifying the\nsize and initial configuration of the terrain for generation 0.\n";
    cout << "It then determinesand displays the state of each cell as generations advance.\n";
    cout << "\nOPTIONS:\n";
    cout << "\t-h\t\tDisplays this help screen.\n";
    cout << "\t-tx L,H\t\tSet the x range of the terrain from L (low) to H (high).\n\t\t\tThis overrides the input file.\n";
    cout << "\t-ty L,H\t\tSet the y range of the terrain from L (low) to H (high).\n\t\t\tThis overrides the input file.\n";
    cout << "\nWhen the TUI is running:\n\tP starts or stops the simulation\n\tS advances the simulation by 1 generation\n";
    cout << "\tQ quits sim-tui\n";
    cout << "\t'+' increases the delay between generations\n\t'-' decreases the delay between generations.\n";
    cout << "\tIf the range of the terrain is too large to display, the arrow keys scroll the view.\n";
    exit(status);
}

WINDOW* make_win(int height, int width, int starty, int startx)
{
    WINDOW* local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW* local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

void print_gen(WINDOW* local_win, Board* b, int start_y, int start_x, int height, int width)
{
    const string &board_string = b->to_string(start_x, start_y - height, start_x + width, start_y);
    vector<string>* bs = split(board_string, '\n');

    for(unsigned int i = 1; i < bs->size(); ++i)
    {
        mvwprintw(local_win, i, 1, (*bs)[i].c_str());
    }

    delete bs;
}

void disp_x_scroll(WINDOW* local_win, int y_pos, int start, int scroll_size, int width)
{
    mvwaddch(local_win, y_pos, 0, '+');
    waddch(local_win, '<');

    for(int i = 2; i < width - 2; ++i)
    {
        if(i >= start && i < start + scroll_size)
        {
            waddch(local_win, '#');
        }
        else
        {
            waddch(local_win, '-');
        }
    }

    waddch(local_win, '>');
    waddch(local_win, '+');
}

void disp_y_scroll(WINDOW* local_win, int x_pos, int start, int scroll_size, int height)
{
    mvwaddch(local_win, 0, x_pos, '+');
    mvwaddch(local_win, 1, x_pos, '^');

    for(int i = 2; i < height - 2; ++i)
    {
        if(i >= start && i < start + scroll_size)
        {
            mvwaddch(local_win, i, x_pos, '#');
        }
        else
        {
            mvwaddch(local_win, i, x_pos, '|');
        }
    }

    mvwaddch(local_win, height - 2, x_pos, 'V');
    mvwaddch(local_win, height - 1, x_pos, '+');
}
