#include <stdio.h>
#include <time.h> 
#include "console.h"

#define MAX_NOM  8
#define MAX_RECORDS_SAUV 5
#define MAX_DEPLACEMENT 1000
#define MAX_VOITURES 18
#define NB_MAX_NIVEAU 5
#define APPUIE_H 3

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
	Voiture list_voitures[MAX_VOITURES];
	unsigned int nb_voiture;
} Plateau;


int mod(int a, int n);
/* FONCTIONS GESTION DES DONNEES (declaration)*/
Plateau generer_plateau_vide(void);
void refresh_tab(Plateau* pl);
int deplacement(char commande, Voiture* voiture_actu, Plateau* pl);
void affectation_niveau(Plateau* pl,  int* niveau);
int enregistrer_niveau(char* nom_joueur, int* niveau_actu, int* nb_deplacement, char* deplacements);
int selectionner_sauvegarde(char* nom_joueur, int* niveau_actu, int* nb_deplacement, char* deplacements);
void enregistrer_records(char* nom_joueur, int* niveau_actu, int* nb_deplacement);
int charger_records(char list_noms[][MAX_NOM], int* list_niveaux, int* list_nb_deplacement);

/*FONCTIONS AFFICHAGES (declaration)*/
void print_plateau(Plateau* pl);
void print_records(char list_noms[][MAX_NOM], int * list_niveaux, int * list_nb_deplacement, int imax);
void print_game(char * nom_joueur, int * nb_deplacement, int * nb_touches, int * help, Plateau * pl, int* is_error, int* niveau, int afficher_tableau_score);


