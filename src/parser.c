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

void parseMap(char *fileName) {
	Map* map = malloc(sizeof(Map));
	FILE *file = fopen(fileName, "r");
	if (file) {
		map->cityCount(parseCount(file));

		loadCities(file);
		fclose(file);
	} else {
		fatal("Error loading map file.");
	}
}

void loadExampleCompanies() {
	const char filename[] = "resources/loads/empresas.txt";
	FILE *file = fopen(filename, "r");
	if (file) {
		loadCompanies(file);
		fclose(file);
	} else {
		perror(filename);
	}
}

int isNewLine(char *line) {
	return !strcmp("", line);
}

int parseCount(FILE *stream) {
	char line[BUFSIZ];
	int cityCount;
	while (fgetstr(line, sizeof line, stream)) {
		if (sscanf(line, "%d", &cityCount) == 1) {
			return cityCount;
		}
	}
	fatal("integer not found");
	return 0;
}

void loadCities(FILE *stream) {
	int cityCount;
	int status = S_LINES;
	char line[BUFSIZ];
	int parsedCities, itemCount;
	char cityName[50], itemName[50];
	City cities[100];
	while (fgetstr(line, sizeof line, stream)) {
		char *ptr = line;
		switch (status) {
		case S_LINES:
			if (sscanf(line, "%d", &cityCount) == 1) {
				printf("Cantidad ciudades: %d\n", cityCount);
				status = S_DATA1;
				parsedCities = 0;
			} else {
				printf("invalid input, first line must be an integer: %s\n",
						line);
				exit(-1);
			}
			break;
		case S_DATA1:
			if (isNewLine(line)) {
				if (parsedCities == cityCount) {
					status = S_DATA2;
				} else {
					fgetstr(cityName, sizeof cityName, stream);
					parsedCities++;
					City tmpCity;
					printf("City name %d: %s\n", parsedCities, cityName);
					tmpCity.id = parsedCities;
					cities[tmpCity.id] = tmpCity;
				}
			} else {
				if (sscanf(line, "%s %d", itemName, &itemCount)) {
					printf("two: %s %d\n", itemName, itemCount);
				} else {
					printf("invalid input, could not parse city: %s\n", line);
				}
			}
			break;
		case S_DATA2:
			printf("debug2: %s\n", line);
			break;
		default:
			break;
		}
	}
}

void loadCompanies(FILE *stream) {

}

