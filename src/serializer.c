#include "serializer.h"

void _serializer_serializeMessage(int param1, int param2, int param3);
void _serializer_unserializeMessage(int *param1, int *param2, int *param3);

char message[DATA_SIZE];

int serializer_write_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    int retVal;
    _serializer_serializeMessage(pkg->cityId, pkg->itemId, pkg->amount);
    retVal = ipc_write(from, to, message);
    if (retVal < 0) {
        perror("Error writing serializer");
    }
	return retVal;
}

int serializer_read_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    int retVal;
    retVal = ipc_read(to, from, message);
    _serializer_unserializeMessage(&(pkg->cityId), &(pkg->itemId), &(pkg->amount));
    return retVal;
}

int serializer_write_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    int retVal;
    _serializer_serializeMessage(-1, pkg->companyId, pkg->status);
    retVal = ipc_write(from, to, message);
	return retVal;
}

int serializer_read_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    int unused, retVal;
    retVal = ipc_read(to, from, message);
    _serializer_unserializeMessage(&unused, &(pkg->companyId), &(pkg->status));
    return retVal;
}

// Private functions
void _serializer_unserializeMessage(int *param1, int *param2, int *param3) {
    sscanf(message, MSG, param1, param2, param3);
}

void _serializer_serializeMessage(int param1, int param2, int param3) {
    sprintf(message, MSG, param1, param2, param3);
}