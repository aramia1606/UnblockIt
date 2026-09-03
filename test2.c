#include <stdio.h>
#include <unistd.h> // sleep()

int main() {
    // Efface l'écran
    printf("\033[2J");

    // --- Démonstration simple ---
    allerxy(10, 5);
    printf("Texte en (10,5)");

    sleep(1);

    allerxy(30, 10);
    printf("Texte en (30,10)");

    sleep(1);

    // --- Superposition de textes ---
    allerxy(10, 5);
    printf("XXXXXXXXXXXXXXX");
	printf("NNN\nBBBB");
    sleep(1);

    allerxy(10, 5);
    printf("HELLO");

    sleep(1);

    // Superposition partielle
    allerxy(12, 5);
    printf("WORLD");

    sleep(1);

    // Remet le curseur en bas
    int i = 0;
	for(i=0; i<40;i++){
		allerxy(i,i);
		printf("%d", i);
	}
    return 0;
}
