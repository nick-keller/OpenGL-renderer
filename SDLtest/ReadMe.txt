Concevoir une scene � l'aide des fichiers de config :
1. Choisir les shaders � charger dans le fichier Shaders
2. Ajouter tous les diff�rents objets pr�sent dans la sc�ne dans le fichier MeshLoader.
3. Placer ces objets � l'aide du fichier Map.txt ( correspondance faite sur les labelz )


D�tail :

	MeshLoader :
		LABEL	|	label du TYPE de l'objet
		FILE	|	fichier .obj de la mesh
		SHADER	|	shader � appliquer sur la mesh
		TEXTURE	|	texture � appliquer sur la mesh
		END		|	ajoute l'objet � la sc�ne

	Map	:
		LABEL	|	label du TYPE d'objet � placer
		SCALE	|	�chelle de l'objet
		COOR	|	coordonn�es de l'objet	
		END		|	ajout de l'entit�e � la sc�ne
	Shaders:
		LABEL	|	shader name (vert + frag auto)