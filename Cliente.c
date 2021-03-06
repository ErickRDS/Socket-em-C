﻿#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <time.h>
 
int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server, client;
	char server_reply[80];
	int number[10],i, protocol, aux = 1, slen = sizeof(client), somatorio = 0;
	double pacotes = 0;
	int tempomax = 0;
	int tempomin = 9999;
	int tempo = 0;	
	char ip[16];
	char temp[10];
	clock_t inicio, fim;

	while(aux){
		printf("Qual protocolo usar: 1 para TCP / 2 para UDP\n");
		scanf("%d", &protocol);
		
		if ( protocol != 1 && protocol != 2){
			printf("Escolha de protocolo inválida, por favor tente de novo.\n");
		} // fim if
		else {aux = 0;}

	} // fim while

	printf("Endereço de ip (loopback neste exemplo): ");
	scanf("%s", ip);

	printf("Quantidade de pacotes a serem enviados: ");
	scanf("%lf", &pacotes);
 
	if( protocol == 1){ // Se o protocolo escolhido é TCP

		printf("Mensagem que deseja enviar: ");
		scanf("%s", temp);	

		//Create socket TCP
		sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
		if (sock == -1) {
		 	printf("Could not create socket");
		 } // fim if

		puts("Socket created");

		server.sin_addr.s_addr = inet_addr(ip);
		server.sin_family = AF_INET;
		server.sin_port = htons( 8880 );
		 
		//Connect to remote server
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
			 perror("connect failed. Error");
			 return 1;
		 } // fim if
		 
		puts("\nConnected");
		
		for( i = 1 ; i <= pacotes; i++ ){
			inicio = clock();

			if( send(sock , &temp, 10*sizeof(int) , 0) < 0) {
				puts("Send failed");
				return 1;
			} // fim if
			 
			 //Receive a reply from the server
			if( recv(sock , &server_reply , 10*sizeof(int), 0) < 0) {
				puts("recv failed");
				return;
			} // fim if
			
			fim = clock();
			tempo = fim - inicio;
			somatorio = somatorio + tempo;
			if (tempo > tempomax){
				tempomax = tempo;
			}
			if (tempo < tempomin){
				tempomin = tempo;
			}

		} // fim for
		 
		puts("Server reply :\n");
		puts(server_reply);
		
		printf("\nTempo médio gasto: %lf microsegundos.\n",((double)(1000000*(somatorio/pacotes)/CLOCKS_PER_SEC)));
		printf("Tempo máximo gasto: %lf microsegundos.\n",((double)(1000000*tempomax/CLOCKS_PER_SEC)));
		printf("Tempo mínimo gasto: %lf microsegundos.\n",((double)(1000000*tempomin/CLOCKS_PER_SEC)));

	} // fim if de TCP
	else if( protocol == 2){ // Se o protocolo escolhido é UDP

		printf("Mensagem que deseja enviar: ");
		scanf("%s", temp);

		//Create socket UDP
		sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
		if (sock == -1) {
		 	printf("Could not create socket");
		 } // fim if

		puts("Socket UDP created");

		server.sin_addr.s_addr = inet_addr(ip);
		server.sin_family = AF_INET;
		server.sin_port = htons( 8880 );
		 
		for( i = 1 ; i <= pacotes; i++ ){
			inicio = clock();

			if( sendto(sock , &temp, 10*sizeof(int) , 0, (struct sockaddr *) &server, sizeof(client) ) < 0) {
				puts("Send failed");
				return 1;
			}
			 
			 //Receive a reply from the server
			if( recvfrom(sock , &server_reply , 10*sizeof(int), 0, (struct sockaddr *)&server, &slen) < 0) {
				puts("recv failed");
				return;
			}
			
			fim = clock();
			tempo = fim - inicio;
			somatorio = somatorio + tempo;
			if (tempo > tempomax){
				tempomax = tempo;
			}
			if (tempo < tempomin){
				tempomin = tempo;
			}

		} // fim for

		puts("Server reply :\n");
		puts(server_reply);

		printf("\nTempo médio gasto: %lf microsegundos.\n",((double)(1000000*(somatorio/pacotes)/CLOCKS_PER_SEC)));
		printf("Tempo máximo gasto: %lf microsegundos.\n",((double)(1000000*tempomax/CLOCKS_PER_SEC)));
		printf("Tempo mínimo gasto: %lf microsegundos.\n",((double)(1000000*tempomin/CLOCKS_PER_SEC)));
	} // fim else if de UDP
	 
	close(sock);
	
	return 0;
}

