#include "parser.h"

City *parser_parseCity(FILE *stream, Server *server, Map* map);
void parseCityDistances(FILE *stream, Map *map);
Plane *parser_parsePlane(FILE* stream, Server *server, Map* map, int companyId);
Company* parser_parseCompany(FILE* stream, char* name, int id, Server* server, Map* map);
void fillRoutes(Map* map);
int findAndFillRoute(Map* map, int from, int to);
void fillDistances(Map* map);
int findAndFillDistance(Map* map, int from, int to);

char *fgetstr(char *string, int n, FILE *stream);
int parseInt(FILE *stream);
int isNewLine(char *line);
void fillWithZeros(int* vec, int start, int limit);
char lastLine[MAX_NAME_LENGTH];
int serverItemsDim;


//======================================
//		       Map Parser
//======================================

int parser_parseCitiesFile(char *dir, Server* server, Map* map) {
	char fileName[MAX_NAME_LENGTH];
	int status = 0;
	sprintf(fileName, "%s%s", dir, MAP_FILE_NAME);
	FILE *file = fopen(fileName, "r");
	int maxCityCount;
	if (file) {
		maxCityCount = parseInt(file);
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
	return city;
}

void parseCityDistances(FILE *stream, Map *map) {
	char line[BUFSIZ];
	rewind(stream);
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
			map->cityRoute[c1][c2] = c2;
			map->cityRoute[c2][c1] = c1;
		}
	}
	fillRoutes(map);
	fillDistances(map);
}

void fillRoutes(Map* map) {
	int changes = 0;
	do {
		changes = 0;
		for (int i = 0; i < map->cityCount; ++i) {
			for (int j = 0; j < map->cityCount; ++j) {
				if (map->cityRoute[i][j] == -1) {
					changes += findAndFillRoute(map, i, j);
				}
			}
		}
	} while(changes);
}

int findAndFillRoute(Map* map, int from, int to) {
	for (int i = 0; i < map->cityCount; ++i) {
		int possibleRoute = map->cityRoute[from][i];
		if (possibleRoute >= 0) {
			if (map->cityRoute[possibleRoute][to] != -1) {
				map->cityRoute[from][to] = possibleRoute;
				return 1;
			}
		}
	}
	return 0;
}

void fillDistances(Map* map) {
	int changes = 0;
	do {
		changes = 0;
		for (int i = 0; i < map->cityCount; ++i) {
			for (int j = 0; j < map->cityCount; ++j) {
				if (map->cityDistance[i][j] == -1) {
					changes += findAndFillDistance(map, i, j);
				}
			}
		}
	} while(changes);
}

int findAndFillDistance(Map* map, int from, int to) {
	int distance = 0;
	int fromCity = from;
	int targetCity = NO_ROUTE;
	do {
		targetCity = map->cityRoute[fromCity][to];
		int targetCityDistance = map->cityDistance[fromCity][targetCity];
		if (targetCityDistance > 0) {
			distance += targetCityDistance;
			fromCity = targetCity;
		} else {
			return 0;
		}
	} while (fromCity != to);
	map->cityDistance[from][to] = distance;
	map->cityDistance[to][from] = distance;
	return 2;
}

// ======================================
//		      Company parser
// ======================================

//TODO: this function is juat a mockup, needs to be re-done...
int parser_parseCompanies(char *dir, Server *server, Map *map) {
	char fileName[MAX_NAME_LENGTH];
	int numberOfCompanies = 0;
    int status = 0;
    DIR *dp;
    struct dirent *ep;
    
    dp = opendir(dir);
    if (dp == NULL) {
        fatal("Error reading directory");
    }
	//  FIXME: this should be dynamic
    char companies[50][50];
    struct stat s;
    
	while ((ep = readdir(dp))) {
	    sprintf(fileName, "%s%s", dir, ep->d_name);
	    if (stat(fileName, &s) != 0) {
            fatal("Error using stat");
	    }
	    if (!S_ISDIR(s.st_mode)) {
            int filenameLen = strlen(ep->d_name);
	        if ((filenameLen > 4) && (filenameLen < MAX_COMPANY_NAME) && (strcmp(".txt", &ep->d_name[filenameLen - 4]) == 0)) {
                strcpy(companies[numberOfCompanies++], ep->d_name);
	        }
	    }
	}
    
    server->companyCount = numberOfCompanies;
	server->company = malloc(sizeof(Plane*) * numberOfCompanies);
	
	for (int i = 0; i < numberOfCompanies; ++i) {
	    sprintf(fileName, "%s%s", dir, companies[i]);
    	FILE *file = fopen(fileName, "r");
		server->company[i] = parser_parseCompany(file, fileName + strlen(dir), i, server, map);
        fclose(file);
	}
	return status;
}

Company* parser_parseCompany(FILE* stream, char* name, int id, Server* server, Map* map) {
	int numberOfPlanes = parseInt(stream);
	Company* company = newCompany(id, name, numberOfPlanes);
	lastLine[0] = '\0';
	for (int i = 0; i < numberOfPlanes; ++i) {
		company->plane[i] = parser_parsePlane(stream, server, map, PLANE_ID(id, i));
	}
	return company;
}

Plane *parser_parsePlane(FILE* stream, Server *server, Map* map, int planeId) {
	char cityName[MAX_NAME_LENGTH];
	int itemCount;
	if (lastLine[0]!='\0') {	// if last line, scan it before start reading again from the stream
		strcpy(cityName, lastLine);
	} else {
		fscanf(stream, "%s", cityName);
	}
	int cityId = map_getCityId(map, cityName);
	if (cityId == -1) {
		return NULL;
	}
	Plane* plane = newPlane(planeId, cityId, server->itemCount);
	while (fscanf(stream, "%s %d", lastLine, &itemCount) == 2) { // Mientras hallan items
		int id = server_getItemId(server, lastLine);
		if (id == -1) {
			return NULL;
		}
		plane->itemStock[id] += itemCount;
	}
	//TODO: de reallocs!!
	return plane;
}

void fillWithZeros(int* vec, int start, int limit) {
	for (int i = start; i < limit; ++i) {
		vec[i] = 0;
	}
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


