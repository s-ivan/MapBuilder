#ifndef MBarray_c
#define MBarray_c


///////////////////////////////////////////////////////////////////////////////////////////
// array helpers


void*		array_new(int elementsize, int* arraylength, int newelementscount)
{
sys_marker("sn1");	
	
	if (elementsize <= 0)			return NULL;
	if (newelementscount <= 0)		return NULL;
	
	int newsize = newelementscount;																						// cannot be smaller than 0
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
//			printf( "array_new error!" );																					// error			
			return NULL;
		}
	else
		{
			memset(arrayout, (char)0, elementsize * newsize);														// set everything to 0, char based!, not essential
			
			if (arraylength != NULL)
				{
					*arraylength = newelementscount;
				}
		}
	
sys_marker(NULL);    
	
	return(arrayout);
}


void*		array_new_default(int elementsize, int* arraylength, int newelementscount, char defaultvalue)
{
sys_marker("sn2");	
	
	if (elementsize <= 0)			return NULL;
	if (newelementscount <= 0)		return NULL;
	
	int newsize = newelementscount;																						// cannot be smaller than 0
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
//			printf( "array_new_default error!" );																		// error			
			return NULL;
		}
	else
		{
			memset(arrayout, defaultvalue, elementsize * newsize);												// set everything to defaultvalue, char based!
			
			if (arraylength != NULL)
				{
					*arraylength = newelementscount;
				}
		}
	
sys_marker(NULL);    
	
	return(arrayout);
}


// -> char
void		array_fill(void* arrayin, int elementsize, int arraylength, void* newelement)
{
sys_marker("sfi");
	
	if (arrayin == NULL)			return;
	if (elementsize <= 0)		return;
	if (arraylength <= 0)		return;
	if (newelement == NULL)		return;
	
	int i;								
	for(i=0; i<arraylength; ++i)
		{
//			memcpy( &arrayin[i] , newelement , elementsize );  													// copy new values
			memcpy( arrayin + i * elementsize , newelement , elementsize );  									// copy new values
		}
	
sys_marker(NULL);
}


void*		array_realloc(void* arrayin, int elementsize, int* arraylength, int newelementscount)
{
sys_marker("sre");
	
	if (arrayin == NULL)			return arrayin;
	if (elementsize <= 0)		return arrayin;
	if (arraylength == NULL)	return arrayin;
	
	int oldsize = *arraylength;
	int newsize = *arraylength + newelementscount;																	// can be smaller than arraylength!
	
	if (newsize <= 0)				return arrayin;
		
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
			printf( "sys_realloc error!" );																				// error
			
			return arrayin;
		}
	else
		{
			memcpy( arrayout , arrayin , (int)minv((var)oldsize, (var)newsize) * elementsize);			// copy old content						
			
			*arraylength += newelementscount;																			// increase element number
		}
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL);
	
	return arrayout;
}


// -> char
void*		array_add(void* arrayin, int elementsize, int* arraylength, int newelementscount, void* newelement)
{
sys_marker("sad");
	
	if (arrayin == NULL)			return arrayin;
	if (elementsize <= 0)		return arrayin;
	if (arraylength == NULL)	return arrayin;
	
//	if ((newelementscount > 0) && (newelement == NULL))															// newelementscount can be negative to decrease size, if not: default value must be set
//		return arrayin;																										// REMOVED - because of a later workaround
	
	int oldsize = *arraylength;
	int newsize = *arraylength + newelementscount;																	// can be smaller than arraylength!
	
	if (newsize <= 0)				return arrayin;
	
	void* arrayout = (void*)sys_malloc(elementsize * newsize);
	
	if (!arrayout) 
		{
			printf( "sys_add error!" );																					// error
			
			return arrayin;
		}
	else
		{
			memcpy( arrayout , arrayin , (int)minv((var)oldsize, (var)newsize) * elementsize);			// copy old content
			
			if (newelementscount > 0)																						// add new content if needed
				{
					if (newelement != NULL)
						{
							int i;								
							for(i=0; i<newelementscount; ++i)
								{
//									memcpy( &arrayout[oldsize + i] , newelement , elementsize ); 						 				// copy new values
									memcpy( arrayout + (oldsize + i) * elementsize , newelement , elementsize );  				// copy new values
								}
						}
					else
						{
//							memset( &arrayout[oldsize], (char)0, elementsize * (newsize-oldsize));									// set all bytes of new elements to 0, char based!
							memset( arrayout + oldsize * elementsize, (char)0, elementsize * (newsize-oldsize));				// set all bytes of new elements to 0, char based!
						}
				}
			
			*arraylength += newelementscount;																			// increase element number
		}
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL);    
	
	return arrayout;
}


