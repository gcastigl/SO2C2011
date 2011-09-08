#include "serializer.h"

void _serializer_serializeMessage(int param1, int param2, int param3);
void _serializer_unserializeMessage(int *param1, int *param2, int *param3);

int serializer_write_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    _serializer_serializeMessage(pkg->cityId, pkg->itemId, pkg->amount);
    ipc_write(from, to, message);
	return -1;
}

int serializer_read_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    ipc_read(to, from, message);
    _serializer_unserializeMessage(&(pkg->cityId), &(pkg->itemId), &(pkg->amount));
    log_debug("Read city update: \ncityId: %d\nitemId: %d\namount: %d", pkg->cityId, pkg->itemId, pkg->amount);
    return -1;
}

int serializer_write_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    _serializer_serializeMessage(-1, pkg->companyId, pkg->status);
    ipc_write(from, to, message);
	return -1;
}

int serializer_read_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    int unused;
    ipc_read(to, from, message);
    _serializer_unserializeMessage(&unused, &(pkg->companyId), &(pkg->status));
    log_debug("Read company update: \ncompanyId: %d\nstatus: %d", pkg->companyId, pkg->status);
    return -1;
}

// Private functions
void _serializer_unserializeMessage(int *param1, int *param2, int *param3) {
    sscanf(message, MSG, param1, param2, param3);
}

void _serializer_serializeMessage(int param1, int param2, int param3) {
    sprintf(message, MSG, param1, param2, param3);
}