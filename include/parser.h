#ifndef PARSER_H_
#define PARSER_H_

#include "common.h"
#include "company.h"
#include "map.h"
#include "server.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAP_FILE_NAME	"ciudades.txt"
#define COMPANY_FILE_NAME	"empresa.txt"
#define MAX_COMPANY_NAME 40
#define COMPANY_DELTA 5

int parser_parseCitiesFile(char *dir, Server* server, Map* map);

int parser_parseCompanies(char *dir, Server *server, Map *map);

#endif
