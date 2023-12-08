## DOCUMENTO DE DISEÑO T0 IIC2133

- Nombre Alumno: Matías Pineda
- Email Alumno: matias.pineda@uc.cl

### Análisis de enunciado:
---
Primero se entregará un archivo .txt donde al principio estará la cantidad de escuadrones de cada ejército, luego se entregan los eventos con su identificador al principio.

* ENLISTAR: Se ingresa un Guerrero a su ejército y escuadrón correspondiente, si es el primer guerrero en su escuadrón se debe asignar como cabecilla, en caso contrario se debe asignar a la cola del último guerrero enlistado de su escuadrón.

* EQUIPAR: Se agrega el daño de un artefacto al guerrero, hay que chequear si es que su arsenal está lleno.

* CONTEO-CAMPAL: Hay que imprimir las cantidad de guerreros de cada facción por ejército e imprimir la cantidad total de guerreros.

* ORDEN-EJERCITO: Hay que imprimir una lista ordenada de los escuadrones del ejército pedido según su cantidad de guerreros.

* DESERTAR: Un guerrero abandona de su escuadrón, hay que eliminarlo sin quebrar la continuidad del escuadrón.

* ATACA: Se debe identificar al cabecilla del escuadrón más fuerte del ejercito atacante quién va pelear contra el cabecilla del escuadrón con menos guerreros del ejercito defensor. Cuando ataca se debe sumar el daño base, el artefacto con mayor bonus y un buff dependiendo de su facción, si el daño es mayor o igual a la vida del defensor este es eliminado, en caso contrario se debe dejar la vida restante del defensor. Una vez hecho el ataca se debe eliminar el artefacto usado y el atacante debe ser movido a la cola de su escuadrón siempre y cuando haya más de un guerrero es este.

* TRAICIONAR: El guerrero dado se va hacia el escuadrón enemigo con más guerreros, se debe eliminar del escuadrón original y puesto en la cola del nuevo escruadrón.

* ALTA-TRAICION: El lo mismo que el traicionar, pero ahora traicionan todos los guerreros desde el guerrero dado hasta la cola de su escuadrón. Deben ser agregados al escuadrón enemigo con más guerreros en el mismo orden en el que estaban originalmente.

### Planificación de la solución
---

Tipos de `STRUCT'S` usadas:
1. `Guerrero`: Es un `nodo` de una Lista Ligada, presenta los parámetros dados en ENLISTAR y tres más: `*pArsenal` va a ser un puntero hacia el `Array` donde se van a guardar los Artefactos; `*previous` va a ser un puntero hacia el nodo anterior, si es que es el primero el puntero será `NULL`; `*next` va a ser un puntero hacia el siguiente nodo, si es que es el último el puntero será `NULL`. 
2. `Squad`: Es una estructura que representará a los escuadrones, tiene como parámetro el `len`, osea, la cantidad de guerreros en el escuadrón; `*head` un puntero hacia el nodo del cabecilla del escuadrón; `*tail` un puntero hacia el nodo de la cola del escuadrón. 

Cada ejército será un `Array` de tamaño `SQUADRON_COUNT` correspondiente que almacenará `structs` tipo `Squad`.

Los ejércitos serán guardados en un `Array` de tamaño 2 llamado `war`. Además, será creado un `Array` que guarde los `SQUADRON_COUNT` de ambos ejércitos.

Eventos:
1. ENLISTAR: Todos los guerreros tipo `Guerrero` se guardan con `calloc` y se guardan los parámetros dados en el `input_file`. Se crea un `Array` de tamaño `c_arsenal` para almacenar los artefactos del guerrero y se guarda su puntero en el parámetro `pArsenal`. 

    Si el escuadrón tiene como `len` igual a 0, significa que el escuadrón está vació, por lo que el `Guerrero` creado pasa a ser el Cabecilla del Escuadrón. En otro caso, se debe conectar la cola y el `Guerrero`.

    Finalmente, el `Guerrero` pasa a a ser la cola del escuadrón. Y el `len` debe aumentar en 1.

