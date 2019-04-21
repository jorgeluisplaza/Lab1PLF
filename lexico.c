#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char reservedWordsAndEspecialSigns[128][32] = {"main", "auto", "extern", "register", "static", "break", "continue", "default", "do", "while", "for", "goto", "if", "else", "return", "sizeof", "short", "int", "unsigned", "long", "float", "char", "double", "==", "!=", "<=", ">=", "&&", "||", "<<", ">>", "++", "--"};

char operatorsAndSigns[128][16] = {"(", ")", "{", "}", ",", ".", ";", "*", "=", ":", "&", "-", "~", "|", "<", ">", "!", "?", "+", "/", "%", "^"};


int main (int argc, char *argv[]) {

	// Si no hay parametros
	if (argc == 1) {
		printf("Error: Faltan par\240metros.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}
	// Si solo hay un parametro
	else if (argc == 2) {
		printf("Error: Falta par\240metro.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}
	// Si hay demasiados parametros
	else if (argc > 3) {
		printf("Error: Demasiados par\240metros.\n");
		printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
		return 0;
	}

	// Se declara archivo de entrada
	FILE * entryFile;

	// Se abre el archivo y se entrega el nombre ingresado como parametro
	entryFile = fopen(argv[1], "r");

	// Se comprueba si existe
	if (entryFile == NULL) {
		printf("Error: El archivo de entrada no existe.\n");
		return 0;
	}

	// Se declara archivo de salida
	FILE * exitFile;

	// Se abre en modo lectura para comprobar si existe
	exitFile = fopen(argv[2], "r");

	// Se comprueba si existe
	if (exitFile != NULL){
		printf("Error: El archivo de salida ya existe.\n");
		return 0;	
	}
	else {
		exitFile = fopen(argv[2], "w");
	}

	// Se declara la linea del archivo
	char line[1024];

	// Se declara la lista de palabras
	char fileWords[1024][64];

	// Contadores para los ciclos for
	int i, j, k, h, l;

	//Para contar la cantidad de lineas obtenidas
	int wordCount = 0;

	// Se recorre el archivo
	while (fgets(line, 1024, entryFile)) {
		char * word = strtok(line, " 	\n");
		while (word != NULL) {
			strcpy(fileWords[wordCount], word);
			wordCount++;
			word = strtok(NULL, " 	 \" \n  \0");
		}
	}
	int foundReservedWord = 0;
	for (i = 0; i < wordCount; ++i)
	{
		for (j = 0; j < 32; j++){
			if (strcmp(fileWords[i], reservedWordsAndEspecialSigns[j]) == 0) {
				int c = 0;
				while(fileWords[i][c] != '\0') {
					if(fileWords[i][c] >= 'a' && fileWords[i][c] <= 'z') {
						fileWords[i][c] = fileWords[i][c] - 32;
					}
					c++;
				}
				fprintf(exitFile, "%s\n", fileWords[i]);
				foundReservedWord = 1;
			}
		}
		if (foundReservedWord == 0){
			for (k = 0; k < 32; ++k) {
				for (h = 0; h < 32; h++) {
					if (fileWords[i][k] == '\0') {
						break;
					}
					else if (fileWords[i][k] == operatorsAndSigns[h][0]) {
						fprintf(exitFile, "%c\n", fileWords[i][k]);
					}
				}
			}
		}

		if (foundReservedWord == 0) {
			int wordsTemporalCount = 0;
			char temporalWords[128][32];
			char * wordAux = strtok(fileWords[i], "(){}");
			while (wordAux != NULL) {
            	strcpy(temporalWords[wordsTemporalCount], wordAux);
            	wordsTemporalCount++;
				wordAux = strtok(NULL, "(){}");
			}

			for (l = 0; l < wordsTemporalCount; l++) {
				for (j = 0; j < 32; j++) {
					if (strcmp(temporalWords[l], reservedWordsAndEspecialSigns[j]) == 0) {
						int c = 0;
						while(temporalWords[l][c] != '\0') {
							if(temporalWords[l][c] >= 'a' && temporalWords[l][c] <= 'z') {
							temporalWords[l][c] = temporalWords[l][c] - 32;
							}
						c++;
						}				
						fprintf(exitFile, "%s\n", temporalWords[l]);					
					}
				}
			}
		}
		foundReservedWord = 0;
	}

	// Se cierran los archivos
	fclose(entryFile);
	fclose(exitFile);
}