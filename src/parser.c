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
	log_debug("[Parser] opening file %s", fileName);
	FILE *file = fopen(fileName, "r");
	int maxCityCount;
	if (file) {
		maxCityCount = parseInt(file);
		log_debug("parsing %d cities", maxCityCount);
		serverItemsDim = 0;
		map_initialize(map, maxCityCount);
		lastLine[0] = '\0';
		for(int i = 0; i < maxCityCount; i++) {
			map->city[i] = parser_parseCity(file, server, map);
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
			log_debug("Added new item to server: id: %d - name: %s", server->itemCount, server->itemName[server->itemCount]);
			server->itemCount++;
		}
		// Guardar item en la city
		if (cityItemsDim <= id) {
			city->itemStock = realloc(city->itemStock, (id + 1) * sizeof(int));
			cityItemsDim = id + 1;
		}
		city->itemStock[id]-= itemCount;
		city->itemCount = MAX(city->itemCount, id + 1);
	}
	//TODO: de reallocs!!
	log_debug("CityName: %s", city->name);
	for (int i = 0; i < city->itemCount; ++i) {
		log_debug("itemId: %d cant: %d", i, city->itemStock[i]);
	}
	return city;
}

void parseCityDistances(FILE *stream, Map *map) {
	char line[BUFSIZ];
	log_debug("parsing cities distances");
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
	log_debug("cities distances parsed OK");
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
	log_debug("[Parser] opening file %s", fileName);
	FILE *file = fopen(fileName, "r");
	server->company = malloc(sizeof(Plane*) * numberOfCompanies);
	for (int i = 0; i < numberOfCompanies; ++i) {
		server->company[i] = parser_parseCompany(file, fileName, i, server, map);
	}
	return status;
}

Company* parser_parseCompany(FILE* stream, char* name, int id, Server* server, Map* map) {
	int numberOfPlanes = parseInt(stream);
	log_debug("Company %d has %d planes", id, numberOfPlanes);
	Company* company = newCompany(id, name, numberOfPlanes);
	lastLine[0] = '\0';
	for (int i = 0; i < numberOfPlanes; ++i) {
		company->plane[i] = parser_parsePlane(stream, server, map, PLANE_ID(id, i));
		log_debug("Plane parsed!");
	}
	return company;
}

Plane *parser_parsePlane(FILE* stream, Server *server, Map* map, int planeId) {
	char cityName[MAX_NAME_LENGTH];
	int itemCount, planeItemsDim = 0;
	Plane *plane;
	log_debug("starting plane parse");
	if (lastLine[0]!='\0') {
		strcpy(cityName, lastLine);
	} else {
		fscanf(stream, "%s", cityName);
	}
	int cityId = map_getCityId(map, cityName);
	if (cityId == -1) {
		log_error("%s was not found as a valid city", cityName);
		return NULL;
	}
	plane = newPlane(planeId, planeId);
	log_debug("Creating plane %d at city %s (id= %d)", plane->id, cityName, cityId);
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
			log_debug("Added new item to server: id: %d - name: %s", server->itemCount, server->itemName[server->itemCount]);
			server->itemCount++;
		}
		// Guardar item en el avion

		if (planeItemsDim <= id) {
			plane->itemStock = realloc(plane->itemStock, (id + 1) * sizeof(int));
			planeItemsDim = id + 1;
		}
		plane->itemStock[id] += itemCount;
		plane->itemCount = MAX(plane->itemCount, id + 1);
	}
	//TODO: de reallocs!!
	log_debug("Plane id: %d", plane->id);
	for (int i = 0; i < plane->itemCount; ++i) {
		log_debug("itemId: %d cant: %d", i, plane->itemStock[i]);
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


