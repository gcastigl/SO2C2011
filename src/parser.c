#include "../include/parser.h"
#include "../include/map.h"

#define S_LINES 0
#define S_DATA1 1
#define S_DATA2 2

char *fgetstr(char *string, int n, FILE *stream) {
	char *result;
	result = fgets(string, n, stream);
	if (!result) {
		return (result);
	}
	if (string[strlen(string) - 1] == '\n') {
		string[strlen(string) - 1] = 0;
	}
	if (string[strlen(string) - 1] == '\r') {
		string[strlen(string) - 1] = 0;
	}
	return (string);
}

void parseItem(FILE *stream) {

}

City *parseCity(FILE *stream) {
	City *city = newCity("TEST");
	return city;
}

void parseDistance(FILE *stream) {

}

int parseInt(FILE *stream) {
	char line[BUFSIZ];
	int integer;
	while (fgetstr(line, sizeof line, stream)) {
		if (sscanf(line, "%d", &integer) == 1) {
			return integer;
		}
	}
	fatal("integer not found");
	return 0;
}

void parseMap(char *fileName) {
	Map *map;
	FILE *file = fopen(fileName, "r");
	if (file) {
		map = newMap(parseInt(file));
		for(int i = 0; i < map->cityCount; i++) {
			addCity(map, *parseCity(file));
		}
		for(int i = 0; i < map->cityCount; i++) {
			parseDistance(file);
		}
		fclose(file);
	} else {
		fatal("Error loading map file.");
	}
}

int isNewLine(char *line) {
	return !strcmp("", line);
}

