#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define COULEUR 6
#define TAILLE 12

/*#######################################################################*/

int myrand(int rang);
void initialiser(int tab[][TAILLE]);
void afficherGrille(int tab[][TAILLE],SDL_Renderer *);
void afficherIterations(int tab[][TAILLE]);
int fin(int tab[][TAILLE]);
void remplir(int tab[][TAILLE], int oldColor,int newColor,int i,int j,int iteration);

void afficherEcran(SDL_Renderer *);


	int tab[TAILLE][TAILLE];
	int varUser = 0;
	int iteration = 0;


int main(){

	/* ############# Initialisation graphique  SDL ############# */

	int flags = IMG_INIT_JPG|IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	SDL_Window * window;
	int width = 480;
	int height = 480;
	int running = 1;
	SDL_Event event;
	SDL_Renderer *renderer;

	initialiser(tab);


	
	if((initted&flags) != flags)
	{
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() != 0)
	{
		fprintf(stderr, "Erreur d'initialisation TTF : %s/n", TTF_GetError());
	}


	window = SDL_CreateWindow("SDL FLoodIt 0.1", SDL_WINDOWPOS_CENTERED,
														SDL_WINDOWPOS_CENTERED, 
														width, height, 
														SDL_WINDOW_RESIZABLE);
	if (window == 0) 
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",
		SDL_GetError()); 
	}


	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED ); /*
	SDL_RENDERER_SOFTWARE */
	if (renderer == 0) {
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",
		SDL_GetError()); 
		/* faire ce qu'il faut pour quitter proprement */
	}

	/* #### Boucle d evenement ###*/

	while (running) {

		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_WINDOWEVENT:
					printf("window event\n");
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							printf("appui sur la croix\n");
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							width = event.window.data1;
							height = event.window.data2;
							printf("Size : %d%d\n", width,height);
						default:
							afficherEcran(renderer);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					printf("Appui :%d %d\n",event.button.x,event.button.y);
					break;
				case SDL_QUIT:
					printf("on quitte\n");
					running = 0;
			}
		}   
		SDL_Delay(1);
	}

	/* ######### moteur texte ########## */
	/* tab);
	while(!fin(tab) && (iteration <22) && (varUser != 999))
	{
		iteration++;
		system("clear");
		afficherGrille(tab);
		printf("\n\nchoix de couleur : ");
		scanf("%d",&varUser);
		remplir(tab,(tab[0][0] / 100),varUser,0,0,iteration);
	}
	printf("\n");
	afficherIterations(tab);

	printf("##########################\n");
	fin(tab)?printf("### GAGNE ###\n"):printf("### PERDU ###\n");
	printf("##########################\n");
*/
	/* rendu des ressources */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}



/*#######################################################################*/



void initialiser(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			tab[i][j] = myrand(COULEUR)*100;
		}
	}
}
void afficherGrille(int tab[][TAILLE], SDL_Renderer *renderer){
	int i,j;
	SDL_Rect rect;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			switch((int) tab[i][j] /100)
			{
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
					break;
				case 4:
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
					break;
				case 5:
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			}
			rect.x = 40*i;
			rect.y = 40*j;
			rect.w = 40;
			rect.h = 40;
			SDL_RenderFillRect(renderer,&rect);
		}
		printf("\n");
	}
}
void afficherIterations(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			printf("%d ",(int) tab[i][j] % 100);
		}
		printf("\n");
	}
}


int myrand(int rang){
	static int first = 0;

	if (first == 0)
	{
		srand (time (NULL));
		first = 1;
	}
	return (int)(rand() / (double)RAND_MAX * (rang));
}

int fin(int tab [][TAILLE])
{
	int i,j;
	int res = 1;
	int couleur = (tab[1][1] /100);
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			if ((tab[i][j]/100) != couleur) res = 0;
		}
	}
	return res;
}

void remplir(int tab[][TAILLE], int oldColor,int newColor,int i,int j,int iteration)
{
	if(i>=0 && j>=0 && i < TAILLE && j < TAILLE)
	{
		if(((int)tab[i][j] / 100) == oldColor){
			tab[i][j] = (newColor * 100) + (tab[i][j] % 100);
			if((tab[i][j] % 100) == 0) tab[i][j] += iteration;
			remplir(tab, oldColor, newColor, i-1,j,iteration);
			remplir(tab, oldColor, newColor, i+1,j,iteration);
			remplir(tab, oldColor, newColor, i,j-1,iteration);
			remplir(tab, oldColor, newColor, i,j+1,iteration);
		}
	}
}

void afficherEcran(SDL_Renderer *renderer){
	SDL_Rect rect;
	
	/* couleur de fond */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	/* dessiner en blanc */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	rect.x = rect.y = 0;
	rect.w = rect.h = 600;
	afficherGrille(tab, renderer);
	SDL_RenderFillRect(renderer, &rect );
	SDL_RenderPresent(renderer);
	
}
