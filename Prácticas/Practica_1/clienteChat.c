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
	int udp_socket,lbind,tam,tam2,lrecv;
	struct sockaddr_in local,remota;
	unsigned char msj[100];
	unsigned char buffer[512];
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
			printf("\n------------APERTURA DE CHAT--------------------");
			remota.sin_family=AF_INET;
			remota.sin_port=htons(8080);
			remota.sin_addr.s_addr=inet_addr("127.0.0.1");
			do{
				printf("\nMensaje: ");
 				fgets(msj, sizeof(buffer), stdin);
				tam=sendto(udp_socket,msj,strlen(msj)+1,0,(struct sockaddr *)&remota,sizeof(remota));
				if(tam == -1){
					perror("\nError al enviar mensaje");
					exit(0);
				}
				else{
					perror("\nExito al enviar mensaje");
					lrecv = sizeof(remota); 
 					tam2 = recvfrom(udp_socket, buffer , 512, 0, (struct sockaddr *)&remota, &lrecv);
 					if(tam2 == -1){
 						perror("\nError al recibir");
 						exit(0);
 					}	
 					else{
 						printf("\nMensaje Recibido: %s\n", buffer);
 					}
				}	
			}
			while(strlen(msj)!= 0);

		}
	}
	close(udp_socket);
	return 0;
}