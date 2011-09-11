#include "view/mapview.h"

static WINDOW *city_win;
static WINDOW *comp_win;

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
		wattron(city_win, COLOR_PAIR(1));
		mvwprintw(city_win, x++, y, "City: %s\n", city->name);
		wattroff(city_win, COLOR_PAIR(1));
		for (int j = 0; j < city->itemCount; ++j) {
			mvwprintw(city_win, x++, y+4, "%15s: %5d", server->itemName[j], city->itemStock[j]);
		}
	}
}

void printCompanies(Server *server, Map *map) {
	int x = 2, y = 2;
	for (int i = 0; i < server->companyCount; ++i) {
		Company *company = server->company[i];
		wattron(comp_win, COLOR_PAIR(2));
		mvwprintw(comp_win, x++, y, "Company: %s\n", company->name);
		wattroff(comp_win, COLOR_PAIR(2));
		for (int j = 0; j < company->planeCount; ++j) {
			Plane *plane = company->plane[j];
			char* target = (plane->cityIdTo != NO_TARGET) ? map->city[plane->cityIdTo]->name : "\"No Target\"";
			mvwprintw(comp_win, x++, y+4, "Plane: %5d @ %15s -> %-15s [Distance: %3d]",
				plane->id,
				map->city[plane->cityIdFrom]->name,
				target,
				plane->distanceLeft
			);
			for (int k = 0; k < plane->itemCount; ++k) {
				mvwprintw(comp_win, x++, y+8, "%-15s: %-5d", server->itemName[k], plane->itemStock[k]);
			}
		}
	}
}

void view_start() {
#ifdef DEBUG_MODE
	printf("view-start\n");
	return;
#endif
	initscr();
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(9, COLOR_CYAN, COLOR_BLACK);
	city_win = newwin(LINES-4, (COLS/2)-2, 2, 2);
	comp_win = newwin(LINES-4, (COLS/2)-2, 2, (COLS/2));
	refresh();
}

void view_renderMap(Server *server, Map *map) {
#ifdef DEBUG_MODE
	printf("view-render turn: %d\n", server->turn);
	return;
#endif
	printTurn(server);
	printCities(server, map);
	printCompanies(server, map);
	attron(COLOR_PAIR(9));
	mvprintw(LINES -2, 2, "Press CTRL+C to exit.");
	attroff(COLOR_PAIR(9));
	box(stdscr, 0 , 0);
	touchwin(stdscr);
	wrefresh(stdscr);
	box(city_win, 0 , 0);
	touchwin(city_win);
	wrefresh(city_win);
	box(comp_win, 0 , 0);
	touchwin(comp_win);
	wrefresh(comp_win);
}

void view_end() {
#ifdef DEBUG_MODE
	printf("view-end\n");
	return;
#endif
	attron(COLOR_PAIR(9));
	mvprintw(LINES-2, COLS/2, "SIMULATION DONE. Press Any Key To Exit");
	attroff(COLOR_PAIR(9));
	getch();
	endwin();
}