int main(void){
	Plateau pl1 = generer_plateau_vide();
	int i_voiture_select = 0, quitte = 0;
	char entree;
	char list_deplacement[MAX_DEPLACEMENT] = "";
	int is_error = 0, is_gagne = 0,affiche_score = 0, nb_deplacement = 0, nb_car = 0, nb_touches = 0, niveau_actu = 1, H = 0, nb_w = 0;
	int i = 0, i2 = 0;
	char nom_joueur[MAX_NOM];

	affectation_niveau(&pl1, &niveau_actu);
	refresh_tab(&pl1);
	efface_ecran();
	printf("Voulez-vous charger une partie enregistree ? O/N\n");
	entree = recupcar();
	switch (entree){ /*Il faut créer une erreur lorsqu'il n'y a pas de niveau enregistré*/
		case 'O':
		case 'o':
			if(selectionner_sauvegarde(nom_joueur, &niveau_actu, &nb_car, list_deplacement)) {
				printf("Le niveau a ete charge.\n");
				affectation_niveau(&pl1, &niveau_actu);
				refresh_tab(&pl1);


				for(i=0; i<nb_car; i++){
					if(list_deplacement[i] == 'w' || list_deplacement[i] == 'W'){
						i_voiture_select = (i_voiture_select+1) % (pl1.nb_voiture);
						for(i2 = 0; i2 < (int)pl1.nb_voiture; i2++){
							pl1.list_voitures[i2].is_selectionne = (i2 == i_voiture_select);
						}
					}
					else if(deplacement(list_deplacement[i], &pl1.list_voitures[i_voiture_select], &pl1)){
						nb_deplacement++;
					}
				}
				break;
			}
			else {
				printf("Une erreur s'est produite.\n");
				printf("Vous reprendrez a partir du niveau 1.\n");
				printf("Entrez votre nom : ");
				scanf("%s", nom_joueur);
			}
			break;
		default:
			printf("Vous reprendrez a partir du niveau 1.\n");
			printf("Entrez votre nom : ");
			scanf("%s", nom_joueur);
			break;
	}


	do{
		print_game(nom_joueur, &nb_deplacement, &nb_touches, &H, &pl1, &is_error, &niveau_actu, affiche_score);
		entree = recupcar();
		nb_touches++;

		if(entree=='p' || entree =='P' ){
			affiche_score = !affiche_score;
			continue;
		}
		else if (!deplacement(entree, &pl1.list_voitures[i_voiture_select], &pl1)){
			switch (entree){
				case 'Q':
				case 'q':
					efface_ecran();
					quitte = 1;
					break;
				case 'W':
					i_voiture_select = mod((i_voiture_select-1) , (pl1.nb_voiture));
					for(i = 0; i < (int)pl1.nb_voiture; i++){
						pl1.list_voitures[i].is_selectionne = (i == i_voiture_select);
					}
					nb_w =mod( (nb_w - 1), (int)pl1.nb_voiture);
					break;
				case 'w':
					i_voiture_select = mod((i_voiture_select+1) , (pl1.nb_voiture));
					for(i = 0; i < (int)pl1.nb_voiture; i++){
						pl1.list_voitures[i].is_selectionne = (i == i_voiture_select);
					}
					nb_w = mod((nb_w + 1),(int)pl1.nb_voiture);
					break;
				case 'R':
				case 'r':
					nb_deplacement = 0;
					nb_touches = 0;
					nb_car = 0;
					affectation_niveau(&pl1, &niveau_actu);
					refresh_tab(&pl1);
					list_deplacement[0] = '\0';
					break;
				case 'H':
					H = (H+1)%4;
					break;
				default:
					is_error += 1;
					break;
			}
		} else {
			is_error = 0;
			H = 0;
			for(i = 0; i<nb_w; i++){
				list_deplacement[nb_car] = 'w';
				nb_car ++;
			}
			nb_w = 0;
			list_deplacement[nb_car] = entree;
			list_deplacement[nb_car+1] = '\0';
			nb_car++;
			nb_deplacement++;
		}

		if(pl1.tab[2][4] == &pl1.list_voitures[0] && pl1.tab[2][5] == &pl1.list_voitures[0]){
			is_gagne = 1;
			efface_ecran();
			printf("Vous avez gagne ! \nVoulez-vous replay votre partie ? O/N\nAppuyez sur n'importe quelle autre touche pour passer au niveau suivant.\n");
			entree = recupcar();
			for(i2 = 0; i2 < (int)pl1.nb_voiture; i2++){
				pl1.list_voitures[i2].is_selectionne = 0;
			}
			efface_ecran();
			switch(entree){
				case 'O':
				case 'o':
					affectation_niveau(&pl1, &niveau_actu); 
					echooff();
					printf("=============== REPLAY ===============\n");
					print_plateau(&pl1);
					for(i = 0; i<nb_deplacement; i++){
						if(list_deplacement[i] == 'w' || list_deplacement[i] == 'W'){
							i_voiture_select = (i_voiture_select+1) % (pl1.nb_voiture);
						}
						else{
							deplacement(list_deplacement[i], &pl1.list_voitures[i_voiture_select], &pl1);
							refresh_tab(&pl1);
						}
						efface_ecran();
						printf("=============== REPLAY ===============\n");
						print_plateau(&pl1);
						delai(1);
						efface_ecran();
					}
					echoon();
					break;
				case 'N':
				case 'n':
				default :
					efface_ecran();
					printf("Niveau suivant.");
					delai(2);
					efface_ecran();
			}
			/*Actualisation des paramètres de jeu*/
			niveau_actu = (niveau_actu) % NB_MAX_NIVEAU + 1;
			enregistrer_records(nom_joueur, &niveau_actu, &nb_deplacement);
			/*Réinitialisation des paramètres de jeu (sauf nom de joueur)*/
			nb_deplacement = 0;
			nb_touches = 0;
			nb_car = 0;
			is_gagne = 0;
			affectation_niveau(&pl1, &niveau_actu);
			refresh_tab(&pl1);
			list_deplacement[0] = '\0';
		}
	} while(!quitte);

	efface_ecran();
	printf("Voulez-vous enregistrer votre partie ?\n");
	entree = recupcar();
	switch (entree){
		case 'o':
		case 'O':
			if(enregistrer_niveau(nom_joueur, &niveau_actu, &nb_car, list_deplacement)) {
				printf("Votre partie a ete enregistree avec succes.\nAppuyez sur n'importe quelle touche pour quitter.\n");
				recupcar();
			}
			else{
				printf("Une erreur s'est produite.\n");
			}
			break;

		default:
			printf("Vous avez quitte la partie.\n");
			break;
	}

	return 0;
}

