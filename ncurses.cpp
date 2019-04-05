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

void Ncurses::init_draw() //draw the screen
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
void Ncurses::redraw() //redraw the screen so additional input can be made
{
	//wrefresh();
}


//function that returns string. this function will then send string info in somewhere in chat client so that the message can be printed
void Ncurses::getInput() //trying to accept user string
{
	//wgetstr(); //accept line of user string from a window. need to figure out how to pass the inpuwin from init_draw so this can use that window, as well as other the function
}

void Ncurses::exit()
{
	endwin();
}
