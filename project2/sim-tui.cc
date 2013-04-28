#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <string>

#include "lib/board.h"
#include "lib/io/aut.h"
#include "lib/util.h"

using namespace std;

void print_usage(int status);
WINDOW* make_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW* local_win);
void clear_line(WINDOW* local_win, int line);

int main(int argc, char* argv[])
{
    WINDOW* board_win;
    int startx, starty, width, height;
    int ch;
    
    int generation = 0;
    int delay = 100;
    bool paused = true;
    
    char* cur_arg;
    string file("");
    int* x_range = NULL;
    int* y_range = NULL;
    ifstream* input = NULL;

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

    if(file == "")
    {
        input = (ifstream*) &cin;
    }
    else
    {
        input = new ifstream();
        input->open(file.c_str());
    }

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
    starty = 2;
    startx = 0;
    refresh();
    board_win = make_win(height, width, starty, startx);
   
    string board_name = a->get_name();

    mvwprintw(stdscr, 0, COLS/2 - board_name.length()/2, board_name.c_str());
    mvwprintw(stdscr, 1, 0, "<DELAY>");
    mvwprintw(stdscr, 1, COLS - 16, "Generation %5d", 20);
    mvwprintw(stdscr, LINES - 1, 0, "(Q)UIT\t(P)LAY\t(S)TEP\tArrows: scroll");

    while((ch = getch()) != 'q')
    {
        switch(ch)
        {
            case KEY_LEFT:
                destroy_win(board_win);
                board_win = make_win(height, width, starty,--startx);
                break;
            case KEY_RIGHT:
                destroy_win(board_win);
                board_win = make_win(height, width, starty,++startx);
                break;
            case KEY_UP:
                destroy_win(board_win);
                board_win = make_win(height, width, --starty,startx);
                break;
            case KEY_DOWN:
                destroy_win(board_win);
                board_win = make_win(height, width, ++starty,startx);
                break;
            case 'p':
            case 'P':       
                break;
            case '+':
                ++delay;
                timeout(delay);
                break;
            case '-':
                --delay;
                timeout(delay);
                break;
            default:
                break;
        }

        b->next_generation();
        const string &board_string = b->to_string();
        vector<string>* bs = split(board_string, '\n');

        for(int i = 1; i < bs->size(); ++i)
        {
            mvwprintw(board_win, i, 1, (*bs)[i].c_str());
        }

        delete bs;

        wrefresh(board_win);
    }
		
    endwin();

    delete input;

    return 0;
}

void print_usage(int status)
{
    return;
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
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

void clear_line(WINDOW* local_win, int line)
{
    int x, y;
    x = -1;
    y = -1;
    getmaxyx(local_win, y, x);
    for(int i = 0; i < x; ++i)
    {
        mvwaddch(local_win, line, i, ' ');
    }
}
