#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "balle/balle.h"
#include "joueur/joueur.h"
#include "joueur/barre.h"
#include "game/game.h"
#include "textures/textures.h"
#include "game/masterLauncher.h"


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

static float ABCISSE_REPERE_MAX_GAME = 200;
static float ORDONNE_REPERE_MAX_GAME = 175;

static float ABCISSE_REPERE_MAX_GLOBAL = 200;
static float ORDONNE_REPERE_MAX_GLOBAL = 200;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape() {
	float ratio = (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(ratio*-ABCISSE_REPERE_MAX_GLOBAL, ratio*ABCISSE_REPERE_MAX_GLOBAL, -ORDONNE_REPERE_MAX_GLOBAL, ORDONNE_REPERE_MAX_GLOBAL);
}

void setVideoMode(SDL_Surface ** ecran) {
	if( (*ecran = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) == NULL) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		exit(EXIT_FAILURE);
	}
	reshape();

	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}


int main(int argc, char * argv []){  	
	SDL_putenv ("SDL_VIDEO_CENTERED=center");

	if(SDL_Init(SDL_INIT_VIDEO |SDL_INIT_AUDIO)==-1) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	SDL_Surface * ecran = NULL;
 	setVideoMode(&ecran);
  	SDL_WM_SetCaption("Arkano-Pong", NULL);

	int game = 1;
	int exit_arkana=0;
	int playmusic=1;
/*
	if(Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
  	 	printf("INIT OGG FAIL : %s", Mix_GetError());
  	 else
  	 	printf("INIT OGG ok\n");
  */
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
  	 	printf("%s", Mix_GetError());
  	}
	do{
	  	char theme[100],level[100],mode_jeu[100];
		Item_menu menu[6];
		Textures textures_menu;
		genereMenu(menu,&textures_menu,ABCISSE_REPERE_MAX_GLOBAL,ORDONNE_REPERE_MAX_GLOBAL);

	
		Mix_VolumeMusic(20);
	  	Mix_Music *musique_menu = Mix_LoadMUS("../son/menu.ogg");
	  	if(!musique_menu) {
		    printf("Mix_LoadMUS(\"Gramatik_No_Way_Out.ogg\"): %s\n", Mix_GetError());
		    // this might be a critical error...
		}
		Mix_FadeInMusic(musique_menu,-1,2000);
		playmusic=1;

		/********* MENU ********/
	
	  	int loop=1,indice_fleches_verticales=0,indice_fleches_horizontales=0;
	  	exit_arkana=0;
	  	while(loop){
			glClear(GL_COLOR_BUFFER_BIT);
			drawItem(&(menu[WALLPAPER]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);

			drawItem(&(menu[START_GAME]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			drawItem(&(menu[MODE_JEU]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			drawItem(&(menu[THEME]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			drawItem(&(menu[LEVEL]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			drawItem(&(menu[QUIT]),ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			SDL_GL_SwapBuffers();

			SDL_Event e;
		    while(SDL_PollEvent(&e)) {
		    	if(e.type == SDL_QUIT) {
		    		loop = 0;
		    		exit_arkana=1;
		        	break;
		      	}

		      	switch(e.type) {
		        	case SDL_MOUSEBUTTONUP:
		          
		        	break;

		          	case SDL_VIDEORESIZE:
			        	WINDOW_WIDTH = e.resize.w;
			          	WINDOW_HEIGHT = e.resize.h;
			          	setVideoMode(&ecran);
			        break;

		        	case SDL_KEYDOWN:
		      	  		switch(e.key.keysym.sym){
		      	  			case 'm' :
		      	    			if(playmusic){
		      	    				playmusic=0;
		      	    				Mix_PauseMusic();
		      	    			}
		      	    			else{
		      	    				playmusic=1;
		      	    				Mix_ResumeMusic();
		      	    			}
		      	    		break;
		      	    		case SDLK_ESCAPE : 
		        		  		loop = 0;
		        		  		exit_arkana=1;
		        			break;

		            		case SDLK_UP:
		            			indice_fleches_verticales--;
		            			handleMenu(&indice_fleches_verticales,&indice_fleches_horizontales,	menu,5); // on ne compte pas le dernier(wallpaper)
		            		break;

		            		case SDLK_DOWN:
		            			indice_fleches_verticales++;
		            			handleMenu(&indice_fleches_verticales,&indice_fleches_horizontales, menu,5); // on ne compte pas le dernier(wallpaper)

		           			break;

		           			case SDLK_RIGHT:
		           				indice_fleches_horizontales++;
		           				handleItem(&indice_fleches_horizontales,indice_fleches_verticales,&(menu[indice_fleches_verticales]),menu);
		           			break;

		           			case SDLK_LEFT:
		           				indice_fleches_horizontales--;
		           				handleItem(&indice_fleches_horizontales,indice_fleches_verticales,&(menu[indice_fleches_verticales]),menu);
		           			break;
		      	    		
		      	    		case SDLK_RETURN :
		      	    			if(indice_fleches_verticales == START_GAME){
		      	    				strcpy(theme,menu[THEME].values[menu[THEME].indice_courant]);
		      	    				strcpy(level,menu[LEVEL].values[menu[LEVEL].indice_courant]);
		      	    				strcpy(mode_jeu,menu[MODE_JEU].values[menu[MODE_JEU].indice_courant]);
		      	    				loop=0;
		      	    			}
		      	    			else if(indice_fleches_verticales == QUIT){
		      	    				loop=0;
		      	    				exit_arkana=1;
		      	    				game=0;
		      	    			}
		      	    		break;
		      	    		default : break;
		      	  	}

		          	break;

		         	case SDL_KEYUP:
		          		switch(e.key.keysym.sym){
		          			case SDLK_UP:
		            		break;

				            case SDLK_DOWN:

				            break;

		            		default:break;
		         	} 
		        	default:break;

		      	}
	    	}
		}

		detruireTextures(&textures_menu);


		if(playmusic){
			Mix_FadeOutMusic(1000);
		}

		Mix_FreeMusic(musique_menu);
		if(!exit_arkana){

			Mix_Music *musique_game = NULL;

			if(!strcmp(theme,"mario"))
				musique_game = Mix_LoadMUS("../son/mario.ogg");
			else if(!strcmp(theme,"espace"))
				musique_game = Mix_LoadMUS("../son/espace.ogg");
			else
				musique_game = Mix_LoadMUS("../son/flat.ogg");

			if(!musique_game) {
			    printf("Mix_LoadMUS(\"game.ogg\"): %s\n", Mix_GetError());
			    // this might be a critical error...
			}

		  	Mix_VolumeMusic(50);
		  	Mix_FadeInMusic(musique_game,-1,4000);
		  	if(!playmusic)
		  		Mix_PauseMusic();

			/********* JEU ********/
			/**********************/

		  	/******** INIT SCREEN & JOUEUR *******/
		  	Textures textures_barres_coeur;
		  	loadTexturesBarresCoeur(&textures_barres_coeur,theme);

		  	float default_taille_barre_x = 80, default_taille_barre_y = 10, x_vitesse_barre_default = 5, norme_vitesse_balle_default = 3;
		  	if(!strcmp(theme,"mario"))
		  		default_taille_barre_y = 20;

		  	Barre barre_joueur1 = initBarre(0,0,default_taille_barre_x,default_taille_barre_y,x_vitesse_barre_default,100,187,205);
		  	barre_joueur1.texture = textures_barres_coeur.identifiants[TEXTURE_BARRE_BAS];
		  	Joueur j1 = initJoueur("Alexis","Alex",10,&barre_joueur1);
		  	Balle balle_joueur1 = initBalle(0,-ORDONNE_REPERE_MAX_GAME+30,0,0,5,200,187,205,&j1);

		  	Barre barre_joueur2 = initBarre(0,0,default_taille_barre_x,default_taille_barre_y,x_vitesse_barre_default,255,204,0);
		  	barre_joueur2.texture = textures_barres_coeur.identifiants[TEXTURE_BARRE_HAUT];
		  	Joueur j2 = initJoueur("Machine","Armand",10,&barre_joueur2);
		  	Balle balle_joueur2 = initBalle(0,ORDONNE_REPERE_MAX_GAME-30,0,0,5,255,204,0,&j2);


			/**********INIT GAME**************/
			Brique ** arrayBrique = NULL;
		  	GLuint texture_wallpaper;
		  	Textures textures_briques;
			int nbBriques,nbBalles = 2;
			Balle * balles[2]={&balle_joueur1,&balle_joueur2};


			loadGame(theme, &texture_wallpaper, level, &arrayBrique,balles, &nbBriques, ABCISSE_REPERE_MAX_GLOBAL, ORDONNE_REPERE_MAX_GLOBAL,&textures_briques);
			SDL_Delay(1000); // voir le fond de loading et pas un flash
			initScreenGame(&barre_joueur1, &barre_joueur2, &balle_joueur1, &balle_joueur2, ABCISSE_REPERE_MAX_GAME, ORDONNE_REPERE_MAX_GAME,theme);
	  	
			if(TTF_Init() == -1){
			    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
			    exit(EXIT_FAILURE);
			}

			TTF_Font *police_infos_joueur = NULL;
			police_infos_joueur= TTF_OpenFont("../font/DS-DIGI.ttf",20);

		
			loop = 1; 
			int partie_stopped=1, end=0, IA = !strcmp(mode_jeu,"solo")?1:0;
		
		  	while(loop){
		    	Uint32 startTime = SDL_GetTicks();
			    /* dessin */			    
			    glClear(GL_COLOR_BUFFER_BIT);
				

			    glMatrixMode(GL_MODELVIEW);
			    glLoadIdentity();


			    glColor3ub(255,255,255); // init
			    dessinWallpaper(texture_wallpaper,ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			    dessinInfosJoueur(2,&j2,police_infos_joueur,&textures_barres_coeur,0,204,255,-ABCISSE_REPERE_MAX_GLOBAL,ORDONNE_REPERE_MAX_GLOBAL,-1);
			    dessinInfosJoueur(1,&j1,police_infos_joueur,&textures_barres_coeur,205,187,200,ABCISSE_REPERE_MAX_GLOBAL,-ORDONNE_REPERE_MAX_GLOBAL,1);

			    dessinBalle(&balle_joueur1,ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			   	dessinBalle(&balle_joueur2,ABCISSE_REPERE_MAX_GAME,ORDONNE_REPERE_MAX_GAME);
			   	dessinBarre(&barre_joueur1);
			    dessinBarre(&barre_joueur2);
			    dessinBriques(arrayBrique,nbBriques);

			    if(end){
					if(j1.vie == 0)
						dessinMessageWin("Joueur 2 vainqueur !",&j2);
					else
						dessinMessageWin("Joueur 1 vainqueur !",&j1);
				}
			    SDL_GL_SwapBuffers(); 
			    /* fin dessin */

			    if(!partie_stopped){

				    //gestion touche enfoncée
				    Uint8 *keystates = SDL_GetKeyState(NULL);
				    //joueur 1
				    if(keystates[ SDLK_RIGHT])
				    	deplacerBarreX(&barre_joueur1,1,ABCISSE_REPERE_MAX_GAME);
				    else if(keystates[ SDLK_LEFT])
				    	deplacerBarreX(&barre_joueur1,-1,ABCISSE_REPERE_MAX_GAME);

				    //joueur 2
				    if(keystates[ SDLK_r])
				    	deplacerBarreX(&barre_joueur2,1,ABCISSE_REPERE_MAX_GAME);
				    else if(keystates[ SDLK_e])
				    	deplacerBarreX(&barre_joueur2,-1,ABCISSE_REPERE_MAX_GAME);

				    /************BARRES****************/
					handleBalleBarres(&balle_joueur1,&j1,&j2);
					handleBalleBarres(&balle_joueur2,&j1,&j2);

				    /***********BONUS******************/
				    updateBonusJoueur(&j1,default_taille_barre_x,default_taille_barre_y,x_vitesse_barre_default);
				    updateBonusJoueur(&j2,default_taille_barre_x,default_taille_barre_y,x_vitesse_barre_default);
				    updateBonusBalle(&balle_joueur1,norme_vitesse_balle_default);
				    updateBonusBalle(&balle_joueur2,norme_vitesse_balle_default);

				    /*************BRIQUES ***************/
					handleGrilleBrique(arrayBrique, nbBriques, nbBalles,textures_briques);



				    /*****  IA  ******/
				    if(IA)
				   		handleBarreIAJ2( &balle_joueur1, &balle_joueur2, &j1, &j2, ORDONNE_REPERE_MAX_GAME);
				   
				    /****** GESTION PERTE ******/
					int perte_potentiel_balle1 = handleBalleBorder(&balle_joueur1, ABCISSE_REPERE_MAX_GAME, ORDONNE_REPERE_MAX_GAME);
					if(perte_potentiel_balle1 == -1){
						j1.vie -=1;
						setBalleJ1ToScreen(&balle_joueur1,&barre_joueur1);
						initVitesseBalles(&balle_joueur1,NULL);

						//partie_stopped = 1;
					}
					else if(perte_potentiel_balle1 == -2){
						setBalleJ2ToScreen(&balle_joueur2,&barre_joueur2);
						setBalleJ1ToScreen(&balle_joueur1,&barre_joueur1);
						initVitesseBalles(&balle_joueur1,NULL);

						j2.vie -=1;
						//partie_stopped = 1;
					}
					
					int perte_potentiel_balle2 = handleBalleBorder(&balle_joueur2, ABCISSE_REPERE_MAX_GAME, ORDONNE_REPERE_MAX_GAME);
					if(perte_potentiel_balle2 == -1){
						j1.vie -=1;
						setBalleJ1ToScreen(&balle_joueur1,&barre_joueur1);
						setBalleJ2ToScreen(&balle_joueur2,&barre_joueur2);
						initVitesseBalles(NULL,&balle_joueur2);
						//partie_stopped = 1;
					}
					else if(perte_potentiel_balle2 == -2){
						setBalleJ2ToScreen(&balle_joueur2,&barre_joueur2);
						initVitesseBalles(NULL,&balle_joueur2);

						j2.vie -=1;
						//partie_stopped = 1;
					}
					
					if(j1.vie == 0 || j2.vie==0){
						end = 1;
						stopGame(&barre_joueur1, &barre_joueur2, &balle_joueur1, &balle_joueur2);
					}
				}


			    SDL_Event e;
			    while(SDL_PollEvent(&e)) {
			    	if(e.type == SDL_QUIT) {
			    		loop = 0;
			    		game=0;
			        	break;
			      	}
			      
			      	switch(e.type) {
			        	case SDL_MOUSEBUTTONUP:
			          
			        	break;
			          
				        case SDL_VIDEORESIZE:
				          WINDOW_WIDTH = e.resize.w;
				          WINDOW_HEIGHT = e.resize.h;
				          setVideoMode(&ecran);
				        break;

			        	case SDL_KEYDOWN:
			      	  		switch(e.key.keysym.sym){
			      	    		case 'q' : 
			      	    		case SDLK_ESCAPE : 
			        		  		loop = 0;
			        			break;

			            		case SDLK_UP:
			            		break;
			            		case 'm' :
			      	    			if(playmusic){
			      	    				playmusic=0;
			      	    				Mix_PauseMusic();
			      	    			}
			      	    			else{
			      	    				playmusic=1;
			      	    				Mix_ResumeMusic();
			      	    			}
			      	    		break; 
			            		case SDLK_DOWN:
			           			break;
			      	    		
			      	    		case SDLK_RETURN :
			      	    			if(!end){
			      	    				initScreenGame(&barre_joueur1, &barre_joueur2, &balle_joueur1, &balle_joueur2, ABCISSE_REPERE_MAX_GAME, ORDONNE_REPERE_MAX_GAME,theme);
										//vitesse default balles
										initVitesseBalles(&balle_joueur1,&balle_joueur2);
										initVitesseBarres(&barre_joueur1,&barre_joueur2);
					      	    		if(partie_stopped){
											initBonusJoueurs(&j1,&j2);
											iniBonusBalles(&balle_joueur1,&balle_joueur2);
											partie_stopped = 0;
					      	    		}
			      	    			}
			      	    			else{
			      	    				//retour menu
			      	    				loop=0;
			      	    			}
			      	    				
			      	    		break;
			      	    		default : break;
			      	  	}
			          	break;
			      	}
		    	}
		    
				Uint32 elapsedTime = SDL_GetTicks() - startTime;
			    if(elapsedTime < FRAMERATE_MILLISECONDS) {
			      	SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
			    }
			}
		
			detruireTextures(&textures_briques);
			detruireTextures(&textures_barres_coeur);
			detruireBriques(arrayBrique,nbBriques);
		 	glBindTexture(GL_TEXTURE_2D,0);
		    glDeleteTextures(1,&texture_wallpaper);
		    Mix_FreeMusic(musique_game);
			TTF_CloseFont(police_infos_joueur);
		}

	}while(game && !exit_arkana);

	TTF_Quit();
    Mix_CloseAudio();
    //Mix_Quit();
  	SDL_Quit(); 
	return EXIT_SUCCESS;
}