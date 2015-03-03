//////////////////////////////////////////////////////////////////////
// terraintex.fx
// Terrain material for an unlimited number of terrain textures
// Tiled texture on RGB, mask on alpha 
// Now first texture is blended texture for water mirror shader

Texture entSkin2; // basic tiled terrain texture - increased by 1 !!! - modified for MapBuilder
Texture LightMap; // lightmap created by the map compiler

bool PASS_SOLID; // enforce rendering on the solid pass

//bool AUTORELOAD;

//////////////////////////////////////////////////////////////////////

// technique without lightmap
technique terraintex
{
   pass multi_repeat11
   {
      ZWriteEnable 		= True;
		AlphaBlendEnable 	= True;
		SrcBlend 			= SrcAlpha;
      DestBlend 			= InvSrcAlpha;
		
		Texture[0]			= <entSkin2>;
		TexCoordIndex[0] 	= 0;
		AlphaOp[0] 			= SelectArg1;
		AlphaArg1[0] 		= Texture;
		
		Texture[1] 			= <entSkin2>;
		TexCoordIndex[1] 	= 1;
      ColorArg1[1] 		= Texture; 
	   ColorArg2[1]		= Diffuse;
	   ColorOp[1] 			= Modulate2x;
	   AlphaArg1[1] 		= Current;
	   AlphaOp[1] 			= SelectArg1;

	   ColorOp[2] 			= Disable;
	   AlphaOp[2] 			= Disable;
	}
}

// technique with lightmap
technique terraintex_lm
{
	pass multi_repeat11
	{
      ZWriteEnable 		= True;
		AlphaBlendEnable 	= True;
		SrcBlend 			= SrcAlpha;
      DestBlend 			= InvSrcAlpha;
		
		Texture[0] 			= <entSkin2>;
		TexCoordIndex[0] 	= 0;
		AlphaOp[0] 			= SelectArg1;
		AlphaArg1[0] 		= Texture;
		
		Texture[1] 			= <LightMap>;
		TexCoordIndex[1] 	= 0;
      ColorArg1[1] 		= Texture; 
	   ColorArg2[1]		= Diffuse;
	   ColorOp[1] 			= AddSigned;
	   AlphaArg1[1]		= Current;
	   AlphaOp[1] 			= SelectArg1;

		Texture[2] 			= <entSkin2>;
		TexCoordIndex[2] 	= 1;
      ColorArg1[2] 		= Texture; 
	   ColorArg2[2]		= Current;
	   ColorOp[2] 			= Modulate2x;
	   AlphaArg1[2] 		= Current;
	   AlphaOp[2] 			= SelectArg1;

	   ColorOp[3] 			= Disable;
	   AlphaOp[3] 			= Disable;
	}
}

// fallback if nothing works
technique fallback 
{ 
	pass one 
		{ 
			
		} 
}
