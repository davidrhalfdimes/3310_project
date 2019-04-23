//ncurses.cpp
//#ifndef NCURSES_GUI_HPP
//#define NCURSES_GUI_HPP

#include <iostream>
#include <string>
#include <ncurses.h>
#include <thread>
#include "ncurses.h"
#include <mutex>

Ncurses::Ncurses()
{
	//std::cout << "in Ncurses constructor" << std::endl; //proves use of constructor
}
Ncurses::~Ncurses()
{

}

void Ncurses::warning_message()
{
	initscr();
	char warning1[100] = "Warning: Cannot remove/ban user unless you are ADMIN";
//	char warning2[100] = "Warning: Cannot delete group unless you are ADMIN of the group";
	
	//if-else conditions printing appropriate warnings for different uses
	//system messages
	attron(A_BOLD);
	printw(warning1);
	attroff(A_BOLD);


}

void Ncurses::welcome_draw() //change to make_welcome_draw(). same applies for other functions
{
	initscr();//comment this out
	cbreak();

	int yMax,xMax;

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

	if(choices[highlight]=="Exit")
	{
	//	printw("Exiting\n");
		endwin();
		std::exit(0);
	}

	endwin(); //remove?
}


void Ncurses::login_screen() //enter username. change this return type to return string for main
{
	initscr(); // //
	cbreak();
//	noecho();

	char str[80];

	int yMax,xMax;

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

	//str += ": ";
	//return str; //returning username to chat_client.cpp
}

void Ncurses::init_draw() //draw the screen. probably don't need this
{
      	initscr();
	cbreak(); //enable CTRL+C just in case
	 
	int yMax,xMax;

	getmaxyx(stdscr,yMax,xMax);
	 
	WINDOW * inputwin = newwin(3,xMax-12,yMax-5,5); //height,widt,starty,startx
	refresh();
	box(inputwin,0,0);
	wrefresh(inputwin);
}

void Ncurses::lobby_draw()
{	
	initscr(); // //
	cbreak();
//	noecho();
	//char str[80];
	int yMax,xMax;

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
	
	WINDOW * win_message_history = newwin(49*yMax/64,5*xMax/8,7*yMax/64+1,xMax/5);
	box(win_message_history,0,0);
	wrefresh(win_message_history);

	wmove(win_message,1,1); //move cursor to type input	
	
	wrefresh(win_message);
	refresh();
//	wscanw(win_message,"%s",str); //this is handled in getInput function

//	mvprintw(win_message_history,20,20,"test"); //move cursor to print input <- not working
//	getch();
}

void Ncurses::group_screen_draw() //return type will change
{
/*

Add in functions for different types of system
warning messages, use color/attributes appropriately

Make second window group and final window for exit

*/

	initscr();// //
	cbreak();
//	noecho();
	//char user_message[80];
	int yMax, xMax;

	getmaxyx(stdscr,yMax,xMax);

	start_color();

	//WINDOW * ret_lobby = newwin(yMax-2,xMax/5,2,0); //height,width,starty,startx
	WINDOW * group_title_box = newwin(0,xMax,0,0); //height,width,starty,startx

	refresh();
	box(group_title_box,0,0);

	int left,right,top,bottom,tlc,trc,blc,brc;
	left = right = 124; //124 ASCII for |
	top = bottom = 42; //42 ASCII for *
	tlc = trc = blc = brc = 42; //*
	wborder(group_title_box,left,right,top,bottom,tlc,trc,blc,brc);

	mvwprintw(group_title_box,1,xMax/2,"Group N");
	wrefresh(group_title_box); //win_groups = ret_lobby

	//box containing button to return to lobby
	WINDOW * ret_lobby = newwin(yMax-2,11*xMax/64+1,1,1);
	box(ret_lobby,0,0);
	mvwprintw(ret_lobby,1,1,"  Return to Lobby..........<-]");
	wrefresh(ret_lobby);

	//box containing button to invite user
	WINDOW * add_user = newwin(yMax-5,11*xMax/64+1,5,1);
	box(add_user,0,0);
	mvwprintw(add_user,4,1,"Add User..........+");
	wrefresh(add_user);

	init_pair(3,COLOR_RED, COLOR_WHITE);
	wbkgd(add_user,COLOR_PAIR(3));
	wrefresh(add_user);

	//title box containing number or name of group, name may change dynamically depending on what is selected
	WINDOW * win_groupname = newwin(3*yMax/32,5*xMax/8,1,xMax/5);
	box(win_groupname,0,0);
	mvwprintw(win_groupname,1,xMax/3-10,"Group N");
	wrefresh(win_groupname);

//	The order is pair_number, foreground, background
	init_pair(2,COLOR_BLUE, COLOR_WHITE);
	wbkgd(win_groupname,COLOR_PAIR(2));
	wrefresh(win_groupname);


	move(0,0);
	hline('~',xMax);// line of ~
	move(7*yMax/64,0);//move to draw top border
	hline('-',xMax);// line of -
	move(yMax-1,0);
	hline('~',xMax);// line of ~ on bottom


	WINDOW * win_message_history = newwin(49*yMax/64,5*xMax/8,7*yMax/64+1,xMax/5);
	box(win_message_history,0,0);
	wrefresh(win_message_history);

	//bottom box for sending messages
	WINDOW * send_message_box = newwin(yMax/8,5*xMax/8,7*yMax/8,xMax/5+0.5);
	refresh();
	//creates box
	box(send_message_box,0,0);
	//refreshes the box created
	wrefresh(send_message_box); 

//	The order is pair_number, foreground, background
	init_pair(1,COLOR_RED, COLOR_CYAN);
	wbkgd(send_message_box,COLOR_PAIR(1));

	wmove(send_message_box,1,1); //move cursor to type input	
	wrefresh(send_message_box);
	refresh();
	//wscanw(send_message_box,"%s",&user_message);

//	return *user_message; //need to determine syntax here
//	mvprintw(win_message_history,20,20,"test"); //move cursor to print input <- not working
//	getch();
}





//redraw function
void Ncurses::redraw() //redraw the screen so additional input can be made
{
	//wrefresh();
}


//function that returns string. this function will then send string info in somewhere in chat client so that the message can be printed
void Ncurses::getInput() //getInput(WINDOW *target, int column_limit) column limit not even used here
{
	/*
	std::string input;
	input.clear();

	nocbreak();
	echo();

	int ch = wgetch(target);
	while(ch!='\n')
	{
	if(ch >= 32 && ch <= 126)
	{	
		input.push_back(ch);
	}

	ch = wgetch(target);

	}

	input.push_back('\0'); //cutoff junk

	cbreak();
	noecho();

	return input;
	*/
}

void Ncurses::exit()
{
	endwin();
}

//#endif
