#include "parser.h"

City *parser_parseCity(FILE *stream, Server *server, Map* map);
void parseCityDistances(FILE *stream, Map *map);
Plane *parser_parsePlane(FILE* stream, Server *server, Map* map, int companyId);
Company* parser_parseCompany(FILE* stream, char* name, int id, Server* server, Map* map);

char *fgetstr(char *string, int n, FILE *stream);
int parseInt(FILE *stream);
int isNewLine(char *line);

char lastLine[MAX_NAME_LENGTH];
int serverItemsDim;


//======================================
//		       Map Parser
//======================================

int parser_parseCitiesFile(char *dir, Server* server, Map* map) {
	char fileName[MAX_NAME_LENGTH];
	int status = 0;
	sprintf(fileName, "%s%s", dir, MAP_FILE_NAME);
	log_debug("[Parser] opening file %s\n", fileName);
	FILE *file = fopen(fileName, "r");
	int maxCityCount;
	if (file) {
		maxCityCount = parseInt(file);
		log_debug("parsing %d cities\n", maxCityCount);
		serverItemsDim = 0;
		map_initialize(map, maxCityCount);
		lastLine[0] = '\0';
		for(int i = 0; i < maxCityCount; i++) {
			map->city[i] = parser_parseCity(file, server, map);
			log_debug("city[%d] = %s\n", i, map->city[i]->name);
		}
		parseCityDistances(file, map);
		fclose(file);
	} else {
		status = -1;
		log_error("Error loading map file.");
	}
	return status;
}

City *parser_parseCity(FILE *stream, Server *server, Map* map) {
	char cityName[MAX_NAME_LENGTH];
	int itemCount, cityItemsDim = 0;
	City *city;
	if (lastLine[0]!='\0') {
		strcpy(cityName, lastLine);
	} else {
		fscanf(stream, "%s", cityName);
	}
	city = newCity(cityName);
	while (fscanf(stream, "%s %d", lastLine, &itemCount) == 2) { //Mientras hallan items
		int id = server_getItemId(server, lastLine);
		if (id == -1) {
			// El item es nuevo para el servidor
			if (server->itemCount == serverItemsDim) {
				// Asegurar memoria para en el vector de items(ciudad)
				server->itemName = realloc(server->itemName, (serverItemsDim + BLOCK_SIZE) * sizeof(char*));
				serverItemsDim += BLOCK_SIZE;
			}
			server->itemName[server->itemCount] = malloc(MAX_NAME_LENGTH * sizeof(char));
			memcpy(server->itemName[server->itemCount], lastLine, MAX_NAME_LENGTH);
			id = server->itemCount;
			log_debug("Added new item to server: id: %d - name: %s\n", server->itemCount, server->itemName[server->itemCount]);
			server->itemCount++;
		}
		// Guardar item en la city
		if (city->itemCount == cityItemsDim) {
			city->itemStock = realloc(city->itemStock, (cityItemsDim + BLOCK_SIZE) * sizeof(int));
			cityItemsDim += BLOCK_SIZE;
		}
		city->itemStock[id]-= itemCount;
		city->itemCount++;
	}
	//TODO: de reallocs!!
	log_debug("CityName: %s\n", city->name);
	for (int i = 0; i < city->itemCount; ++i) {
		log_debug("itemId: %d cant: %d\n", i, city->itemStock[i]);
	}
	return city;
}

void parseCityDistances(FILE *stream, Map *map) {
	char line[BUFSIZ];
	log_debug("parsing cities distances\n");
	while (fgetstr(line, sizeof(line), stream)) {
		if (isNewLine(line)) {
			continue;
		}
		int distance;
		char cityName1[MAX_NAME_LENGTH];
		char cityName2[MAX_NAME_LENGTH];
		if (sscanf(line, "%s %s %d", cityName1, cityName2, &distance) == 3) {
			int c1 = map_getCityId(map, cityName1);
			int c2 = map_getCityId(map, cityName2);
			// Routes are always in both directions
			map->cityDistance[c1][c2] = distance;
			map->cityDistance[c2][c1] = distance;
		}
	}
	log_debug("cities distances parsed OK\n");
}

// ======================================
//		      Company parser
// ======================================

//TODO: this function is juat a mockup, needs to be re-done...
int parser_parseCompanies(char *dir, Server *server, Map *map) {
	//system("/bin/sh -c str");
	char fileName[MAX_NAME_LENGTH];
	int numberOfCompanies = 1;
	int status = 0;

	sprintf(fileName, "%s%s", dir, COMPANY_FILE_NAME);
	log_debug("[Parser] opening file %s\n", fileName);
	FILE *file = fopen(fileName, "r");
	server->company = malloc(sizeof(Plane*) * numberOfCompanies);
	for (int i = 0; i < numberOfCompanies; ++i) {
		server->company[i] = parser_parseCompany(file, fileName, i, server, map);
	}
	return status;
}

Company* parser_parseCompany(FILE* stream, char* name, int id, Server* server, Map* map) {
	int numberOfPlanes = parseInt(stream);
	log_debug("Company %d has %d planes\n", id, numberOfPlanes);
	Company* company = newCompany(id, name, numberOfPlanes);
	for (int i = 0; i < numberOfPlanes; ++i) {
		lastLine[0] = '\0';
		company->plane[i] = parser_parsePlane(stream, server, map, PLANE_ID(id, i));
	}
	return company;
}

Plane *parser_parsePlane(FILE* stream, Server *server, Map* map, int planeId) {
	char cityName[MAX_NAME_LENGTH];
	int itemCount, planeItemsDim = 0;
	Plane *plane;
	if (lastLine[0]!='\0') {
		strcpy(cityName, lastLine);
	} else {
		fscanf(stream, "%s", cityName);
	}
	int cityId = map_getCityId(map, cityName);
	if (cityId == -1) {
		log_error("%s was not found as a valid city\n", cityName);
		return NULL;
	}
	plane = newPlane(planeId, cityId);
	log_debug("Creating plane %d at city %s (id= %d)\n", plane->id, cityName, cityId);
	while (fscanf(stream, "%s %d", lastLine, &itemCount) == 2) { // Mientras hallan items
		int id = server_getItemId(server, lastLine);
		if (id == -1) {
			// El item es nuevo para el servidor
			if (server->itemCount == serverItemsDim) {
				// Asegurar memoria para en el vector de items(ciudad)
				server->itemName = realloc(server->itemName, (serverItemsDim + BLOCK_SIZE) * sizeof(char*));
				serverItemsDim += BLOCK_SIZE;
			}
			server->itemName[server->itemCount] = malloc(MAX_NAME_LENGTH * sizeof(char));
			memcpy(server->itemName[server->itemCount], lastLine, MAX_NAME_LENGTH);
			id = server->itemCount;
			log_debug("Added new item to server: id: %d - name: %s\n", server->itemCount, server->itemName[server->itemCount]);
			server->itemCount++;
		}
		// Guardar item en el avion
		if (plane->itemCount == planeItemsDim) {
			plane->itemStock = realloc(plane->itemStock, (planeItemsDim + BLOCK_SIZE) * sizeof(int));
			planeItemsDim += BLOCK_SIZE;
		}
		plane->itemStock[id]-= itemCount;
		plane->itemCount++;
	}
	//TODO: de reallocs!!
	log_debug("Plane id: %d\n", plane->id);
	for (int i = 0; i < plane->itemCount; ++i) {
		log_debug("itemId: %d cant: %d\n", i, plane->itemStock[i]);
	}
	return plane;
}


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


int isNewLine(char *line) {
	return !strcmp("", line);
}
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

