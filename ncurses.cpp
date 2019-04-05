//ncurses.cpp

#include <iostream>
#include <string>
#include <ncurses.h>

#include "ncurses.h"

Ncurses::Ncurses()
{
	std::cout << "in Ncurses constructor" << std::endl;
}
Ncurses::~Ncurses()
{
}

void Ncurses::init_draw()
{
      	initscr();
	cbreak(); //enable CTRL+C just in case
	 
	int y, x, yBeg, xBeg, yMax, xMax;

	getyx(stdscr,y,x);
	getbegyx(stdscr,yBeg,xBeg);
	getmaxyx(stdscr,yMax,xMax); //store maximum values into these variables
	 
	WINDOW * inputwin = newwin(3,xMax-12,yMax-5,5); //height,widt,starty,startx
	refresh();
	box(inputwin,0,0);
	wrefresh(inputwin);
}

//redraw function
void Ncurses::redraw()
{

}

//function that returns string. this function will then send string info in somewhere in chat client so that the message can be printed

