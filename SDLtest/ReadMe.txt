Concevoir une scene à l'aide des fichiers de config :
1. Choisir les shaders à charger dans le fichier Shaders
2. Ajouter tous les différents objets présent dans la scène dans le fichier MeshLoader.
3. Placer ces objets à l'aide du fichier Map.txt ( correspondance faite sur les labelz )


Détail des options:

#--------------- MESHLOADER -------------------------------#
		LABEL	|	label du TYPE de l'objet
		FILE	|	fichier .obj de la mesh
		SHADER	|	shader à appliquer sur la mesh
		TEXTURE	|	texture à appliquer sur la mesh
		END		|	ajoute l'objet à la scène


#--------------- MAP -------------------------------#
		LABEL	|	label du TYPE d'objet à placer
		SCALE	|	échelle de l'objet
		COOR	|	coordonnées de l'objet	
		PLACE	LEFT	ObjectNAME		(x+)
				RIGHT	ObjectNAME		(x-)
				ABOVE	ObjectNAME		(z+)
				UNDER	ObjectNAME		(z-)
				BEHIND	ObjectNAME		(y-)
				BEFORE	ObjectNAME		(y+)
		OFFSET	|	Ajout aux coordonnées après les transformations
		ROT_A	|	Angle de rotation
		ROT_V	|	Axe de rotation 
		END		|	ajout de l'entitée à la scène


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
