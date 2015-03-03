//////////////////////////////
// Created by Emre SASMAZ 
// pantharay@gmail.com
// blog.pantharay.com
// sayha.pantharay.com
//////////////////////////////
// Modified by Ivan Stummer
// sivan@mailbox.hu
// mapbuilder.zxq.net
//////////////////////////////

#ifndef MBsaveaswmb_c
#define MBsaveaswmb_c


//////////////////////////////////////////////////////////////
// code

void		SaveWmb_Flags_startup()
{
	entflags[0]=0;
	entflags[1]=0;
	entflags[2]=0;
	entflags[3]=0;
	entflags[4]=0;
	//--------------
	entflags[5]=POLYGON;
	entflags[6]=0;				
	entflags[7]=0;
	entflags[8]=CAST;
	entflags[9]=DECAL;		// and METAL
	entflags[10]=BRIGHT;		
	entflags[11]=NOFOG;
	entflags[12]=LIGHT;
	entflags[13]=SHADOW;
	entflags[14]=UNLIT;
	entflags[15]=NOFILTER;
	entflags[16]=0;			
	entflags[17]=ZNEAR;
	entflags[18]=SPOTLIGHT;
	entflags[19]=OVERLAY;
	entflags[20]=0;
	entflags[21]=TRANSLUCENT;
	entflags[22]=PASSABLE;
	entflags[23]=INVISIBLE;
	entflags[24]=FLAG8;
	entflags[25]=FLAG7;
	entflags[26]=FLAG6;
	entflags[27]=FLAG5;
	entflags[28]=FLAG4;
	entflags[29]=FLAG3;
	entflags[30]=FLAG2;
	entflags[31]=FLAG1;							// (1<<0)
}


void		SaveWmb_Set_Flags(var flag16)
{
	STRING* temp_str 		= str_create("");
//	str_cpy(temp_str,"");
	
	if(flag16>0)
		{
			
			str_cat(temp_str,"0");	//-?
			str_cat(temp_str,"0");	//-?
			str_cat(temp_str,"0");	
			str_cat(temp_str,"0");	//-?
			str_cat(temp_str,"0");	//-?
			ifiselse(POLYGON)			//6	
			str_cat(temp_str,"0");	//LOCAL //7
			str_cat(temp_str,"0");	//8
			ifiselse(CAST)				//9
			ifiselse(DECAL)			//10			was METAL
			ifiselse(BRIGHT)			//11
			ifiselse(NOFOG)			//12
			str_cat(temp_str,"0");	//13
			ifiselse(SHADOW)
			ifiselse(UNLIT)
			ifiselse(NOFILTER)
		}
	
	str_cat(temp_str,"0");	//17
	ifiselse(ZNEAR)			//18
	ifiselse(SPOTLIGHT)		//-? //19
	ifiselse(OVERLAY)			//20
	str_cat(temp_str,"0");	//17
	ifiselse(TRANSLUCENT)	//21
	ifiselse(PASSABLE)		//22
	ifiselse(INVISIBLE)		//23
//	ifiselse(FLAG8)			//24
	str_cat(temp_str,"1");				// always set FLAG8 for MB wmb export, because they are disabled from saving, needed  on map reload
	ifiselse(FLAG7)			//25
	ifiselse(FLAG6)			//26
	ifiselse(FLAG5)			//28
	ifiselse(FLAG4)			//29
	ifiselse(FLAG3)			//30
	ifiselse(FLAG2)			//31
	ifiselse(FLAG1)			//32
	
	writetab;
	file_str_write (wmb_handle,temp_str); 
	skipline;
	
	ptr_remove(temp_str);
}


