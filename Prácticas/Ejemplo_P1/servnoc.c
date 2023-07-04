#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <arpa/inet.h>
int main(){
	//pthread_mutex_t mutex=_PTHREAD_MUTEX_INITIALIZER;
	int udp_socket,lbind,tam,lrecv;
	struct sockaddr_in servidor,cliente;
	unsigned char msj[100]="hola red soy Ulises";
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
			lrecv=sizeof(cliente);
			tam=recvfrom(udp_socket,paqRec,512,0,(struct sockaddr *)&cliente,&lrecv);
			if(tam == -1){
				perror("\nError al recibir");
				exit(0);
			}
			else{
				printf("\nEl mensaje es: %s",paqRec);
			}
			
		}
	}
	close(udp_socket);
	return 0;
}
