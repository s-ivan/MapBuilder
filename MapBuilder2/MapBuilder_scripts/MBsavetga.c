
#ifndef MBsavetga_c
#define MBsavetga_c

//////////////////////////////////////////////////////////////////////////
// variables
var savetga_filehandle;

//////////////////////////////////////////////////////////////////////////
// functions

void SaveTga_WriteByte(var shadow_byte);
void SaveTga_WriteShort(var shadow_short);
void SaveTga_SaveFile(BMAP* bmaptosave_bmp, STRING* filename_str, var bitdepth);

//////////////////////////////////////////////////////////////////////////
// code

// 8b and 16b writers

// write a var into file as 8 bit data i.e. as a byte
void SaveTga_WriteByte(var shadow_byte)
{
	//	// original
	//	file_asc_write (savetga_filehandle,shadow_byte);								
	//----------------------------------------
	char mybyte = (char)(int)shadow_byte;
	file_str_writeto( savetga_filehandle, (char*)&mybyte , sizeof(char) );	
}

// write a var into file as 16 bit data i.e. as a short
void SaveTga_WriteShort(var shadow_short)
{
	//	// original
	//	file_asc_write (savetga_filehandle,shadow_short&255);
	//	file_asc_write (savetga_filehandle,(shadow_short>>8)&255);
	//---------------------------------------------------
	short myshort = (short)(int)shadow_short;
	file_str_writeto( savetga_filehandle, (char*)&myshort , sizeof(short) );	
}

/////////////////////////////////////////////////////////////////////////
// save tga function 
// parameters: image bmap (cannot be compressed!) , filename string , tga format: 24 or 32
void SaveTga_SaveFile(BMAP* bmaptosave_bmp, STRING* filename_str, var bitdepth)
{
	COLOR 	temp1_col;
	var 		format 		= 0;
	var 		pixel 		= 0;
	var 		pixel_alpha = 0;
	
	long 		li;
	long 		lsize_x 		= bmap_width (bmaptosave_bmp);
	long 		lsize_y 		= bmap_height (bmaptosave_bmp);
	long 		lsize_xy 	= lsize_x*lsize_y;

	format = bmap_lock (bmaptosave_bmp,0);

	savetga_filehandle = file_open_write (filename_str);
	//----------------------------------------------------------------------------------------------// schreibe Header der tga Datei
	if (savetga_filehandle)
		{
			SaveTga_WriteByte(0);					// 0: No image data included.
			
			SaveTga_WriteByte(0);					// 0: No color map
			SaveTga_WriteByte(2); 					// 2: Image Type Code: 10 = RLE compressed tga, 2 = uncompressed tga
			
			SaveTga_WriteShort(0);					// 0: Color Map Origin. No colormap. 
			SaveTga_WriteShort(0);					// 0: Color Map Length. No colormap.
			SaveTga_WriteByte(0);					// 0: Color Map Entry Size. No colormap.
			
			SaveTga_WriteShort(0);					// 0: X Origin of Image.
			SaveTga_WriteShort(0);					// 0: Y Origin of Image.
			
			SaveTga_WriteShort((var)lsize_x); 	// Width of Image. Integer ( lo-hi ) width of the image in pixels. 
			SaveTga_WriteShort((var)lsize_y); 	// Height of Image. Integer ( lo-hi ) width of the image in pixels. 
			
			SaveTga_WriteByte(bitdepth); 			// Image Pixel Size. 24 RGB, or 32 ARGB			
			if (bitdepth==(var)24)
				{
					SaveTga_WriteByte(0);			// 0: Image Descriptor Byte.
				}
			else if (bitdepth==(var)32)
				{
					SaveTga_WriteByte(8);			// 8: Image Descriptor Byte.
				}			
//														Image Descriptor Byte Description:                                    
//														Bits 3-0 - number of attribute bits associated with each  
//														           pixel.  For the Targa 16, this would be 0 or   
//														           1.  For the Targa 24, it should be 0.  For     
//														           Targa 32, it should be 8.                      
//														Bit 4    - reserved.  Must be set to 0.                   
//														Bit 5    - screen origin bit.                             
//														           0 = Origin in lower left-hand corner.          
//														           1 = Origin in upper left-hand corner.          
//														           Must be 0 for Truevision images.               
//														Bits 7-6 - Data storage interleaving flag.                
//														           00 = non-interleaved.                          
//														           01 = two-way (even/odd) interleaving.          
//														           10 = four way interleaving.                    
//														           11 = reserved. 			
			// Image Identification Field. It does not exist because no image data included. But could be 255 character long.
			// ...
			// Color map data. If no colormap it does not exist.
			// ...
			//----------------------------------------------------------------------------------------------
			// Image Data Field.       
			for (li=0; li < lsize_xy; li++)
				{
					pixel = pixel_for_bmap (bmaptosave_bmp, li%lsize_x, (lsize_y-1)-integer(li/lsize_x));
					pixel_to_vec (temp1_col,pixel_alpha,format,pixel);
					pixel_alpha *= 2.55; 												// convert alpha 0-100 to 0-255
					SaveTga_WriteByte (temp1_col.blue); 							// blue
					SaveTga_WriteByte (temp1_col.green); 							// green
					SaveTga_WriteByte (temp1_col.red); 								// red					
					if (bitdepth==(var)32) 
						{
							SaveTga_WriteByte (pixel_alpha); 						// alpha
						}
				}
			file_close(savetga_filehandle);
		}
	bmap_unlock(bmaptosave_bmp);
}


#endif