void		SaveWmb_Save_Wmb(STRING* filename_str)		// char* file_name // = "levels\\filename.wmb"
//void		save_wmb()
{
	// safety checks - because of menu, if enter was hit for file name input, not the okay button, it can stop the compilation
	while (key_enter) 
		{
			wait(1); 					
		}		
	if (proc_status(SaveWmb_Save_Wmb)>1)
		{
			return;
		}
	
	// cut off folder name
	str_clip( filename_str , str_len(lev_foldername) );	// "levels\\"
	// cut off extension
	str_trunc( filename_str , 4 );	// ".wmb"
	// protection from overwrite MB default wmb
	if (str_cmpi(filename_str,"mapbuilder"))
		{
			return;
		}
	
	// show panel----------------------------------------------------------------------
	set( map_loadpanel , SHOW); 
	set( map_savetext1 , SHOW);
	wait(3);
	
	//variables------------------------------------------------------------------------
	var 		num 		= 0;
	ENTITY* 	ent;
	char* 	funcname;
	var 		yourtype;
	var 		say		= 0;
	int 		i;
	STRING* 	temp_str 	= str_create("");
	STRING* 	temp2_str 	= str_create("");
//	str_cpy(save_dir,"");
	
	//Remove $$m & $$w & expalette.pcx-------------------------------------------------
//	remove("levels\\export.$$M");
//	remove("levels\\export.$$w");	
//	remove("export.$$w");									// for proper compilation and recreation
//	remove("levels\\expalette.pcx");
//	wait(-0.5);
	
	//Create $$w file (wad7)-----------------------------------------------------------
	str_cpy( temp_str , filename_str );
	str_cat( temp_str , ".$$w" );
		
	FILE *myfile = fopen(_chr(temp_str), "wb");		// write binary mode (truncates file to zero length or creates new file)
	int i=0;
	for(i=0;i<12;i++)
		{
			putc(bufferforwad[i], myfile);
		}
	fclose(myfile);	
	
	//Create expalette.pcx file (wad7)-------------------------------------------------
	str_cpy( temp_str , filename_str );
	str_cat( temp_str , "palette.pcx" );
	
	FILE *myfile = fopen(_chr(temp_str), "wb");		// write binary mode (truncates file to zero length or creates new file)
	int i=0;
	for(i=0;i<954;i++)
		{
			putc(bufferforpalette[i], myfile);
		}
	fclose(myfile);
	
	//Create $$m------------------------------------------------------------------------
	str_cpy( temp_str , filename_str );
	str_cat( temp_str , ".$$m" );
	
	wmb_handle = file_open_write(temp_str);			// instead of file_open_append() !!!
	
	//header
	file_str_write (wmb_handle,"// Exported by MapBuilder for 3D GameStudio"); skipline;
	skipline;
	file_str_write (wmb_handle,"version 711"); skipline;
	skipline;
	skipline;
		
	//level
	obrackets;file_str_write (wmb_handle," //level"); skipline;
	
	writetab;
	file_var_write (wmb_handle,0);
	skipline;
	writetab;
	str_cpy( temp_str , filename_str );
	str_cat( temp_str , ".$$w" );
	file_str_write (wmb_handle,temp_str); 				// without path for proper usage of $$m file (could be used with path if wmb is needed only)
	skipline;
	writetab;
	str_cpy( temp_str , filename_str );
	str_cat( temp_str , "palette.pcx" );
	file_str_write (wmb_handle,temp_str); 				// without path for proper usage of $$m file (could be used with path if wmb is needed only)
	skipline;
	
	cbrackets;skipline;
	
	//sun
	obrackets;file_str_write (wmb_handle," //sun"); skipline;
	
	writetab;
	file_var_write (wmb_handle,5);
	skipline;
	
	writetab;													//¤¤¤ mod. place exchanged with tilt
	file_var_write (wmb_handle,sun_angle.pan);
	skipline;
	
	writetab;
	file_var_write (wmb_handle,sun_angle.tilt);
	skipline;		
	
	cbrackets;skipline;	

	
	//----------------------------------------------------------------------
	//{ //model
	//	ent id
	//	xyz 
	//	pan tilt roll
	//	scale_xyz 
	//	file name
	//	ent name
	//	action name
	//	skills
	//	flags
	//	ambient 
	//	albedo
	//	path
	//	attached entity 
	//	material
	// string1
	// string2
	//}
	
	int counter_int = 0;
	
	//entities
	you = ent_next(NULL); 
	
	while (you)
		{
			if (!str_cmpi(str_for_entfile(NULL, you), "")) 																		// protection from some strange entities got from previously saved wmb
			{	
			if ( 																																// protection from other editor helper entities:
					((you.ENT_CATEGORY < (2*(var)UIMODE_SHADOWSHIFT)) && (you.ENT_CATEGORY > (var)0))				// no camera and other system entities
					|| (you.ENT_CATEGORY == (var)UIMODE_GROUNDTERR) 															// but ground terrain yes
					|| (you.ENT_CATEGORY == (var)UIMODE_WATERTERR) 																// and water terrain yes too
				)															
			{		
				
		//		draw_text("PLEASE WAIT",10,10,COLOR_RED);
								
				obrackets;file_str_write (wmb_handle," //model"); skipline;						
				
				#ifndef ent_id
				#define ent_id
					
					yourtype=0;
					/*
					from sdk:
					'wmpio_ModelEntity  * model;	// id == 3 (7, for model-ex)'
					SO CHECK IF EX OR NOT (if has a material or string1-2 or has a action it's 7 else it's 3)
					*/
					
					//if(your.materialtype!=NULL)
					if ( (your.material!=mat_model) && (your.material!=NULL) && (your.material!=mat_terrain) )
						{
							yourtype+=1;
						}
					//if(your.string1||string2!=NULL)
					if ( (str_len(your.string1)>0) || (str_len(your.string2)>0) )
						{
							yourtype+=1;
						}	
					
					num = 0;
					say=0;
					//Check Entity has an action or not.
					for(num = 0; engine_gettaskinfo(num,&funcname,&ent); num++)
						{
							if(ent == you)
							{ 
								yourtype+=1;
								break;
							}
						}
					
					//----------------------------------------------------------------------------------------------------
					// !!! MapBuilder specific - always set it to 7, because those extra flags and skills are needed !!!
					yourtype+=1;
					//----------------------------------------------------------------------------------------------------
					
					if(yourtype>0)
						{
							writetab;
							file_var_write (wmb_handle,7);
							skipline;//model-ex
						}
					else
						{
							writetab;
							file_var_write (wmb_handle,3);
							skipline;//model
						}
				#endif
				
				
				//xyz
				writetab;
				file_var_write (wmb_handle,you.x);
				file_var_write (wmb_handle,you.y);
				file_var_write (wmb_handle,you.z);
				skipline;
				//pan-tilt-roll
				writetab;
				file_var_write (wmb_handle,you.pan);
				file_var_write (wmb_handle,you.tilt);
				file_var_write (wmb_handle,you.roll);
				skipline;
				//scale
				writetab;
				file_var_write (wmb_handle,you.scale_x);
				file_var_write (wmb_handle,you.scale_y);
				file_var_write (wmb_handle,you.scale_z);
				skipline;
				//Entitiy file name
				str_for_entfile(temp_str,you); 
				writetab;
				file_str_write (wmb_handle,temp_str); 
				skipline;
				//Ent name
				str_for_entname(temp_str,you); 
				writetab;
				file_str_write (wmb_handle,temp_str); 
				skipline;
				
				//----------------------------------------------------------------------------------
				num = 0;
				say=0;
				//Get action name of Entity (Thanks to MasterQ32&JustSid for tip) - works only if has a loop
				if (ent_type(you)==(var)4)	//terrain
					{
						if(is(you, PASSABLE))
							{
								writetab;
								file_str_write (wmb_handle,"TerrHmp_WaterAction"); 
								skipline;
							}
						else
							{
								writetab;
								file_str_write (wmb_handle,"TerrHmp_GroundAction"); 
								skipline;
							}
					}
				else
					{
						for(num = 0; engine_gettaskinfo(num,&funcname,&ent); num++)
							{
								if(ent == you)
								{ 
									writetab;
									file_str_write (wmb_handle,_str(funcname)); 
									skipline;
									say=1;
									break;
								}
								
							}
						//if entity has no action, put ndef
						if(say==0)
							{
								writetab;
								file_str_write (wmb_handle,"ndef"); 
								skipline;
							}
					}
				//----------------------------------------------------------------------------------
				// !!! MapBuilder Specific !!! store bounding box in skills 1-6
				
				//	#define MINX				skill1		// bounding box
				//	#define MINY				skill2
				//	#define MINZ				skill3
				//	#define MAXX				skill4
				//	#define MAXY				skill5
				//	#define MAXZ				skill6
				//	
				//	#define MYALPHA			skill7		// WMB alpha
				//					
				// skill8-9 					used for clipping settings to store and to set eflags unstored by wmb ¤¤¤¤¤¤				
				//
				//	#define ABSTILE_POS		skill14		// 10000..250000
				//	#define XTILE_POS		 	skill15		// 0..500 x pos
				//	#define YTILE_POS		 	skill16		// 0..500 y pos
				//	#define ZTILE_POS		 	skill17		// 0..2   z pos - might be different later - i.e. snapping vertical building levels to whole tile heights
				//	
				//	#define AREA_SIZEX		skill18		// area occupied by entity in tiles - x&y must be the same for units
				//	#define AREA_SIZEY		skill19		// area occupied by entity in tiles - x&y can differ for buildings (, and ships )
				//	
				//	#define ENT_CATEGORY		skill20		// differentiate between entity categories, and other system entities. under 100 it is an deletable entity
				
				you.skill1 = you.min_x;
				you.skill2 = you.min_y;
				you.skill3 = you.min_z;
				you.skill4 = you.max_x;
				you.skill5 = you.max_y;
				you.skill6 = you.max_z;
				
				you.skill7 = you.alpha;
				
				//¤¤¤ store clipping
				if (you.eflags&(CLIP0))
					{
						you.skill8 = 1;
					}
				else
					{
						you.skill8 = 0;
					}
					
				if (you.eflags&(CLIP1))
					{
						you.skill9 = 1;
					}
				else
					{
						you.skill9 = 0;
					}
				
				//----------------------------------------------------------------------------------
				
				//skills
				writetab;
				file_var_write (wmb_handle,you.skill1);
				file_var_write (wmb_handle,you.skill2);
				file_var_write (wmb_handle,you.skill3);
				file_var_write (wmb_handle,you.skill4);
				file_var_write (wmb_handle,you.skill5);
				file_var_write (wmb_handle,you.skill6);
				file_var_write (wmb_handle,you.skill7);
				file_var_write (wmb_handle,you.skill8);				
				
				if(yourtype>0)//type==7 									// in MapBuilder always used !!!
					{
						file_var_write (wmb_handle,you.skill9);
						file_var_write (wmb_handle,you.skill10);
						file_var_write (wmb_handle,you.skill11);
						file_var_write (wmb_handle,you.skill12);
						file_var_write (wmb_handle,you.skill13);
						file_var_write (wmb_handle,you.skill14);
						file_var_write (wmb_handle,you.skill15);
						file_var_write (wmb_handle,you.skill16);
						file_var_write (wmb_handle,you.skill17);
						file_var_write (wmb_handle,you.skill18);
						file_var_write (wmb_handle,you.skill19);
						file_var_write (wmb_handle,you.skill20);
						
					}
				
				skipline;
				
				//--------------------------------------------------
				//flags 1-16 and 17-32
				
				var myflag16 = 0;
				
				i = 0;
				for(i=0;i<16;i++)						// write 1st 16 flags only if one of them set
					{
						if (is(you, entflags[i]))
						{
							myflag16 = 1;
							break;
						}
					
					}
				
				SaveWmb_Set_Flags(myflag16);		// writes flag values: 16 or 32 flags in total
				wait_for(SaveWmb_Set_Flags);
				
				//--------------------------------------------------
				// ambient, albedo, path, attached to
				
				writetab;
				file_var_write (wmb_handle,your.ambient);
				skipline;
				
				//Get material name of Entity
				if(your.material!=mat_model&&your.material!=NULL&&your.material!=mat_terrain)
					{	
						writetab;
						file_var_write (wmb_handle,your.albedo);
						skipline;
						
						writetab;
						file_var_write (wmb_handle,0);
						skipline; //path
						
						writetab;
						file_var_write (wmb_handle,0);
						skipline; //attach
						
						if (ent_type(you)!=(var)4)	//¤¤¤
							{
								str_cpy(temp_str,_chr(your.material.link.name));
								writetab;
								file_str_write (wmb_handle,temp_str); 
								skipline;
							}
						else
							{
								writetab;
								file_str_write (wmb_handle,"ndef"); 
								skipline; // if no material put ndef
							}
					}
				else
					{
						if(yourtype>0)
						{
							writetab;
							file_var_write (wmb_handle,your.albedo);
							skipline;
							
							writetab;
							file_var_write (wmb_handle,0);
							skipline;//path
							
							writetab;
							file_var_write (wmb_handle,0);
							skipline;//attach
							
							writetab;
							file_str_write (wmb_handle,"ndef"); 
							skipline; // if no material put ndef
						}
					}

				//--------------------------------------------------
				//string1 string2	
				
				// !!! MapBuilder specific !!! store shadow entity name in string1 !!!
//				if (ent_type(you)!=(var)4)	//¤¤¤
					{
						if (you.shadow)
							{
								str_for_entfile(temp_str,you.shadow);				// decal shadow name from extisting shadow entity - supports free evrsion decal shadows						
								writetab;
								file_str_write (wmb_handle,temp_str); 
								skipline;
							}
						else
							{
								// !!! MapBuilder specific !!!						// only one shrub sprite should have it per group...
								if ( (!is(you,SHADOW)) && (is(you,FLAG6)) )		// has a ent_decal shadow - no shadow flag , not in lightmap - FLAG5 is only for forcing it in pssm mode
									{
										// get shadow entitiy name
										str_for_entfile(temp_str,you);
										
										str_cpy(temp2_str,shadow_foldername);
										str_cat(temp2_str,temp_str);
										str_trunc(temp2_str,4);
										str_cat(temp2_str,"_shadow.dds");	
																		
										if ( file_exists(temp2_str) )							// has an own image - cheching with shadow folder name
											{
												str_trunc(temp_str,4);
												str_cat(temp_str,"_shadow.dds");				// saving is without foldername	
												writetab;
												file_str_write (wmb_handle,temp_str); 
												skipline;
											}
										else
											{
												if (is(you,FLAG7))						// tree
													{
														str_for_entfile(temp_str,ent_treeshadow_fix);
													}
												else											// other
													{
														str_for_entfile(temp_str,ent_unitshadow_rot);
													}
												writetab;
												file_str_write (wmb_handle,temp_str); 
												skipline;
											}
									}
								else
									{
										// original by Emre				
										if(str_len(your.string1)>0)
											{
		//										str_cpy(temp_str,_chr(your.string1));	// original by Emre
												str_cpy(temp_str,"");
												writetab;
												file_str_write (wmb_handle,temp_str); 
												skipline;
											}
									}
							}				
						
						if(str_len(your.string2)>0)
							{
		//						str_cpy(temp_str,_chr(your.string2));	// original by Emre
								str_cpy(temp_str,"");
								writetab;
								file_str_write (wmb_handle,temp_str); 
								skipline;
							}	
					}
				
				cbrackets;skipline;
				//-----------------------------------------------------------------------
			}
			}
			
			you = ent_next(you);
//			wait(1);																						// not needed, faster if commented out
			
			// progress bar
			if (counter_int%100==0)																	// at every 100th entity loading
				{
					Map_Progressbar(counter_int/num_entities*100);
					wait_for(Map_Progressbar);
					wait(1);
				}	
			counter_int++;
		}
	
	file_close(wmb_handle);
	
	// $$m file ready in root folder
	
	wait(1);
	reset(map_loadbar,SHOW); 
	
////////////////////////////////////////////////////////////////////////////////////////////////		
	
	// wmb compilation from $$m by wwmp2wmb.exe
	
	//------------------------------------------------------------------------------
	// copy terrains to MABuilder root folder - not needed, all paths are set by MapBuilder.wdl
	
//	if (terrain_entity!=NULL)
//		{
//			str_cpy(temp_str,hmp_foldername);	
//			str_for_entfile(temp2_str,terrain_entity); 
//			str_cat(temp_str,temp2_str);
//			if (file_exists(temp_str))
//				{
//					file_cpy(str_for_entfile(NULL,terrain_entity),temp_str);		// deleted after complilation
//				}
//			else
//				{
//					printf("Terrain not found!");
//					return;
//				}
//		}
//	if (water_entity!=NULL)
//		{	
//			str_cpy(temp_str,hmpw_foldername);	
//			str_for_entfile(temp2_str,water_entity); 
//			str_cat(temp_str,temp2_str);
//			if (file_exists(temp_str))
//				{
//					file_cpy(str_for_entfile(NULL,water_entity),temp_str);		// deleted after complilation
//				}
//			else
//				{
//					printf("Water not found!");
//					return;
//				}
//		}
		
	//--------------------------------------------------------------------------------
	
	// rgb values are iterated from 0..255 to 0..100 !!! and from BRG to RGB !!!
	
	// start it with "-dialog " if manual settings are needed !!!
	// in this case only file name, palette, sun, ambient, fog, sould be added by script
#ifdef USE_DIALOG
	str_cpy(temp_str,"-dialog ");
#else
	str_cpy(temp_str,"");
#endif

	// create string for exec options - to set sun pos, sun color, ambient color, fog colors, and default settings
	str_cat(temp_str,filename_str);
	str_cat(temp_str,".$$m -pal ");
	str_cat(temp_str,filename_str);
	str_cat(temp_str,"palette.pcx ");
	
	// sun azimuth
	str_cat(temp_str,"
-az ");
	str_cat(temp_str,str_for_int(NULL,(int)sun_angle.pan));
	str_cat(temp_str," ");
	// sun elevation
	str_cat(temp_str,"-el ");
	str_cat(temp_str,str_for_int(NULL,(int)sun_angle.tilt));
	str_cat(temp_str," ");
	// sun color
	str_cat(temp_str,"
-sunrgb ");
	str_cat(temp_str,str_for_int(NULL,integer(sun_color.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(sun_color.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(sun_color.blue/2.55)));
	str_cat(temp_str," ");
	// ambient
	str_cat(temp_str,"
-ambrgb ");
	str_cat(temp_str,str_for_int(NULL,integer(ambient_color.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(ambient_color.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(ambient_color.blue/2.55)));
	str_cat(temp_str,"  ");
	// fog1
	str_cat(temp_str,"
-fog1 ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor1.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor1.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor1.blue/2.55)));
	str_cat(temp_str,"  ");
	// fog2
	str_cat(temp_str,"
-fog2 ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor2.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor2.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor2.blue/2.55)));
	str_cat(temp_str,"  ");
	// fog3
	str_cat(temp_str,"
-fog3 ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor3.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor3.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor3.blue/2.55)));
	str_cat(temp_str,"  ");
	// fog4
	str_cat(temp_str,"
-fog4 ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor4.red/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor4.green/2.55)));
	str_cat(temp_str," ");
	str_cat(temp_str,str_for_int(NULL,integer(d3d_fogcolor4.blue/2.55)));
	str_cat(temp_str,"  ");
	
