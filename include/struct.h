#ifndef _STRUCT_
#define _STRUCT_

#define FALSE	0
#define TRUE	!FALSE

typedef struct {
	char* name;
	int amount;
	int id;
} Item;

typedef struct {
	char* name;
	int id;
	Item* needs;
} City;

typedef struct {
	City* cities;
	float** distances;
} Map;

#endif
