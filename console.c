/*
 * console.c
 * PoPS ESR v1.7
 *
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.h"

/* Attente d'un appui sur Entrée */
void attendre(void) {
    while (fgetc(stdin) != '\n');
}


/* Efface l'affichage */
void efface_ecran(void) {
  printf("\033[2J\033[?6h");
}


/* Déplacement en x,y du curseur */
void allerxy(int x, int y) {
  printf("\033[%d;%dH", y, x);
}


/* Couleur de la police de caractères */
void couleurpolice(int color) {
  printf("\033[%dm", color);
}

/* Couleur du fond */
void couleurfond(int color) {
  printf("\033[%dm", color+10);
}

/* Police en mode bold (gras ou en évidence) */
void policegras(void) {
  printf("\033[1m");
}

/* Retour à la police d'origine */
void policenormale(void) {
  printf("\033[0m");
}

void mode_souligne(void) {
  printf("\033[4m");
}

void desactive_mode_souligne(void) {
  printf("\033[24m");
}

void mode_clignotant(void) {
  printf("\033[5m");
}

void desactive_mode_clignotant(void) {
  printf("\033[25m");
}

/* Définit le titre du terminal */
void titre(char const* title) {
  printf("\033]0;%s\x7", title);
}

/* Désactive l'affichage du curseur */
void desactive_curseur(void) {
  printf("\033[?25l");
}

/* Réactive l'affichage du curseur */
void active_curseur(void) {
  printf("\033[?25h");
}

void forme_curseur(int shape) {
  /* vt520/xterm-style; linux terminal uses ESC[?1;2;3c,
   * not implemented
   */
  printf("\033[%d q", shape);
}

/* Récupération de la taille du terminal */
struct termsize recup_tailleterminal(void) {
  struct termsize size;
  struct winsize win;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
  size.cols = win.ws_col;
  size.rows = win.ws_row;
  return size;
}

/* Récupération d'un caractère au clavier
 * sans écho du caractère ni besoin d'appuyer
 * sur Entrée.  Fonction bloquante: on attend
 * qu'un caractère soit tapé.
 */
int recupcar(void) {
  int ch;
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  ch = getc(stdin);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

  return ch;
}

/* Récupération d'un caractère au clavier
 * avec écho du caractère sans besoin d'appuyer
 * sur Entrée.  Fonction bloquante: on attend
 * qu'un caractère soit tapé.  L'écho ne fonctionne
 * que s'il n'a pas été désactivé auparavant.
 */
int recupcar_echo(void) {
  int ch;
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  ch = getc(stdin);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

  return ch;
}

/* Indique si une touche a été appuyée en
 * renvoyant le nombre d'octets présents
 * dans le buffer de l'entrée standard stdin.
 * 0 est renvoyé si aucune touche n'a été
 * appuyée.
 */
int touche_appuyee(void) {
  struct termios oldattr, newattr;
  int nbchars;
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  ioctl(0, FIONREAD, &nbchars);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

  return nbchars;
}

/* Désactivation du mode echo du terminal:
 * tout appui sur une touche ne sera pas
 * affiché
 */
void echooff(void) {
  struct termios termattr;
  tcgetattr(STDIN_FILENO, &termattr);
  termattr.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &termattr);
}

/* Activation du mode echo du terminal:
 * tout appui sur une touche sera affiché
 */
void echoon(void) {
  struct termios termattr;
  tcgetattr(STDIN_FILENO, &termattr);
  termattr.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &termattr);
}

void efface_ligne(void) {
  printf("\033[2K\033E");
}

void resetcouleurs(void) {
  printf("\033[0m");
}

void resetterminal(void) {
  printf("\033c");
}

/* sous Unix/Linux la sortie standard
 * stdout est bufférisée, i.e., un printf
 * ne s'affiche que si un \n est dans le buffer
 * Cette fonction désactive le buffer
 */
void nostdoutbuff(void) {
  setvbuf(stdout, NULL, _IONBF, 0);
}

/* Réactive le buffer de la sortie standard */
void setstdoutbuff(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
}

/* Pour vider les caractères restant dans le
 * buffer de saisie de caractères
 */
void flushstdin(void) {
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}

/* Pour vider les caractères restant dans le
 * buffer de sortie
 */
void flushstdout(void) {
  fflush(stdout);
}
