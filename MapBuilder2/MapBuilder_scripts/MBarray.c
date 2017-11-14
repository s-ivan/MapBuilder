#ifndef MBarray_c
#define MBarray_c


///////////////////////////////////////////////////////////////////////////////////////////
// array helpers


void*		array_new(int elementsize, int* arraylength, int newelementscount)
{
	if (elementsize <= 0)			return NULL;
	if (newelementscount <= 0)		return NULL;
	
	int newsize = newelementscount;																				// cannot be smaller than 0
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
//			printf( "array_new error!" );																			// error			
			return NULL;
		}
	else
		{
//			memset(arrayout, (char)0, elementsize * newsize);												// set everything to 0, char based! - not essential, sys_malloc sets them to 0
			
			if (arraylength)
				{
					*arraylength = newelementscount;
				}
		} 
	
	return(arrayout);
}


void*		array_new(int elementsize, int* arraylength, int newelementscount, int defaultvalue)
{
	if (elementsize <= 0)			return NULL;
	if (newelementscount <= 0)		return NULL;
	
	int newsize = newelementscount;																				// cannot be smaller than 0
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
//			printf( "array_new with default byte value error!" );	
			return NULL;
		}
	else
		{
			memset(arrayout, (char)defaultvalue, elementsize * newsize);								// set everything to defaultvalue, char based!
			
			if (arraylength)
				{
					*arraylength = newelementscount;
				}
		} 
	
	return(arrayout);
}


//////////////////////////////////////////////////////////////////////////////////////


void		array_fill_char(char* arrayin, int arraylength, char elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
		
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			arrayin[i] = elementvalue;
		}
}


void		array_fill_short(short* arrayin, int arraylength, short elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			arrayin[i] = elementvalue;
		}
}


void		array_fill_int(int* arrayin, int arraylength, int elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			arrayin[i] = elementvalue;
		}
}


void		array_fill_var(var* arrayin, int arraylength, var elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			arrayin[i] = elementvalue;
		}
}


void		array_fill_vec(VECTOR* arrayin, int arraylength, VECTOR* elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	if (!elementvalue)			return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			memcpy( &arrayin[i] , elementvalue , sizeof(VECTOR) );
//			memcpy( arrayin + i * sizeof(VECTOR) , elementvalue , sizeof(VECTOR) );					// not working...
		}
}


//////////////////////////////////////////////////////////////////////////////////////


void		array_fill(void* arrayin, int elementsize, int arraylength, void* elementvalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	if (!elementvalue)			return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
			char* j = (char*)arrayin + i * elementsize;														// char* casting is required!
			memcpy( j , elementvalue , elementsize ); 
		}
}


//////////////////////////////////////////////////////////////////////////////////////


void		array_fillbytes(void* arrayin, int elementsize, int arraylength, char bytevalue)
{
	if (!arrayin)					return;
	if (arraylength <= 0)		return;
	
	memset(arrayin, bytevalue, elementsize * arraylength);
}


//////////////////////////////////////////////////////////////////////////////////////


void*		array_resize(void* arrayin, int elementsize, int* arraylength, int newsize)
{
	//--------------------------------------------------------------
	
	if (!arrayin)					return arrayin;
	if (elementsize <= 0)		return arrayin;
	if (!arraylength)				return arrayin;
	
	//--------------------------------------------------------------
	
	int oldsize = *arraylength;
	
	//--------------------------------------------------------------
	
	if (newsize <= 0)				return arrayin;
	if (newsize == oldsize)		return arrayin;
	
	//--------------------------------------------------------------
	
	int sizetocopy = 0;
	if (newsize > oldsize)
		{
			sizetocopy = oldsize;
		}
	else
		{
			sizetocopy = newsize;
		}
	
	//--------------------------------------------------------------
		
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	//--------------------------------------------------------------
	
	if (!arrayout) 
		{
//			printf( "array_resize error!" );
			return arrayin;
		}
	else
		{
			memcpy( arrayout, arrayin, sizetocopy * elementsize);										// copy old content						
			
			*arraylength = newsize;																				// set array length
		}
	
	//--------------------------------------------------------------
	
	sys_free(arrayin);
	arrayin = NULL;
	
	return arrayout;
	
	//--------------------------------------------------------------
}


