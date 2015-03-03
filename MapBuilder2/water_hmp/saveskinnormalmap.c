#include <acknex.h>
#include <default.c>

function main()
{
	level_load(NULL);	
	wait(1);
	
	ENTITY* ent = ent_create("water5.hmp",nullvector,NULL);	
	wait(3);
	
	// get all skins
	int i;																			
	for(i=0;i<ent_skins(ent);i++)
		{
			STRING* temp_str = str_create( str_for_entfile(NULL, ent) );
			str_trunc(temp_str,4);
			str_cat(temp_str,"_skin");
			str_cat(temp_str,str_for_int(NULL,i+1));
			str_cat(temp_str,"_na.tga");								// bmp or dds or tga or png or jpg
			
			bmap_to_normals(bmap_for_entity(ent,i+1), 1);
			
			bmap_save(ent_getskin(ent,i+1),temp_str);
		}
	
	printf("Done.");
	sys_exit("");
}
