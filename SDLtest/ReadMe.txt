Concevoir une scene à l'aide des fichiers de config :
1. Choisir les shaders à charger dans le fichier Shaders
2. Ajouter tous les différents objets présent dans la scène dans le fichier MeshLoader.
3. Placer ces objets à l'aide du fichier Map.txt ( correspondance faite sur les labelz )


Détail :

	MeshLoader :
		LABEL	|	label du TYPE de l'objet
		FILE	|	fichier .obj de la mesh
		SHADER	|	shader à appliquer sur la mesh
		TEXTURE	|	texture à appliquer sur la mesh
		END		|	ajoute l'objet à la scène

	Map	:
		LABEL	|	label du TYPE d'objet à placer
		SCALE	|	échelle de l'objet
		COOR	|	coordonnées de l'objet	
		END		|	ajout de l'entitée à la scène
	Shaders:
		LABEL	|	shader name (vert + frag auto)