#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>



int estructura_msj(unsigned char *trama){
    unsigned char nombre[50];
    unsigned char modo[]="octet";
    unsigned char opcLect[2]={0x00,0x01};
    int tam;
    printf("\nInserta el nombre del archivo: ");
    gets(nombre);
    memcpy(trama+0,opcLect,2);
    strcpy(trama+2,nombre);
    strcpy(trama+strlen(nombre)+3,modo);
    tam=2+strlen(nombre)+1+strlen(modo)+1;
    return tam;
    
}

int estructuraACK(int Nblock,unsigned char *msj){
 unsigned char opcACK[2]={0x00,0x04};
 unsigned char bloque[2];
 bloque[0]=Nblock>>8;
 bloque[1]=Nblock&0x00FF;
 memcpy(msj+0,opcACK,2);
 memcpy(msj+2,bloque,2);
 return 4;
}


 int main(){
 int udp_socket,tam, tammsj,lbind,lrecv,tamr,Nbloq;
 struct sockaddr_in local,remota;
 unsigned char paq[516],buffer[516];
 udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
 if(udp_socket==-1){
     perror("\nError en socket");
     exit(1);
 }
 else{
     local.sin_family=AF_INET; /* address family: AF_INET */
     local.sin_port=htons(0);   /* port in network byte order */
     local.sin_addr.s_addr=INADDR_ANY;
     lbind=bind(udp_socket,(struct sockaddr *)&local,sizeof(local));
     if(lbind==-1){
         perror("\nError en bind");
         exit(1);
     }
     else{
	     remota.sin_family=AF_INET; /* address family: AF_INET */
	     remota.sin_port=htons(69);   /* port in network byte order */
	     //remota.sin_addr.s_addr=inet_addr("192.168.1.68");
	     //remota.sin_addr.s_addr=inet_addr("192.168.0.13");
	     //remota.sin_addr.s_addr=inet_addr("192.168.0.11");
	     //remota.sin_addr.s_addr=inet_addr("8.40.1.25");
	     //remota.sin_addr.s_addr=inet_addr("8.40.1.77");//En esta se debe poner la IP del servidor, NO LA MIA
	     remota.sin_addr.s_addr=inet_addr("169.254.110.233");
	     tammsj=estructura_msj(paq);
	     while(1){
	     tam=sendto(udp_socket,paq,tammsj,0,(struct sockaddr *)&remota,sizeof(remota));
	     if(tam==-1){
		 perror("\nError al enviar");
		 exit(1);
	     }
	     else{
		printf("\nExito al enviar");
		lrecv=sizeof(remota);
		tamr=recvfrom(udp_socket,buffer,516,0,(struct sockaddr *)&remota,&lrecv);
		if(tam==-1){
		  perror("\nError al enviar");
		  exit(1);
		}
		else{
		  Nbloq=buffer[2];
		  Nbloq=(Nbloq<<8)+buffer[3];
		  printf("\nExito al recibir bloque: %d",Nbloq);
		  //Almacenar los datos a partir del byte 4
		  tammsj=estructuraACK(Nbloq,paq);
               }
	     }
	     }
      }
 }
 close(udp_socket);
 return 0;
}