void*		array_add(void* arrayin, int elementsize, int* arraylength, int newelementscount, void* newelement)
{
	//--------------------------------------------------------------
	
	if (!arrayin)					return arrayin;
	if (elementsize <= 0)		return arrayin;
	if (!arraylength)				return arrayin;
		
	//--------------------------------------------------------------
	
	int oldsize = *arraylength;
	int newsize = *arraylength + newelementscount;																	// can be smaller than arraylength!
	
	//--------------------------------------------------------------
	
	if (newsize <= 0)				return arrayin;
	if (newsize == oldsize)		return arrayin;
	
	//--------------------------------------------------------------
	
	int sizetocopy = 0;
	if (newsize > oldsize)
		{
			sizetocopy = oldsize;
		}
	else
		{
			sizetocopy = newsize;
		}
	
	//--------------------------------------------------------------
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	//--------------------------------------------------------------
	
	if (!arrayout) 
		{
//			printf( "array_add error!" );			
			return arrayin;
		}
	else
		{
			memcpy( arrayout, arrayin, sizetocopy * elementsize);													// copy old content
			
			if (newelementscount > 0)																						// add new content if needed
				{
					if (newelement)
						{
							int i;								
							for(i=0; i<newelementscount; ++i)
								{
									char* j = (char*)arrayout + oldsize * elementsize + i * elementsize;		// char* casting is required!
									memcpy( j , newelement , elementsize ); 	
								}
						}
//					else
//						{
//							memset( &arrayout[oldsize], (char)0, elementsize * abs(newsize-oldsize));		// set all bytes of new elements to 0, char based! - not essential, sys_malloc sets them to 0
//						}
				}
			
			*arraylength += newelementscount;																			// increase array length
		}
	
	//--------------------------------------------------------------
	
	sys_free(arrayin);
	arrayin = NULL;
	
	return arrayout;
	
	//--------------------------------------------------------------
}


//////////////////////////////////////////////////////////////////////////////////////


void		array_destroy(void* arrayin)
{
	if (arrayin)
		{
			sys_free(arrayin);
			arrayin = NULL;
		}	
}


void		array_destroy(void* arrayin, int* arraylength)
{	
	if (arrayin)
		{
			sys_free(arrayin);
			arrayin = NULL;
		}
	
	if (arraylength)
		{
			*arraylength = 0;
		}	
}


void		array_destroy2(void** arrayin)
{
	if (*arrayin)
		{
			sys_free(*arrayin);
			*arrayin = NULL;
		}	
}


