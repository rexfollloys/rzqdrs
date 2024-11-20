#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "gestion_requete.h"

#define PORT 8005
#define MAX_CLIENTS 5

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {

    //////////////// Connexion Client
    int socket_service = *((int *)arg);
    /// Conexion TCP Communication-ClientChat coté serveur
    
    //Port du serveur local ecoute
    int port_local_ecoute = 8005;
    // socket d'écoute et de service
    // buffer qui contiendra le message reçu
    char message[TAILLEBUF];
    // chaîne reçue du client
    char choice;
    char message_broad[TAILLEBUF];
    // chaîne renvoyée au client
    char *reponse = "bien recu";
    // nombre d'octets reçus ou envoyés
    int nb_octets;
    
    // création socket TCP d'écoute

    printf("Ouverture du serveur sur le port : %d\n", port_local_ecoute);

    /////////// Connexion RMI

    int sockfd = creerSocketUDP(8007);
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    int hey = 1;
    // Attente de la réception de données
    while (hey) {
        int len, n, n2=0;
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Message du client : %s\n", buffer);

        // Réponse au client
        sendto(sockfd, (const char *)"Message reçu par le serveur", strlen("Message reçu par le serveur"), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);

        n2 = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        if(n2!=0) hey = 0;
    }
    int exit = 1;
    while(exit){

        // Reception du code d'instruction
        nb_octets = read(socket_service, message, TAILLEBUF);
        memcpy(&choice, message, nb_octets);
        // Confirmation reception
        write(socket_service, reponse, strlen(reponse)+1);

        // Gestion des instructions
        switch(choice)
        {
            case '1':
                printf("Choix : Se connecter au système avec un pseudo et un mot de passe.\n");  
                if(verify_user(socket_service)==0){
                    printf("Erreur de verification");
                }
                break;
            case '2':
                printf("Choix : Envoyer message.\n"); 

                ///Serveur recois le message
                nb_octets = read(socket_service, message, TAILLEBUF);
                memcpy(&message_broad, message, nb_octets);

                // Confirmation reception
                write(socket_service, message_broad, strlen(reponse)+1);
                break;
            case '3':
                printf("Choix : Se déconnecter du système.\n"); 
                break;
            case '4':
                printf("Choix : Afficher la liste de tous les utilisateurs présents.\n"); 
                if(list_users(socket_service)==0){
                    printf("Erreur liste users");
                }
                break;
            case '5':
                printf("Choix : Créer un nouveau compte.\n"); 
                break;
            case '6':
                printf("Choix : Supprimer un compte existant.\n"); 
                break;
            case '7':
                printf("Choix : Quitter le client.\n");
                exit = 0;
                if(del_memory()==0){
                    printf("Erreur del_memory");
                }
                break;
            default :
                printf("Choix inexistant");   
                break;
        }
    }
    // on ferme les sockets
    printf("Fermeture du socket %d\n", socket_service);
    close(socket_service);
    close(sockfd);
    
    return 0;
}


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Créez le socket du serveur
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Erreur lors de la création du socket du serveur");
        exit(EXIT_FAILURE);
    }

    // Initialisez les informations sur le serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Liez le socket du serveur à l'adresse et au port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la liaison du socket du serveur");
        exit(EXIT_FAILURE);
    }

    // Mettez le socket du serveur en mode écoute
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Erreur lors de la mise en écoute du socket du serveur");
        exit(EXIT_FAILURE);
    }

    // Acceptez les connexions des clients
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Erreur lors de l'acceptation de la connexion du client");
            continue;
        }

        // Gérez chaque client dans un thread séparé
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)&client_socket);
        
        // Ajoutez le client à la liste des clients
        pthread_mutex_lock(&mutex);
        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = client_socket;
        } else {
            printf("Trop de clients, connexion refusée.\n");
            close(client_socket);
        }
        pthread_mutex_unlock(&mutex);
    }

    // Fermez le socket du serveur
    close(server_socket);

    return 0;
}
