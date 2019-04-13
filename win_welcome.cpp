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

	WINDOW * win_welcome = newwin(6,xMax/2+5,yMax/2-5,xMax/2-(xMax/4)); //height,width,starty,startx
	refresh();
	box(win_welcome,0,0);

	int left,right,top,bottom,tlc,trc,blc,brc;
	left = right = 124; //124 ASCII for |
	top = bottom = 42; //42 ASCII for *
	tlc = trc = blc = brc = 42; //*
	wborder(win_welcome,left,right,top,bottom,tlc,trc,blc,brc);

	mvwprintw(win_welcome,1,xMax/10,"Welcome to Superchat! Choose an option:");
	wmove(win_welcome,2,xMax/5);
	//refresh();
	wrefresh(win_welcome);
	
	//create selection window for choice of 'Sign Up' or 'Login'
	keypad(win_welcome,true);


	std::string choices[3]={"Sign Up","Login", "Exit"};
	int choice;
	int highlight=0;

	while(1) //infinite loop
	{
		for(int i=0;i<3;i++)
		{
			if(i==highlight)
				wattron(win_welcome,A_REVERSE);
			mvwprintw(win_welcome,i+2,1,choices[i].c_str());
			wattroff(win_welcome,A_REVERSE);
		}
	
	choice = wgetch(win_welcome);

	switch(choice)
	{
		case KEY_UP:
			highlight--;
			if(highlight==-1) //prevent out of bounds
				highlight=0;
			break;
		case KEY_DOWN:
			highlight++;
			if(highlight==3) //prevent out of bounds
				highlight=2;
			break;
		default:
			break;
	}
	if(choice==10) //ENTER key is 10
		break; //exit loop after hitting ENTER
	}

	//getch();
	endwin();

}
