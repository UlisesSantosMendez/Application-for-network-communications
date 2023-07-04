#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

//PROTOTIPOS DE FUNCION
char mensaje(char);
int main(){
	int udp_socket,lbind,tam;
	struct sockaddr_in local,remota;
	unsigned char desp[5]="adios"
	unsigned char msj[100];
	udp_socket = socket(AF_INET, SOCK_DGRAM, 0); //DECLARACION DEL SOCKET
	if(udp_socket == -1){
		perror("Error al abrir el socket");
		exit(0);
	}
	else{
		perror("Exito al abrir el socket");
		local.sin_family=AF_INET;
		local.sin_port=htons(0);
		local.sin_addr.s_addr=INADDR_ANY;
		lbind=bind(udp_socket,(struct sockaddr *)&local,sizeof(local));//SE ESTABLECE CONEXION
		if(lbind == -1){
			perror("Error en bind");
			exit(0);
		}
		else{
			perror("Exito en bind");
			//printf("------------APERTURA DE CHAT--------------------");
			remota.sin_family=AF_INET;
			remota.sin_port=htons(8080);
			remota.sin_addr.s_addr=inet_addr("192.168.1.149");
			//remota.sin_addr.s_addr=inet_addr("127.0.0.1");
			mensaje(msj);
			do{
				tam=sendto(udp_socket,msj,strlen(msj)+1,0,(struct sockaddr *)&remota,sizeof(remota));
				if(tam == -1){
					perror("\nError al enviar mensaje");
					exit(0);
				}
				else{
					perror("\nExito al enviar");
				}	
			}
			while(strcmp msj != desp);

		}
	}
	close(udp_socket);
	return 0;
}

char mensaje(char msj){
	printf("Ulises send:");
	scanf("%s",&msj);	
	return msj;
}

