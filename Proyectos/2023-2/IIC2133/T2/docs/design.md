# DOCUMENTO DE DISEÑO T0 IIC2133

- Nombre Alumno: Matías Armando Pineda
- Email Alumno: matias.pineda@uc.cl

## Análisis de enunciado

### Parte 1: Un mundo alternativo.

Se debe crear una especie de mapa a partir de piezas (`tiles`) inicialmente dispersas, según el tipo de valor que poseen. 

Al inicio se entrega en tamaño de el "mapa" que se debe entregar, luego se entrega la cantidad de tiles, donde los siguentes inputs son los valores de los bordes de cada tile. 

Finalmente, se entrega las tiles de referencia, osea, las coordenadas y el id de la tile, para así crear el mapa según estos puntos.

### Parte 2: Conteo de escuadrones

Se debe crear una forma de ingresar patrones de caracteres para después retornar la cantidad de veces que el patrón se ha ingresado.

La cadena de letras no tiene orden fijo, por lo que puede estar en cualquier posición para referirse al mismo patrón.

Se entregará una cantidad indefinida de patrones hasta que el archivo lea `"q"`. Cada vez que llegue un valor se debe registrar o sumar 1 al registro existente.

## Planificación de la solución

### Parte 1: Un mundo alternativo:

Primero se leerá todas las tiles que podemos usar, y se guardarán en un struct `Tile` que guardará su id, y los valores de sus lados.

Luego, se creará una matriz que será el Mapa a entregar, cada casilla del mapa será Nula por el momento.

Se leerá las tiles de Refencia que nos entrega el input y se agregarán en sus posiciones correspondientes.

Se llamará a la función `Solve` que se va a encargar de resolver el problema, esta función va a recibir el Mapa, la primera coordenada nula que tenga el mapa, el tamaño del mapa, las tiles y el tamaño del array de las tiles.

La función `Solve` será creada con la técnica `BackTracking`. De modo que irá rellenando el Mapa de izquierda a derecha y de arriba hacia abajo. Finalizará cuando la coordenada y de la iteración se salga del Mapa.

Primero, se verá si en la coordenada dada está vacía, en caso contrario continuerá a la siguiente coordenada. Si está vacía, va a buscar una tile en el array de tiles que se pueda insertar según los valores de las tiles vecinas, si es así la tile se agrega a la coordenada y se hace el llamado recursivo a la siguiente coordenada, en caso contrario se anula la tile que está en la coordenada y se retorna Nulo.

### Parte 2: Conteo de escuadrones

Se creará una tabla Hash que guardará en primer lugar el tamaño de los escuadrones, luego cada casilla no nula de la tabla apuntará a una lista ligada con cada secuencia ordenada, sin repeticion, de escuadrones. Los nodos de la lista ligada tendrán el parámetro `rep` que será cuantas veces se ha registrado el escuadrón y tendrá la secuencia ordenada alfabéticamente. 

Cuando se ingrese un escuadrón, se ordenará mediante `InsertionSort`. Se buscará primero el tamaño, si es que no existe se crea y se ingresa al array se tamanos con primer nodo el escuadrón dado, en caso se busca el escuadrón y se le suma 1 al parámetro `rep`.

Cuando se consulta por las veces que se ingreso el escuadrón, se busca el tamaño primero, si no existe el tamaño se retorna inmediatamente 0. Si existe, se busca el escuadrón si existe se retorna el valor esperado, si no se retorna 0. 

## Justificación

El tamano del array que guarda los tamaños de los escuadrones, es de 100, un número suficientemente grande para que no se tenga que crear un nuevo array, y como se crea mediante calloc su memoria después es liberada.

## Referencias (Del documento de diseño)

<!-- Enlace para bonus https://bonus.iic2133.org/redeem?code=eh32ixan1b8onef  -->
