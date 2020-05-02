#include"newGame.h"
#include<ncurses.h>
#include<unistd.h>
#include<chrono>
#include<thread>
#include "menu.h"
int main() {
	int choice=menu();
	if(choice!=1) exit(1);
  
	// initialise ncurses stuff
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);

	int win_y, win_x, hud_y=5, hud_x;
	getmaxyx(stdscr, win_y, win_x);
	win_y-=6;
	hud_x=win_x;
	WINDOW * win = newwin(win_y, win_x, 0, 0);
	WINDOW * hud = newwin(hud_y, hud_x, win_y+1, 0);
	int count=0;
	char c;
	int px=win_x/2, py=(win_y/6) * 5;
	int pe[5][5]={{2,5,0,-1,-1},{1,20,0,-1,-1},{4,30,0, -1,-1},{3,11,0,-1,-1},{6,15,0,-1,-1}};
	//int pu[5][2]={{15,3},{15,4},{15,2},{15,40},{-1,-1}};
	int pu[1][3] = {{3,7,1}};
	game game1(win, hud, 0,px,py, 3, pe, pu);
	game1.generate_enemies(20);
	game1.generate_powerups(4);
	while(count++ <= 10000){
		if((c=getch()) != ERR) 
                    game1.playerMove(c);
		if(game1.enemies_empty())
		    game1.add_enemies();
		if(game1.powerups_empty())
		    game1.add_powerups();
		game1.display(win, hud);
		wrefresh(win);
		wrefresh(hud);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		werase(win);
		werase(hud);
		game1.update(count);
		if(game1.isOver()) break;

	}
	game1.display(win, hud);

	// wrapping up ncurses output
	wrefresh(win);
	wrefresh(hud);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	wmove(win, 0, 0);
	waddstr(win, "You Lost! Game over...");
	wrefresh(win);
	sleep(2);
	delwin(win);
	delwin(hud);
	endwin();
	
	return 0;
}
