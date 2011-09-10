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

int serializer_write_company(Company* company, int from, int to);
int serializer_write_cityUpdate(CityUpdatePackage* msg, int from, int to);
int serializer_write_companyUpdate(CompanyUpdatePackage* msg, int from, int to);

#endif
