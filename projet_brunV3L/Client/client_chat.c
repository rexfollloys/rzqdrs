#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define max_char_pseudo 15
#define max_char_password 15
#define max_connect 10
#define TAILLEBUF 80

char reponse[TAILLEBUF];

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int connect_user(int sock) {
    int logged = 0;
    int nb_octets = 0;
    // chaîne où sera écrit le message reçu
    char bufferPseudo[max_char_pseudo];
    char bufferPassword[max_char_password];
    nb_octets = read(sock, reponse, TAILLEBUF);
    if (nb_octets == 0) printf("hello");
    printf("Saisissez votre pseudo : \n");
    scanf("%s", bufferPseudo);
    printf("Saisissez votre mot de passe : \n");
    scanf("%s", bufferPassword);
    //envoi du password et pseudo par tcp pour verifier si l'utilisateur est inscrit dans le RMI
    nb_octets = write(sock, bufferPseudo, sizeof(bufferPseudo));
    nb_octets = read(sock, reponse, TAILLEBUF);
    nb_octets = write(sock, bufferPassword, sizeof(bufferPassword));
    nb_octets = read(sock, reponse, TAILLEBUF);
    if (!strcmp(reponse, "true")) {
        printf("Connecté en tant que : %s.\n", bufferPseudo);
        logged = 1;
    }
    return logged;
}

void envoi_message(int sock) {
    char message[TAILLEBUF];
    char reponse[TAILLEBUF];
    char reponse_mess[TAILLEBUF];
    int pipe; 
    ssize_t nb_octets = 0;
    
    const char *chemin_du_pipe = "./Client/pipe"; // Chemin d'accès relatif au pipe nommé


    // connexion etablie, on envoie le message
    nb_octets = read(sock, reponse, TAILLEBUF);
    printf(" reponse recue : %s\n", reponse);
    printf("Saisir message à envoyer (max %d caractères) : ", TAILLEBUF - 1);
    viderBuffer();
    scanf("%s", &message);
    //fgets(message, TAILLEBUF, stdin);
    printf("le message est : %s", message);
    nb_octets = write(sock, message, sizeof(message));
    printf("\n");
    // on attend la réponse du serveur
    nb_octets = read(sock, reponse_mess, TAILLEBUF);
    if (nb_octets == -1) {
        perror("Erreur lors de la lecture de la réponse du serveur");
        close(pipe);
        exit(EXIT_FAILURE);
    }    
    printf(" message : %s\n", reponse_mess);


    // Ouvrir le pipe en ecriture
    if ( (pipe = open(chemin_du_pipe, O_WRONLY)) < 0){
        perror("Erreur lors de l'ouverture du pipe en ecriture.");
    }
    // Envoyer le message au pipe nommé

    if ( write(pipe, reponse_mess, nb_octets) != nb_octets) { 
        perror("Erreur lors de l'envoi du message par le pipe");
    }


    //Fermer le descripteur de fichier du pipe
    if (close(pipe) == -1) {
        perror("Erreur lors de la fermeture du pipe");
        exit(EXIT_FAILURE);
    }
    
}


int main(void){

    /// Conexion TCP Communication-ClientChat coté client

    int port_serveur = 8005;
    char* nom_serveur = "10.1.13.52";

    // identification socket d'écoute du serveur
    // adresse socket locale
    static struct sockaddr_in addr_serveur;
    // identifiants de la machine où tourne le serveur
    struct hostent *host_serveur;
    // socket locale coté client
    int sock;

    // nombre d'octets envoyés/reçus
    // création socket TCP

    printf("Connexion du client au serveur : %s sur le port %d\n", nom_serveur, port_serveur);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("creation socket");
        exit(1);
    }

    // récupération identifiants de la machine serveur local
    host_serveur = gethostbyname(nom_serveur);

    if (host_serveur == NULL) {
        perror("erreur récupération adresse serveur\n");
        exit(1);
    }

    // Création de l'identifiant de la socket d'écoute du serveur
    memset(&addr_serveur, 0, sizeof(addr_serveur));
    addr_serveur.sin_family = AF_INET;
    addr_serveur.sin_port = htons(port_serveur);
    memcpy(&addr_serveur.sin_addr.s_addr, host_serveur->h_addr_list[0], host_serveur->h_length);

    // Connexion de la socket client locale à la socket coté serveur
    if (connect(sock, (struct sockaddr *)&addr_serveur, sizeof(addr_serveur)) == -1) {
        perror("erreur connexion serveur");
        exit(1);
    }

    int exit = 1;
    while(exit){
        char code;
        int choice;
        printf("Que voulez vous faire :\n[1] : Se connecter au système avec un pseudo et un mot de passe.\n[2] : Envoyer un message.\n[3] : Se déconnecter du système.\n[4] : Afficher la liste de tous les utilisateurs présents.\n[5] : Créer un nouveau compte.\n[6] : Supprimer un compte existant.\n[7] : Quitter le client.\n");
        if(scanf("%d", &choice)==1){
            switch (choice)
            {
                case 1:
                    printf("Choix : Se connecter au système avec un pseudo et un mot de passe.\n");  
                    code = '1';
                    write(sock, &code, sizeof(code));
                    if(connect_user(sock)==0){
                        printf("erreur lors de la correction");
                    }
                    break;
                case 2:
                    printf("Choix : Envoyer un message.\n"); 
                    
                    /*pid_t pid = fork();
                    if (pid == -1) {
                        perror("Erreur lors de la création du processus fils");
                        exit(EXIT_FAILURE);
                    } else if (pid == 0) { // Processus fils
                        execl("./afficheur_message", "afficheur_message", NULL);
                        perror("Erreur lors de l'exécution de l'afficheur_message");
                        exit(EXIT_FAILURE);
                    }*/
                    code = '2';
                    write(sock, &code, sizeof(code));
                    
                    envoi_message(sock);
                    break;
                case 3:
                    printf("Choix : Se déconnecter du système.\n"); 
                    
                    code = '3';
                    write(sock, &code, sizeof(code));                    
                    break;
                case 4:
                    printf("Choix : Afficher la liste de tous les utilisateurs présents.\n"); 
                    code = '4';
                    write(sock, &code, sizeof(code));
                    break;
                case 5:
                    printf("Choix : Créer un nouveau compte.\n"); 
                    code = '5';
                    write(sock, &code, sizeof(code));
                    break;
                case 6:
                    printf("Choix : Supprimer un compte existant.\n"); 
                    code = '6';
                    write(sock, &code, sizeof(code));
                    break; 
                case 7:
                    printf("Choix : Quitter le client.\n");
                    exit = 0; 
                    code = '7';
                    write(sock, &code, sizeof(code));
                    break; 
                default :
                    printf("Choix inexistant");           
            }
        }
        else{printf("Please enter a number.\n");exit = 0;}
    }
    // on ferme la socket
    close(sock); 
}


