Concevoir une scene � l'aide des fichiers de config :
1. Choisir les shaders � charger dans le fichier Shaders
2. Ajouter tous les diff�rents objets pr�sent dans la sc�ne dans le fichier MeshLoader.
3. Placer ces objets � l'aide du fichier Map.txt ( correspondance faite sur les labelz )


D�tail des options:

#--------------- MESHLOADER -------------------------------#
		LABEL	|	label du TYPE de l'objet
		FILE	|	fichier .obj de la mesh
		SHADER	|	shader � appliquer sur la mesh
		TEXTURE	|	texture � appliquer sur la mesh
		END		|	ajoute l'objet � la sc�ne


#--------------- MAP -------------------------------#
		LABEL	|	label du TYPE d'objet � placer
		SCALE	|	�chelle de l'objet
		COOR	|	coordonn�es de l'objet	
		PLACE	LEFT	ObjectNAME		(x+)
				RIGHT	ObjectNAME		(x-)
				ABOVE	ObjectNAME		(z+)
				UNDER	ObjectNAME		(z-)
				BEHIND	ObjectNAME		(y-)
				BEFORE	ObjectNAME		(y+)
		OFFSET	|	Ajout aux coordonn�es apr�s les transformations
		ROT_A	|	Angle de rotation
		ROT_V	|	Axe de rotation 
		END		|	ajout de l'entit�e � la sc�ne


		EXEMPLE : 

			LABEL	elbox
			NAME	elbox_A
			SCALE	1 1 1
			COOR	0 0 0
			PLACE	ABOVE	road_E
			OFFSET	2 4 0
			ROT_A	90
			ROT_V	0 0 1
			END

#--------------- SHADERS -------------------------------#
		LABEL	|	shader name (vert + frag auto)
