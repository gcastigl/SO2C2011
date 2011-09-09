#include "serializer.h"

void _serializer_serializeMessage(int param1, int param2, int param3);
void _serializer_unserializeMessage(int *param1, int *param2, int *param3);

char message[DATA_SIZE];

char* _serialize_company(Company* company, int* finalDim);
char* _serialize_plane(Plane* plane, int* finalDim);
char* _serialize_intVector(int* vec, int lenght, int* finalDim);

Company* _unserialize_company(char* serializedMsg);
Plane* _unserialize_plane(char* serializedMsg, int* charsRead);

void* serializer_read(int myId, int from, int* packageType) {
	char package[DATA_SIZE];
	int read = ipc_read(myId, from, package);
	if (read < 0) {
		*packageType = -1;
		return NULL;
	}
	int type = ((int*)package)[0];
	switch(type) {
		case PACKAGE_TYPE_COMPANY:
			log_debug(0, "ENCONTRE UN PACKETE DE TIPO COMPANIA!!");
			break;
		default:
			log_debug(0, "FUCKIT! => %d", type);
	}
	return NULL;
}

int serializer_write_company(Company* company, int from, int to) {
	int serialLenght;
	char* serializedcompany = _serialize_company(company, &serialLenght);
	char* package = malloc(serialLenght + sizeof(int));
	int packageType = PACKAGE_TYPE_COMPANY;
	memcpy(package, &packageType, sizeof(int));
	memcpy(package + sizeof(int), serializedcompany, serialLenght);
	free(serializedcompany);
	log_debug(0, "asdsaas: %d", ((int*)package)[0]);
	return ipc_write(from, to, package);
	/*int err = ipc_write(from, to, "hola this is a test message");
	log_debug(0, "return status from write = %d", err);
	char readData[MAX_NAME_LENGTH];
	ipc_read(to, from, readData);
	log_debug(0, "reding from the ipc: %s", readData);
	*/
	//log_debug(0, "final size: %d", dim);
	//Company* newCompany = _unserialize_company(serializedcompany);
	/*log_debug(0, "id=%d", newCompany->id);
	log_debug(0, "name=%s", newCompany->name);
	log_debug(0, "planes=%d", newCompany->planeCount);
	for(int i = 0; i < company->planeCount; i++) {
		log_debug(0, "plane %d id=%d", i, newCompany->plane[i]->id);
		log_debug(0, "plane %d cityIdFrom=%d", i, newCompany->plane[i]->cityIdFrom);
		log_debug(0, "plane %d cityIdTo=%d", i, newCompany->plane[i]->cityIdTo);
		log_debug(0, "plane %d itemcount=%d", i, newCompany->plane[i]->itemCount);
		for(int j = 0; j < newCompany->plane[i]->itemCount; j++) {
			log_debug(0, "item plane %d itemcount=%d", i, newCompany->plane[i]->itemStock[j]);
		}
	}
	log_debug(0, "dasdsadjasldjkasdjasd asljdkasjdaldasjldjlaskd");*/
}

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

// TODO: this is a little inneficient... it could be improved;
char* _serialize_company(Company* company, int* finalDim) {
	int lenght = sizeof(int) * 2 + MAX_NAME_LENGTH;
	char* planesToChar = malloc(lenght);
	int offset = 0;
	memcpy(planesToChar, &(company->id), sizeof(int)); offset += sizeof(int);
	memcpy(planesToChar + offset, &(company->name), MAX_NAME_LENGTH); offset += MAX_NAME_LENGTH;
	memcpy(planesToChar + offset, &(company->planeCount), sizeof(int)); offset += sizeof(int);
	int planesToCharSize = 0;
	for(int i = 0; i < company->planeCount; i++) {
		char *planeToChar = _serialize_plane(company->plane[i], &planesToCharSize);
		planesToChar = realloc(planesToChar, lenght + planesToCharSize);
		memcpy(planesToChar + lenght, planeToChar, planesToCharSize);
		lenght += planesToCharSize;
		free(planeToChar);
	}
	*finalDim = lenght;
	return planesToChar;
}

char* _serialize_plane(Plane* plane, int* finalDim) {
	int planeItemsSerialSize;
	char *planeItemsAsChars = _serialize_intVector(plane->itemStock, plane->itemCount, &planeItemsSerialSize);
	int lenght = (sizeof(int) * 5 + planeItemsSerialSize) * sizeof(char);
	char *planeToChar = malloc(lenght);
	int offset = 0;
	memcpy(planeToChar + offset, &(plane->id), sizeof(int)); offset += sizeof(int);
	memcpy(planeToChar + offset, &(plane->cityIdTo), sizeof(int)); offset += sizeof(int);
	memcpy(planeToChar + offset, &(plane->cityIdFrom), sizeof(int)); offset += sizeof(int);
	memcpy(planeToChar + offset, &(plane->distanceLeft), sizeof(int)); offset += sizeof(int);
	memcpy(planeToChar + offset, &(plane->itemCount), sizeof(int)); offset += sizeof(int);
	memcpy(planeToChar + offset, planeItemsAsChars, planeItemsSerialSize);
	free(planeItemsAsChars);
	*finalDim = lenght;
	return planeToChar;
}

char* _serialize_intVector(int* vec, int lenght, int* finalDim) {
	int charLenght = lenght * sizeof(int);
	char *intAsChars = (char*) vec;
	char *chars = malloc(charLenght * sizeof(char));
	memcpy(chars, intAsChars, charLenght);
	*finalDim = charLenght;
	return chars;
}

Company* _unserialize_company(char* serializedMsg) {
	Company* company = malloc(sizeof(company));
	int offset = 0;
	memcpy(&company->id, serializedMsg, sizeof(int)); offset += sizeof(int);
	memcpy(&company->name, serializedMsg + offset, MAX_NAME_LENGTH); offset += MAX_NAME_LENGTH;
	memcpy(&company->planeCount, serializedMsg + offset, sizeof(int)); offset += sizeof(int);
	company->plane = malloc(sizeof(Plane) * company->planeCount);
	for(int i = 0; i < company->planeCount; i++) {
		int charsRead;
		company->plane[i] = _unserialize_plane(serializedMsg + offset, &charsRead);
		offset += charsRead;
	}
	return company;
}

Plane* _unserialize_plane(char* serializedMsg, int* charsRead) {
	Plane* plane = malloc(sizeof(Plane));
	int offset = 0;
	memcpy(&(plane->id), serializedMsg, sizeof(int)); offset += sizeof(int);
	memcpy(&(plane->cityIdTo), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
	memcpy(&(plane->cityIdFrom), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
	memcpy(&(plane->distanceLeft), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
	memcpy(&(plane->itemCount), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
	plane->itemStock = malloc(sizeof(int) * plane->itemCount);
	for(int i=0; i < plane->itemCount; i++) {
		memcpy(plane->itemStock + i, serializedMsg + offset, sizeof(int));
		offset += sizeof(int);
	}
	*charsRead = offset;
	return plane;
}
