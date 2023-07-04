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


int estructura_msj(unsigned char *trama){
	int tam;
	unsigned char id[2]={0x10,0x20};//ponerlo aleatorio
	unsigned char banderas[2]={0x01,0x00};//dejamos fijass
	unsigned char cont[8]={0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00};//dejamos fijos
	unsigned char entrada[2]={0x03,'w','w','w',0x03,'i','p','n',0x02,'m','x',0x00,0x00,0x01,0x00,0x01};
	memcpy(trama+0,id,2);
	memcpy(trama+2,banderas,2);
	memcpy(trama+4,cont,8);
	memcpy(trama+12,entrada,16);
	tam=28;
	return tam;

}

int main(){
	int udp_socket,tam,tammsj,lbind;
	struct sockaddr_in local,remota;
	unsigned char paq[50];
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
			remota.sin_family=AF_INET;
			remota.sin_port=htons(53);
			remota.sin_addr.s_addr=inet_addr("8.40.1.25");
			tammsj=estructura_msj(paq);
			tam=sendto(udp_socket,paq,tammsj,0,(struct sockaddr *)&remota,sizeof(remota));
			if(tam == -1){
				perror("\nError al enviar mensaje");
				exit(1);
			}
			else{
				perror("\nExito al enviar");
			}	
		}
	}
	close(udp_socket);
	return 0;
}