/** Fontion mathematique modulo 'a mod n' 
* Renvoi le reste d'une division euclidienne
* @param a nombre entier a diviser (dividende)
* @param n nombre entier avec lequel diviser (diviseur)
* @return r nombre entier reste de la division a par n
*/
int mod(int a, int n)
{
    int r = a % n;
    if (r < 0)
        r += n;
    return r;
}
void delai(int nbsecondes) { 
  /* Calcul: chaque système définit le nombre de périodes
   * d'horloge par seconde
   * La fonction clock() renvoie le temps en nombre de périodes
   * d'horloge (ou clock ticks)
   */
  int pause = nbsecondes*CLOCKS_PER_SEC;

  /* Mémorisation de l'heure de départ */
  clock_t start_time = clock(); 

  /* On boucle tant que la temporisation n'est pas atteinte */
  while (clock() < start_time + pause); 
}

/*FONCTION GESTION DES DONNEES */
/** Charge la configuration d'un plateau vide à la déclaration pour qu'on puisse ensuite en modifier les valeurs selon les besoins
* @return res plateau vide
*/
Plateau generer_plateau_vide(void){
	Plateau res = { {{NULL,NULL,NULL,NULL,NULL,NULL},
					{NULL,NULL,NULL,NULL,NULL,NULL},
					{NULL,NULL,NULL,NULL,NULL,NULL},
					{NULL,NULL,NULL,NULL,NULL,NULL},
					{NULL,NULL,NULL,NULL,NULL,NULL},
					{NULL,NULL,NULL,NULL,NULL,NULL}}, {{0}}, 0};
	return res;
}

/** Permet de mettre à jour le tableau du plateau lorsqu'une voiture est ajouté ou a été modifié
* Elle doit donc être appelé arès chaque changement de position d'une voiture
* @param pl Parameter pointeur sur le plateau à mettre à jour
*/
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


/** Permet une tentative de déplacer une voiture appartenant au plateau selon la commande
* Si le déplacement est valide, il met à jour la position d'origine de la voiture et rafraichit le plateau
* Une voiture ne peut bouger que dans une direction (Une voiture vertical ne peut bouger que vers le haut(k) et le bas(j)
* et une voiture horizontale ne peut bouger que de gauche(h) à droite(l))
* @param commande pour diriger la voiture h(gauche),l(droite),k(haut),j(bas)
* @param voiture_actu pointeur sur la voiture à déplacer
* @param pl pointeur sur le plateau de jeux actuel
* @return 1 si le déplacement est valide, 0 sinon
*/
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

