#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "common.h"
#include "communicator.h"

#define PACKAGE_TYPE_COMPANY		1
#define PACKAGE_TYPE_COMPANY_UPDATE	2
#define PACKAGE_TYPE_CITY_UPDATE	3

typedef struct {
	int cityId;
	int itemId;
	int amount;
} CityUpdatePackage;

typedef struct {
	int companyId;
	int status;			// ON or OFF
} CompanyUpdatePackage;

void* serializer_read(int myId, int from, int* packageType);
int serializer_write(void* pkg, int from, int to, int type);

#endif
