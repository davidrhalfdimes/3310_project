#pragma once
class Ncurses
{   
	public:
	Ncurses();
	~Ncurses();
	void init_draw();
	void redraw(); //when making new functions, they must be declared here like so
};
