#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    return false;
  }

  return true;
}

/* [Crear] la estructura Guerrero en forma de Nodo para que los Squads sean Listas Ligadas */
typedef struct Guerrero
  {
    int ejercitoID;
    int escuadronID;
    int guerreroID;
    int vidaBase;
    int danoBase;
    int faccionID;
    int cArsenal;
    int *pArsenal;
    struct Guerrero *previous;
    struct Guerrero *next;
  } Guerrero;

/* [Crear] estructura de los Squad's, posee el largo de la lista ligada para evitar despues tener que contar manualmente el largo */
/* Ademas de guardar al primer Guerrero y al último, para facilitar la creacion del juego. */
typedef struct Squad
  {
    int len;
    Guerrero *head;
    Guerrero *tail;
  } Squad;

int main(int argc, char **argv) {
  /* Si los parámetros del programa son inválidos */
  if (!check_arguments(argc, argv)) {
    /* Salimos del programa indicando que no terminó correctamente */
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE *input_file = fopen(argv[1], "r");

  /* Abrimos el archivo de output */
  FILE *output_file = fopen(argv[2], "w");

  /* Leemos la cantidad de escuadrones en el primer ejercito */
  int S_SQUADRON_COUNT;
  fscanf(input_file, "%d", &S_SQUADRON_COUNT);

  /* Leemos la cantidad de escuadrones en el segundo ejercito */
  int F_SQUADRON_COUNT;
  fscanf(input_file, "%d", &F_SQUADRON_COUNT);

  int Squads_lens[2] = {S_SQUADRON_COUNT, F_SQUADRON_COUNT};

  /* [Creación] de Entidades globales del Juego */
  Squad** war = calloc(2, sizeof(Squad*)); /* Array que contiene ambos ejercitos */
  war[0] = calloc(S_SQUADRON_COUNT, sizeof(Squad)); /* Ejercito de Shek */
  war[1] = calloc(F_SQUADRON_COUNT, sizeof(Squad)); /* Ejercito de Farquaad */

  /* Inicializar cada elemento del array de punteros */
  for (int i = 0; i < S_SQUADRON_COUNT; i++) {
      war[0][i].len = 0;
      war[0][i].head = NULL;
      war[0][i].tail = NULL;
  }

  for (int i = 0; i < F_SQUADRON_COUNT; i++) {
      war[1][i].len = 0;
      war[1][i].head = NULL;
      war[1][i].tail = NULL;
  }

  /* Leemos la cantidad de eventos */
  int N_EVENTS;
  fscanf(input_file, "%d", &N_EVENTS);

  /* String para guardar la instrucción actual*/
  char command[32];

  for (int event = 0; event < N_EVENTS; event++) {
    /* Leemos el evento */
    fscanf(input_file, "%s", command);

    if (string_equals(command, "ENLISTAR")) {
      /* [Instanciar] el guerrero y sus atributos */
      Guerrero *warrior = calloc(1, sizeof(Guerrero));
      int armyID; int squadID; int warriorID; int vida; int dano; int factionID; int c_arsenal; 

      /* [Leer] la linea de input y guardar la info en sus variables respectivas */
      fscanf(input_file, "%d %d %d %d %d %d %d", &armyID, &squadID, &warriorID, &vida, &dano, &factionID, &c_arsenal);

      /* [Asignar] las variables a los tributos del guerrero */
      warrior -> ejercitoID = armyID; warrior -> escuadronID = squadID; warrior -> guerreroID = warriorID; warrior -> vidaBase = vida;
      warrior -> danoBase = dano; warrior -> faccionID = factionID; warrior->cArsenal = c_arsenal; warrior->next = NULL; warrior->previous = NULL;

      /* [Guardo] el array de Arsenal en memoria dinámica para al final liberar y minimizar la memoria usada */
      int *Arsenal = calloc(c_arsenal, sizeof(int)) ; warrior -> pArsenal = Arsenal;

      /* [Iniciar] el array del Arsenal*/
      for (int i = 0; i < c_arsenal; i++){
        warrior->pArsenal[i] = 0;
      }

      /* [Guardar] el primer guerrero en la Lista Ligada */
      if (war[armyID][squadID].len == 0){
        war[armyID][squadID].head = warrior;
      }
      /* [Guardar] el guerrero creado a la cola de la Lista ligada */
      else if (war[armyID][squadID].len >= 1){
        /* [Aumenta] el largo de la Lista */
        warrior->previous = war[armyID][squadID].tail;
        war[armyID][squadID].tail->next = warrior;
      }

      war[armyID][squadID].len += 1;  
      war[armyID][squadID].tail = warrior;

      /* [Guardar] el output esperado */
      fprintf(output_file, "ENLISTADO %d %d\n", warriorID, squadID);
    }
    else if (string_equals(command, "EQUIPAR")) {
      /* [Instanciar] inputs */
      int bonusArtefacto; int armyID; int squadID; int warriorID;
      fscanf(input_file, "%d %d %d %d", &bonusArtefacto, &armyID, &squadID, &warriorID);

      /* [Buscar] al guerrero a buffar */
      Guerrero *p_buscado = war[armyID][squadID].head;
      while (p_buscado->guerreroID != warriorID){
        p_buscado = p_buscado->next; 
      }

      /* [Chequear] si es arsenal está completo */
      for (int i = 0; i < p_buscado->cArsenal; i++){
        if (p_buscado->pArsenal[i] == 0){
          p_buscado->pArsenal[i] = bonusArtefacto;
          fprintf(output_file, "ARTEFACTO %d ASIGNADO A %d\n",bonusArtefacto, warriorID);
          break;
        }
        if ((i == p_buscado->cArsenal -1) && (p_buscado->pArsenal[p_buscado->cArsenal - 1] > 0)){
          fprintf(output_file, "ARSENAL LLENO %d\n", warriorID);
        }
      }   
    }
    else if (string_equals(command, "CONTEO-CAMPAL")) {
      fprintf(output_file, "CONTEO-CAMPAL\n");

      int total_warriors = 0; int *contadores = calloc(3, sizeof(int));
      for (int army = 0; army < 2; army++){
        fprintf(output_file, "\tEJERCITO %d\n", army);

        /* [Iniciar] el contador de cada faccion */
        for (int i = 0; i < 3; i++){
          contadores[i] = 0;
        }
        
        int size_array = Squads_lens[army];
        /* [Contar] cada guerrero en su faccion correspontiente */
        for (int squad = 0; squad < size_array; squad++) {
          if (war[army][squad].head != NULL) {
            Guerrero *actual = war[army][squad].head;
            while (actual != NULL) {
              contadores[actual->faccionID] += 1;
              actual = actual->next;
            }
          }
        }
        
        /* [Imprimir] el numero total de guerreros por faccion y agregarlo al conteo total */
        for (int faction = 0; faction < 3; faction++){
          fprintf(output_file, "\t\tFACCION %d: %d\n", faction, contadores[faction]);
          total_warriors += contadores[faction];
        }
      }
      free(contadores);
      
      /* [Imprimir] el total de guerreros y liberar la memoria de contadores */
      fprintf(output_file, "TOTAL DE GUERREROS : %d\n", total_warriors);
    }
    else if (string_equals(command, "ORDEN-EJERCITO")) {
      /* [Identificar] el ejercito a imprimir */
      int armyID;
      fscanf(input_file, "%d", &armyID);

      /* [Determinar] el la cantidad de squads automaticamente */
      int size_array = Squads_lens[armyID];
      
      /* [Copiar] el ejercito para no cambiar las posiciones originales */
      Squad *copy_army = calloc(size_array, sizeof(Squad));
      memcpy(copy_army, war[armyID], sizeof(war[armyID][0]) * size_array);

      fprintf(output_file, "EJERCITO %d\n", armyID);

      /* [Buscar] los elementos mayores, y se imprime el elemento mayor*/
      for (int i = 0; i < size_array; i++){
        int cantidad_mayor = -1; int pos_mayor = 0;
        for (int j = 0; j < size_array; j++){
          if (cantidad_mayor < copy_army[j].len){
            cantidad_mayor = copy_army[j].len; pos_mayor = j;
          }
          else if (cantidad_mayor == copy_army[j].len){
            if (j < pos_mayor){
              pos_mayor = j;
            }
          }
        }
        /* [Imprimir] el elemento mayor y hacer el len de ese escuadron irrelevante */
        fprintf(output_file, "\tESCUADRON %d %d\n", pos_mayor, cantidad_mayor);
        copy_army[pos_mayor].len = -1;
      }

      /* [Liberar] la memoria del array copia, osea, que se elimina */
      free(copy_army);
      copy_army = NULL;
      
    }
    else if (string_equals(command, "DESERTAR")) {
      /* [Instanciar] inputs */
      int armyID; int squadID; int warriorID;

      /* [Guardar] inputs y crear variable Guerrero para la busqueda */
      fscanf(input_file, "%d %d %d", &armyID, &squadID, &warriorID);
      Guerrero *actual = war[armyID][squadID].head;
      war[armyID][squadID].len -= 1;

      /* Caso si es que el Desertor es el Cabecilla del Escuadron */
      if (actual->guerreroID == warriorID){
        if (war[armyID][squadID].len > 0){
          war[armyID][squadID].head = actual->next;
          actual->next->previous = NULL;
        }
        else{
          war[armyID][squadID].head = NULL;
          war[armyID][squadID].tail = NULL;
        } 
      }
      else{
        /* [Buscar] el guerrero anterior al que deserta y se asigna correctamente la secuencia de la Lista */
        while (actual->guerreroID != warriorID){
          actual = actual->next;
        }
        if (actual == war[armyID][squadID].tail){
          war[armyID][squadID].tail = actual->previous;
          actual->previous->next = NULL;
        }
        else{
          actual->previous->next = actual->next;
          actual->next->previous = actual->previous;
        } 
      }

      free(actual->pArsenal);
      free(actual);

      fprintf(output_file, "ABANDONA %d %d\n", warriorID, armyID);
    }
    else if (string_equals(command, "ATACA")) {
      /* [Leer] input */
      int armyID;
      fscanf(input_file, "%d", &armyID);

      int len_army = Squads_lens[armyID]; int mas_fuerte = -1; int index_fuerte = 0;

      /* [Buscar] el escuadron con mayor dano base sumado */
      for (int i = 0; i < len_army; i++){
        int c_actual = 0;
        Guerrero *actual = war[armyID][i].head;

        while (actual != NULL){
          c_actual += actual->danoBase;
          actual = actual->next;
        }
        if (c_actual > mas_fuerte){
          mas_fuerte = c_actual; index_fuerte = i;
        }
      }

      /* [Obtener] la referencia del Guerrero atacante */
      Guerrero *atacante = war[armyID][index_fuerte].head;

      /* [Identificar] el indice del ejercito enemigo */
      int enemigo = 0;
      if (armyID == 0){enemigo = 1;}

      /* [Buscar] el indice del escuadron con menos guerreros */
      int len_enemigo = Squads_lens[enemigo]; int menos_warriors = N_EVENTS; int index_menos = -1;
      for (int i = 0; i < len_enemigo; i++){
        if ((menos_warriors > war[enemigo][i].len) && (war[enemigo][i].len > 0)){
            menos_warriors = war[enemigo][i].len; index_menos = i;
          }
      }

      /* [Obtener] la referencia del Guerrero defensor */
      Guerrero *defensor = war[enemigo][index_menos].head;

      /* [Buscar] el artefacto con mas daño bonus */
      int dano_atk = atacante->danoBase;
      int artefacto_mejor = 0; int index_artefacto = 0;
      for (int i = 0; i < atacante->cArsenal; i++){
        if (atacante->pArsenal[i] > artefacto_mejor){
          artefacto_mejor = atacante->pArsenal[i], index_artefacto = i;
        }
      }
      
      /* [Calcular] daño final a hacer */
      int buffs[3] = {5, 10, 15};

      dano_atk += artefacto_mejor;
      dano_atk += buffs[atacante->faccionID];

      /* [Aplicar] daño al Guerrero defensor */
      defensor->vidaBase -= dano_atk;

      /* [Eliminar] artefacto usado */
      if (artefacto_mejor > 0){
        atacante->pArsenal[index_artefacto] = 0;
      }

      /* [Guardar] el output antes de cambiar los datos */
      fprintf(output_file, "ATACA %d %d\n", atacante->guerreroID, defensor->guerreroID);

      /* [Cambiar] la posición del Guerrero atacante */
      if (war[armyID][index_fuerte].len > 1){
        war[armyID][index_fuerte].head = atacante->next;
        atacante->next->previous = NULL;

        war[armyID][index_fuerte].tail->next = atacante;
        atacante->previous = war[armyID][index_fuerte].tail;
        war[armyID][index_fuerte].tail = atacante;
        atacante->next = NULL;
      }

      /* [Verificar] la vida del defensor */
      if (defensor->vidaBase <= 0){
        if (war[enemigo][index_menos].len == 1){
          war[enemigo][index_menos].len = 0;
          war[enemigo][index_menos].head = NULL;
          war[enemigo][index_menos].tail = NULL;
        }
        else if (war[enemigo][index_menos].len > 1){
          war[enemigo][index_menos].head = defensor->next;
          war[enemigo][index_menos].head->previous = NULL;
          war[enemigo][index_menos].len -= 1;
        }


        free(defensor->pArsenal);
        free(defensor);
      }
    }
    else if (string_equals(command, "TRAICIONAR")) {
      /* [Leer] input */
      int armyID; int squadID; int warriorID; int oponenteID = 0;
      fscanf(input_file, "%d %d %d", &armyID, &squadID, &warriorID);

      if (armyID == 0){oponenteID = 1;}

      /* [Buscar] el indice del escuadron con mas guerreros */
      int len_enemigo = Squads_lens[oponenteID]; int mas_warriors = -1; int index_mas = -1;
      for (int i = 0; i < len_enemigo; i++){
        if (mas_warriors < war[oponenteID][i].len){
          mas_warriors = war[oponenteID][i].len; index_mas = i;
        }
      }

      Guerrero *actual = war[armyID][squadID].head;

      /* Caso si el traidor es el único del escuadron */
      if (war[armyID][squadID].len == 1){
        /* [Resetar] el escuadron */
        war[armyID][squadID].head = NULL;
        war[armyID][squadID].tail = NULL;
      }
      /* Caso si es que el Traidor es el Cabecilla del Escuadron */
      else if (actual->guerreroID == warriorID){
        war[armyID][squadID].head = actual->next;
        actual->next->previous = NULL;
      }
      /* Otro caso */
      else {
        while (actual->guerreroID != warriorID){
          actual = actual->next;
        }
        if (war[armyID][squadID].len == 2){
          war[armyID][squadID].tail = war[armyID][squadID].head;
          war[armyID][squadID].head->next = NULL;
        }
        else if (actual == war[armyID][squadID].tail){
          war[armyID][squadID].tail = actual->previous;
          actual->previous->next = NULL;
        }
        else{
          actual->previous->next = actual->next;
          actual->next->previous = actual->previous; 
        }        
      }

      /* [Agregar] el traidor al ejercito enemigo */
      war[oponenteID][index_mas].tail->next = actual;
      actual->previous = war[oponenteID][index_mas].tail;
      war[oponenteID][index_mas].tail = actual;

      actual->next = NULL;
      actual->ejercitoID = oponenteID;
      actual->escuadronID = index_mas;

      war[armyID][squadID].len -= 1;
      war[oponenteID][index_mas].len += 1;

      fprintf(output_file, "TRAICION %d\n", armyID);
    }
    else if (string_equals(command, "ALTA-TRAICION")) {
      /* [Leer] input */
      int armyID; int squadID; int warriorID; int oponenteID = 0;
      fscanf(input_file, "%d %d %d", &armyID, &squadID, &warriorID);

      if (armyID == 0){oponenteID = 1;}

      /* [Buscar] el indice del escuadron con mas guerreros */
      int len_enemigo = Squads_lens[oponenteID]; int mas_warriors = 0; int index_mas = -1;
      for (int i = 0; i < len_enemigo; i++){
        if (mas_warriors < war[oponenteID][i].len){
          mas_warriors = war[oponenteID][i].len; index_mas = i;
        }
      }

      /* [Encontrar] al guerrero que traiciona */
      Guerrero *actual = war[armyID][squadID].head;
      while (actual->guerreroID != warriorID){
        actual = actual->next;
      }
      /* [Cortar] la conección con el escuadrón antiguo */
      if (actual == war[armyID][squadID].head){
        /* [Resetar] el escuadron */
        war[armyID][squadID].head = NULL;
        war[armyID][squadID].tail = NULL;
      }
      else{
        actual->previous->next = NULL;
        war[armyID][squadID].tail = actual->previous;
      }

      /* [Anexar] al primer guerrero traidor a la cola del escuadron enemigo */
      war[oponenteID][index_mas].tail->next = actual;
      actual->previous = war[oponenteID][index_mas].tail;

      /* [Agregar] todos los seguidores */
      while (actual->next != NULL){
        actual->ejercitoID = oponenteID;
        actual->escuadronID = index_mas;

        war[armyID][squadID].len -= 1;
        war[oponenteID][index_mas].len += 1;

        actual = actual->next;
      }

      /* [Actualizar] la cola del ejercito enemigo */
      actual->ejercitoID = oponenteID;
      actual->escuadronID = index_mas;

      war[armyID][squadID].len -= 1;

      war[oponenteID][index_mas].tail = actual;
      war[oponenteID][index_mas].len += 1;

      
      
      fprintf(output_file, "ALTA-TRAICION %d\n", armyID);
    }
  }

  /*  [Por implementar] Liberamos nuestras estructuras */
  for (int army = 0; army < 2; army++) {
    int len_squads = Squads_lens[army];
    for (int squad = 0; squad < len_squads; squad++){
      Guerrero *actual = war[army][squad].head;
      while (actual != NULL) {
        Guerrero *temp = actual;
        actual = actual->next;
        free(temp->pArsenal);
        free(temp);
      }
    }
    free(war[army]);
  }
  
  free(war); 
  
  fclose(input_file);
  fclose(output_file);

  return 0;
}
