//sample code that reads user input and prints that input using ncurses

//compilation instructions:
//g++ -o ncurses_example ncurses_example.cpp -lncurses
//./ncurses_example
#include <ncurses.h>
#include <string.h>

int main()
{
	char mesg[] = "Enter a string: ";
	char str[80];
	int row, col;

	initscr();
	getmaxyx(stdscr,row,col);
	getstr(str);
	mvprintw(LINES -2 ,0,"You entered: %s", str);

	getch();
	endwin();

	return 0;

}
