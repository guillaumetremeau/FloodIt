#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

#define COULEUR 6
#define TAILLE 12

/*#######################################################################*/

int my_rand(int rang);
void initialiser(int tab[][TAILLE]);
void afficherGrille(int tab[][TAILLE]);
void afficherIterations(int tab[][TAILLE]);
int fin(int tab[][TAILLE]);
void remplir(int tab[][TAILLE], int oldColor,int newColor,int i,int j,int iteration);

void afficheGraphisme();
void saisie();

/*#######################################################################*/

/* ------- Declaration de variables globales ---------------------------- */


Bool           sortie = False; /* Indicateur de fin de pgm             */

Window         root;           /* Fenetre racine (fond de l'ecran)     */
Window         win;            /* Fenetre de travail                   */
Window         subwin;         /* Sous fenetre                         */
Display       *dpy;            /* Serveur de fenetrage de ce pgm       */

int            ecran;          /* Declaration de l'ecran par defaut    */

unsigned long  wpx,            /* Variables temporaires pour la        */
bpx;            /* Couleur whithe et black (fg, bg)     */

XEvent         ev;             /* Declare une variable evenement       */
unsigned long  mask;           /* Declare un mask d'evenement          */

GC             gcontext;         
XGCValues      gcv;               
unsigned long  gcmask;             

Cursor         curseur;        /* Definition d'un curseur souris       */
Visual        *visual;

int            pol;
int            count;
char **        list;

Colormap       colormap;
XColor         color1, color2;
int            bleu;



/*###############################################################*/




int main(int argc, char** argv){
	/* ######### moteur xlib ########### */
	
	dpy = XOpenDisplay(0);        /* Connexion au serveur, il faudrait    */
																/* tester le resultat */

	ecran    = DefaultScreen(dpy);
	root     = DefaultRootWindow(dpy);

	/* ------- Recup. couleur de l'avant plan et arriere plan ------------ */

	bpx      = BlackPixel(dpy,ecran);
	wpx      = WhitePixel(dpy,ecran);

	visual   = DefaultVisual(dpy, ecran);

	/* ------- Creation de la fenetre: Epaiss 6 pixels, L 600 H 500 ------ */

	win = XCreateSimpleWindow(dpy,root,30,40,600,500,6,wpx,bpx);


	XStoreName(dpy,win,"ISIMA 1: Utilisation de la Xlib");

	/* ------- Definition des evenements qui les concernent --------------  */

	XSelectInput(dpy,win,ButtonPressMask |
											 ButtonReleaseMask |
											 Button1MotionMask |
											 KeyPressMask |
											 ExposureMask );

	/* ------------------------------------------------------------------- */

	XMapWindow(dpy,win);                      /* Affiche la fenetre        */


	/* ------- Obtention des couleurs ------------------------------------ */
	/* Le visual est suppose TrueColor */
	/* la palette est en lecture seule, d'ou le AllocNone */
	colormap = XCreateColormap(dpy, win, visual, AllocNone);

	printf("Color alloc: %d\n", XAllocNamedColor(dpy, colormap, "blue", &color1, &color2));
	/* proche/obtenue, exacte/demandee */
	bleu = color1.pixel;
	printf("Color alloc: %d\n", XAllocNamedColor(dpy, colormap, "red", &color1, &color2));
	/* proche/obtenue , exacte/demandee */
	/* on demande une couleur et on obtient deux resultats : la couleur de la palette la plus proche
	* de celle que l'on veut et celle que l'on aurait voulu*/

	/* -------Obtention des polices de taille 20------------------------ */
	list = XListFonts(dpy, "*-20-*", 1000, &count);
	printf("Fonts count: %d\n", count);

	/* ------- Creation des contextes standards -------------------------- */

	gcv.foreground = color1.pixel;                     /* Contexte standard*/
	gcv.background = bpx;
	gcv.line_width = 3;
	gcv.function   = GXcopy;
	gcmask         = GCForeground | GCBackground | GCLineWidth | GCFunction;
	gcontext       = XCreateGC(dpy,win,gcmask,&gcv);


	/* -------------------------------------------------------------------*/


	while(!sortie)
	{
		XNextEvent(dpy,&ev);
		switch(ev.type)
		{
			case Expose      : afficheGraphisme();
												 break;
			case KeyPress    : saisie();
												 break;
			case ButtonPress :
			default          : puts("Evenement Inconnu !");
												 break;
		}
	}

	XFreeFontNames(list);
	XFreeColormap(dpy, colormap);
	XDestroyWindow(dpy , win );
	XCloseDisplay(dpy);



	/* ######### moteur texte ##########
	int tab[TAILLE][TAILLE];
	int varUser = 0;
	int iteration = 0;
	initialiser(tab);
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
void afficherGrille(int tab[][TAILLE]){
	int i,j;
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			printf("%d ",(int) tab[i][j] / 100);
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


/* ---------------------------------------------------------------------- */
/* afficheGraphisme         Test de primitives graphiques                 */
/* ---------------------------------------------------------------------- */

void afficheGraphisme()
{                                     
	char          chaine[255] ="Texte sur ecran graphique";
	static int    numcur = 0;
	Font          police;

	/* effacer la fenetre */
	XClearWindow(dpy, win);

	/* dans la couleur d avant plan si pas redefini */
	XDrawLine(dpy,win,gcontext,10,10,200,200);
	/* changement de couleur */
	XSetForeground(dpy, gcontext, color1.pixel);
	/* XDrawRectangle(dpy,win,gcontext,50,80,200,100);
	* */
	XFillRectangle(dpy,win,gcontext,150,380,100,30);
	XDrawArc(dpy,win,gcontext,350,350,50,50,0,360*64);
	XFillArc(dpy,win,gcontext,200,250,80,80,30*64,330*64);  

	/* --------- Changement de couleur ----------------------------------*/
	XSetForeground(dpy, gcontext, bleu);

	/* --------- Modes: ArcPieSlice (defaut) ou ArcChord --------------- */

	XSetArcMode(dpy,gcontext,ArcChord);  
	XFillArc(dpy,win,gcontext,100,450,100,50,90*64,270*64);  

	/* --------- Affichage de Texte ------------------------------------ */

	XDrawString(dpy,win,gcontext,80,20,chaine,strlen(chaine));

	pol = rand() % count;
	printf("Choix: %s\n", list[pol]);

	police = XLoadFont(dpy, list[pol] );


	XSetFont(dpy,gcontext,police);

	strcpy(chaine,"Hajimemashite");
	XDrawString(dpy,win,gcontext,80,40,chaine,strlen(chaine));

	XUnloadFont(dpy,police); 


	/* --------- Changement du Curseur Souris -------------------------- */

	curseur = XCreateFontCursor(dpy,numcur % 100);  /* Init du curseur souris */
	XDefineCursor(dpy,win,curseur);
	if (numcur > 0) XFreeCursor(dpy,curseur);
	numcur++;


}

/* ---------------------------------------------------------------------- */
/* saisie                Saisie de touches                                */
/* ---------------------------------------------------------------------- */

void saisie()
{   
#define    FIN   0xff57

	char       chaine[2];
	KeySym     ks;
	int        nbc;    
	static int x = 0, y = 0;

	nbc = XLookupString(&ev.xkey,chaine, 2, &ks, NULL);
	printf("Nbc : %d Code touche : %x\n",nbc, (unsigned int)ks);
	if (ks == FIN) sortie = True;

	XDrawString(dpy,win,gcontext,x,y,chaine,nbc);
	y += 20;
}

/* ---------------------------------------------------------------------- */
