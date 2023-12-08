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

typedef struct Tile {
  int id;
  int x;
  int y;
  char up;
  char right;
  char left;
  char down;
} Tile;


// Funcion que se encarga ver si una tile en cierta posicion encaja con sus vecinos
bool Check_Neighbors(struct Tile ***Mapa, struct Tile *tile, int x, int y, int max_x, int max_y){
  // Chequear arriba
  if ((y - 1 >= 0) && (Mapa[x][y - 1]->id != -1) && (Mapa[x][y - 1]->down != tile->up)) {
    return false;
  }

  // Chequear abajo
  if ((y + 1 < max_y) && (Mapa[x][y + 1]->id != -1) && (Mapa[x][y + 1]->up != tile->down)) {
    return false;
  }

  // Chequear izquierda
  if ((x - 1 >= 0) && (Mapa[x - 1][y]->id != -1) && (Mapa[x - 1][y]->right != tile->left)) {
    return false;
  }

  // Chequear derecha
  if ((x + 1 < max_x) && (Mapa[x + 1][y]->id != -1) && (Mapa[x + 1][y]->left != tile->right)) {
    return false;
  }
  
  return true;
}

// Funcion que se encarga de ver si una tile se puede insertar
bool Check_Insertable(struct Tile ***Mapa, int x, int y, int max_x, int max_y) { 
  if ((x >= 0) && (x < max_x)) {
    if ((y >= 0) && (y < max_y)) {
      if (Mapa[x][y]->id == -1) {
        return true;
      }
    }
  }
  return false;
}

// Funcion principal
struct Tile*** Solve(struct Tile ***Mapa, int x, int y, int max_x, int max_y, struct Tile **tiles, int tamano_tiles) {
  if (x == max_x) {
    x = 0;
    y++;
  }
  if (y == max_y) {
      return Mapa;
  }
  
  if (Check_Insertable(Mapa, x, y, max_x, max_y)) {
    for (int i = 0; i < tamano_tiles; i++) {
      if (Check_Neighbors(Mapa, tiles[i], x, y, max_x, max_y)) {
        *Mapa[x][y] = *tiles[i];
        if (Solve(Mapa, x + 1, y, max_x, max_y, tiles, tamano_tiles) != NULL) {
          return Mapa;
        } 
      }
    }
    Mapa[x][y]->id = -1;
    return NULL;
  }

  return Solve(Mapa, x + 1, y, max_x, max_y, tiles, tamano_tiles); 
}

int main(int argc, char** argv) {
    check_arguments(argc, argv);

    /* NO MODIFICAR */
    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    /* NO MODIFICAR */

    // Tamano del mapa
    int width; int height;
    fscanf(input_file, "%d", &width);
    fscanf(input_file, "%d", &height);

    // Cantidad de tiles
    int sizeSet;
    fscanf(input_file, "%d", &sizeSet);

    char input[255];

    /* [Array] que guardará las tiles */
    struct Tile **tiles = calloc(sizeSet, sizeof(struct Tile*));

    /* [Guardar] las tiles dadas en el input */
    for (int i = 0; i < sizeSet; i++) {
      struct Tile *tile = calloc(1, sizeof(struct Tile));
      tile->id = i;

      // Arriba
      fscanf(input_file, "%s", input);
      tile->up = *input; 

      // Derecha
      fscanf(input_file, "%s", input);
      tile->right = *input; 
      
      // Abajo
      fscanf(input_file, "%s", input);
      tile->down = *input; 

      // Izquierda
      fscanf(input_file, "%s", input);
      tile->left = *input; 

      tiles[i] = tile;
    }

    /* [Array] del mapa, e iniciarlo con tiles nulas */
    struct Tile ***output = calloc(width, sizeof(struct Tile**));
    for (int i = 0; i < width; i++) {
      struct Tile **col = calloc(height, sizeof(struct Tile*));
      for (int j = 0; j < height; j++) {
        struct Tile *tile = calloc(1, sizeof(struct Tile));
        tile->id = -1;
        tile->up = 'a';
        tile->right = 'a';
        tile->down = 'a';
        tile->left = 'a';
        col[j] = tile;
      }
      
      output[i] = col;
    }
    
    // Cantidad de tiles iniciales que tenemos de referencia
    int num_ref;
    fscanf(input_file, "%d", &num_ref);

    /* [Guardar] las coordenadas de las tiles de referencia */
    int x; int y; int id_value;
    for (int i = 0; i < num_ref; i++) {
      fscanf(input_file, "%d %d %d", &x, &y, &id_value); 

      output[x][y]->id = tiles[id_value]->id;

      output[x][y]->up = tiles[id_value]->up;
      output[x][y]->right = tiles[id_value]->right;
      output[x][y]->down = tiles[id_value]->down;
      output[x][y]->left = tiles[id_value]->left;

      output[x][y]->x = x;
      output[x][y]->y = y;
    }
    
    // Se llama a resolver el problema
    int criterio = 1; struct Tile ***output_final;
    for (int i = 0; i < width; i++) {
      for (int j = 0; i < height; i++) {
        if (output[i][j]->id == -1) {
          output_final = Solve(output, i, j, width, height, tiles, sizeSet);
          criterio = 0;
          break;
        }
      }
      if (criterio == 0) {
        break;
      }
    }

    /* Escribir el archivo output */
    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        fprintf(output_file, "%d ", output_final[i][j]->id);
      }
      fprintf(output_file, "\n");
    }
    
    // Liberar memoria
    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        free(output[i][j]);
      }
    }
    
    for (int i = 0; i < width; i++) {
      free(output_final[i]);
    }
    free(output_final);

    for (int i = 0; i < sizeSet; i++) {
      free(tiles[i]);
    }
    free(tiles);
    

    fclose(input_file);
    fclose(output_file);
    return 0;
}
