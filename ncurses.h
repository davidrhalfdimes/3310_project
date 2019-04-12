#pragma once
class Ncurses
{   
	public:
	Ncurses();
	~Ncurses();
	void login_screen();
	void init_draw();
	void lobby_draw();
	void redraw(); //when making new functions, they must be declared here like so
	void getInput();
	void exit();
};
