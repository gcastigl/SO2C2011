#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "common.h"
#include "map.h"
#include "communicator.h"

typedef enum {
	CITY_STOCK = 0,
	COMPANY_KILL = 1
} msg_t;

typedef struct {
	int cityId;
	int itemId;
	int amount;
} CityUpdatePackage;

typedef struct {
	int companyId;
	int status;			// ON or OFF
} CompanyUpdatePackage;

#define MSG "%03d_%03d_%03d"

char message[DATA_SIZE];

int serializer_write_cityUpdate(CityUpdatePackage* msg, int from, int to);
int serializer_read_cityUpdate(CityUpdatePackage* msg, int from, int to);

int serializer_write_companyUpdate(CompanyUpdatePackage* msg, int from, int to);
int serializer_read_companyUpdate(CompanyUpdatePackage* msg, int from, int to);

#endif
