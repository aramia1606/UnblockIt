#include <stdio.h>
#include <time.h> 
#include <string.h>
#include "console.h"

#define MAX_NOM  8
#define MAX_RECORDS_SAUV 5
#define MAX_DEPLACEMENT 1000
#define MAX_VOITURES 18
#define NB_MAX_NIVEAU 5
#define APPUIE_H 3

#define H_max_console 30
#define L_max_console 80

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


/* Déclarations des fonctions */
void vider_ecran(void);
void print_banniere(void);
void print_footer(int choix);
void print_bordure_h(void);
void print_bordure_ext(void);
void effacer_centre(void);
void interface(Plateau *pl, char *nom_joueur, int *nb_deplacement, int *niveau, int choix);
void print_plateau(Plateau* pl, Vect pos_origine, int taille);
void print_jeu(Plateau * pl);
Plateau generer_plateau_vide(void);
void affectation_niveau(Plateau* pl, int* niveau);
void placer_voitures_sur_plateau(Plateau* pl);

int main(void){
    Plateau pl;
    int niveau = 1;
    
    pl = generer_plateau_vide();
    affectation_niveau(&pl, &niveau);
    placer_voitures_sur_plateau(&pl);
    
    print_jeu(&pl);
    
    return 0;
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

/** Place les voitures sur le plateau selon leur position et direction
* @param pl pointeur sur le plateau de jeu
*/
void placer_voitures_sur_plateau(Plateau* pl){
    unsigned int i;
    int k;
    Voiture* v;
    
    for(i = 0; i < pl->nb_voiture; i++){
        v = &(pl->list_voitures[i]);
        
        if(v->dir == 0){ /* Horizontal */
            for(k = 0; k < (int)v->taille; k++){
                pl->tab[v->pos_origine.y][v->pos_origine.x + k] = v;
            }
        }
        else{ /* Vertical */
            for(k = 0; k < (int)v->taille; k++){
                pl->tab[v->pos_origine.y + k][v->pos_origine.x] = v;
            }
        }
    }
}

void vider_ecran(void){
    efface_ecran();
    allerxy(0,0);
}

void print_banniere(void){
    allerxy(1,2);
    printf(
"|      _  _  __ _  ____  __     __    ___  __ _    _  _   __    __  ____       |\n"
"|     / )( \\(  ( \\(  _ \\(  )   /  \\  / __)(  / )  / )( \\ /  \\  /  \\(    \\      |\n"
"|     ) \\/ (/    / ) _ (/ (_/\\(  O )( (__  )  (   \\ /\\ /(  O )(  O )) D (      |\n"
"   .  \\____/\\_)__)(____/\\____/ \\__/  \\___)(__\\_)  (_/\\_) \\__/  \\__/(____/      |\n"
" \\ | /\n"
"-.;;;.-'\n"
"=;;;;;==-\n"
"-';;;'-.\n"
" / | \\ \n"
    );
}

void print_footer(int choix){
    static const char* choix0[] = {
        "                                                         _\\||       \" ' \"     ",
        "               _(_)_                          wWWWw   _  --\\      \" \\ | / \"   ",
        "   @@@@       (_)@(_)   vVVVv     _     @@@@  (___) _(_)_   \\    ' --(:)-- '  ",
        "  @@()@@ wWWWw  (_)\\    (___)   _(_)_  @@()@@   Y  (_)@(_)        \" / | \\ \"   ",
        "   @@@@  (___)     `|/    Y    (_)@(_)  @@@@   \\|/   (_)\\          \" '|' \"    ",
        "    /      Y       \\|    \\|/    /(_)    \\|      |/      |       |\\    |    /| ",
        " \\ |     \\ |/       | / \\ | /  \\|/       |/    \\|      \\|/   |  /_ \\  |  / _\\ ",
        " \\\\|//   \\\\|///  \\\\\\|//\\\\\\|/// \\|///  \\\\\\|//  \\\\|//  \\\\\\|////\\ \\\\ /_ \\|/_\\  \\ "
    };
    int i;
    
    switch(choix){
        case 0:
            for(i = 0; i < 8; i++){
                allerxy(2, 22 + i);
                if(i >= 5) couleurpolice(GREEN);
                printf("%s\n", choix0[i]);
            }
            resetcouleurs();
            break;
        case 1: 
            allerxy(1,23);
            printf(
                ".------------------------------------------------------------------------------.\n"
                "|Qu'est ce que c'est ? C'est un puzzle ! Tu coulisse la piece avec pour        |\n"
                "|objectif de faire sortir la piece rouge par la sorti >.                       |\n"
                "|Comment jouer ?                                                               |\n"
                "|Apres avoir lance le jeu, il est possible de coulisser la piece selectonee.   |\n"
                "|De gauche a droite pour les blocks horizontaux et de haut en bas pour ceux    |\n"
                "|verticaux.                                                                    |\n"
            );
            break;
    }
}

void print_bordure_h(void){
    vider_ecran();
    policegras();
    couleurpolice(YELLOW);
    print_banniere();
    resetcouleurs();
    policenormale();
}

void print_bordure_ext(void){
    int i;
    allerxy(1,1);
    for(i = 1; i <= 160; i++){
        printf("=");
        if(i == 80) allerxy(1, H_max_console);
    }
    allerxy(1,2);
    for(i = 2; i <= 29; i++){
        allerxy(L_max_console, i);
        printf("|");
        allerxy(1, i);
        if(i >= 6 && i <= 10) continue;
        else printf("|");
    }
    allerxy(1,31);
}

void effacer_centre(void){
    static const int limite_haute = 6;
    static const int limite_basse = 21;
    static const int limite_gauche[] = {10,10,10,10,9,2,2,2,2,2,2,2,2,2,2,2,2};
    static const int limite_droite = L_max_console - 1;
    int ix, iy;
    
    for(iy = limite_haute; iy <= limite_basse; iy++){
        allerxy(limite_gauche[iy - limite_haute], iy);
        for(ix = limite_gauche[iy - limite_haute]; ix <= limite_droite; ix++){
            printf(" ");
        }
    }
}

void interface(Plateau *pl, char *nom_joueur, int *nb_deplacement, int *niveau, int choix){
    static const char *interface0[] = {  
        "                                                                  _\\|| ",
        "                  +------------------+ .------------------------  --\\ ",
        "                  |                  | |         ",
        "                     |                  | |         Niveau %2d         |  ",
        "                     |                  | |Nombre de deplacement : %3d| ",
        "    //\\         /\\\\       |                  | '---------------------------'",
        "   || * \\ . . / * ||      |                  |                             ",
        "    \\\\____\\X/____//       |                  |     \\\\|//              (\\o/)",
        "     / *  /O\\  * \\        |                  |       |                (/|\\)",
        "     \\__/  '  \\__/        |                  |       :                     ",
        "                          |                  |                 ||/_        ",
        "                          |                  |                  /--        ",
        "                          |                  |                 /           ",
        "                 (\\o/)    |                  |                             ",
        "                 (/|\\)    +------------------+                             ",
        ""
    };
    
    static const char *interface1[] = {
        "                                         _\\||                        ",
        "              .------------------------  --\\                         ",
        "              |         ",
        " ______|______   |         Niveau %2d         |                  (/|\\)   ",
        "|             |  |Nombre de deplacement : %3d|",
        "     |             |  '---------------------------'   \\\\|//                  ",
        "     |             |  .-------------------------------  |  ---------.        ",
        "     |             |  |Liste des touches :              :           |        ",
        "     |             |  |h: gauche  l: droite                         |        ",
        "     |             |  |k: haut    j: bas                            |        ",
        "     |             |  |q: quitter la partie                         |        ",
        "     |_____________|  |w: selectionner la voiture suivante          |        ",
        "           /|\\        |W: selection de la voiture précedente        |        ",
        "          / | \\       |r: recommencer un niveau                     |        ",
        "         /  |  \\      |H:supprimer ou faire reapparaitre ce message.|        ",
        "                      '---------------------------------------------'        "
    };
    
    const char **lignes;
    int y = 6;
    int i = 0;
    int size = 0;
    Vect origine;


    switch(choix){
        default:
        case 0:
            lignes = interface0;
            origine.x = 29;
            origine.y = 8;
            size = 0;
            break;
        case 1:
            lignes = interface1;
            origine.x = 8;
            origine.y = 11;
            size = 1;
            break;
    }
    
    /* Lignes 7-8 */
    for (i = 0; i < 2; i++) {
        allerxy(10, y++);
        printf("%s\n", lignes[i]);
    }
    
    /* Ligne 9 nom du joueur */
    allerxy(10, y++);
    printf("%s", lignes[2]);
    printf("%-8s", nom_joueur);
    switch(choix){
        default:
        case 0:
            printf("            \\ \n");
            break;
        case 1: 
            printf("            \\                (\\o/)   \n");
            break;
        
    }
    
    /* Ligne 10 niveau */
    allerxy(7, y++);
    printf(lignes[3], *niveau);
    printf("\n");
    
    /* Ligne 11 nb deplacement */
    allerxy(7, y++);
    printf(lignes[4], *nb_deplacement);
    printf("\n");
    
    /* Lignes restantes */
    for (i = 5; i < 16; i++) {
        allerxy(2, y++);
        printf("%s\n", lignes[i]);
    }
    
    /* Affichage du plateau */
    print_plateau(pl, origine, size);
}

void dialogue(char* titre){
        static const char *boite[] = {  
         " +----------------------------------------------------+              ",
         " |                                                    |     (\\o/)    ",
         " |                                                    |     (/|\\)    ",
      "    |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          |                                                    |              ",
"          +----------------------------------------------------+              ",
    };
    int i;
    int y;
    
    i = 0;
    y = 7;
    /*lignes 7 a 9*/
    for (i = 0; i < 3; i++) {
        allerxy(10, y++);
        printf("%s\n", lignes[i]);
    }
    
    /*ligne 10 niveau */
    allerxy(7, y++);
    printf(lignes[3], *niveau);
    printf("\n");
    
    /*ligne 11 nb deplacement */
    allerxy(7, y++);
    printf(lignes[4], *nb_deplacement);
    printf("\n");
    
    /*ligness restantes */
    for (i = 5; i < 15; i++) {
        allerxy(2, y++);
        printf("%s\n", lignes[i]);
    }
    allerxy(35,7);
    printf(" %s ", *titre);

}


void print_plateau(Plateau* pl, Vect pos_origine, int taille){
    int i, j, ligne;
    int couleur_actu;
    Voiture * voiture_actu;
    
    if(taille == 1){
        /*cases 2x1 */
        for(i = 0; i < 6; i++){
            for(j = 0; j < 6; j++){
                allerxy(pos_origine.x + (j * 2), pos_origine.y + i);
                
                if(pl->tab[i][j] == NULL){
                    printf(". ");
                }
                else {
                    voiture_actu = pl->tab[i][j];
                    couleur_actu = voiture_actu->couleur;
                    
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

                if(i == 2 && j == 5){
                    mode_clignotant();
                    printf(">");
                    desactive_mode_clignotant();
                }
            }
            printf("\n");
        }
    }
    else{
        /*cases 3x2 */
        for(i = 0; i < 6; i++){
            for(ligne = 0; ligne < 2; ligne++){
                for(j = 0; j < 6; j++){
                    allerxy(pos_origine.x + (j * 3), pos_origine.y + (i * 2) + ligne);
                    
                    if(pl->tab[i][j] == NULL){
                        printf(" . ");
                    }
                    else {
                        voiture_actu = pl->tab[i][j];
                        couleur_actu = voiture_actu->couleur;
                        
                        if(voiture_actu->is_selectionne){
                            couleurpolice(WHITE);
                        }
                        else{
                            couleurpolice(couleur_actu);
                        }
                        couleurfond(couleur_actu);
                        printf("###");  
                        couleurfond(BLACK);
                        couleurpolice(WHITE);
                    }

                    if(i == 2 && j == 5 && ligne == 0){
                        mode_clignotant();
                        printf(">");
                        desactive_mode_clignotant();
                    }
                }
                printf("\n");
            }
        }
    }
}

void print_jeu(Plateau * pl){
    char *nom_joueur = "Andam";
    int nb_deplacement = 23;
    int niveau = 3;
    titre("Unblock Wood");
    print_bordure_h();
    print_bordure_ext();

    interface(pl, nom_joueur, &nb_deplacement, &niveau, 0);
    
    print_footer(0);
    allerxy(1,31);
}