/** Modifie un plateau pour y mettre un niveau au choix parmis ceux disponibles (1 à 5)
* Lorsqu'il y a erreur dans le niveau, celui qui se lancera par défaut sera le niveau 0
* @param pl pointeur sur le plateau de jeu
* @param niveau pointeur sur le niveau choisi
*/
void affectation_niveau(Plateau* pl, int* niveau){
	static const int niveau1[30] = {3,2,2,0,RED,  2,0,3,1,YELLOW,   5,1,3,1,MAGENTA,  0,3,3,0,BLUE,   3,3,2,1,GREEN,    4,4,2,0,CYAN };
	static const int niveau2[50] = {0,2,2,0,RED,  0,0,2,1,BRIGHTGREEN, 1,0,2,1,CYAN,   2,0,2,0,BLUE,   2,1,2,0,MAGENTA,  4,0,2,1,BRIGHTMAGENTA,   2,2,2,1,GREEN,  3,2,3,1,YELLOW,  4,3,2,1,BRIGHTBLUE,  2,4,2,1,BRIGHTBLACK };
	static const int niveau3[50] = {0,2,2,0,RED,  2,1,2,1, BRIGHTGREEN,   4,1,3,1,YELLOW, 5,2,2,1,BRIGHTWHITE,  0,3,3,0,MAGENTA,  0,4,2,1,BRIGHTBLUE, 1,4,2,1,BRIGHTMAGENTA,   2,4,2,1,BLUE,  4,4,2,0,GREEN, 4,5,2,0,BRIGHTBLACK };
	static const int niveau4[50] = {1,2,2,0,RED,   0,0,2,0,BRIGHTGREEN,   2,0,2,1,BRIGHTCYAN, 3,0,3,1,YELLOW, 4,0,2,1, MAGENTA,   0,1,2,1,CYAN, 5,2,3,1,BRIGHTBLUE,   1,4,2,1,BRIGHTMAGENTA,  2,4,2,0,BLUE, 3,5,3,0,BRIGHTGREEN};
	static const int niveau5[45] = {2,2,2,0,RED,   0,0,2,0,BRIGHTGREEN,   2,0,2,1,BRIGHTCYAN,  3,1,3,0,YELLOW, 0,1,2,1,CYAN, 4,2,2,1,BRIGHTMAGENTA, 0,4,2,0,BLUE, 2,4,3,0,MAGENTA,   5,4,2,1,GREEN};
	static const int* list_niveaux[] = {niveau1, niveau2, niveau3, niveau4, niveau5};
	static const int nb_voiture_par_niveau[] = {6, 10, 10, 10, 9};

	const int* niveau_actu;
	int nb_voitures, i, j, index_niveau;

	if(*niveau >= 1 && *niveau <= 5)
		index_niveau = (*niveau - 1);
	else
		index_niveau = 0;

	niveau_actu = list_niveaux[index_niveau];
	nb_voitures = nb_voiture_par_niveau[index_niveau];

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

/** Enregistre dans un fichier registre_niveau.txt le actions du joueur ainsi que les informations le concernant pour pouvoir regénerer sa la partie inachevé plus tard (il est supposé que le fichier existe déjà)
* Si le fichier n'existe pas, il sera créé automatiquement
* @param nom_joueur pointeur sur le nom du joueur en cours de jeu
* @param niveau_actu pointeur sur le niveau actuellement joué par le joueur
* @param deplacement pointeur sur le charactère de déplacement (h,j,k,l,w)
* @return 1 si le niveau à été correctement enregistré, 0 sinon
*/
int enregistrer_niveau(char* nom_joueur, int* niveau_actu, int* nb_deplacement, char* deplacements){
	FILE* fpdest;
	int i;

	fpdest = fopen("registre_niveau.txt", "w");
	if (fpdest == NULL) {
		printf("Erreur d'ouverture du fichier\n");
		return 0;
	}

	fprintf(fpdest, "%s %d %d \n", nom_joueur, *niveau_actu, *nb_deplacement);
	for(i = 0; i < *nb_deplacement; i++) {
		fprintf(fpdest, "%c", deplacements[i]);
	}
	fprintf(fpdest, "\n");
	fclose(fpdest);
	return 1;
}


/** Liste toutes les sauvegardes disponibles dans le fichier registre_niveau.txt,
 * permet à l'utilisateur de choisir laquelle charger, puis charge les données
 * dans les paramètres fournis.
 *
 * @param nom_joueur Pointeur pour stocker le nom du joueur chargé
 * @param niveau_actu Pointeur vers le niveau à charger
 * @param nb_deplacement Pointeur vers le nombre de déplacements
 * @param deplacements Pointeur pour stocker la séquence de déplacements
 * @return 1 si une sauvegarde a été chargee avec succès, 0 sinon
 */
int selectionner_sauvegarde(char* nom_joueur, int* niveau_actu, int* nb_deplacement, char* deplacements){
	FILE *fp;
	char noms_sauvegardes[MAX_RECORDS_SAUV][MAX_NOM];
	int niveaux_sauvegardes[MAX_RECORDS_SAUV], nb_depl_sauvegardes[MAX_RECORDS_SAUV];
	int nb_sauvegardes, choix, i, j;
	char c;

	fp = fopen("registre_niveau.txt", "r");
	if (fp == NULL) {
		printf("Aucune sauvegarde trouvee.\n");
		printf("Appuyez sur une touche pour continuer...\n");
		recupcar();
		return 0;
	}

	nb_sauvegardes = 0;
	while(fscanf(fp, "%s %d %d ", noms_sauvegardes[nb_sauvegardes],
	             &niveaux_sauvegardes[nb_sauvegardes],
	             &nb_depl_sauvegardes[nb_sauvegardes]) == 3
	      && nb_sauvegardes < MAX_RECORDS_SAUV) {
		while(fgetc(fp) != '\n' && !feof(fp));
		nb_sauvegardes++;
	}
	fclose(fp);

	if(nb_sauvegardes == 0) {
		printf("Aucune sauvegarde trouvee.\n");
		printf("Appuyez sur une touche pour continuer...\n");
		recupcar();
		return 0;
	}

	efface_ecran();
	printf("=== SAUVEGARDES DISPONIBLES ===\n\n");
	for(i = 0; i < nb_sauvegardes; i++) {
		printf("%d. %8s - Niveau %d (%d deplacements)\n",
		       i+1, noms_sauvegardes[i], niveaux_sauvegardes[i], nb_depl_sauvegardes[i]);
	}
	printf("\n0. Annuler\n");

	printf("\nChoix du niveau a jouer (0-%d) : ", nb_sauvegardes);
	scanf("%d", &choix);

	if(choix < 1 || choix > nb_sauvegardes) {
		printf("Chargement de sauvegarde annulee.\n");
		return 0;
	}

	/* Debut de chargement de la sauvegarde sélectionnee */
	fp = fopen("registre_niveau.txt", "r");
	if (fp == NULL) {
		printf("Erreur de lecture du fichier.\n");
		return 0;
	}
	for(i = 0; i < choix; i++) {
		if(fscanf(fp, "%s %d %d ", nom_joueur, niveau_actu, nb_deplacement) != 3) {
			printf("Erreur lors du chargement.\n");
			fclose(fp);
			return 0;
		}
		if(i < choix - 1) {
			while(fgetc(fp) != '\n' && !feof(fp));
		}
	}

	for(j = 0; j < *nb_deplacement; j++) {
		if(fscanf(fp, "%c", &c) == 1) {
			deplacements[j] = c;
		}
	}
	deplacements[*nb_deplacement] = '\0';

	fclose(fp);
	return 1;
}



/** Enregistre dans un fichier meilleur_score.txt le meilleure score pour pouvoir regénerer sa la partie inachevé plus tard (il est supposé que le fichier existe déjà)
* Si le fichier n'existe pas, il sera créé automatiquement
* @param nom_joueur pointeur sur le nom du joueur en cours de jeu
* @param niveau_actu pointeur sur le niveau actuellement joué par le joueur
* @param deplacement pointeur sur le charactère de déplacement (h,j,k,l,w)
* @return 1 si le niveau à été correctement enregistré, 0 sinon
*/
void enregistrer_records(char* nom_joueur, int* niveau_actu, int* nb_deplacement){
	FILE* fpdest;

	fpdest = fopen("meilleur_score.txt", "w");
	if (fpdest == NULL) {
		printf("Erreur d'ouverture du fichier\n");
		return;
	}

	fprintf(fpdest, "%s %d %d \n", nom_joueur, *niveau_actu, *nb_deplacement);
	fprintf(fpdest, "\n");
	fclose(fpdest);
}

/** Rempli plusieurs tableaux des éléments contenu dans le fichier des records
* @param list_nom adresse d'un tablaau de noms (8 lettres max)
* @param list_niveau adresse d'un tableau de niveaux
* @param list_deplacement adresse d'un tableau
* @return nombre de records trouvés
*/
int charger_records(char list_noms[][MAX_NOM], int* list_niveaux, int* list_nb_deplacement){
	FILE *fp;
	int i=0;

	fp = fopen("meilleur_score.txt", "r");
	if (fp == NULL) {
		printf("Erreur de lecture fichier\n");
		return 0;
	}

	while (fscanf(fp, "%s %d %d \n", list_noms[i], &list_niveaux[i], &list_nb_deplacement[i]) == 3) {
		i++;
	}
	fclose(fp); /*Si on n'est pas rentré une seul fois dans la boucle donc qu'il y a eu une erreur, on renvoie 0*/
	return i;
}

/*FONCTION AFFICHAGES =========================================================================================*/
/** Permet d'afficher le plateau dans la sortie standard
* @param pl pointeur sur le plateau de jeu en cours
 */
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
				if(voiture_actu->is_selectionne){
					couleurpolice(WHITE);
				}
				else{
					couleurpolice(couleur_actu);
				}
				couleurfond(couleur_actu);
				printf("##");
				couleurfond(BLACK);
				couleurpolice(WHITE);
			}

			if(j == 5){
				if(i != 2)
					printf("|\n");
				else{
					mode_clignotant();
					printf(">\n");
					desactive_mode_clignotant();
				}
			}
		}
	}
	printf("%s", bordure);
}

