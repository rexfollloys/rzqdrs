#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *chemin_du_pipe = "./pipe"; // Chemin relatif d'accès au pipe nommé

    // Créer le pipe nommé
    int resultat = mkfifo(chemin_du_pipe, 0666); // 0666 donne les permissions pour lire et écrire au pipe
    if (resultat == -1) {
        perror("Erreur lors de la création du pipe nommé");
        exit(EXIT_FAILURE);
    }

    printf("Pipe nommé créé avec succès : %s\n", chemin_du_pipe);

    return 0;
}
