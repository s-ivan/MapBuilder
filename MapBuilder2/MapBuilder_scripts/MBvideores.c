// code to find the resolution of a graphic card by djfeeler
//#include <acknex.h>  // Pure-Mode
//#include <default.c> // Standard-Keys
//#include <windows.h> // Windows-API-Calls

#ifndef MBvideores_c
#define MBvideores_c

typedef struct _devicemode { 
  char  dmDeviceName[32]; 
  WORD   dmSpecVersion; 
  WORD   dmDriverVersion; 
  WORD   dmSize; 
  WORD   dmDriverExtra; 
  DWORD  dmFields; 
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
      short dmScale; 
      short dmCopies; 
      short dmDefaultSource; 
      short dmPrintQuality; 

  short  dmColor; 
  short  dmDuplex; 
  short  dmYResolution; 
  short  dmTTOption; 
  short  dmCollate; 
  BYTE  dmFormName[32]; 
  WORD  dmLogPixels; 
  DWORD  dmBitsPerPel; 
  DWORD  dmPelsWidth; 
  DWORD  dmPelsHeight; 
  DWORD  dmDisplayFlags; 
  DWORD  dmDisplayFrequency; 
  DWORD  dmICMMethod;
  DWORD  dmICMIntent;
  DWORD  dmMediaType;
  DWORD  dmDitherType;
  DWORD  dmReserved1;
  DWORD  dmReserved2;
  DWORD  dmPanningWidth;
  DWORD  dmPanningHeight;
} SCRMODE; 


//PANEL* pan_results = 
//{
//	pos_x = 0;
//	pos_y = 0;
//	layer = 1;
//	flags = SHOW;
//	digits(0, 100, 4, *, 1, screen_size.x);
//	digits(0, 120, 4, *, 1, screen_size.y);
//}
  
SCRMODE resolution;

//Table of resolution available
var videores_table_x[30];
var videores_table_y[30];


void	VideoRes_GetScreenRes()
{	
	//create file with all resolution available
	
	//set all value at zero
	int i = 0;
	for (i=0; i<30; i++)
		{
		   videores_table_x[i] = 0;
		   videores_table_y[i] = 0;
		}
		
	// get values
	int n = 0;
	int e = 0;
	while(EnumDisplaySettings(NULL,n , &resolution)!=0)
		{
			var p =0;
			var VL_exist = 0;
			for (p=0; p<30; p++)
				{
					if((videores_table_x[p]==resolution.dmPelsWidth)&&(videores_table_y[p]==resolution.dmPelsHeight))
						{
							VL_exist = 1;
						}
						
				}
			
			if(VL_exist==0)
				{
					videores_table_x[e] = resolution.dmPelsWidth;
					videores_table_y[e] = resolution.dmPelsHeight;
					e = e+1;
				}
			
			n = n + 1;
		}
		
	/*var m=0;
	for (m=0; m<30; m++)
		{
			if(videores_table_x[m]>videores_table_x[m+1])
				{
					videores_table_x[m]=0;
					videores_table_y[m]=0;
				}	
		}*/
		
	//-----------------------------------------------------------------------
	// write into file - for debugging
	
	var VL_file_write_resolution = file_open_write("resolution1.txt");
	if(VL_file_write_resolution)
		{
			int l=0;
			while(videores_table_x[l]!=(var)0)
			{
				file_var_write(VL_file_write_resolution,videores_table_x[l]);
				file_str_write(VL_file_write_resolution," ");
				
				file_var_write(VL_file_write_resolution,videores_table_y[l]);
				file_str_write(VL_file_write_resolution,"\r\n");
				
				l++;
//				wait(1);
			}
			file_close(VL_file_write_resolution);
		}
	
	//----------------------------------------------------------------------	
	//----------------------------------------------------------------------
	// keep only if over 1024x768
	
	int m=0;
	for (m=0; m<30; m++)
		{
			if ((videores_table_x[m] < main_minresx) || (videores_table_y[m] < main_minresy))		// was 1024 768
				{
					videores_table_x[m]=0;
					videores_table_y[m]=0;
				}	
		}
		
	//-----------------------------------------------------------------------
	// sort them to begin from array pos 0
	
	int p = 0;
	int q = 0;
	for (p=0; p<30; p++)
		{
			if ((videores_table_x[p]==(var)0) || (videores_table_y[p]==(var)0))
				{
					for (q=p; q<30; q++)
						{
							if ((videores_table_x[q] >= main_minresx) && (videores_table_y[q] >= main_minresy))	// was 1024 768
								{
									videores_table_x[p] = videores_table_x[q];
									videores_table_y[p] = videores_table_y[q];
									
									videores_table_x[q] = 0;
									videores_table_y[q] = 0;
									
									break; // q
								}
						}
				}
		}
	
	//-----------------------------------------------------------------------
	// write into file - for debugging
	
	VL_file_write_resolution = file_open_write("resolution2.txt");
	if(VL_file_write_resolution)
		{
			var l=0;
			while(videores_table_x[l]!=(var)0)
			{
				file_var_write(VL_file_write_resolution,videores_table_x[l]);
				file_str_write(VL_file_write_resolution," ");
				
				file_var_write(VL_file_write_resolution,videores_table_y[l]);
				file_str_write(VL_file_write_resolution,"\r\n");
				
				l++;
//				wait(1);
			}
			file_close(VL_file_write_resolution);
		}
}

//void main() 
//{
//	video_set(1024,576,0,2);
//	level_load(NULL);
//	wait(1);
//	getscreenres();	
//}

#endif