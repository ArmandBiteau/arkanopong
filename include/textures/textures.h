#ifndef __TEXTURES_H__
#define __TEXTURES_H__ 
//briques
#define TEXTURE_CLASSIQUE_1 				0
#define TEXTURE_INDESTRUCTIBLE_1 			1
#define TEXTURE_AGRANDISSEMENT_BARRE_3 		2
#define TEXTURE_AGRANDISSEMENT_BARRE_2 		3
#define TEXTURE_AGRANDISSEMENT_BARRE_1 		4
#define TEXTURE_REDUCTION_BARRE_1 			5
#define TEXTURE_VITESSE_PLUS_BALLE_2 		6
#define TEXTURE_VITESSE_PLUS_BALLE_1 		7
#define TEXTURE_VITESSE_MOINS_BALLE_2 		8
#define TEXTURE_VITESSE_MOINS_BALLE_1 		9
#define TEXTURE_INVERSION_VITESSE_BARRE_1 	10
#define TEXTURE_VITESSE_BARRE_3 			11
#define TEXTURE_VITESSE_BARRE_2 			12
#define TEXTURE_VITESSE_BARRE_1 			13
#define NB_TEXTURES_BRIQUES 				14

//menu
#define TEXTURE_START_GAME_ON 			0
#define TEXTURE_START_GAME_OFF 			1
#define TEXTURE_MODE_JEU_SOLO_ON 		2
#define TEXTURE_MODE_JEU_SOLO_OFF 		3
#define TEXTURE_MODE_JEU_MULTI_ON 		4
#define TEXTURE_MODE_JEU_MULTI_OFF 		5
#define TEXTURE_THEME_MARIO_ON 			6
#define TEXTURE_THEME_MARIO_OFF 		7
#define TEXTURE_THEME_ESPACE_ON 		8
#define TEXTURE_THEME_ESPACE_OFF 		9
#define TEXTURE_THEME_FLAT_ON 			10
#define TEXTURE_THEME_FLAT_OFF 			11
#define TEXTURE_LEVEL_CLASSIQUE_ON 		12
#define TEXTURE_LEVEL_CLASSIQUE_OFF 	13
#define TEXTURE_LEVEL_FULL_BONUS_ON 	14
#define TEXTURE_LEVEL_FULL_BONUS_OFF 	15
#define TEXTURE_LEVEL_ARKANO_ON 		16
#define TEXTURE_LEVEL_ARKANO_OFF 		17
#define TEXTURE_LEVEL_NO_BONUS_ON 		18
#define TEXTURE_LEVEL_NO_BONUS_OFF 		19
#define TEXTURE_LEVEL_PONG_ON 			20
#define TEXTURE_LEVEL_PONG_OFF 			21
#define TEXTURE_QUIT_ON 				22
#define TEXTURE_QUIT_OFF				23
#define TEXTURE_WALLPAPER_MARIO			24
#define TEXTURE_WALLPAPER_ESPACE		25
#define TEXTURE_WALLPAPER_FLAT			26
#define NB_TEXTURES_MENU				27


//barre (&& coeur : eviter de recréer un tableau que pour une seule texture)
#define TEXTURE_BARRE_BAS 0
#define TEXTURE_BARRE_HAUT 1
#define TEXTURE_COEUR 2
#define NB_TEXTURES_BARRES_COEUR 3


#define NB_TEXTURES_MAX 27

#include <GL/gl.h>
#include <SDL/SDL.h>

typedef struct{
	GLuint identifiants[NB_TEXTURES_MAX];
	int nb_textures;
}Textures;

void loadTexturesBriques(Textures * textures, char * theme);
void loadTexturesMenu(Textures * textures);
void loadTexturesBarresCoeur(Textures * textures, char * menu);
void loadTexture(GLuint * id, char * chemin_fichier);
void detruireTextures(Textures * textures);
void dessinSurfaceInfos(SDL_Surface * surface, float xPos,float yPos);
void dessinTexture(GLuint texture,float xPos, float yPos, float xSize, float ySize);

#endif
