#include <acknex.h>
#include <default.c>

function main()
{
	level_load(NULL);	
	wait(1);
	
	ENTITY* ent = ent_create("sc_001s_withmat.hmp",nullvector,NULL);	
	wait(3);
	
	// get all skins
	int i;																			
	for(i=0;i<ent_skins(ent);i++)
		{
			STRING* temp_str = str_create( str_for_entfile(NULL, ent) );
			str_trunc(temp_str,4);
			str_cat(temp_str,"_skin");
			str_cat(temp_str,str_for_int(NULL,i+1));
			str_cat(temp_str,".png");												// bmp or dds or tga or png or jpg			
				
			BMAP* temp_bmap = ent_getskin(ent, i+1);
			
			if (temp_bmap)																// empty skins now result in NULL bitmap (corretcly), so protetcion is needed
				{
					bmap_save(temp_bmap, temp_str);								// ent_getskin or bmap_for_entity				
				}
			
			temp_bmap = NULL;
	   	bmap_remove(temp_bmap);
		}
	
	printf("Done.");
	sys_exit("");
}
