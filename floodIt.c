#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COULEUR 6
#define TAILLE 12

int my_rand(int);
void initialiser(int tab[][TAILLE]);
void afficherGrille(int tab[][TAILLE]);
void afficherIterations(int tab[][TAILLE]);
int fin(int tab[][TAILLE]);
void remplir(int tab[][TAILLE], int oldColor, newColor, i, j, iteration);

int main(){
	int tab[TAILLE][TAILLE];

	initialiser(tab);
	while(!fin(tab))
	{
		system(clear);
		afficherGrille(tab);
	}
	printf("\n");
	afficherIterations(tab);

	return 0;
}


void initialiser(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			tab[i][j] = myrand(COULEUR)*100;
		}
	}
}
void afficherGrille(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			printf("%d",(int) tab[i][j] / 100);
		}
		printf("\n");
	}
}
void afficherIterations(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			printf("%d",(int) tab[i][j] % 100);
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
	int couleur = tab[1][1];
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			if (tab[i][j] != couleur) res = 0;
		}
	}
	return res;
}

void remplir(int tab[][TAILLE], int oldColor, newColor, i, j, iteration)
{
	if(i>=0 && j>=0 && i < TAILLE && j < TAILLE)
	{
		if(((int)tab[i][j] / 100) == oldColor){
			tab[i][j] = newColor + (tab[i][j] % 100);
			remplir(tab, oldColor, newColor, i-1,j,iteration);
			remplir(tab, oldColor, newColor, i+1,j,iteration);
			remplir(tab, oldColor, newColor, i,j-1,iteration);
			remplir(tab, oldColor, newColor, i,j+1,iteration);
		}
	}
}
