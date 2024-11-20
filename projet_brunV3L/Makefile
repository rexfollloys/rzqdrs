CC = gcc
CC2 = javac
CFLAGS = -Wall -g -D_GNU_SOURCE
RM = rm -fv

.PHONY: all clean

all: main_server client afficheur_message creation_pipe serverjava clientjava

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

main_server: ./Server/gestion_requete.c ./Server/socket.c ./Server/communication.c 
	$(CC) $(CFLAGS) -o $@ $^ -pthread

client: ./Client/client_chat.c
	$(CC) $(CFLAGS) -o $@ $^

afficheur_message: ./Client/afficheur_message.c
	$(CC) $(CFLAGS) -o ./Client/$@ $^

creation_pipe: ./Client/creation_pipe.c
	$(CC) $(CFLAGS) -o ./Client/$@ $^
clean:
	$(RM) ./*o ./Server/*.o ./Client/*.o main_server client gestion_requete ./Server/main ./Server/shared_memory ./Client/afficheur_message ./Client/creation_pipe ./Client/client_chat
