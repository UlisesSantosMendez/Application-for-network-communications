//Copiar un archivo en modo binario (el modo eficiente que incluye los archivos de texto)
//En el modo binario se lee el byte (0 a 255) y no solo el carácter ASCII (de 0 a 127)
//Como en el modo texto que solo se lee el carácter ASCII y no se puede leer el byte completo 

#include<stdio.h>

int main()
{
	int bandera=0,tam=512,tam2;
	//se declaran las variables de tipo archivo, que son apuntadores
	FILE *origen,*destino;
	//se declaran las variables para poner el nombre del archivo de entrada y salida
	char cadentrada[20],cadsalida[20];
	char buffer[512];
	char car;
	//Se solicita el nombre del archivo a copiar
	printf("\nInserta el nombre del archivo origen: ");
	gets(cadentrada);
	//Se solicita el nombre del archivo donde se copiara
	printf("\nInserta el nombre del archivo destino: ");
	gets(cadsalida);
	//Se abre el archivo a copiar en modo lectura binaria "read Binary"
	origen=fopen(cadentrada,"rb");
	//Se abre el archivo donde se copiara en modo escritura binaria "write Binary"
	destino=fopen(cadsalida,"wb");
	//Se inicia la copia del archivo Byte a Byte
	/*while(feof(origen)==0) //el puntero archivo de origen se compara con el fin de archivo
	{                      //mientras regrese el valor cero no es el fin del archivo
		//Se lee el primer byte del archivo de origen (el puntero se incrementa en uno) 
		car=fgetc(origen); //y se asigna a la variable car 
		//El byte en la variable car se copia al archivo destino (el puntero se incrementa en uno)
		fputc(car,destino);
	}*/
	while(1)
	{
	tam=fread(buffer,1,512,origen);
	tam2=fwrite(buffer,1,tam,destino);
	printf("\nsal: %d, ent: %d",tam,tam2);
	if(tam<512)
	  break;	
	}
	
	//Se cierran los dos archivos
	fclose(origen);
	fclose(destino);
	return 0;
}
