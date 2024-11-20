#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define TAILLEBUF 100


void main() {
    const char *chemin_du_pipe = "./pipe"; // Chemin d'accès relatif au pipe nommé


    int pipe,exit;
    ssize_t nb_octets_lus;
    char buf[TAILLEBUF];
    char message[TAILLEBUF];

    // Ouvrir le pipe en lecture
    if ( (pipe = open(chemin_du_pipe, O_RDONLY)) < 0)
        perror("Erreur lors de l'ouverture du pipe en lecture");

    // Lire les messages du pipe et les afficher
        nb_octets_lus = read(pipe, buf, TAILLEBUF - 1);
        memcpy(&message, buf, nb_octets_lus);
        printf("hey ? %s\n", message);
        //write(chemin_du_pipe, buf, nb_octets_lus);

    close(pipe);
}
