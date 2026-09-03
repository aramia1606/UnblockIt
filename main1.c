#include <stdio.h>
#include "console.h"


typedef struct{
	int x;
	int y;
} Vect;

typedef struct{
	Vect pos_origine;
	unsigned int taille;
	int dir;
	int couleur;
	int is_selectionne;
} Voiture;

typedef struct{
	Voiture * tab[6][6];
	Voiture list_voitures[18];
	unsigned int nb_voiture;
} Plateau;

/* FONCTIONS GESTION DES DONNEES (declaration)*/
Plateau generer_plateau_vide(void);
void refresh_tab(Plateau* pl);
int deplacement(char commande, Voiture* voiture_actu, Plateau* pl);
void affectation_niveau(Plateau* pl,  int niveau);

/*FONCTIONS AFFICHAGES (declaration)*/
void print_plateau(Plateau* pl);
void print_all(char * nom_joueur,int * nb_deplacement, int * help, Plateau * pl, int* is_error);

int main(void){
	Plateau pl1 = generer_plateau_vide();
	int i_voiture_select = 0, quitte = 0;
	char entree;
	int is_error = 0, is_gagne = 0, nb_deplacement = 0, niveau_actu = 5, H = 0;
	int i = 0;
	char nom_joueur[8];
	
	affectation_niveau(&pl1, niveau_actu);
	refresh_tab(&pl1);
	efface_ecran();
	printf("Entrer est votre nom : ");
	scanf("%s", nom_joueur);
	
	do{
		print_all(nom_joueur, &nb_deplacement, &H, &pl1, &is_error);
		entree = recupcar();

		if (!deplacement(entree, &pl1.list_voitures[i_voiture_select], &pl1)){
			switch (entree){
				case 'Q':
				case 'q':
					efface_ecran(); 
					quitte = 1;
					break;
				case 'W':
				case 'w':
					i_voiture_select = (i_voiture_select+1) % (pl1.nb_voiture);
					for(i = 0; i < (int)pl1.nb_voiture; i++){
						pl1.list_voitures[i].is_selectionne = (i == i_voiture_select);
					}
					break;
				case 'R':
				case 'r':
					nb_deplacement = 0;
					affectation_niveau(&pl1, niveau_actu);
					refresh_tab(&pl1);
					break;
				case 'H':
				case 'h':
					H = !H;
					break;
				default:
					is_error += 1;
					break;
			}
		} else {
			is_error = 0;
			nb_deplacement++;
		}

		if(pl1.tab[2][4] == &pl1.list_voitures[0] && pl1.tab[2][5] == &pl1.list_voitures[0]){
			is_gagne = 1;
		}
	} while(!is_gagne && !quitte);
	
	if(is_gagne){
		efface_ecran(); 
		printf("Gagne !\n");
	} else {
		efface_ecran();
		printf("Partie arretee.\n");
	}
	return 0;
}

/*FONCTION GESTION DES DONNEES */
Plateau generer_plateau_vide(void){
	Plateau res = { {{NULL,NULL,NULL,NULL,NULL,NULL}, 
					{NULL,NULL,NULL,NULL,NULL,NULL}, 
					{NULL,NULL,NULL,NULL,NULL,NULL}, 
					{NULL,NULL,NULL,NULL,NULL,NULL}, 
					{NULL,NULL,NULL,NULL,NULL,NULL}, 
					{NULL,NULL,NULL,NULL,NULL,NULL}}, {{0}}, 0};
	return res;
}

void refresh_tab(Plateau* pl){
	int i = 0;
	int j = 0;
	Voiture* voiture_actu;
	Vect pos;

	/*vider le tableau*/
	for(i = 0; i < 6; i++){
		for(j = 0; j < 6; j++){
			pl->tab[i][j] = NULL;
		}
	}

	for(i = 0; i < (int)pl->nb_voiture; i++){
		voiture_actu = &pl->list_voitures[i];
		pos.x = (*voiture_actu).pos_origine.x;
		pos.y = (*voiture_actu).pos_origine.y;
		pl->tab[pos.y][pos.x] = voiture_actu;
		for(j = 0; j < (int)((*voiture_actu).taille-1); j++){ 
			if(!(*voiture_actu).dir){
				pl->tab[pos.y][++pos.x] = voiture_actu;
			} else {
				pl->tab[++pos.y][pos.x] = voiture_actu;
			}
		}
	}
}

int deplacement(char commande, Voiture* voiture_actu, Plateau* pl){
	switch (commande){
		case 'l':
			if(!voiture_actu->dir &&
			   voiture_actu->pos_origine.x + (int)voiture_actu->taille < 6 &&
			   pl->tab[voiture_actu->pos_origine.y]
					  [voiture_actu->pos_origine.x + voiture_actu->taille] == NULL){
				voiture_actu->pos_origine.x++;
				refresh_tab(pl);
				return 1;
			}
			return 0;
			
		case 'h':
			if(!voiture_actu->dir &&
			   voiture_actu->pos_origine.x > 0 &&
			   pl->tab[voiture_actu->pos_origine.y]
					  [voiture_actu->pos_origine.x - 1] == NULL){
				voiture_actu->pos_origine.x--;
				refresh_tab(pl);
				return 1;
			}
			return 0;
			
		case 'k':
			if(voiture_actu->dir &&
			   voiture_actu->pos_origine.y > 0 &&
			   pl->tab[voiture_actu->pos_origine.y - 1]
					  [voiture_actu->pos_origine.x] == NULL){
				voiture_actu->pos_origine.y--;
				refresh_tab(pl);
				return 1;
			}
			return 0;
			
		case 'j':
			if(voiture_actu->dir &&
			   voiture_actu->pos_origine.y + (int)voiture_actu->taille < 6 &&
			   pl->tab[voiture_actu->pos_origine.y + voiture_actu->taille]
					  [voiture_actu->pos_origine.x] == NULL){
				voiture_actu->pos_origine.y++;
				refresh_tab(pl);
				return 1;
			}
			return 0;
			
		default:
			return 0;
	}
}

