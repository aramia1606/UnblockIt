/*
 * console.h
 * PoPS ESR v1.7
 *
 */

#ifndef __CONSOLE_H
#define __CONSOLE_H

#define BLACK   30
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36
#define WHITE   37

/* Couleurs supplémentaires
 * ce sont les mêmes qui sont utilisées
 * pour le mode "bold" de la police de caractères
 */
#define BRIGHTBLACK   90
#define BRIGHTRED     91
#define BRIGHTGREEN   92
#define BRIGHTYELLOW  93
#define BRIGHTBLUE    94
#define BRIGHTMAGENTA 95
#define BRIGHTCYAN    96
#define BRIGHTWHITE   97

#define BLOCK_BLINK     1
#define BLOCK           2
#define UNDERLINE_BLINK 3
#define UNDERLINE       4
#define BAR_BLINK       5
#define BAR             6
#define INVERT          7
#define HIDDEN          8

#define TRUE    1
#define FALSE   0

struct termsize {
    int cols;
    int rows;
};


void attendre(void);

void efface_ecran(void);

void allerxy(int x, int y);

void couleurpolice(int color);

void couleurfond(int color);

void policegras(void);
void policenormale(void);

void mode_souligne(void);
void desactive_mode_souligne(void);

void mode_clignotant(void);
void desactive_mode_clignotant(void);

void titre(char const* title);

void desactive_curseur(void);
void active_curseur(void);

void forme_curseur(int shape);

struct termsize recup_tailleterminal(void);

int recupcar(void);
int recupcar_echo(void);

void echooff(void);
void echoon(void);

int touche_appuyee(void);

void efface_ligne(void);

void resetcouleurs(void);
void resetterminal(void);

void nostdoutbuff(void);
void setstdoutbuff(void);
void flushstdin(void);
void flushstdout(void);
#endif
