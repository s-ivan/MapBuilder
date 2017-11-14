#ifndef mapbuilder2_c
#define mapbuilder2_c


////////////////////////////////////
// to remap keys (used for f1-f12)


void Key_Do_Nothing() 
{
	wait(1);
}


////////////////////////////////////
// on exit from file menu, back to startup menu


#ifndef MB_GAME

void	Main_Restart()
{
sys_marker("MR0");
	//------------------------------------
	// close editor
	
	if (UIMode < (var)UIMODE_NEWMAP)
		{
			Map_DestroyMapData();
			wait_for(Map_DestroyMapData);
			wait(3);
			
			UIMode = (var)UIMODE_STARTUP;
			wait(5);	
		}
	
	level_load(NULL);
	wait(3);
	
	//------------------------------------
	// reopen startup menu
sys_marker("MR1");	
	// startup main menu
	MainMenu_Panel_Init();
	wait_for(MainMenu_Panel_Init);
sys_marker("MR2");	
	// set startup settings again - it could change because of wmb loads
	Main_Read_Settings(main_currentres);
	wait_for(Main_Read_Settings);
sys_marker("MR3");	
	// refresh non created and main menu panel positions due to resolution
	Main_RefreshPanels();
	wait_for(Main_RefreshPanels);
	
	//------------------------------------
	// hide static panels and texts
sys_marker("MR4");	
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
	
	reset( map_loadmap_text1 , SHOW);
	reset( map_loadmap_text2 , SHOW);
	reset( map_loadmap_text3 , SHOW);
	reset( map_loadmap_text3skip , SHOW);
	reset( map_loadmap_text4 , SHOW);	
	reset( map_loadmap_text4skip , SHOW);
	reset( map_loadmap_text5 , SHOW);
	reset( map_loadmap_text6 , SHOW);
	
#ifndef MB_RELEASE	
	reset( cluster_data_panel , SHOW );		
	reset( tile_data_panel , SHOW );
#endif
	
	reset( ui_mainmenu_panel , SHOW );
	reset( ui_helptext , SHOW );
	
	//------------------------------------
sys_marker(NULL);
}

#endif


////////////////////////////////////
// on normal exit or close button on window


void	Main_Exit()
{	
	if (UIMode < (var)UIMODE_NEWMAP)
		{
			Map_DestroyMapData();
			wait_for(Map_DestroyMapData);
			wait(3);
			
			UIMode = (var)UIMODE_STARTUP;
			wait(5);	
		}
	
	sys_exit(NULL);
}


////////////////////////////////////
// on error exit and after sys_exit - only one frame time before shut down


void Exit_Event() 
{
	if (terrhmp_watervertices != NULL) sys_free(terrhmp_watervertices);
{}
#ifndef MB_RELEASE	
	Terrain_StaticFree();	
#endif
}


////////////////////////////////////
// adapt the engine resolution to the window size - not needed now


void ScreenSize_Event() 			
{
	if (video_screen==(var)2)								// if windowed
		{
		   RECT r;
   		GetClientRect(hWnd,&r); 
   		video_set(r.right,r.bottom,0,0); 
   	}
	
	// it should adapt here panel positions within screen area !!!
	
	//...
	
	//-----------------------------------------------------
//	// switch mode on maximize?
//	if (video_screen==(var)1)								
//		{
//			video_switch(8,0,0);
//			video_switch(0,0,2);								// mode , 0 , window
//		}
//	else															// if windowed
//		{
//			video_switch(main_videomode,0,0);
//			video_switch(0,0,1);
//		}
}


////////////////////////////////////
// display a simple progress bar when a level is loaded


#ifndef MB_GAME

function on_level_event(percent)
{
	if (UIMode > (var)UIMODE_STARTUP)	return;		// do not show on startup screen
	
	set(map_loadbar,SHOW | LIGHT);   					// switch load bar on
	vec_set(map_loadbar.blue,vector(225,200,200));	// set own light color
	map_loadbar.pos_x = 0;
	map_loadbar.pos_y = screen_size.y - 32;
	map_loadbar.size_x = (screen_size.x%100) + integer(screen_size.x/100)*percent;   		// 24..1024 pixels
	map_loadbar.size_y = 32;
	if (percent >= 100) 										// level loading finished
	  	{	
			wait(1);          								// display the load bar for a bit
			reset(map_loadbar,SHOW);  						// switch it off
		}
}

#endif	


////////////////////////////////////


#endif