2. EQUIPAR: Se crea un puntero auxiliar quien en primera instacia apunta al cabecilla del escuadrón dado en el input, si el puntero no apunta al `Guerrero` dado en el input, entonces se actualiza al `Guerrero` siguiente de la Lista Ligada.

    Ya identificado el `Guerrero` a equipar, se debe buscar casilla a casilla del `Array` guardado en `pArsenal`, si una casilla es igual a 0 significa que está vacío por lo que se guarda en esa posición el bonusArtefacto dado. Si ya llega al final del `Array` y este está ocupado significa que no se pueden agregar más artefactos.

3. CONTEO-CAMPAL: Se crea un `Array` de tamaño 3, que van a almacenar la cantidad de `Guerreros` por facción, de modo que en la posición 0, esté el contador de los `Guerreros` de facción 0. También se crea una variable que va a guardar la cantidad total de `Guerreros`.

    Se recorre la Lista Ligada de cada escuadrón, `Guerrero` a `Guerrero` y se suma 1 al contador correspondiente hasta llegar a la cola del escuadrón.

    Ya contado todos los guerreros del ejército, se imprime las facciones y se suman los contadores al total de `Guerreros`.

    Finalmente, se imprime el toatal de `Guerreros` y se libera el `Array` con los contadores.

4. ORDEN-EJERCITO: Se crea una copia del éjercito pedido, se hace dos `for's`: el primero representa a que debo impricar a todos los escuadrones una vez y el segundo es para buscar el escuadrón con más `Guerreros`. 

    Una vez identificado el escuadrón mayor, se imprime y se cambia el parámetro `len` a -1, para que en la siguente iteración no se repita.

    Fintalmente, se libera la copia.

5. DESERTAR: Se crea un puntero auxiliar que primero apunta al cabecilla del escuadrón pedido.

    Si el desertor es el cabecilla, se ve si es el único del escuadron entonces el escuadron se resetea, en caso contrario el cabecilla es el siguiente del desertor.

    Si el desertor no es el cabecilla, se actualiza el puntero hasta llegar al desertor. Si el desertor es la cola del escuadrón entonces se actualiza la cola con el `previo` del desertor, en caso contrario se conecta el anterior del desertor con el siguiente del desertor.

    Finalmente, se libera el `Array pArsenal` del desertor y el desertor en sí.  

6. ATACA: Para buscar al escuadron con mayor dano base sumado del ejercito dado se crea dos variables que van a almacenar el daño sumado mayor y el índice correspondiente, se hace un `for`, se crea un puntero auxiliar y una variable que almacena la cantidad total de `Guerreros`. Se recorre toda la Lista Ligada y se suma el daño base de cada `Guerrero`. Luego se compara el daño sumado calculado con el daño sumado mayor almacenado fuera del `for`, si el daño sumado calculado es mayor, se actualiza la variable y el índice.

    Para encontrar el escuadron con menos guerreros del ejercito contrario se crea dos variables que van a almacenar la cantidad de `Guerreros` menor y su índice correspondiente, se hace un `for` y se compara si el escuadrón `i` tiene menor `len` a la cantidad de `Guerreros` menor y si este es mayor a 0, si es así se actualiza la cantidad de `Guerreros` menor y su índice.

    Ya obtenidas las referencias al atacante y al defensor, se crea una variable que va a almacenar el daño total que se va a hacer, al principio será igual al daño base del atacante, luego se itera por cada casilla del `Array` de `pArsenal` buscando el artefacto mayor.

    Al dano total, se le suma el artefacto mayor y el buff correspondiente a la facción del `Guerrero`.

    Si el bonus del artefacto fué distinto de 0 significa que se usó, por lo que se setea denuevo a 0.

    Luego, se cambia la posición del Atacante a la cola del escuadrón dejando como cabecilla al siguiente del atacante.

    Finalmente se verifica la vida del defensor, si este es mayor o igual a su vida se debe liberar y seterar el escuadron como corresponda.

7. TRAICIONAR: Para buscar el escuadron con mas guerreros se hace el mismo proceso que se hizo en ATACA, con la diferencia que ahora se va a comparar si el `len` del escuadrón `i` es mayor a la variable con mayor cantidad de `Guerreros`.

    Ya 
8. ALTA-TRAICION:

### Justificación
---