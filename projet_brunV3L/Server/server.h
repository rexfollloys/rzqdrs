#ifndef REQUETENUM_H
#define REQUETENUM_H

enum requete_t{
    FIN = 0,
    LOGIN,
    LOGOUT,
    CREATEACC,
    DELETEACC,
    USERLIST,
    SENDMESSAGE
};
#endif
#ifndef REQUETE_H
#define REQUETE_H
struct requete{
    enum requete_t type_requete;
    int taille_requete;
    char content_requete[50];
};
#endif

void gererClient(int sock_client);
int verify_user(int socket_service);
int list_users();
int del_memory();
void *handle_client(void *arg);