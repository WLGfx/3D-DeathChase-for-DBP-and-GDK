#include "main.h"

// Nothing special here apart from the fact as one TGC
// member suggested that it could be quicker if I started
// the search using a random number.

int free_object() {
	int c = rand() + 1;
	while ( dbObjectExist( c ) ) c++;
	return c;
}

int free_bitmap() {
	int c = 1;
	while ( dbBitmapExist( c ) && c <= 32 ) c++;
	if ( c >= 32 ) c = 0;
	return c;
}

int free_image() {
	int c = rand() + 1;
	while ( dbImageExist( c ) ) c++;
	return c;
}

