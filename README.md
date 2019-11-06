# Canvas
Proyecto de estudiantes de la cátedra Algoritmos y Estructura de Datos de la Universidad Tecnológica Nacional, Facultad Regional Santa Fe, para brindar un entorno gráfico sencillo para el desarrollo de proyectos académicos.
<br>
Basado en el funcionamiento de Processing, permite dibujar en la pantalla utilizando como motor gráfico de fondo OpenGL.
<br>
La librería no tiene dependencias fuera de OpenGL (Ya viene en los compiladores estándares).
<br><br>
## Para empezar
Primero que nada, tenemos que incluir la librería.
```
#include "Canvas.h" //Replace with yout path lo library
```
Después ya estamos en condiciones de definir las dos funciones que nos permitirán dibujar. La ejecución de estas funciones es manejada por la librería, solo debemos definirlas para indicarle a Canvas.h qué queremos que hagan.
<br>
Por su parte setUp se ejcuta una única vez al inicializar la ventana. Toma como parametro por referencia un tipo de dato abstracto llamado Canvas en donde se guardan varias de las variables que necesita la librería para trabajar. Por ahora, las variables mas importantes son width y height, en donde podemos especificar el ancho y el alto de la ventana (en pixeles).
```
int setUp(Canvas &canvas)
{
    canvas.width = 500;
    canvas.height = 500;
    return 1;
}
```
Ya con la función setUp definida, pasamos a definir el metodo en el que le diremos a la librería que deseamos dibujar. Este toma como parametro un double time en el que tenemos el tiempo en segundos desde el último frame. Si bien no es necesario usarlo, nos sirve para sujetar nuestras animaciones a un tiempo real, y no a la velocidad del procesador de nuestra computadora.
```
int loop(double time)
{
    fillRect(0.15*WIDTH,0.15*HEIGHT,0.7*WIDTH,0.7*HEIGHT,0xffffff); //Imprimimos un cuadrado blanco con sus coordenadas x e y, y su  ancho y alto.
    write("HOLA MUNDO",0.15*WIDTH+10,0.15*HEIGHT+10,10,0xff0000); //Imprimimos dentro del cuadrado 'HOLA MUNDO' en Rojo.
    return 1;
}
```
Ya casi.... ahora solo falta decirle a nuestro compilador o IDE (Zinjai por ejemplo) que estamos usando openGL y debe linkear sus funciones a nuestro programa. (No tenes que instalar nada, OpenGL ya viene instalado en tu compilador).
Para hacerlo en Zinjai, tenes que ir a ->Run->Configure->Extra arguments for compiler => Y en el textbox agrega: "-l gdi32 -l opengl32"
<br>
<br>
Listo :wink: ya podes compilar y ejecutar tu programa. Deberías ver algo como esto...
<br>
![Contribution guidelines for this project](GettinStarted.PNG)
