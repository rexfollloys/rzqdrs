#include <pthread.h>
#include <stdio.h>
#include <string.h>
#define max_char_pseudo 15
#define max_connect 10

// Global variable:
char users[max_connect][max_char_pseudo];
int n = 5;

void* add_user(char users[][max_char_pseudo], char *username){
    for(int i=0; i<max_connect;i++){
        if(users[i][0]!=' '){
            //printf("User %d : ", i);
            //*users[i] = *username;
            
            for(int j=0; j<max_char_pseudo;j++){
                while(username[j]!=' '){
                    * (char *)users[i][j]= username[j];
                    printf("%s",&users[i][j]);
                }
            }
            //printf(".\n");
        }
    }

}
/*
void* delete_user(char users[][max_char_pseudo], char username){
    // Print value received as argument:

}*/

void init_users(char users[][max_char_pseudo]){
    for(int i=0; i<max_connect;i++){
        for(int j=0; j<max_char_pseudo;j++){
            users[i][j]=' ';
        }
    }
}

void consult_user(char users[][max_char_pseudo]){
    printf("List of users : \n");
    for(int i=0; i<max_connect;i++){
        if(users[i][0]!=' '){
            printf("User %d : ", i);
            for(int j=0; j<max_char_pseudo;j++){
                printf("%s", &users[i][j]);
            }
            printf(".\n");
        }
    }
}

int main(void){
    // Declare variable for thread's ID:
    init_users(users);
    char username[max_char_pseudo];
    for(int i = 0; i<max_char_pseudo; i++){
        username[i] = ' ';
    }
    username[0] = 'h';
    username[1] = 'e';
    username[2] = 'l';
    username[3] = 'l';
    username[4] = 'o';
    add_user(users, username);
    consult_user(users);
    //delete_user(users, username);
    consult_user(users);
    /*
    pthread_t id;
    pthread_t id2;
    pthread_create(&id2, NULL, add_user, &users);
    pthread_create(&id, NULL, delete_user, &users);


    int* ptr;
    int* ptr2;
    // Wait for func() and retrieve value in ptr;
    pthread_join(id, (void**)&ptr);
    pthread_join(id2, (void**)&ptr2);*/
}