void		array_destroy(void* arrayin, int* arraylength)
{
sys_marker("sde");
	
	if (arrayin == NULL)		return;
	
	if (arraylength != NULL)
		{
			*arraylength = 0;
		}
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL); 	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D array helpers


int		array_getx(int sizex, int sizey, int abspos)
{
	return abspos % sizex;
}


int		array_gety(int sizex, int sizey, int abspos)
{
	return integer(abspos / sizex);
}


int		array_getabs(int sizex, int sizey, int posx, int posy)
{
	return posx + posy * sizex;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D array operations


// -> char
void*		array_rotate(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey)
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
//			// diagonal - enlarged area
//			temp_sizex 			= (old_sizex + old_sizey - 1);
//			temp_sizey 			= (old_sizex + old_sizey - 1);			
			
			return arrayin;														// currently not supported			
		}
	
	// swap x-y if rotated by 90 or 270 degree
	if ((direction == 2) || (direction == 6))
		{
			temp_sizex 			= old_sizey;
			temp_sizey 			= old_sizex;
		}			
	
	int	temp_size 			= temp_sizex * temp_sizey;	
	
	//--------------------------------
	// temp array		
	
	temp_array 					= array_new( elementsize, &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
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
	
//	// *** TEST *** okay, returns original array fine
//	memcpy( &temp_array[0] , &arrayin[0] , elementsize * temp_size );
	
	int i;
	int j;
	int k = 0;
	
//	// *** TEST *** fails !
//	for (i=0; i<temp_size; ++i)	
//		{
//			memcpy( (&temp_array[0] + (i * elementsize)) , (&arrayin[0] + (i * elementsize)) , elementsize );	// void array, we do not know the address of items, thus needed to be calculated exasctly!
//		}
	
	
	// cardinal directions : k counts in new array, i-j loops through old array positions
	// diagonal directions : k counts in old array, i-j steps through new array positions
	
	// only cardinal directions supported
	if (direction == 0)																								// 0°
		{
			for (j=0; j<old_sizey; ++j)		
				{
					for (i=0; i<old_sizex; ++i)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, j);
							
							memcpy( (&temp_array[0] + (k * elementsize)) , (&arrayin[0] + (temp * elementsize)) , elementsize );
//							memcpy( &temp_array[k] , &arrayin[temp] , elementsize );
//							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 2)																						// 90°
		{
			for (i=0; i<old_sizex; ++i)			
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, old_sizey-1-j);
							
							memcpy( (&temp_array[0] + (k * elementsize)) , (&arrayin[0] + (temp * elementsize)) , elementsize );
//							memcpy( &temp_array[k] , &arrayin[temp] , elementsize );
//							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 4)																						// 180°
		{
			for (j=0; j<old_sizey; ++j)	
				{
					for (i=0; i<old_sizex; ++i)
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j);
							
							memcpy( (&temp_array[0] + (k * elementsize)) , (&arrayin[0] + (temp * elementsize)) , elementsize );
//							memcpy( &temp_array[k] , &arrayin[temp] , elementsize );						
//							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 6)																						// 270°
		{
			for (i=0; i<old_sizex; ++i)
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, j);
							
							memcpy( (&temp_array[0] + (k * elementsize)) , (&arrayin[0] + (temp * elementsize)) , elementsize );
//							memcpy( &temp_array[k] , &arrayin[temp] , elementsize );
//							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength != NULL)
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


char*		array_rotate_char(char* arrayin, int* arraylength, int direction, int* sizex, int* sizey)
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
	
	//--------------------------------
	// temp array parameters
	
	char* temp_array 			= NULL;
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
	
	int	temp_size 			= temp_sizex * temp_sizey;	
	
	//--------------------------------
	// temp array		
	
	temp_array 					= array_new( sizeof(char), &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
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
	
	int i;
	int j;
	int k = 0;	
	
	// cardinal directions : k counts in new array, i-j loops through old array positions, thus old_sizex-y needed to be used!
	if (direction == 0)																								// 0°
		{
			for (j=0; j<old_sizey; ++j)		
				{
					for (i=0; i<old_sizex; ++i)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 2)																						// 90°
		{
			for (i=0; i<old_sizex; ++i)			
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, old_sizey-1-j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 4)																						// 180°
		{
			for (j=0; j<old_sizey; ++j)	
				{
					for (i=0; i<old_sizex; ++i)
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j);
														
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 6)																						// 270°
		{
			for (i=0; i<old_sizex; ++i)
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength != NULL)
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


var*		array_rotate_var(var* arrayin, int* arraylength, int direction, int* sizex, int* sizey)
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
	
	//--------------------------------
	// temp array parameters
	
	var* temp_array 			= NULL;
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
	
	int	temp_size 			= temp_sizex * temp_sizey;	
	
	//--------------------------------
	// temp array		
	
	temp_array 					= array_new( sizeof(var), &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
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
	
	// old array x-y
	int i;
	int j;
	
	// new array counter
	int k = 0;	
	
	// cardinal directions : k counts in new array, i-j loops through old array positions
	if (direction == 0)																								// 0°
		{
			for (j=0; j<old_sizey; ++j)		
				{
					for (i=0; i<old_sizex; ++i)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 2)																						// 90°
		{
			for (i=0; i<old_sizex; ++i)			
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, i, old_sizey-1-j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 4)																						// 180°
		{
			for (j=0; j<old_sizey; ++j)	
				{
					for (i=0; i<old_sizex; ++i)
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j);
														
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}
	else if (direction == 6)																						// 270°
		{
			for (i=0; i<old_sizex; ++i)
				{
					for (j=0; j<old_sizey; ++j)	
						{
							int temp = array_getabs(old_sizex, old_sizey, old_sizex-1-i, j);
							
							temp_array[k] = arrayin[temp];
							
							++k;
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength != NULL)
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3D array helpers


int		array_getx_multi(int sizex, int sizey, int sizez, int abspos)
{
	return (abspos % (sizex * sizey)) % sizex;
}


int		array_gety_multi(int sizex, int sizey, int sizez, int abspos)
{
	return (integer((abspos % (sizex * sizey)) / sizex));
}


int		array_getabs_multi(int sizex, int sizey, int posx, int posy, int posz)
{
	return (posx + posy * sizex) + (sizex * sizey * posz);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3D array operations


// -> char
void*		array_rotate_multi(void* arrayin, int elementsize, int* arraylength, int direction, int* sizex, int* sizey, void* defaultelement)
{
sys_marker("srm");

	//--------------------------------
	// convert to 1..7 range
	
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
	
	int	old_arraylength	= *arraylength;
	int	old_sizex 			= *sizex;
	int	old_sizey 			= *sizey;
	int	old_size 			= old_sizex * old_sizey;
	
	int	old_sizez			= old_arraylength / old_size;
	
	//--------------------------------
	// temp array parameters
	
	void* temp_array 			= NULL;
	int   temp_arraylength 	= 0;																					// will be set to temp_size by array_new()
	
	int	temp_sizex 			= old_sizex;
	int	temp_sizey 			= old_sizey;
	
	// cardinal / diagonal
	if (direction % 2 > 0)
		{
//			// diagonal - enlarged area
//			temp_sizex 			= (old_sizex + old_sizey - 1);
//			temp_sizey 			= (old_sizex + old_sizey - 1);
			
			return arrayin;																							// currently not supported				
		}
	else
		{
			// cardinal - no size change
		}
	
	// swap x-y if rotated by 90 or 270 degree
	if ((direction == 2) || (direction == 6))
		{
			temp_sizex 			= old_sizey;
			temp_sizey 			= old_sizex;
		}	
	
	int	temp_size 			= temp_sizex * temp_sizey;		
	
	int	temp_sizez			= old_sizez;
	
	//--------------------------------
	// temp array
	
	temp_array 					= array_new( elementsize, &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
		{
			printf("temp_array == NULL !!!");
		}
	
	if (defaultelement != NULL)
		{
			array_fill( temp_array, elementsize, temp_arraylength, defaultelement );				// useful only in case of diagonal rotations to fill new corner parts
		}
	
	//--------------------------------
	// valid rotation
	
	int i;			// x
	int j;			// y
	int k = 0;		// abs
	int m;			// z
	
	// cardinal directions : k counts in new array, i-j-m loops through old array positions
	// diagonal directions : k counts in old array, i-j-m steps through new array positions
	
	// only cardinal directions supported
	if (direction == 2)																								// 90°
		{
			for (m=0; m<temp_sizez; ++m)			
				{
					for (i=0; i<temp_sizex; ++i)			
						{
							for (j=temp_sizey-1; j>=0; --j)
								{
									int temp = array_getabs_multi(temp_sizex, temp_sizey, i, j, m);
									
		//							temp_array[k] = arrayin[temp];												// does not work here because of void
									memcpy( &temp_array[k] , &arrayin[temp] , elementsize );  
									
									++k;
								}
						}
				}
		}
	else if (direction == 4)																						// 180°
		{
			for (m=0; m<temp_sizez; ++m)			
				{
					for (j=temp_sizey-1; j>=0; --j)
						{
							for (i=temp_sizex-1; i>=0; --i)
								{
									int temp = array_getabs_multi(temp_sizex, temp_sizey, i, j, m);
									
		//							temp_array[k] = arrayin[temp];												// does not work here because of void
									memcpy( &temp_array[k] , &arrayin[temp] , elementsize );  
									
									++k;
								}
						}
				}
		}
	else if (direction == 6)																						// 270°
		{
			for (m=0; m<temp_sizez; ++m)			
				{
					for (i=temp_sizex-1; i>=0; --i)
						{
							for (j=0; j<temp_sizey; ++j)	
								{
									int temp = array_getabs_multi(temp_sizex, temp_sizey, i, j, m);
									
		//							temp_array[k] = arrayin[temp];												// does not work here because of void
									memcpy( &temp_array[k] , &arrayin[temp] , elementsize );  
									
									++k;
								}
						}
				}
		}
		
	//--------------------------------
	// set parameters if were modified
	
	*arraylength 	= temp_arraylength;
	*sizex 			= temp_sizex;
	*sizey 			= temp_sizey;
	
	//--------------------------------
	
	sys_free(arrayin);
	arrayin = NULL;
	
sys_marker(NULL);
	
	return temp_array;
}


char*		array_rotate_multi_char(char* arrayin, int* arraylength, int direction, int* sizex, int* sizey)
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
	
	char* temp_array 			= NULL;
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
	
	temp_array 					= array_new( sizeof(char), &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
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
									int temp = array_getabs_multi(old_sizex, old_sizey, i, j, m);
									
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, i, old_sizey-1-j, m);
									
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j, m);
																
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, old_sizex-1-i, j, m);
									
									temp_array[k] = arrayin[temp];
									
									++k;
								}
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength != NULL)
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


var*		array_rotate_multi_var(var* arrayin, int* arraylength, int direction, int* sizex, int* sizey)
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
	
	var*	temp_array 			= NULL;
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
	
	temp_array 					= array_new( sizeof(var), &temp_arraylength, temp_size );	
	
	if (temp_array == NULL)
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
									int temp = array_getabs_multi(old_sizex, old_sizey, i, j, m);
									
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, i, old_sizey-1-j, m);
									
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, old_sizex-1-i, old_sizey-1-j, m);
																
									temp_array[k] = arrayin[temp];
									
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
									int temp = array_getabs_multi(old_sizex, old_sizey, old_sizex-1-i, j, m);
									
									temp_array[k] = arrayin[temp];
									
									++k;
								}
						}
				}
		}

	//--------------------------------
	// set parameters if were modified
	
	if (arraylength != NULL)
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


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//// List
//
//// by Rackscha at http://www.opserver.de/ubb7/ubbthreads.php?ubb=showflat&Number=430807&gonew=1#UNREAD
//int*		sys_realloc(void** AIn, int ASize, int AOldCount, int ANewCount)	
//{
//sys_marker("sre");
//
//    int i = 0;
//    void** LOut;
//    
//    LOut = sys_malloc(ASize*ANewCount);
//    
//    for(i = 0; i < minv(AOldCount, ANewCount); i++)
//	    {
//	        (LOut)[i] = (AIn)[i];
//	    }
//    
//    sys_free(AIn);
//    
//sys_marker(NULL);    
//    
//    return(LOut);
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////
//// list by Rackhscha
//
//TList* list_create()
//{
//sys_marker("llc");
//	TList* LList = (TList*)sys_malloc(sizeof(TList));
//	LList.count = 0;
//	LList.items = NULL;
//sys_marker(NULL);	
//	return(LList);
//}
//
//void list_free(TList* AList)
//{
//	if (AList)
//	{
//		list_clear(AList);
//		sys_free(AList);
//	}
//}
//
//
//
//
//void list_add(TList* AList, void* AItem)
//{
//sys_marker("lla");
//	AList.items = sys_realloc(AList.items, sizeof(int), AList.count, AList.count + 1);
//	AList.count += 1;
//	(AList.items)[AList.count - 1] = AItem;
//sys_marker(NULL);	
//}
//
//void list_remove(TList* AList, int AIndex)
//{
//
//    int i;
//    if(clamp(AIndex, 0, AList.count-1) == AIndex)
//    {
//        for (i = AIndex; i < AList.count-1; i++)
//        {
//            (AList.items)[i] = (AList.items)[i+1];
//        }
//        AList.items = sys_realloc(AList.items, sizeof(int), AList.count, AList.count-1);
//        AList.count -= 1;
//    }
//}
//
//int list_index_of(TList* AList, void* AItem)
//{
//
//    int i;
//    int LResult = -1;
//    for(i = 0; i < AList.count; i++)
//    {
//        if (AItem == (AList.items)[i])
//        {
//            LResult = i;
//            break;
//        }
//    }
//    return(LResult);
//}
//
//void list_delete(TList* AList, void* AItem)
//{
//
//    int LIndex = list_index_of(AList,AItem);
//    if (LIndex >= 0)
//    {
//        list_remove(AList,LIndex);
//    }
//}
//
//void list_clear(TList* AList)
//{
//	if(AList.items)
//	{
//		sys_free(AList.items);
//		AList.items = NULL;
//		AList.count = 0;	
//	}
//}


#endif