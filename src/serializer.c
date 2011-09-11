#include "serializer.h"

char package[DATA_SIZE];

char* _serialize_company(Company* company, int* finalDim);
char* _serialize_plane(Plane* plane, int* finalDim);
char* _serialize_intVector(int* vec, int lenght, int* finalDim);
int _serialize_buildTypedPackage(int type, char* data, int dataSize);

void serializer_write_company(Company* company, int from, int to);
void serializer_write_cityUpdate(CityUpdatePackage* msg, int from, int to);
void serializer_write_companyUpdate(CompanyUpdatePackage* msg, int from, int to);


Company* _unserialize_company(char* serializedMsg);
Plane* _unserialize_plane(char* serializedMsg, int* charsRead);
CompanyUpdatePackage* _unserialize_companyUpdatePackage(char *serializedMsg);
CityUpdatePackage *_unserialize_cityUpdatePackage(char *serializedMsg);

void* serializer_read(int myId, int from, int* packageType) {
	int read = ipc_read(myId, from, package);
	if (read < 0) {
        log_debug("[Serializer] Read: Nothing");
		*packageType = -1;
		return NULL;
	}
	int type = ((int*)package)[0]; // The fist int marks the package type
	log_debug("[Serializer] Read: %d (%d -> %d)", type, from, myId);
	switch(type) {
		case PACKAGE_TYPE_COMPANY:
			*packageType = type;
			return _unserialize_company(package + sizeof(int));
		case PACKAGE_TYPE_COMPANY_UPDATE:
		    *packageType = type;
            return _unserialize_companyUpdatePackage(package + sizeof(int));
		case PACKAGE_TYPE_CITY_UPDATE:
		    *packageType = type;
            return _unserialize_cityUpdatePackage(package + sizeof(int));
		default:
			log_error("the server read an unknown package type: %d", packageType);
	}
	return NULL;
}

void serializer_write_company(Company* company, int from, int to) {
	int serialLength;
	char* serializedCompany = _serialize_company(company, &serialLength);
	int packageType = PACKAGE_TYPE_COMPANY;
	log_debug("[Serializer] Write company pack from %d to %d", from, to);
    int ok = _serialize_buildTypedPackage(packageType, serializedCompany, serialLength);
    if (!ok) {
        perror("Package is TOO big!!!");
    }
	free(serializedCompany);
}

void serializer_write_cityUpdate(CityUpdatePackage* pkg, int from, int to) {
    int packageSize = (sizeof(int) * 3);
    int packageType = PACKAGE_TYPE_CITY_UPDATE;
    char *buffer = calloc(1, packageSize);
    int offset = 0;
    memcpy(buffer + offset, &(pkg->cityId), sizeof(int)); offset += sizeof(int);
    memcpy(buffer + offset, &(pkg->itemId), sizeof(int)); offset += sizeof(int);
    memcpy(buffer + offset, &(pkg->amount), sizeof(int));
    log_debug("[Serializer] Write city update pack from %d to %d", from, to);
    int ok = _serialize_buildTypedPackage(packageType, buffer, packageSize);
    if (!ok) {
        perror("Package is TOO big!!!");
    }
    free(buffer);
}

void serializer_write_companyUpdate(CompanyUpdatePackage* pkg, int from, int to) {
    int packageSize = sizeof(int) * 2;
    int packageType = PACKAGE_TYPE_COMPANY_UPDATE;
    char *buffer = calloc(1, packageSize);
    int offset = 0;
    memcpy(buffer + offset, &(pkg->companyId), sizeof(int)); offset += sizeof(int);
    memcpy(buffer + offset, &(pkg->status), sizeof(int));
    int ok = _serialize_buildTypedPackage(packageType, buffer, packageSize);
    if (!ok) {
        perror("Package is TOO big!!!");
    }
    free(buffer);
}

int serializer_write(void* pkg, int from, int to, int type) {
    switch(type) {
		case PACKAGE_TYPE_COMPANY:
            serializer_write_company((Company*)pkg, from, to);
            break;
		case PACKAGE_TYPE_COMPANY_UPDATE:
            serializer_write_companyUpdate((CompanyUpdatePackage*)pkg, from, to);
			break;
		case PACKAGE_TYPE_CITY_UPDATE:
            serializer_write_cityUpdate((CityUpdatePackage*)pkg, from, to);
			break;
		default:
            break;
	}
	int ret = ipc_write(from, to, package);
    if (ret < 0) {
        log_error("Error writing to ipc");
    }
    return ret;
}

// TODO: this is a little inneficient... it could be improved;
char* _serialize_company(Company* company, int* finalDim) {
	int lenght = sizeof(int) * 2 + MAX_NAME_LENGTH;
	char* planesToChar = calloc(1, lenght);
	int offset = 0;
	memcpy(planesToChar + offset, &(company->id), sizeof(int)); offset += sizeof(int);
	memcpy(planesToChar + offset, company->name, MAX_NAME_LENGTH); offset += MAX_NAME_LENGTH;
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

int _serialize_buildTypedPackage(int packageType, char* data, int dataSize) {
    if ((dataSize + sizeof(int)) > DATA_SIZE) {
        //The serialized company is bigger than the allowed
        return FALSE;   
    }
    memcpy(package, &packageType, sizeof(int));
	memcpy(package + sizeof(int), data, dataSize);
	
    return TRUE;
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
	Company* company = calloc(1, sizeof(Company));
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
	Plane* plane = calloc(1, sizeof(Plane));
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

CityUpdatePackage* _unserialize_cityUpdatePackage(char *serializedMsg) {
    int offset = 0;
    CityUpdatePackage *pkg = calloc(1, sizeof(CityUpdatePackage));
    memcpy(&(pkg->cityId), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
    memcpy(&(pkg->itemId), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
    memcpy(&(pkg->amount), serializedMsg + offset, sizeof(int));
    return pkg;
}

CompanyUpdatePackage *_unserialize_companyUpdatePackage(char *serializedMsg) {
    int offset = 0;
    CompanyUpdatePackage *pkg = calloc(1, sizeof(CompanyUpdatePackage));
    memcpy(&(pkg->companyId), serializedMsg + offset, sizeof(int)); offset += sizeof(int);
    memcpy(&(pkg->status), serializedMsg + offset, sizeof(int));
    return pkg;
}
