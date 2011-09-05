#ifndef PARSER_H_
#define PARSER_H_

#include "common.h"
#include "company.h"
#include "map.h"

Map *parseMap(char *fileName);
Company *parseCompany(Map *map, char *fileName, int companyId);

#endif /* HEADER_H_ */