#ifndef USE_DIALOG
	// remaining default values
	str_cat(temp_str,"
-hiprec 
-terralit 
-writelog 
-mesh 
-tesselate 
-solidsky 
-nodetail 
-threads 1 
-gamma 1.50 
-phongangle 120  
-lmambient 50  
-sizeshaded 236 
-litres 0.64 
-litcomp 0.50 
-litmax 196 
-litscale 0.50 
-radsubdiv 64 
-radbounce 0 
-noportals 
-nobsp 
-bound 56000 
-fat 64  48  8  
-narrow 32  32  0  ");
#endif

//-close

//after hiprec, makes black dots:
//-litmapsonly 

// after merge, for BSP only:
//-mergeacross 

//after mergeacross:
//-terralit 

// after solidsky, invisible cast shadow and set for collision?:
//-invisibletonone 

//after nodetail:
//-quiet 
//-preview 

//alternative CPU:
//-threads 2 

// alternative litres, better but much slower:
//-litres 1.00 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -close is required with exec_wait - it closes compiler, and gets back to mapbuilder,
	// but in case of an error it is risky to use, better to close compiler manually !!!
//	if (file_exists("%EXE_DIR%\\wwmp2wmb.exe"))									// run from SED - works !!!
//		{
//			exec_wait( "%EXE_DIR%\\wwmp2wmb.exe" , temp_str );		
//			while(!sys_active(NULL)) wait(1);      
//			printf("Compilation done\nHit Enter!");
//		}
//	else if (file_exists("%PROG_DIR%\\Gstudio8\\wwmp2wmb.exe"))				// run as exe - works !!!
//		{
//			exec_wait( "%PROG_DIR%\\Gstudio7\\wwmp2wmb.exe" , temp_str );		
//			while(!sys_active(NULL)) wait(1);      
//			printf("Compilation done\nHit Enter!");
//		}
//	else if (file_exists("%PROG_DIR%\\Gstudio7\\wwmp2wmb.exe"))				// run as exe - works !!!
//		{
//			exec_wait( "%PROG_DIR%\\Gstudio7\\wwmp2wmb.exe" , temp_str );		
//			while(!sys_active(NULL)) wait(1);      
//			printf("Compilation done\nHit Enter!");
//		}
//	else if (file_exists("wwmp2wmb.exe"))											// run as exe - works !!!
//		{
//			exec_wait( "wwmp2wmb.exe" , temp_str );		
//			while(!sys_active(NULL)) wait(1);      
//			printf("Compilation done\nHit Enter!");
//		}
//	else
//		{			
//			printf("3D Gamestudio is not installed in default folder!");	
//			wait(3);
//			printf("Please copy wwmp2wmb.exe to MapBuilder folder!");						
//			// or file_cpy(destname,sourcename);
//		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// manual closing of compiler - safer solution
	// wwmp2wmb.exe's caption = "Map Compiler"

	if (file_exists("%EXE_DIR%\\wwmp2wmb.exe"))									// run from SED - works !!!
		{
			exec( "%EXE_DIR%\\wwmp2wmb.exe" , temp_str );		
			while(!sys_active("Map Compiler")) { wait(1); }    				// wait for opening map compiler
			while(sys_active("Map Compiler")) { wait(1); }     				// wait for closing map compiler
//			printf("Compilation done\nHit Enter!");
		}
	else if (file_exists("%PROG_DIR%\\GStudio8\\wwmp2wmb.exe"))				// run as exe - works !!!
		{
			exec( "%PROG_DIR%\\GStudio8\\wwmp2wmb.exe" , temp_str );		
			while(!sys_active("Map Compiler")) { wait(1); }     
			while(sys_active("Map Compiler")) { wait(1); }  
//			printf("Compilation done\nHit Enter!");
		}
	else if (file_exists("%PROG_DIR%\\Gstudio7\\wwmp2wmb.exe"))				// run as exe - works !!!
		{
			exec( "%PROG_DIR%\\Gstudio7\\wwmp2wmb.exe" , temp_str );		
			while(!sys_active("Map Compiler")) { wait(1); }    
			while(sys_active("Map Compiler")) { wait(1); }   
//			printf("Compilation done\nHit Enter!");
		}
	else if (file_exists("wwmp2wmb.exe"))											// run as exe - works !!!
		{
			exec( "wwmp2wmb.exe" , temp_str );		
			while(!sys_active("Map Compiler")) { wait(1); }    
			while(sys_active("Map Compiler")) { wait(1); }   
//			printf("Compilation done\nHit Enter!");
		}
	else
		{			
			printf("3D Gamestudio is not installed in default folders!");	
			wait(3);
			printf("Please copy wwmp2wmb.exe to MapBuilder folder!");						
			// or file_cpy(destname,sourcename);
		}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// exec_wait only with -close !!! 3dgs stays on top in case of exec_wait !!! but not perfect, finally 3dgs gets minimized
	// so exec without -close is better to see compilation was successful or not
	
//	while(!sys_active(NULL)) wait(1); 
//	sys_message("[Alt-][TAB]");								// works with exec, but if more applications are open the order is not okay
	
//	str_cpy(temp_str,"export.$$M -pal expalette.pcx 
//-az 315 -el 60 
//-sunrgb 56 49 43 
//-ambrgb 19 19 19  
//-fog1 40 38 40  
//-fog2 71 81 96 
//-fog3 25 24 33
//-fog4 14 15 19
//-hiprec 
//-litmapsonly 
//-writelog 
//-mesh 
//-mergeacross 
//-tesselate 
//-solidsky 
//-nodetail 
//-quiet 
//-threads 1 
//-gamma 1.50 
//-phongangle 120  
//-lmambient 50  
//-sizeshaded 236 
//-litres 1.00 
//-litcomp 0.50 
//-litmax 196 
//-litscale 0.50 
//-radsubdiv 64 
//-radbounce 0 
//-noportals 
//-nobsp
//-bound 56000 
//-fat 64  48  8  
//-narrow 32  32  0  ");	
//	
//	//build it with default			  before filename: if dialog panel required: "-dialog "
//	exec("%EXE_DIR%\\wwmp2wmb.exe",temp_str);

// -close      - close window after compilation
// -hiprec 	   - not needed for testing
//	-nodetail 	- ?
// -threads 2  - if dual core
//	-noportals 	- it is not in my log, instead fogs are included		
	
	//-------------------------------------------------------------------------------------	
	// after compiler execution closed:
	
	// rename to move files to levels subfolder: 	
	wait(10);
	str_cpy(save_dir,"");
	
	// *.wmb , 
	str_cpy( temp2_str,filename_str );
	str_cat( temp2_str , ".wmb" );							// create wmb filename just exported - error if added to filename_str !!!																
	if (file_exists(temp2_str))
		{
			str_cpy( temp_str , lev_foldername );			// create new wmb filename with path // lev_foldername "test"
			str_cat( temp_str , temp2_str );				
			remove( _chr(temp_str) );							// delete because if exists , renaming does not work
			file_rename( temp2_str , temp_str );			// old name , new name
		}
		
	// *.$$m ,
	str_cpy( temp2_str,filename_str );
	str_cat( temp2_str , ".$$m" );							// create wmb filename just exported - error if added to filename_str !!!
	if (file_exists(temp2_str))
		{
			str_cpy( temp_str , lev_foldername );			// create new wmb filename with path // lev_foldername "test"
			str_cat( temp_str , temp2_str );	
			remove( _chr(temp_str) );							// delete because if exists , renaming does not work
			file_rename( temp2_str , temp_str );			// old name , new name
		}
		
	// *.$$w ,
	str_cpy( temp2_str,filename_str );
	str_cat( temp2_str , ".$$w" );							// create wmb filename just exported - error if added to filename_str !!!
	if (file_exists(temp2_str))
		{
			str_cpy( temp_str , lev_foldername );			// create new wmb filename with path // lev_foldername "test"
			str_cat( temp_str , temp2_str );	
			remove( _chr(temp_str) );							// delete because if exists , renaming does not work
			file_rename( temp2_str , temp_str );			// old name , new name
		}
	
	// *palette.pcx
	str_cpy( temp2_str,filename_str );
	str_cat( temp2_str , "palette.pcx" );					// create wmb filename just exported - error if added to filename_str !!!
	if (file_exists(temp2_str))
		{
			str_cpy( temp_str , lev_foldername );			// create new wmb filename with path // lev_foldername "test"
			str_cat( temp_str , temp2_str );	
			remove( _chr(temp_str) );							// delete because if exists , renaming does not work, or file_delete
			file_rename( temp2_str , temp_str );			// old name , new name
		}
	
	//-------------------------------------------
	// remove copied terrains from root folder - not needed, paths are set by mapbuilder.wdl
	
//	if (terrain_entity!=NULL)
//		remove( _chr( str_for_entfile(NULL,terrain_entity) ) );		// file_delete( str_for_entfile(NULL,terrain_entity) );
//	
//	if (water_entity!=NULL)
//		remove( _chr( str_for_entfile(NULL,water_entity) ) );			// file_delete( str_for_entfile(NULL,water_entity)	);
	
	//-------------------------------------------------------------------------------------
	// create wmbi wmb info file **********************************************************
	str_cpy( temp_str, lev_foldername );
	str_cat( temp_str, filename_str );
	str_cat( temp_str, ".i" );											// wmbi
	
	var file_handle = file_open_write(temp_str);
	
	str_for_num( 			temp_str 	, shadow_stencil );		// var
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_num( 			temp_str 	, main_blur );				// var
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_num( 			temp_str 	, fog_color );				// var
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_num( 			temp_str 	, num_entities );			// var (for entity post-processing progress bar)
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_close(file_handle);
	//*************************************************************************************
	//-------------------------------------------------------------------------------------
	
	reset( map_loadpanel , SHOW); 
	reset( map_savetext1 , SHOW);
	wait(3);
	
	ptr_remove(temp_str);
	ptr_remove(temp2_str);
}

#endif