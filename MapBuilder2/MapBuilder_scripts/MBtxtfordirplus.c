//=========================================================================================
//txt_for_dir_plus( TEXT** targ, STRING* pathname, STRING* wildcards): var
//Reads filenames that match the given wildcard/s into separate strings of a text object. 
//This instruction can be used to scan a folder for files matching MULTIPLE wildcards. 
//By EvilSOB
//modified by Sivan - TEXT* targ should exist now !!!
//-----------------------------------------------------------------------------------------
//Parameters:
//targ			Pointer to the target TEXT* pointer for the TEXT object to be filled.
//					If the target TEXT* pointer is NULL, a new TEXT object is created there.
//					If the target TEXT* pointer contains a TEXT object, then the filenames
//					found by this function are ADDED to the END of this existing text object.
//
//pathname		Absolute or Relative Pathname of folder to be searched
//
//wildcards		File names (C-style wildcards) to be matched, '|' separates wildcards.
//					eg: "*.MDL|*.HMP|-*_open.*" This will include all MDL and HMP files,
//					and from that list it will then exclude all "*_open.*" files.
//-----------------------------------------------------------------------------------------
//Returns:
//Number of strings ADDED to 'targ' TEXT object. 
//=========================================================================================
#ifndef MBtxtfordirplus_c
#define MBtxtfordirplus_c


function	Txt_For_Dir_PlusMod( TEXT** targ, var textlength, STRING* pathname, STRING* wildcards)
{
sys_marker("tfd");	
	if((!targ)||(!pathname)||(!wildcards))			
		return(0);	//un-resolvable parameter set
	//--------------------------------------------------------------------------------------
	TEXT *targ_txt, *tmp_txt=NULL, *sub_txt=NULL;		
	STRING* tmp_str = str_create("");
	var f_count,t_cnt, wilds, i,ii,sub;					
	//--------------------------------------------------------------------------------------
	//Extract / separate wildcards
	
	str_cpy(tmp_str, wildcards);		
	wilds = 0;
	
	while(str_stri(tmp_str,"|"))	
		{
			str_clip(tmp_str,str_stri(tmp_str,"|"));	
			wilds++;	
		}
	
	STRING* *all_wilds = (STRING**)sys_malloc(sizeof(STRING*) * (++wilds));							// was wilds++
	memset(all_wilds, 0, sizeof(STRING*) * wilds);			
	
	str_cpy(tmp_str, wildcards);	
	
	for(i=0;	i<wilds;	i++)
		{	
			all_wilds[i] = str_create(tmp_str);	
			
			if(str_stri(tmp_str,"|"))	
				{	
					str_clip(tmp_str,str_stri(tmp_str,"|"));
					str_trunc(all_wilds[i],str_len(tmp_str)+1);	
				}	
		}
	//--------------------------------------------------------------------------------------
	//ADD files to temporary TEXT for each wildcard that DOESNT start with a '-' sign
	tmp_txt=txt_create(0,0);	
	tmp_txt.strings=0;	
	sub_txt=txt_create(16,0);		
	for(t_cnt=0; t_cnt<wilds; t_cnt++)
		{	
			if(str_to_asc(all_wilds[t_cnt])!='-')
				{	
					str_cpy(tmp_str, pathname);	
					if(((tmp_str.chars)[tmp_str.length-2]!='\\')&&(str_len(tmp_str)))
						{	
							str_cat(tmp_str, "\\");		
						}							
					str_cat(tmp_str, all_wilds[t_cnt]);
					
					while((sub=txt_for_dir(sub_txt,tmp_str))>=sub_txt.strings)
						{	
							for(i=0; i<sub; i++)	
								{	
									txt_addstring(sub_txt, "");	
								}	
						}
					for(i=0; i<sub_txt.strings; i++)	
						{	
							if(str_len((sub_txt.pstring)[i])) txt_addstring(tmp_txt,(sub_txt.pstring)[i]);
							str_cpy((sub_txt.pstring)[i], "");													
						}	
				}	
		}
	//--------------------------------------------------------------------------------------
	//REMOVE files from temporary TEXT for each wildcard that DOES start with a '-' sign
	for(t_cnt=0; t_cnt<wilds; t_cnt++)
		{	
			if(str_to_asc(all_wilds[t_cnt])=='-')
				{	
					str_cpy(tmp_str, pathname);	
					if(((tmp_str.chars)[tmp_str.length-2]!='\\')&&(str_len(tmp_str)))
						{	
							str_cat(tmp_str, "\\");		
						}	
					str_cat(tmp_str, str_clip(all_wilds[t_cnt],1));
					
					while((sub=txt_for_dir(sub_txt,tmp_str))>=sub_txt.strings)
						{	
							for(i=0; i<sub; i++)	
								{	
									txt_addstring(sub_txt, "");	
								}	
						}
					for(i=0; i<sub_txt.strings; i++)	
						{	
							if(str_len((sub_txt.pstring)[i]))
								for(ii=0; ii<tmp_txt.strings; ii++)
									if(str_cmp((tmp_txt.pstring)[ii],(sub_txt.pstring)[i])==1)
										{	
											str_cpy((tmp_txt.pstring)[ii], "");									
										}
							str_cpy((sub_txt.pstring)[i], "");													
						}	
				}	
		}
	//--------------------------------------------------------------------------------------
	//TRANSFER from temporary TEXT to target TEXT, after creating target if it is needed
	targ_txt=*targ;	
//	if(!targ_txt)	
//		{	
//			*targ = targ_txt = txt_create(textlength,0);		// original was (0,0)
//			reset(targ_txt, SHOW);	
////			targ_txt.strings=0;									// can't be changed runtime
//		}
	f_count=0;	
	for(i=0; i<tmp_txt.strings; i++)
		{
			// check length
			if (i >=(var)textlength )
				break;
			
			// copy strings from start string
			if(str_len((tmp_txt.pstring)[i]))
				{
					str_cpy( (targ_txt.pstring)[i] , (tmp_txt.pstring)[i] );
					f_count++;	
				}			
			
			// append new string to text end						// original
//			if(str_len((tmp_txt.pstring)[i]))
//				{					
//					txt_addstring(targ_txt, (tmp_txt.pstring)[i]);	
//					f_count++;	
//				}	
		}
	//--------------------------------------------------------------------------------------
	//Cleanup Workspace
	for(t_cnt=0; t_cnt<wilds; t_cnt++)	
		{ 
			str_remove(all_wilds[t_cnt]); 
		}
	sys_free(all_wilds);		
	str_remove(tmp_str);
	if(tmp_txt)	
		{	
			for(i=0; i<tmp_txt.strings; i++)	
				if((tmp_txt.pstring)[i]) 
					str_remove((tmp_txt.pstring)[i]);	
			txt_remove(tmp_txt);	
		}
	if(sub_txt)	
		{	
			for(i=0; i<sub_txt.strings; i++)	
				if((sub_txt.pstring)[i]) 
					str_remove((sub_txt.pstring)[i]);	
			txt_remove(sub_txt);	
		}
	//--------------------------------------------------------------------------------------
sys_marker(NULL);	
	return(f_count);																								
}
//=========================================================================================

#endif