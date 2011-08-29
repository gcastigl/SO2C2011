#ifndef PARSER_H_
#define PARSER_H_

#include "common.h"
#include "company.h"
#include "map.h"

void parseMap(char *fileName);
Company *parseCompany(char *fileName);

#endif /* HEADER_H_ */
