#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char reservedWords[128][32] = {"main", "auto", "extern", "register", "static", "break", "continue", "default", "double", "while", "if", "goto", "for", "else", "return", "sizeof", "short", "int", "unsigned", "long", "float", "char", "do"};

char * relationalOperators[10] = {"==", "!=", "<=", ">=", "&&", "||", "<<", ">>", "++", "--"};

char * operatorsAndSigns[21] = {"(", ")", "{", "}", ",", ";", "*", "=", ":", "&", "-", "~", "|", "<", ">", "!", "?", "+", "/", "%", "^"};

FILE * entryFile;

FILE * exitFile;

int foundLexicComponents(int argc, char * argv[]);
int recognizeErrors(int argc, char * argv[]);
int searchRelationalOperator(char * sentence, int wordPosition);
int searchOperatorOrSign(char * sentence, int wordPosition);
int searchReservedWord(char * sentence, int wordPosition);

int main (int argc, char *argv[]) {
	foundLexicComponents(argc, argv);
	return 0;
}

int foundLexicComponents(int argc, char * argv[]) {
		// Si no hay parametros
	if(!recognizeErrors(argc, argv)) {
		return 0;
	}

	// Se declara la linea del archivo
	char line[1024];

	// Se declara la lista de palabras
	char fileWords[1024][256];

	// Contadores para los ciclos for
	int i, j, h;

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
		h = 0;
		// Se cambia todo a minusculas
		while(fileWords[i][h] != '\0') {
			if(fileWords[i][h] >= 'A' && fileWords[i][h] <= 'Z') {
				fileWords[i][h] = fileWords[i][h] + 32;
			}
			h++;
		}

		for (j = 0; j < 23; j++){

			// Si es una palabra reservada
			if (strcmp(fileWords[i], reservedWords[j]) == 0) {
				int c = 0;

				// Se transforma a mayusculas
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

			// Se obtiene la frase a analizar
			char * sentence = fileWords[i];

			// Se calcula su largo
    		int sentenceLen = strlen(sentence);
    		int wordPosition = 0, foundLexicComponent = 0;

    		while(sentenceLen > wordPosition) {

    			// Variable para saber si se encontro un componente
        		foundLexicComponent = 0;

        		// Primero se analizar si hay un operador relacional
        		if(searchRelationalOperator(sentence, wordPosition)) {
        			sentence = sentence + 2;
        			wordPosition = wordPosition + 2;
        			foundLexicComponent = 1;
        		}

        		// Se analiza si hay un signo u operador
        		if(foundLexicComponent == 0){
					if(searchOperatorOrSign(sentence, wordPosition)) {
						sentence = sentence + 1;
						wordPosition = wordPosition + 1;
						foundLexicComponent = 1;
					}      			
        		}

        		// Se analiza si hay una palabra reservada
        		if(foundLexicComponent == 0){
        			int reservedWordLen = searchReservedWord(sentence, wordPosition);
        			if(reservedWordLen != 0) {
        				sentence = sentence + reservedWordLen;
        				wordPosition = wordPosition + reservedWordLen;
        				foundLexicComponent = 1;
        			}
        		}

        		// Si no encuentra nada, se avanza una sola posicion
        		if(foundLexicComponent == 0) {
        			wordPosition++;
        			sentence++;
        		}
    		}
		}
		foundReservedWord = 0;
	}

	// Se cierran los archivos
	fclose(entryFile);
	fclose(exitFile);
	return 1;
}

int recognizeErrors(int argc, char * argv[]) {
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

	entryFile = fopen(argv[1], "r");

	if (entryFile == NULL) {
		printf("Error: El archivo de entrada no existe.\n");
		return 0;
	}

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

	return 1;
}

int searchRelationalOperator(char * sentence, int wordPosition) {
	int j;

	for(j = 0; j < 10; j++) {

    	// Se obtiene de la variable global
    	char * relationalOperator = relationalOperators[j];

    	// Se compara cada variable con la posicion actual de la frase analizada
    	if(strncmp(sentence, relationalOperator, 2) == 0) {

        	// Se escribe en el archivo
        	fprintf(exitFile, "%s\n", relationalOperator);

        	// Se indica que se encontro un componente
        	return 1;
        }
    }
	return 0;
}

int searchOperatorOrSign(char * sentence, int wordPosition) {
	int j;

	for(j = 0; j < 21; j++) {

        char * operatorOrSign = operatorsAndSigns[j];

       	if(strncmp(sentence, operatorOrSign, 1) == 0) {

        	// Se escribe en el archivo
        	fprintf(exitFile, "%s\n", operatorOrSign);

        	// No es necesario seguir iterando
        	return 1;
        }
    }
    return 0; 
}

int searchReservedWord(char * sentence, int wordPosition) {
	int j;

    for(j = 0; j < 23; j++) {
        char * reservedWord = reservedWords[j];
        if(strncmp(sentence, reservedWord, strlen(reservedWord)) == 0) {

        	// En este caso se debe pasar a mayusculas antes de escribir
        	char * ch = (char*)malloc(1 + strlen(reservedWord));
        	char aux;
        	strcpy(ch, reservedWord);
        	int c = 0;

        	while (ch[c] != '\0') {
      			aux = ch[c];
      			if (aux >= 'a' && aux <= 'z')
         			ch[c] = ch[c] - 32;
      				c++;  
   			}

   			// Se escribe
   			fprintf(exitFile, "%s\n", ch);

        	// No es necesario seguir iterando
        	return strlen(reservedWord);
        }
    }
    return 0;
}

