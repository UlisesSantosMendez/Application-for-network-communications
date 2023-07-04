#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <string.h>
int main(){
	int udp_socket,lbind,tam,lrecv,tam2;
	struct sockaddr_in servidor,cliente;
	unsigned char msj[100];
	unsigned char paqRec[512];
	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_socket == -1){
		perror("Error al abrir el socket");
		exit(0);
	}
	else{
		perror("Exito al abrir el socket");
		servidor.sin_family=AF_INET;
		servidor.sin_port=htons(8080);
		servidor.sin_addr.s_addr=INADDR_ANY;
		lbind=bind(udp_socket,(struct sockaddr *)&servidor,sizeof(servidor));
		if(lbind == -1){
			perror("Error en bind");
			exit(0);
		}
		else{
			perror("Exito en bind");
			printf("\n-------------------APERTURA DE CHAT-------------------------");
			lrecv=sizeof(cliente);
			do{
				tam=recvfrom(udp_socket,paqRec,512,0,(struct sockaddr *)&cliente,&lrecv);
				if(tam == -1){
					perror("\nError al recibir mensaje");
					exit(0);
				}
				else{
					printf("\nEl mensaje es: %s",paqRec);
					printf("\nMensaje: ");
					fgets(msj, sizeof(paqRec), stdin);
					tam2=sendto(udp_socket,msj,strlen(msj)+1, 0, (struct sockaddr *)&cliente, sizeof(cliente));
					if(tam2 == -1){
						perror("\nError al enviar mensaje");
						exit(0);
					}
					else{
						perror("\nExito al enviar mensaje");
					}
				}
			}
			while(strlen(msj)!= 0);
		}
	}
	close(udp_socket);
	return 0;
}
