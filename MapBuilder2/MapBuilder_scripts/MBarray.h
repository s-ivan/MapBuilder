#ifndef MBarray_h
#define MBarray_h


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// macros


#define new(structtype) sys_malloc(sizeof(structtype))
#define new_array(structtype, length) sys_malloc(length * sizeof(structtype))


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// functions


//////////////////////////////////////////////////////////////////////////////////////
/*
	array operations	
	
	requirements:
	- the array (initially set to NULL),
	- an int variable storing array length (optional)
	
	if structs contain inner dynamic arrays, they should be separately allocated and freed!
*/


/* 
	allocate memory of a new array with the given number of elements,
	initializes all bytes to zero
	
	(initially MyArray should be NULL)
	(arraylength can be NULL)
	
	returns the array, sets arraylength (optional, if not NULL)
	
	example-1 : MYARRAYTYPE* MyArray = array_new( sizeof(MYARRAYTYPE), NULL, 10 );							
					
	example-2 : int MyArrayLength = 10;
					MYARRAYTYPE* MyArray = array_new( sizeof(MYARRAYTYPE), NULL, MyArrayLength );							
					
	example-3 : int MyArrayLength = 0;
					MYARRAYTYPE* MyArray = array_new( sizeof(MYARRAYTYPE), &MyArrayLength, 10 );	
					(now MyArrayLength is set to 10)
*/
void*		array_new(int elementsize, int* arraylength, int newelementscount);


/*
	same as array_new() but initializes all bytes to (char)defaultvalue (instead of zero)
	
	returns the array, sets arraylength (optional, if not NULL)
	
	example-1 : MyArray = array_new( sizeof(MYARRAYTYPE), NULL, 10, 255 );
	
	example-2 : int MyArrayLength;
					MYARRAYTYPE* MyArray = array_new( sizeof(MYARRAYTYPE), &MyArrayLength, 10, 255 );
*/
void*		array_new(int elementsize, int* arraylength, int newelementscount, int defaultvalue);


/* 
	fill an array with the given value,
	also can be used to fill part of an array, 
	by giving pointer to the starting element, 
	and defining a few element length as arraylength (must not exceed real arraylength)
	
	example : 	array_fill( MyArray, MyArrayLength, 10 );	
*/
void		array_fill_char(char* arrayin, int arraylength, char elementvalue);								// loop and =
void		array_fill_short(short* arrayin, int arraylength, short elementvalue);							// loop and =
void		array_fill_int(int* arrayin, int arraylength, int elementvalue);									// loop and =
void		array_fill_var(var* arrayin, int arraylength, var elementvalue);									// loop and =
void		array_fill_vec(VECTOR* arrayin, int arraylength, VECTOR* elementvalue);							// loop and memcpy


/* 
	same as array_fill_xxx, but okay for any type, using a locally created element,
	
	example : 	short NewElement = 10;
					array_fill( MyArray, sizeof(short), MyArrayLength, &NewElement );	
*/	
void		array_fill(void* arrayin, int elementsize, int arraylength, void* elementvalue);				// loop and memcpy, for any types or structs


/* 
	fills all bytes of the array with the given char value,
	
	example : 	array_fillbytes( MyArray, sizeof(int), MyArrayLength, (char)10 );	
*/
void		array_fillbytes(void* arrayin, int elementsize, int arraylength, char bytevalue);			// memset, for any types or structs


/* 
	increase or decrease array size,
	reallocate memory of an existing array to the new size,
	if length is increased, all bytes of new elements are set to zero
	
	returns the array, sets arraylength (must not be NULL!)
	
	example : MyArray = array_resize( MyArray, sizeof(MYARRAYTYPE), &MyArrayLength, 10 );
*/
void*		array_resize(void* arrayin, int elementsize, int* arraylength, int newsize);


/* 
	increase or decrease array size,
	reallocate memory of an existing array with a given number of new elements,
	fill new elements with default value given by one locally created element,
	or decrease array length if newelementscount is negative
	
	returns the array, sets arraylength (must not be NULL!)
	
	example : 	short newelement = 10;
					MyArray = array_add( MyArray, sizeof(short), &MyArrayLength, 3, &newelement );	
*/
void*		array_add(void* arrayin, int elementsize, int* arraylength, int newelementscount, void* newelement);


/*
	free array memory, set it to NULL for future usage,
	and set the corresponding length to zero if given
	
	example : array_destroy( MyArray, &MyArrayLength );	
*/
void		array_destroy(void* arrayin);
void		array_destroy(void* arrayin, int* arraylength);

void		array_destroy2(void** arrayin);
void		array_destroy2(void** arrayin, int* arraylength);


//////////////////////////////////////////////////////////////////////////////////////
// pseudo 2D array operations
//	elements are supposed to be organized similarly to coordinates, (X=0;Y=0) element is the bottom left one


// helpers
int		array2d_getx(int sizex, int sizey, int abspos);
int		array2d_gety(int sizex, int sizey, int abspos);
int		array2d_getabs(int sizex, int sizey, int posx, int posy);


/*
	reposition array elements as it would be rotated as a 2D matrix
	to the given direction (angle/45), assuming currently looking towards 0
	
	returns the new array
	
	example : 	int* MyArray is the existing array to be rotated
					int MyRotateArrayLength = MyArrayLength;
					int MyRotateSizeX = MySizeX;
					int MyRotateSizeY = MySizeY;
					MYARRAYTYPE* MyRotatedArray = array2d_rotate( MyArray, sizeof(MYARRAYTYPE), &MyRotateArrayLength, 2, &MyRotateSizeX, &MyRotateSizeY );	
	(direction 2 means rotation by 90 degrees)
	(no diagonal rotation supported i.e. 1,3,5,7)
*/	
void*		array2d_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey);


//////////////////////////////////////////////////////////////////////////////////////
// pseudo 3D array operations
// the first X*Y elements corresponds to Z=0


// helpers
int		array3d_getx(int sizex, int sizey, int sizez, int abspos);
int		array3d_gety(int sizex, int sizey, int sizez, int abspos);
int		array3d_getabs(int sizex, int sizey, int posx, int posy, int posz);


/*
	same as array2d_rotate() but supports 3D arrays organized level by level
*/
void*		array3d_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey);


//////////////////////////////////////////////////////////////////////////////////////


#endif