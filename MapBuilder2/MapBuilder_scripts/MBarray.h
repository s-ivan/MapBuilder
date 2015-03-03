#ifndef MBarray_h
#define MBarray_h

////////////////////////////////////////////
// macros


#define new1(structtype) sys_malloc(sizeof(structtype))
#define newx(structtype, length) sys_malloc(length * sizeof(structtype))


///////////////////////////////////////////
// functions

//----------------------
/*
	array operations	
	
	requirements:
	- the array (initially set to NULL),
	- the type of the array (variable or struct type),
	- an int variable storing array length.
	
	if structs contain inner dynamic arrays, they should be separately allocated and freed!
*/


/* 
	allocate memory for a new array with the given number of elements,
	fill all its bytes with zero for safety

	example : MyArray = array_new( sizeof(MYARRAYTYPE), &MyArrayLength, 10 );							
	(MyArray should be == NULL)
	(arraylength can be NULL)
	
	returns the array, sets arraylength
*/
void*		array_new(int elementsize, int* arraylength, int newelementscount);

/*
	same as array_new() but fills all bytes with (char)defaultvalue (instead of zero)
	
	example : MyArray = array_new( sizeof(MYARRAYTYPE), &MyArrayLength, 10, (char)255 );
	
	returns the array, sets arraylength	
*/
void*		array_new_default(int elementsize, int* arraylength, int newelementscount, char defaultvalue);

/* 
	fill an array with default data given by one locally created element,
	
	example : 	MYARRAYTYPE newarrayelement = MyDefaultValue or MyDefaultStruct;
					array_fill( MyArray, sizeof(MYARRAYTYPE), MyArrayLength, &newarrayelement );	
	where new_arrayelement is a local variable or struct created/allocated and set before calling it
	
	returns nothing
*/
void		array_fill(void* arrayin, int elementsize, int arraylength, void* newelement);


/* 
	allocate memory for an existing array and a given number of new elements,
	or decrease array length
	
	example : MyArray = array_realloc( MyArray, sizeof(MYARRAYTYPE), &MyArrayLength, 10 );
	
	returns the array, sets arraylength
*/
void*		array_realloc(void* arrayin, int elementsize, int* arraylength, int newelementscount);							// not essential, array_add can do it with a NULL newelement


/* 
	allocate memory for an existing array and a given number of new elements,
	and fill it with default data given by one locally created element,
	or decrease array length
	
	example : 	MYARRAYTYPE newarrayelement = MyDefaultValue or MyDefaultStruct;
					MyArray = array_add( MyArray, sizeof(MYARRAYTYPE), &MyArrayLength, 10, &newarrayelement );	
	(where new_arrayelement is a local variable or struct created/allocated and set before calling it,
	if NULL then new elements will be filled with zeros)
	
	returns the array, sets arraylength
*/
void*		array_add(void* arrayin, int elementsize, int* arraylength, int newelementscount, void* newelement);		


/*
	free array memory, set it to NULL for future usage, and set length to zero
	
	example : array_destroy( MyArray, &MyArrayLength );	
	(arraylength can be NULL)
	
	returns nothing, sets the array, arraylength
*/
void		array_destroy(void* arrayin, int* arraylength);


//----------------------
/*
	2D array operations
*/

/*
	2D array helpers
*/
int		array_getx(int sizex, int sizey, int abspos);
int		array_gety(int sizex, int sizey, int abspos);
int		array_getabs(int sizex, int sizey, int posx, int posy);

/*
	reposition array elements as it would be rotated as a 2D matrix
	to the given direction (angle/45), assuming currently looking towards 0
	
	example : 	int MyRotateArrayLength = MyArrayLength;
					int MyRotateSizeX = MySizeX;
					int MyRotateSizeY = MySizeY;
					MYARRAYTYPE defaultelement = MyDefaultValue or MyDefaultStruct or NULL;
					MYARRAYTYPE* MyRotatedArray = array_rotate( MyArray, sizeof(MYARRAYTYPE), &MyRotateArrayLength, 2, &MyRotateSizeX, &MyRotateSizeY, &defaultelement );	
	(direction 2 means rotation by 90 degrees)
	(in case of diagonal angle the new array can be larger in size, and if rotated further does not have the same result as rotating the original to the same direction!)
	
	returns the new array (it can be used to overwrite the old one, without using new variables).
*/
void*		array_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey);		// WRONG !

char*		array_rotate_char(char* arrayin, int* arraylength, int direction, int* sizex, int* sizey);						// okay
var*		array_rotate_var(var* arrayin, int* arraylength, int direction, int* sizex, int* sizey);							// okay

//----------------------
/*
	3D array operations
*/

/*
	3D array helpers
*/
int		array_getx_multi(int sizex, int sizey, int sizez, int abspos);
int		array_gety_multi(int sizex, int sizey, int sizez, int abspos);
int		array_getabs_multi(int sizex, int sizey, int posx, int posy, int posz);

/*
	same as array_rotate() but supports 3D arrays organized in more levels
*/
void		array_rotate_multi(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey);		// WRONG !

char*		array_rotate_multi_char(char* arrayin, int* arraylength, int direction, int* sizex, int* sizey);
var*		array_rotate_multi_var(var* arrayin, int* arraylength, int direction, int* sizex, int* sizey);

////----------------------
////----------------------
//// List - not used
//
//int*		sys_realloc(void** AIn, int ASize, int AOldCount, int ANewCount);
//
//typedef struct TList
//{
//	int	count;
//	int*	items;
//	
//}TList;
//
//TList*	list_create();
//void		list_free(TList* AList);
//
//void		list_add(TList* AList, void* AItem);
//void		list_remove(TList* AList, int AIndex);
//int		list_index_of(TList* AList, void* AItem);
//void		list_delete(TList* AList, void* AItem);
//void		list_clear(TList* AList);
//
////----------------------


#endif