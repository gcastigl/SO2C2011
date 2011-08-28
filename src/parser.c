#include "../include/parser.h"
#include "../include/map.h"

typedef enum {STATUS_NAME = 0, STATUS_STOCK = 1} Status;
int isNewLine(char *line);

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

void parseCityStock(char *line, City* city) {
	char itemName[100];
	int stock;
	if (sscanf(line, "%s %d", itemName, &stock) == 2) {
		city->itemStock[map_getStockId(itemName)] = stock;
		return;
	}
	fatal("Invalid City stock");
}

City *parseCity(FILE *stream) {
	Status status = STATUS_NAME;
	City *city;
	char line[BUFSIZ];
	while (fgetstr(line, sizeof line, stream)) {
		switch (status) {
			case STATUS_NAME:
				if (!isNewLine(line)) {
					city = newCity(line);
					status = STATUS_STOCK;
				}
				break;
			case STATUS_STOCK:
				if (isNewLine(line)) {
					return city;
				} else {
					parseCityStock(line, city);
				}
				break;
		}
	}
	fatal("city not found");
	return NULL;
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
	int maxCityCount;
	if (file) {
		maxCityCount = parseInt(file);
		map_init(maxCityCount);
		for(int i = 0; i < maxCityCount; i++) {
			map_addCity(*parseCity(file));
		}
		for(int i = 0; i < maxCityCount; i++) {
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

