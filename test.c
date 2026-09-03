#include <stdio.h>
#include "console.h"


int main(void) {
  int x=10, y=20;
  int i=0;
  struct termsize taille;

  efface_ecran();
  titre("--==== Test de console.h ====--");
  printf("Appuyer 5 fois sur Espace!");
  allerxy(x,y); /* on déplace le curseur */
  couleurpolice(GREEN);
  printf("Hello World!!!");
  desactive_curseur();
  while(x<15) {
    while(recupcar()!=' ');
    x++;
    efface_ecran();
    allerxy(x,y);
    printf("Hello World!!!");
  }
  efface_ecran();
  policegras();
  couleurpolice(YELLOW);
  printf("Hello World!!!\nAppuyez sur Entrée\n");
  while(recupcar()!='\n');
  for(i=30;i<38;i++) {
    couleurfond(i);
    printf("Une couleur de fond différente\n");
  }
  couleurfond(BLACK);
  for(i=30;i<38;i++) {
    couleurpolice(i);
    printf("Une couleur de police différente\n");
  }
  printf("\nAppuyez sur Entrée à nouveau\n");
  policenormale();

  while(recupcar()!='\n');
  resetcouleurs();
  efface_ecran();
  
  taille = recup_tailleterminal();
  printf("Taille du terminal: %d colonnes et %d lignes\n",taille.cols,taille.rows);
  for (i=0;i<taille.cols;i++) {
	  printf("=");
  }

  printf("Appuyez sur q pour quitter\n");
  while (recupcar() != 'q');

  resetcouleurs();
  resetterminal();

  return 0;
}
