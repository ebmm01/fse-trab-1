#include <curses.h>
#include <unistd.h>
#include "menu.h"
#include "update_temp.h"

int parent_x, parent_y, new_x, new_y;

volatile int manual_temp = 35, highlight = 0;

void* execute_menu() {
    
    int score_size = 3;
    int key = ERR;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    // set up initial windows
    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *field = newwin((parent_y - score_size) -2, (parent_x/2)-1, 0, 0);
    WINDOW *score = newwin((parent_y - score_size) -2, (parent_x/2)-1, 0, (parent_x/2));
    
    char choices[2][25] = { "Potenciometro", "Manual" };

    box(field, 0, 0);
    box(score, 0, 0);

    while(1) {

        for(int i = 0; i < 2; i++) {
            if (i == highlight)
                wattron(score, A_REVERSE);
            mvwprintw(score, 2, i + (i*25) + 1, "[%d] - %s", i + 1, choices[i]);
            wattroff(score, A_REVERSE);
        }

        if (key == KEY_RESIZE) {
            getmaxyx(stdscr, new_y, new_x);

            if (new_y != parent_y || new_x != parent_x) {
                parent_x = new_x;
                parent_y = new_y;

                wresize(field, new_y - score_size, new_x);
                wresize(score, score_size, new_x);
                mvwin(score, new_y - score_size, 0);
                
                wclear(stdscr);
                wclear(field);
                wclear(score);

                box(field, 0, 0);
                box(score, 0, 0);
            }
        }

        

        key = getch();

        switch (key) {
            case '1':
                highlight = 0;
                break;
            case '2':
                highlight = 1;
                break;
            case '+':
                manual_temp++;
                if (manual_temp > 99)
                    manual_temp = 99;
                break;
            case '-':
                manual_temp--;
                if (manual_temp < (int) external_temp)
                    manual_temp = (int) external_temp;
                break;
            default:
                break;
        }

        mvwprintw(field, 0, 2, "Leituras");
        mvwprintw(score, 0, 2, "Menu");
        mvwprintw(score, 1, 1, "Modo de seleção de temperatura de referência:");
        mvwprintw(score, 4, 1, "Temperatura manual selecionada (aperte + ou - para alterar):");
        mvwprintw(field, 1, 53, "                 ");
        mvwprintw(field, 1, 1, "Modo de tempereratura de Referencia atual----------- %s", choices[highlight]);
        mvwprintw(field, 2, 1, "Temperatura interna -------------------------------- %.2f ºC", internal_temp);
        mvwprintw(field, 3, 1, "Temperatura externa -------------------------------- %.2f ºC", external_temp);
        mvwprintw(field, 4, 1, "Temperatura potenciometro--------------------------- %.2f ºC", pot_temp);
        mvwprintw(field, 5, 1, "Temperatura manual---------------------------------- %.2f ºC", (float)manual_temp);
        mvwprintw(field, 6, 1, "PID ------------------------------------------------ %.2f \%", signal_control);

        mvwprintw(score, 5, 1, "%d ºC", manual_temp);

        // refresh each window
        wrefresh(stdscr);
        wrefresh(field);
        wrefresh(score);
        
        usleep(10000);
    }
}
