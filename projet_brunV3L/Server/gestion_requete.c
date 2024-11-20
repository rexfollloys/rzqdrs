#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "server.h"
#include "socket.h"
#include "gestion_requete.h"

void gererClient(int sock_client){
    struct requete req;
    int nb_octet/*, taille*/;
    printf("*** Nouveau client correct ***\n");
    while(1){
        nb_octet = read(sock_client, &req, sizeof(struct requete));
        if(nb_octet<=0){
            perror("reception donnees");
            break;
        }
        if(req.type_requete == LOGIN){
            int nb;
            long res;
            nb_octet = read(sock_client, &nb, sizeof(int));
            if(nb_octet<=0){
                perror("reception donnees");
                break;
            }
            
            if(write(sock_client, (char *)&res, sizeof(long))<=0){
                perror("error reponse\n");
                break;
            }
        }
    }
    printf("*** Sortir de la boucle ***\n");
}

int verify_user(int socket_service){
    // buffer qui contiendra le message reçu
    char message[TAILLEBUF];
    char message2[TAILLEBUF];
    // chaîne reçue du client
    char *chaine_recue, *chaine_recue2;
    // chaîne renvoyée au client
    char *reponse = "bien recu";
    char *connected = "true";
    // nombre d'octets reçus ou envoyés
    int nb_octets, nb_octets2;
    int no_error = 1;

    // lecture username
    nb_octets = read(socket_service, message, TAILLEBUF);
    // affichage du message reçu
    chaine_recue = (char *)malloc(nb_octets * sizeof(char));
    memcpy(chaine_recue, message, nb_octets);
    printf("username : %s\n", chaine_recue);
 
/*
    // attachement de la mémoire partagée
    int shmid = shmget(socket_service, TAILLEBUF, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Erreur lors de la création de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }
    char *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("Erreur lors de l'attachement de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }

    // copie de l'username dans la mémoire partagée
    strncpy(shmaddr, message, TAILLEBUF);*/
   // confirmation reception username
    write(socket_service, reponse, strlen(reponse)+1);  
    //lecture password
    nb_octets2 = read(socket_service, message2, TAILLEBUF);
    chaine_recue2 = (char *)malloc(nb_octets2*sizeof(char));
    memcpy(chaine_recue2,message2,nb_octets2);
    printf("password : %s\n", chaine_recue2);


/*
    // détachement de la mémoire partagée
    if (shmdt(shmaddr) == -1) {
        perror("Erreur lors du détachement de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }*/

    // confirmation reception password et confirmation connection
    write(socket_service,connected, strlen(connected)+1);
    return no_error;

    
}

int list_users(int socket_service) {
    // Obtention de l'identifiant de la mémoire partagée
    int shmid = shmget(socket_service, TAILLEBUF, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Erreur lors de la création de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }
    
    // Attachement de la mémoire partagée
    char *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("Erreur lors de l'attachement de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }
    
    // Lecture des données de la mémoire partagée
    printf("Données dans la mémoire partagée : %s\n", shmaddr);
    
    // Détachement de la mémoire partagée
    if (shmdt(shmaddr) == -1) {
        perror("Erreur lors du détachement de la mémoire partagée");
        return 0; // ou un autre code d'erreur, selon votre logique
    }
    
    return 1; // succès
}

int del_memory(){

    return(1);
}

