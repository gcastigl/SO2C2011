#include "view/mapview.h"

void view_start() {
	initscr();
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(9, COLOR_CYAN, COLOR_BLACK);
	refresh();
}

void printTurn(Server *server) {
	wclear(stdscr);
	char *title = "Pharma-Plane Simulation 1.0";
	mvprintw(1, (COLS-strlen(title))/2, title);
	attron(COLOR_PAIR(5));
	mvprintw(1, 1, "TURN: %d", server->turn);
	attroff(COLOR_PAIR(5));
}

void printCities(Server *server, Map *map) {
	int x = 2, y = 2;
	for (int i = 0; i < map->cityCount; ++i) {
		City *city = map->city[i];
		attron(COLOR_PAIR(1));
		mvprintw(x++, y, "City: %s\n", city->name);
		attroff(COLOR_PAIR(1));
		for (int j = 0; j < city->itemCount; ++j) {
			mvprintw(x++, y+4, "%15s: %5d", server->itemName[j], city->itemStock[j]);
		}
	}
}

void printCompanies(Server *server, Map *map) {
	int x = 2, y = COLS/2;
	for (int i = 0; i < server->companyCount; ++i) {
		Company *company = server->company[i];
		attron(COLOR_PAIR(2));
		mvprintw(x++, y, "Company: %s\n", company->name);
		attroff(COLOR_PAIR(2));
		for (int j = 0; j < company->planeCount; ++j) {
			Plane *plane = company->plane[j];
			char* target = (plane->cityIdTo != NO_TARGET) ? map->city[plane->cityIdTo]->name : "\"No Target\"";
			mvprintw(x++, y+4, "Plane: %5d @ %15s -> %-15s [Distance: %3d]",
				plane->id,
				map->city[plane->cityIdFrom]->name,
				target,
				plane->distanceLeft
			);
			for (int k = 0; k < plane->itemCount; ++k) {
				mvprintw(x++, y+8, "%-15s: %-5d", server->itemName[k], plane->itemStock[k]);
			}
		}
	}
}

void view_renderMap(Server *server, Map *map) {
	printTurn(server);
	printCities(server, map);
	printCompanies(server, map);
	box(stdscr, 0 , 0);
	attron(COLOR_PAIR(9));
	mvprintw(LINES -2, 2, "Press CTRL+C to exit.");
	attroff(COLOR_PAIR(9));
	refresh();
}

void view_end() {
	attron(COLOR_PAIR(9));
	mvprintw(LINES-2, COLS/2, "SIMULATION DONE. Press Any Key To Exit");
	attroff(COLOR_PAIR(9));
	getch();
	endwin();
}
