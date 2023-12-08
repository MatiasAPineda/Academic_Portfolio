#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2)
{
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    exit(1);
  }

  return true;
}

typedef struct Escuadron {
  int rep;
  char *letras;
  struct Escuadron *next;
} Escuadron;

typedef struct Index {
  int value;
  struct Escuadron *head;
} Index;

typedef struct Tupla {
  struct Escuadron *hijo;
  struct Escuadron *padre;
} Tupla;

// Función para ordenar alfabéticamente el str
void sortstr(char* str, int len) {
  for (int i = 0; i < len - 1; i++) {
    for (int j = i + 1; j < len; j++) {
      if (str[i] > str[j]) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
      }
    }
  }
}

// Retorna el indice de la celda que tenga el valor o la primera celda vacía que encuentre
int Search_Tamano(struct Index **tamanos, int value, int tamano_arr){
  for (int i = 0; i < tamano_arr; i++) {
    if ((tamanos[i] == NULL) || (tamanos[i]->value == value)) {
      return i;
    }
  }
  return -1;
}

void addTamano(struct Index **tamanos, int value, int indice){
    struct Index *index = calloc(1, sizeof(struct Index));
    index->value = value;
    index->head = NULL;
    tamanos[indice] = index; 
}

// Funcion de búsqueda parecido a la busqueda en ABB
struct Tupla *Search_Squad(struct Escuadron *squad, struct Escuadron *padre, char string[]) {
  if ((squad == NULL) || (string_equals(squad->letras, string) == true)) {
    struct Tupla *tupla = calloc(1, sizeof(struct Tupla));
    tupla->hijo = squad;
    tupla->padre = padre;
    return tupla;
  }
  return Search_Squad(squad->next, squad, string);
}


void addSquad(struct Index *array, char string[]) {
  struct Tupla *tupla = Search_Squad(array->head, array->head, string);
  if (tupla->hijo == NULL) {
    struct Escuadron *squad = calloc(1, sizeof(struct Escuadron));
    squad->letras = calloc(strlen(string) + 1, sizeof(char));
    strcpy(squad->letras, string);
    squad->rep = 1;
    squad->next = NULL;
    if (tupla->padre != NULL) {
      tupla->padre->next = squad;
    }
    else {
      array->head = squad;
    }
    free(tupla);
    return;
  }
  tupla->hijo->rep += 1;
  free(tupla);
  return; 
}

int main(int argc, char** argv) {
    check_arguments(argc, argv);

    /* NO MODIFICAR */
    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    /* NO MODIFICAR */

    char input[255];
    int num_sec = 100;

    /* Array con los tamanos de los escuadrones dados, como no sabemos cuantos hay, sera de tamano num_sec que se pude modificar */
    struct Index **tamanos = calloc(num_sec, sizeof(struct Index*));
  
    while (!string_equals(input, "q"))
    {
      fscanf(input_file, "%s", input);
      if (string_equals(input, "+") == true) {
        int tam;
        fscanf(input_file, "%d", &tam);

        // Leer la secuencia
        char *string = calloc(tam, sizeof(char));
        for (int i = 0; i < tam; i++) {
          string[i] = '\0';
          char letra[1];
          fscanf(input_file, "%s", letra);
          string[i] = *letra;
        }
        sortstr(string, tam);

        // Buscar si el tamano ya se ha registrado
        int indice = Search_Tamano(tamanos,tam , num_sec);
        if (tamanos[indice] == NULL) { // tamano Nuevo
          addTamano(tamanos, tam, indice);
        }

        addSquad(tamanos[indice], string);
        free(string);
      }    
      else if (string_equals(input, "?") == true) {
        int tam;
        fscanf(input_file, "%d", &tam);

        // Leer la secuencia
        char *string = calloc(tam, sizeof(char));
        for (int i = 0; i < tam; i++) {
          string[i] = '\0';
          char letra[1];
          fscanf(input_file, "%s", letra);
          string[i] = *letra;
        }
        sortstr(string, tam);

        // Buscar si el tamano existe
        int indice = Search_Tamano(tamanos,tam , num_sec);
        if (tamanos[indice] == NULL) { // No existe
          fprintf(output_file, "%d\n", 0);
        }
        else {
          struct Tupla *tupla = Search_Squad(tamanos[indice]->head, tamanos[indice]->head, string);
          if (tupla->hijo == NULL) { // No existe
            fprintf(output_file, "%d\n", 0);
          }
          else {
            fprintf(output_file, "%d\n", tupla->hijo->rep);
          }
          free(tupla);
        }
        free(string);
      }
    }
    
    for (int i = 0; i < num_sec; i++) {
      if (tamanos[i] != NULL){
        struct Escuadron *actual = tamanos[i]->head;
        while (actual != NULL) {
          struct Escuadron *temp = actual;
          actual = actual->next;
          free(temp->letras);
          free(temp);
        }
        free(tamanos[i]);
      }
    }
    free(tamanos);
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}