void		array_destroy2(void** arrayin, int* arraylength)
{
	if (*arrayin)
		{
			sys_free(*arrayin);
			*arrayin = NULL;
		}
	
	if (arraylength)
		{
			*arraylength = 0;
		}
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// 2D array helpers


int		array2d_getx(int sizex, int sizey, int abspos)
{
	return abspos % sizex;
}


int		array2d_gety(int sizex, int sizey, int abspos)
{
	return integer(abspos / sizex);
}


int		array2d_getabs(int sizex, int sizey, int posx, int posy)
{
	return posx + posy * sizex;
}


//////////////////////////////////////////////////////////////////////////////////////
// 2D array operations


void*		array2d_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey)
{
sys_marker("sro");

	//--------------------------------
	// convert to 1..7 range - for safety
	
	while (direction < 0)
		{
			direction = direction + 8;
		}
	
	while (direction > 7)
		{
			direction = direction - 8;
		}
	
	if (direction == 0)		return arrayin;
	
	//--------------------------------
	// store original values
	
	int	old_sizex 			= *sizex;
	int	old_sizey 			= *sizey;
	int	old_size 			= old_sizex * old_sizey;
	
	int	old_arraylength	= *arraylength;
		
	//--------------------------------
	// temp array parameters
	
	void* temp_array 			= NULL;
	int   temp_arraylength 	= 0;												// actual array length is 0, will be set to temp_size by array_new()
	
	int	temp_sizex 			= old_sizex;
	int	temp_sizey 			= old_sizey;
	
	//--------------------------------
	// rotation area
	
	// cardinal / diagonal
	if (direction % 2 > 0)
		{
			return arrayin;														// currently not supported			
		}
	
	// swap x-y if rotated by 90 or 270 degree
	if ((direction == 2) || (direction == 6))
		{
			temp_sizex 			= old_sizey;
			temp_sizey 			= old_sizex;
		}			
	
	int	temp_size 			= old_arraylength;	
	
	//--------------------------------
	// temp array		
	
	temp_array 					= array_new( elementsize, &temp_arraylength, temp_size );	
	
	//--------------------------------
	
	if (!temp_array)
		{
//			printf("temp_array == NULL !!!");
			return arrayin;
		}
	
	if (temp_arraylength <= 0)
		{
//			printf("temp_arraylength <= 0 !!!");
			return arrayin;
		}
		
	//--------------------------------
	// valid rotation
	
	int i;			// x
	int j;			// y
	int k = 0;		// abs
	
	// cardinal directions : k counts in new array, i-j loops through old array positions, thus old_sizex-y needed to be used!
	if (direction == 0)																						// 0°
		{
			for (j=0; j<old_sizey; ++j)		
				{
					for (i=0; i<old_sizex; ++i)	
						{
							int temp = array2d_getabs(old_sizex, old_sizey, i, j);							
							
//							temp_array[k] = arrayin[temp];												// wrong, void size cannot be determined
//							memcpy( &temp_array[k] , &arrayin[temp] , elementsize ); 			// copy new values - wrong
							
							char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
							char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
							memcpy( newaddress , oldaddress , elementsize ); 
							
							++k;
						}
				}
		}
	else if (direction == 2)																				// 90°
		{
			for (i=0; i<old_sizex; ++i)			
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array2d_getabs(old_sizex, old_sizey, i, old_sizey-1-j);
							
							char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
							char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
							memcpy( newaddress , oldaddress , elementsize ); 
							
							++k;
						}
				}
		}
	else if (direction == 4)																				// 180°
		{
			for (j=0; j<old_sizey; ++j)	
				{
					for (i=0; i<old_sizex; ++i)
						{
							int temp = array2d_getabs(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j);
							
							char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
							char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
							memcpy( newaddress , oldaddress , elementsize ); 
							
							++k;
						}
				}
		}
	else if (direction == 6)																				// 270°
		{
			for (i=0; i<old_sizex; ++i)
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array2d_getabs(old_sizex, old_sizey, old_sizex-1-i, j);
							
							char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
							char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
							memcpy( newaddress , oldaddress , elementsize ); 
							
							++k;
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength)
		{
			*arraylength 	= temp_arraylength;
		}
	*sizex 			= temp_sizex;
	*sizey 			= temp_sizey;
	
	//--------------------------------
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL);
	
	return temp_array;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// 3D array helpers


int		array3d_getx(int sizex, int sizey, int sizez, int abspos)
{
	return (abspos % (sizex * sizey)) % sizex;
}


int		array3d_gety(int sizex, int sizey, int sizez, int abspos)
{
	return (integer((abspos % (sizex * sizey)) / sizex));
}


int		array3d_getabs(int sizex, int sizey, int posx, int posy, int posz)
{
	return (posx + posy * sizex) + (sizex * sizey * posz);
}


//////////////////////////////////////////////////////////////////////////////////////
// 3D array operations


