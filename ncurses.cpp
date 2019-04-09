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

void Ncurses::login_screen()
{
	initscr();
	cbreak();
//	noecho();

	char str[80];
	int y,x,yBeg,xBeg,yMax,xMax;


	getyx(stdscr,y,x);
	getbegyx(stdscr,yBeg,xBeg);
	getmaxyx(stdscr,yMax,xMax);

	WINDOW * win_login = newwin(6,xMax/2+5,yMax/2-5,xMax/2-(xMax/4)); //height,width,starty,startx
	refresh();
	box(win_login,0,0);

	int left,right,top,bottom,tlc,trc,blc,brc;
	left = right = 124; //124 ASCII for |
	top = bottom = 42; //42 ASCII for *
	tlc = trc = blc = brc = 42; //*
	wborder(win_login,left,right,top,bottom,tlc,trc,blc,brc);

	mvwprintw(win_login,1,xMax/5,"Enter your username:");
	wmove(win_login,2,xMax/5);
	//refresh();
	wrefresh(win_login);

	wscanw(win_login,"%s",str);
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
