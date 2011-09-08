#include "serializer.h"

void serializer_buildCityMessage(CityUpdatePackage* pkg);
void serializer_buildCompanyMessage(CompanyUpdatePackage* pkg);
void _serializer_buildMessage(int param1, int param2, int param3);

char message[15];

int serializer_write_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    serializer_buildCityMessage(pkg);
    log_debug("City Update: %s", message);
	return -1;
}

int serializer_write_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    serializer_buildCompanyMessage(pkg);
    log_debug("Company Update: %s", message);
	return -1;
}

void serializer_buildCityMessage(CityUpdatePackage* pkg) {
    _serializer_buildMessage(pkg->cityId, pkg->itemId, pkg->amount);
}

void serializer_buildCompanyMessage(CompanyUpdatePackage* pkg) {
    _serializer_buildMessage(-1, pkg->companyId, pkg->status);
}

void _serializer_buildMessage(int param1, int param2, int param3) {
    sprintf(message, MSG, param1, param2, param3);
}