technique standTree
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= True;		   
		}
}

technique fallback 
{ 
	pass 
		{ 
			
		} 
}