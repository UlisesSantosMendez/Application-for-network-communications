//SANTOS MENDEZ ULISES JESUS
//TFTP CLIENTE
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

unsigned char nombre[32];//GENERAMOS EL NOMBRE GLOBAL

//2BYTES|STRING  |1BYTE|STRING|1 BYTE|
//OPCODE|FILENAME|0    |MODE  |0     |

int estructura_lectura(unsigned char *trama){
	unsigned char modo[]="octet";//MODO se debe mantar como string
	//////////////////////CODIGO DE OPERACION///////////////////////////////
	unsigned char opcLect[]={0x00,0x01}; //|00000000|00000001| Mide 2 bytes
	int tam;
	printf("\nInserta el nombre del archivo: ");
	//scanf("%hhu",&nombre[50]);
	gets(nombre);
	//void *memcpy(void *dest, const void *src, size_t n);
	memcpy(trama+0,opcLect,2); //memcpy copy a memoria area n bytes from area to dest
	//char *strcpy(char *dest, const char *src);
	strcpy(trama+2,nombre); // cpy a string (includes the terminating null byte '0')
	strcpy(trama+strlen(nombre)+3,modo);
	tam=2+strlen(nombre)+1+strlen(modo)+1;
	return tam;

}



int estructura_escritura(unsigned char *trama){
	unsigned char modo[]="octet";//MODO se debe mantar como string
	//////////////////////CODIGO DE OPERACION///////////////////////////////
	unsigned char opcWrite[]={0x00,0x02}; //|00000000|00000010| Mide 2 bytes
	int tam;
	printf("\nInserta el nombre del archivo: ");
	gets(nombre);
	//void *memcpy(void *dest, const void *src, size_t n);
	memcpy(trama+0,opcWrite,2); //memcpy copy a memoria area n bytes from area to dest
	//char *strcpy(char *dest, const char *src);
	strcpy(trama+2,nombre); // cpy a string (includes the terminating null byte '0')
	strcpy(trama+strlen(nombre)+3,modo);
	tam=2+strlen(nombre)+1+strlen(modo)+1;
	return tam;
}

int estructuraACK(int Nblock,unsigned char *msj){
	unsigned char opcACK[2]={0x00,0x04}; //|00000000|00000100| Mide 2 bytes
 	unsigned char bloque[2]; //2 bytes de bloque
 	bloque[0]=Nblock>>8;
 	bloque[1]=Nblock&0x00FF; //Es una mascara 
 	memcpy(msj+0,opcACK,2);
 	memcpy(msj+2,bloque,2);
 	return 4;
}

int estructuraBlock(int Nblock, unsigned char *msj, unsigned char *data){
	unsigned char opcBlock[2]={0x00,0x03};
	unsigned char bloque[2];
	bloque[0]=Nblock>>8;
 	bloque[1]=Nblock&0x00FF;
	memcpy(msj+0,opcBlock,2);
 	memcpy(msj+2,bloque,2);
	memcpy(msj+4,data,512);
	return (4+512);
}

int main(){
	FILE *destino;
	int udp_socket,tam,tammsj,lbind,lrecv,tamr,Nbloq,op;
	struct sockaddr_in cliente,servidor;
	unsigned char paq[516], buffer[516], data[516];// aqui lo definiremos como 512
	udp_socket = socket(AF_INET, SOCK_DGRAM, 0); //DECLARACION DEL SOCKET LOCAL
	if(udp_socket == -1){
	//perror("Falla en abrir el socket");
		perror("Falla en comunicacion TFTP");
		exit(0);
	}
	else{
		//perror("Exito al abrir el socket");
		perror("Apertura de comunicacion TFTP\n");
		cliente.sin_family=AF_INET;
		cliente.sin_port=htons(0);
		cliente.sin_addr.s_addr=INADDR_ANY;
		lbind=bind(udp_socket,(struct sockaddr *)&cliente,sizeof(cliente));//SE ESTABLECE CONEXION
		if(lbind == -1){
			perror("Error en bind");
			exit(0);
		}
		else{
			perror("Exito en enlace");
			servidor.sin_family=AF_INET;
			servidor.sin_port=htons(69);
			//PARA LA IP DEBEMOS ESTAR EN LA MISMA RED WIFI
			//servidor.sin_addr.s_addr=inet_addr("192.168.1.47");//AQUI VA LA IP DEL SERVIDOR, LA DEL TFTP64
			//servidor.sin_addr.s_addr=inet_addr("192.168.1.68");
			//servidor.sin_addr.s_addr=inet_addr("8.40.1.77");
			//servidor.sin_addr.s_addr=inet_addr("192.168.1.43");
			servidor.sin_addr.s_addr=inet_addr("192.168.1.72");
			printf("Opcion 1: LECTURA\n");
			printf("Opcion 2: ESCRITURA\n");
			printf("Selecciona la petición que deseas realizar: ");
			scanf("%d",&op);
			//fflush(stdin);
			getchar();
			if(op == 1){
				tammsj=estructura_lectura(paq);
				destino=fopen(nombre,"wb");
			}
			else{
				tammsj=estructura_escritura(paq);
				destino=fopen(nombre,"rb");
			}
			do{
				tam=sendto(udp_socket,paq,tammsj,0,(struct sockaddr *)&servidor,sizeof(servidor)); //paq es el buffer
				if(tam == -1){
					perror("\nError al enviar archivo");
					exit(1);
				}
				else{
					perror("\nExito al enviar archivo");
					lrecv=sizeof(servidor);
					//recvfrom(int sockfd,void *buf,size len, int flags, struct sockaddr *src, socklen *addrlen);
					tamr=recvfrom(udp_socket,buffer,516,0,(struct sockaddr *)&servidor,&lrecv);
					if(tam==-1){
          					perror("\nError al enviar");
          					exit(1);
         				}
        				else{
          				
          					//Almacenar los datos a partir del byte 4
          					if(op==1){
          					//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
								Nbloq=buffer[2];
          						Nbloq=(Nbloq<<8)+buffer[3];
          						printf("\nExito al recibir bloque: %d",Nbloq);
          						fwrite(buffer+4,1,tamr-4,destino);//tamr son los 516-4 dan los 512
								tammsj=estructuraACK(Nbloq,paq);
          						
          					}
          					else{
          					//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
								Nbloq=buffer[2];
          						Nbloq=(Nbloq<<8)+buffer[3];
          						printf("\nExito al recibir bloque: %d",Nbloq);
          						fread(buffer+4,1,tamr-4,destino);
								tammsj=estructuraBlock(Nbloq,buffer,data);

          					}
        					
         				}
				}	
			}while(tamr ==516);
		}
	}
	close(udp_socket);
	return 0;
}
