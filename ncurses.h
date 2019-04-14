#pragma once
class Ncurses
{   
	public:
	Ncurses();
	~Ncurses();
	void login_screen();
	void init_draw();
	void lobby_draw();
	void group_screen_draw();
	void warning_message();
	void redraw(); //when making new functions, they must be declared here like so
	void getInput();
	void exit();
};