void affectation_niveau(Plateau* pl,  int niveau){
	static const int niveau1[] = {3,2,2,0,RED,  2,0,3,1,YELLOW,   5,1,3,1,MAGENTA,  0,3,3,0,BLUE,   3,3,2,1,GREEN,    4,4,2,0,CYAN };
	static const int niveau2[] = {0,2,2,0,RED,  0,0,2,1,BRIGHTGREEN, 1,0,2,1,CYAN,   2,0,2,0,BLUE,   2,1,2,0,MAGENTA,  4,0,2,1,BRIGHTMAGENTA,   2,2,2,1,GREEN,  3,2,3,1,YELLOW,  4,3,2,1,BRIGHTBLUE,  2,4,2,1,BRIGHTBLACK };
	static const int niveau3[] = {0,2,2,0,RED,  2,1,2,1, BRIGHTGREEN,   4,1,3,1,YELLOW, 5,2,2,1,BRIGHTWHITE,  0,3,3,0,MAGENTA,  0,4,2,1,BRIGHTBLUE, 1,4,2,1,BRIGHTMAGENTA,   2,4,2,1,BLUE,  4,4,2,0,GREEN, 4,5,2,0,BRIGHTBLACK };
	static const int niveau4[] = {1,2,2,0,RED,   0,0,2,0,BRIGHTGREEN,   2,0,2,1,BRIGHTCYAN, 3,0,3,1,YELLOW, 4,0,2,1, MAGENTA,   0,1,2,1,CYAN, 5,2,3,1,BRIGHTBLUE,   1,4,2,1,BRIGHTMAGENTA,  2,4,2,0,BLUE, 3,5,3,0,BRIGHTGREEN};
	static const int niveau5[] = {2,2,2,0,RED,   0,0,2,0,BRIGHTGREEN,   2,0,2,1,BRIGHTCYAN,  3,1,3,0,YELLOW, 0,1,2,1,CYAN, 4,2,2,1,BRIGHTMAGENTA, 0,4,2,0,BLUE, 2,4,3,0,MAGENTA,   5,4,2,1,GREEN};

	const int* niveau_actu;
	int nb_voitures;
	int i, j;
	
	switch(niveau){
		case 1:
			niveau_actu = niveau1;
			nb_voitures = 6;
			break;
		case 2:
			niveau_actu = niveau2;
			nb_voitures = 10;
			break;
		case 3:
			niveau_actu = niveau3;
			nb_voitures = 10;
			break;
		case 4:
			niveau_actu = niveau4;
			nb_voitures = 10;
			break;
		case 5:
			niveau_actu = niveau5;
			nb_voitures = 9;
			break;
		default:
			niveau_actu = niveau1;
			nb_voitures = 6;
			break;
	}
	
	for(i = 0; i < nb_voitures; i++){
		j = i * 5;
		pl->list_voitures[i].pos_origine.x = niveau_actu[j];
		pl->list_voitures[i].pos_origine.y = niveau_actu[j+1];
		pl->list_voitures[i].taille = (unsigned int)niveau_actu[j+2];
		pl->list_voitures[i].dir = niveau_actu[j+3];
		pl->list_voitures[i].couleur = niveau_actu[j+4];
		pl->list_voitures[i].is_selectionne = (i == 0);
	}
	pl->nb_voiture = (unsigned int)nb_voitures;
}

/*FONCTION AFFICHAGES*/
void print_plateau(Plateau* pl){
	int i = 0;
	int j = 0;
	int couleur_actu = 0;
	char bordure[50] = "\t+-------------+ \n";
	Voiture * voiture_actu = NULL;

	printf("%s", bordure);
	for(i = 0; i < 6; i++){
		for(j = 0; j < 6; j++){
			if(j == 0){ printf("\t| "); }

			if(pl->tab[i][j] == NULL){
				printf(". ");
			} 
			else {
				voiture_actu = pl->tab[i][j];
				couleur_actu = (*voiture_actu).couleur;
				if(voiture_actu->is_selectionne)
					couleurpolice(WHITE);
				else 
					couleurpolice(couleur_actu);
				couleurfond(couleur_actu);
				printf("##");
				couleurfond(BLACK);
				couleurpolice(WHITE);
			}

			if(j == 5){
				if(i != 2) 
					printf("|\n");
				else 
					printf(">\n");
			}
		}
	}
	printf("%s", bordure);
}

void print_all(char * nom_joueur, int * nb_deplacement, int * help, Plateau * pl, int* is_error){
	refresh_tab(pl);
	efface_ecran();
	printf("%s\n Nombre de deplacement: %3d\n\n", nom_joueur, *nb_deplacement);
	print_plateau(pl);
	if(*is_error > 5) *help = 1;
	if(*help)
		printf("\n\nQu'est ce que c'est ? C'est un puzzle ! Tu coulisse la piece avec pour objectif de faire sortir la piece rouge par la sorti >.\n Comment jouer ? Apres avoir lance le jeu, il est possible de coulisser la piece selectonee. De gauche a droite pour les blocks horizontaux et de haut en bas pour ceux verticaux.\n Liste des touches :\n\t h: gauche\t l: droite\tk: haut\t j: bas\n\t q: quitter la partie \tw: selectionner une autre voiture\tr: recommencer un niveau\n\t h:supprimer ou faire reapparaitre ce message.");
}