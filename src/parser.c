#include "parser.h"

#define MAX_NAME 100

typedef enum {S0 = 0, S1 = 1} Status;
int isNewLine(char *line);

void parseCityStock(Map *map, char *line, City* city);

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

void parseCityStock(Map *map, char *line, City* city) {
	char itemName[MAX_NAME];
	int stock;
	if (sscanf(line, "%s %d", itemName, &stock) == 2) {
		city->itemStock[map_getStockId(map, itemName)] = stock;
		return;
	}
	log_error("Invalid City stock");
}

City *parseCity(Map *map, FILE *stream) {
	Status status = S0;
	City *city;
	char line[BUFSIZ];
	while (fgetstr(line, sizeof line, stream)) {
		switch (status) {
			case S0:
				if (!isNewLine(line)) {
					city = newCity(line);
					status = S1;
				}
				break;
			case S1:
				if (isNewLine(line)) {
					return city;
				} else {
					parseCityStock(map, line, city);
				}
				break;
		}
	}
	log_error("city not found");
	return NULL;
}

void parsePlaneStock(Map *map, char *line, Plane* plane) {
	char itemName[MAX_NAME];
	int stock;
	if (sscanf(line, "%s %d", itemName, &stock) == 2) {
		plane->itemStock[map_getStockId(map, itemName)] = stock;
		return;
	}
	log_error("Invalid City stock");
}

Plane *parsePlane(Map *map, FILE *stream, int owenerCompanyId, int id) {
	Status status = S0;
	Plane *plane;
	char line[BUFSIZ];
	while (fgetstr(line, sizeof line, stream)) {
		switch (status) {
			case S0:
				if (!isNewLine(line)) {
					plane = newPlane(PLANE_ID(owenerCompanyId, id), map_getCityId(map, line));
					status = S1;
				}
				break;
			case S1:
				if (isNewLine(line)) {
					return plane;
				} else {
					parsePlaneStock(map, line, plane);
				}
				break;
		}
	}
	return plane;
}

void parseCityDistances(Map *map, FILE *stream) {
	char line[BUFSIZ];
	while (fgetstr(line, sizeof line, stream)) {
		if (isNewLine(line)) {
			continue;
		} else {
			int distance;
			char cityName1[MAX_NAME];
			char cityName2[MAX_NAME];
			if (sscanf(line, "%s %s %d", cityName1, cityName2, &distance) == 3) {
				map->city[map_getCityId(map, cityName1)]->cityDistance[map_getCityId(map, cityName2)] = distance;
			}
		}
	}
}

int parseInt(FILE *stream) {
	char line[BUFSIZ];
	int integer;
	while (fgetstr(line, sizeof line, stream)) {
		if (sscanf(line, "%d", &integer) == 1) {
			return integer;
		}
	}
	log_error("integer not found");
	return 0;
}

Map *parseMap(char *fileName) {
	Map *map = NULL;
	FILE *file = fopen(fileName, "r");
	int maxCityCount;
	if (file) {
		maxCityCount = parseInt(file);
		map = newMap();
		for(int i = 0; i < maxCityCount; i++) {
			map_addCity(map, parseCity(map, file));
		}
		parseCityDistances(map, file);
		fclose(file);
	} else {
		log_error("Error loading map file.");
	}
	return map;
}

Company *parseCompany(Map *map, char *fileName, int companyId) {
	FILE *file = fopen(fileName, "r");
	int maxPlaneCount;
	if (file) {
		maxPlaneCount = parseInt(file);
		Company *company = newCompany(companyId, fileName, maxPlaneCount);
		for(int i = 0; i < maxPlaneCount; i++) {
			addPlane(company, parsePlane(map, file, company->id, i));
		}
		fclose(file);
		return company;
	} else {
		log_error("Error loading company file.");
		return (Company *)NULL;
	}
}

int isNewLine(char *line) {
	return !strcmp("", line);
}