void print_records(char list_noms[][MAX_NOM], int * list_niveaux, int * list_nb_deplacement, int imax){
	efface_ecran();
	printf("=============== RECORDS ===============\n");
	
	int i = 0;
	printf("Nom      |Niveau |Nombre de deplacements\n");
	for(i=0; i<imax; i++){
		printf("%-8s |%-2d     |%d \n", list_noms[i], list_niveaux[i], list_nb_deplacement[i]);
	}
}

/** Imprime toute l'interface de jeux principale
* @param nom_joueur pointeur sur le nom du joueur
* @param nb_deplacement pointeur sur le nombre de déplacement actuel fait par le joueur
* @param help pointeur sur un nombre permettant de d'afficher la présentation du jeu les instructions lorsqu'il à 3 ou plus
* @param is_error pointeur sur le nombre d'erreurs faites par le joueur, il affiche la présentation et les regles du jeu lorsque 6 commandes ou plus sont erronees
*/
void print_game(char * nom_joueur, int * nb_deplacement, int * nb_touches, int * help, Plateau * pl, int* is_error, int* niveau, int afficher_tableau_score){
	char noms[10][MAX_NOM];
	int niveaux[NB_MAX_NIVEAU], deplacements[MAX_DEPLACEMENT], imax;
	if(! afficher_tableau_score){
		refresh_tab(pl);
		efface_ecran();
		printf("%s - Niveau %d\nNombre de deplacements: %3d \n\n", nom_joueur, *niveau, *nb_deplacement);
		print_plateau(pl);
		if(*help > APPUIE_H-1 || *is_error > 5) { /*Nombre de fois ou il faut appuyer sur H pour afficher l'aide*/
			printf("Qu'est ce que c'est ? C'est un puzzle ! Tu coulisse la piece avec pour objectif de faire sortir la piece rouge par la sorti >.\nComment jouer ? Apres avoir lance le jeu, il est possible de coulisser la piece selectonee. De gauche a droite pour les blocks horizontaux et de haut en bas pour ceux verticaux.\nListe des touches :\n\th: gauche\t l: droite\tk: haut\tj: bas\n\tq: quitter la partie \n\tw: selectionner la voiture suivante \tW: selection de la voiture précedente\n\tr: recommencer un niveau\n\tH:supprimer ou faire reapparaitre ce message.\n");
		}
		else {
			printf("\n\n\n\n\n\n\n\n");
		}
	}
	else{
		imax = charger_records(noms, niveaux, deplacements);
		print_records(noms, niveaux, deplacements, imax);
	}
}