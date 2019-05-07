#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Word {
	char sentence[64];
	char word[64];
	int len;
} Word;

void removeSubstr (char *string, char *sub);

char reservedWordsAndEspecialSigns[128][32] = {"main", "auto", "extern", "register", "static", "break", "continue", "default", "do", "while", "if", "goto", "for", "else", "return", "sizeof", "short", "int", "unsigned", "long", "float", "char", "double", "==", "!=", "<=", ">=", "&&", "||", "<<", ">>", "++", "--", "(", ")", "{", "}", ",", ";", "*", "=", ":", "&", "-", "~", "|", "<", ">", "!", "?", "+", "/", "%", "^"};

char operatorsAndSigns[128][16] = {"(", ")", "{", "}", ",", ";", "*", "=", ":", "&", "-", "~", "|", "<", ">", "!", "?", "+", "/", "%", "^"};

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

	// Auxiliar para saber si encuentro una palabra reservada
	int foundReservedWord = 0;
	for (i = 0; i < wordCount; ++i)
	{
		for (j = 0; j < 32; j++){

			// Si es una palabra reservada
			if (strcmp(fileWords[i], reservedWordsAndEspecialSigns[j]) == 0) {
				int c = 0;

				// Se transoforma a mayusculas
				while(fileWords[i][c] != '\0') {
					if(fileWords[i][c] >= 'a' && fileWords[i][c] <= 'z') {
						fileWords[i][c] = fileWords[i][c] - 32;
					}
					c++;
				}

				// Se escribe en el archivo
				fprintf(exitFile, "%s\n", fileWords[i]);
				foundReservedWord = 1;
			}
		}
		if (foundReservedWord == 0) {
			int c = 0;
			Word* wordList;
			wordList = (Word*)malloc(sizeof(Word) * 64);
			for (j = 0; j < 33; j++) {
				if (strstr(fileWords[i], reservedWordsAndEspecialSigns[j]) != NULL) {
					strcpy(wordList[c].sentence, strstr(fileWords[i], reservedWordsAndEspecialSigns[j]));
					strcpy(wordList[c].word, reservedWordsAndEspecialSigns[j]);
					wordList[c].len = strlen(strstr(fileWords[i], reservedWordsAndEspecialSigns[j]));
					printf("Frase es: %s\n", wordList[c].sentence);
					printf("Palabra es: %s\n", wordList[c].word);
					printf("Largo es: %i\n", wordList[c].len);
					if(j >= 23) {
						removeSubstr(fileWords[i], wordList[c].word);
					}
					c++;
				}
			}
			int count = 0;
			int wordLen = strlen(fileWords[i]);
			while(fileWords[i][count] != '\0') {
				for(k = 0; k < 21; k++) {
					if(fileWords[i][count] == operatorsAndSigns[k][0]) {
						printf("ENTREEEE AQUI\n");
						printf("LA FRASE ES: %s\n", fileWords[i]);
						strcpy(wordList[c].sentence, fileWords[i]);
						strcpy(wordList[c].word, operatorsAndSigns[k]);
						printf("El signo es %s\n", wordList[c].word);
						wordList[c].len = wordLen - count;
						c++;
						break;
					}
				}
				count++;
			}
			//c--;
			for(k = 0; k < c; k++) {
				for(h = k + 1; h < c; h++) {
					if(wordList[h].len > wordList[k].len) {
						Word aux = wordList[k];
						wordList[k] = wordList[h];
						wordList[h] = aux;
					}
				}
			}
			char auxSentence[64];
			strcpy(auxSentence, wordList[0].sentence);
			printf("c vale: %d\n", c);
			for(k = 0; k < c; k++) {
				printf("auxSentence: %s\n", auxSentence);
				printf("La palabra a escribir es: %s\n", wordList[k].word);
				int auxCount = 0;
				while(wordList[k].word[auxCount] != '\0') {
					if(wordList[k].word[auxCount] >= 'a' && wordList[k].word[auxCount] <= 'z') {
						wordList[k].word[auxCount] = wordList[k].word[auxCount] - 32;
					}
					auxCount++;
				}
				fprintf(exitFile, "%s\n", wordList[k].word);
				removeSubstr(auxSentence, wordList[k].word);
			}
		}
		foundReservedWord = 0;
	}

	// Se cierran los archivos
	fclose(entryFile);
	fclose(exitFile);
}

void removeSubstr (char *string, char *sub) {
    char *match = string;
    int len = strlen(sub);
    while ((match = strstr(match, sub))) {
        *match = '\0';
        strcat(string, match+len);
                match++;
    }
}
