//ncurses.cpp
//make this into left_column thing from design document

#include <iostream>
#include <string>
#include <ncurses.h>

int main()
{
	initscr();
	cbreak();
//	noecho();
	char str[80];
	int y,x,yBeg,xBeg,yMax,xMax;


	getyx(stdscr,y,x);
	getbegyx(stdscr,yBeg,xBeg);
	getmaxyx(stdscr,yMax,xMax);

	WINDOW * win_groups = newwin(yMax-2,xMax/5,1,0); //height,width,starty,startx
	refresh();
	box(win_groups,0,0);

	int left,right,top,bottom,tlc,trc,blc,brc;
	left = right = 124; //124 ASCII for |
	top = bottom = 42; //42 ASCII for *
	tlc = trc = blc = brc = 42; //*
	wborder(win_groups,left,right,top,bottom,tlc,trc,blc,brc);

	mvwprintw(win_groups,1,xMax/16,"Group Chats");
	mvwprintw(win_groups,1,3*xMax/16,"+");
	wrefresh(win_groups);

	WINDOW * win_message = newwin(yMax/8,5*xMax/8,7*yMax/8,xMax/5);
	refresh();
	box(win_message,0,0);
	wrefresh(win_message);

	WINDOW * win_users = newwin(yMax-2,11*xMax/64+1,1,33*xMax/40);
	box(win_users,0,0);
	mvwprintw(win_users,1,xMax/12,"Users");
	wrefresh(win_users);

	WINDOW * win_groupname = newwin(3*yMax/32,5*xMax/8,1,xMax/5);
	box(win_groupname,0,0);
	wrefresh(win_groupname);

	move(0,0);
	hline('~',xMax);// line of ~
	move(7*yMax/64,0);//move to draw top border
	hline('-',xMax);// line of -
	move(yMax-1,0);
	hline('~',xMax);// line of ~ on bottom
	
	wmove(win_message,1,1); //move cursor to type input	

	wrefresh(win_message);
	refresh();
//	wscanw(win_message,"%s",str);

	getch();
	endwin();
}

