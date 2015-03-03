technique standTree
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;		   
		}
}

technique fallback 
{ 
	pass 
		{ 
			
		} 
}