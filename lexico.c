#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	// Si no hay parametros
	if(argc == 1){
		printf("Error: Faltan par\240metros.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}
	// Si solo hay un parametro
	else if(argc == 2){
		printf("Error: Falta par\240metro.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}
	// Si hay demasiados parametros
	else if(argc > 3){
		printf("Error: Demasiados par\240metros.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}

	// Se declara archivo de entrada
	FILE * entryFile;

	// Se abre el archivo y se entrega el nombre ingresado como parametro
	entryFile = fopen(argv[1], "r");

	// Se comprueba si existe
	if(entryFile == NULL){
		printf("Error: El archivo de entrada no existe.\n");
		return 0;
	}

	// Se declara archivo de salida
	FILE * exitFile;

	// Se abre en modo lectura para comprobar si existe
	exitFile = fopen(argv[2], "r");

	// Se comprueba si existe
	if(exitFile != NULL){
		printf("Error: El archivo de salida ya existe.\n");
		return 0;	
	}
	else{
		exitFile = fopen(argv[2], "w");
		fprintf(exitFile, "Hola Como Estas\n");
	}
}