void*		array3d_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey)
{
sys_marker("sro");

	//--------------------------------
	// convert to 1..7 range - for safety
	
	while (direction < 0)
		{
			direction = direction + 8;
		}
	
	while (direction > 7)
		{
			direction = direction - 8;
		}
	
//	if (direction == 0)		return arrayin;
	
	//--------------------------------
	// store original values
	
	int	old_sizex 			= *sizex;
	int	old_sizey 			= *sizey;
	int	old_size 			= old_sizex * old_sizey;
	
	int	old_arraylength	= *arraylength;
	
	int	old_sizez			= old_arraylength / old_size;
	
	//--------------------------------
	// temp array parameters
	
	void* temp_array 			= NULL;
	int   temp_arraylength 	= 0;												// actual array length is 0, will be set to temp_size by array_new()
	
	int	temp_sizex 			= old_sizex;
	int	temp_sizey 			= old_sizey;
	
	//--------------------------------
	// rotation area
	
	// cardinal / diagonal
	if (direction % 2 > 0)
		{
			return arrayin;														// currently not supported			
		}
	
	// swap x-y if rotated by 90 or 270 degree
	if ((direction == 2) || (direction == 6))
		{
			temp_sizex 			= old_sizey;
			temp_sizey 			= old_sizex;
		}			
	
	int	temp_size 			= old_arraylength;	
	
	//--------------------------------
	// temp array		
	
	temp_array 					= array_new( elementsize, &temp_arraylength, temp_size );	
	
	// same as
//	temp_arraylength 			= temp_size;
//	void* temp_array 			= (void*)sys_malloc(elementsize * temp_size);
	
	//--------------------------------
	
	if (!temp_array)
		{
//			printf("temp_array == NULL !!!");
			return arrayin;
		}
	
	if (temp_arraylength <= 0)
		{
//			printf("temp_arraylength <= 0 !!!");
			return arrayin;
		}
		
	//--------------------------------
	// valid rotation
	
	int i;			// x
	int j;			// y
	int k = 0;		// abs
	int m;			// z
	
	// cardinal directions : k counts in new array, i-j loops through old array positions, thus old_sizex-y needed to be used!
	if (direction == 0)																								// 0°
		{
			for (m=0; m<old_sizez; ++m)			
				{
					for (j=0; j<old_sizey; ++j)		
						{
							for (i=0; i<old_sizex; ++i)	
								{
									int temp = array3d_getabs(old_sizex, old_sizey, i, j, m);
									
//									temp_array[k] = arrayin[temp];												// wrong, void size cannot be determined
//									memcpy( &temp_array[k] , &arrayin[temp] , elementsize ); 			// copy new values - wrong
									
									char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
									char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
									memcpy( newaddress , oldaddress , elementsize ); 
									
									++k;
								}
						}
				}
		}
	else if (direction == 2)																						// 90°
		{
			for (m=0; m<old_sizez; ++m)			
				{
					for (i=0; i<old_sizex; ++i)			
						{
							for (j=0; j<old_sizey; ++j)	
								{
									int temp = array3d_getabs(old_sizex, old_sizey, i, old_sizey-1-j, m);
									
									char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
									char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
									memcpy( newaddress , oldaddress , elementsize ); 
									
									++k;
								}
						}
				}
		}
	else if (direction == 4)																						// 180°
		{
			for (m=0; m<old_sizez; ++m)			
				{
					for (j=0; j<old_sizey; ++j)	
						{
							for (i=0; i<old_sizex; ++i)
								{
									int temp = array3d_getabs(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j, m);
									
									char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
									char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
									memcpy( newaddress , oldaddress , elementsize ); 
									
									++k;
								}
						}
				}
		}
	else if (direction == 6)																						// 270°
		{
			for (m=0; m<old_sizez; ++m)			
				{
					for (i=0; i<old_sizex; ++i)
						{
							for (j=0; j<old_sizey; ++j)	
								{
									int temp = array3d_getabs(old_sizex, old_sizey, old_sizex-1-i, j, m);
									
									char* newaddress = (char*)temp_array + k * elementsize;				// char* casting is required!
									char* oldaddress = (char*)arrayin + temp * elementsize;				// char* casting is required!
									memcpy( newaddress , oldaddress , elementsize ); 
									
									++k;
								}
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength)
		{
			*arraylength 	= temp_arraylength;
		}
	*sizex 			= temp_sizex;
	*sizey 			= temp_sizey;
	
	//--------------------------------
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL);
	
	return temp_array;
}


//////////////////////////////////////////////////////////////////////////////////////


#endif