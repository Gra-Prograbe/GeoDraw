#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>


// Variables globales
bool modoBorrador = false; // Controla si el modo de borrado está activo

void exportadorConsola() {
    // Obtener el tama�o de la consola
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int width = csbi.dwSize.X;
    int height = csbi.dwSize.Y;

    // Buffer para almacenar el contenido de la consola
    CHAR_INFO *buffer = new CHAR_INFO[width * height];
    COORD bufferSize = { (SHORT)width, (SHORT)height };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT readRegion = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

    // Leer el contenido de la consola
    ReadConsoleOutput(hConsole, buffer, bufferSize, bufferCoord, &readRegion);

    // Abrir el archivo para guardar el contenido
    std::ofstream file("figuras.txt");
    if (!file) {
        std::cerr << "Error al abrir el archivo para escribir." << std::endl;
        delete[] buffer;
        return;
    }

    // Guardar el contenido en el archivo
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = x + y * width;
            char ch = buffer[index].Char.AsciiChar;
            // Reemplazar caracteres no imprimibles con un espacio en blanco
            if (ch == '\0') ch = ' ';
            file << ch;
        }
        file << std::endl;  // Nueva l�nea para cada fila
    }

    // Limpiar
    file.close();
    delete[] buffer;
    
  
  
}

void limpiarLinea() {
    std::cout << "\033[2K"; // Secuencia ANSI para limpiar toda la l�nea actual
    std::cout.flush(); // Forzar la limpieza inmediata en algunos sistemas
}

void gotoxy(int x, int y) {                     //  Definici�n de la funci�n gotoxy que recibe dos enteros x e y como coordenadas.
    COORD coord;                                //  Se declara una estructura COORD que almacenar� las coordenadas x e y.
    coord.X = x;                                //  Asigna el valor de x al campo X de la estructura coord.
    coord.Y = y;                                //  Asigna el valor de y al campo Y de la estructura coord.
    SetConsoleCursorPosition(                   //  Llama a SetConsoleCursorPosition para mover el cursor a la posici�n especificada.
        GetStdHandle(STD_OUTPUT_HANDLE),        //  Obtiene el handle de salida est�ndar (la consola) con GetStdHandle.
        coord                                   //  Pasa las coordenadas almacenadas en coord a SetConsoleCursorPosition.
    );                                          //  Cierra la llamada a SetConsoleCursorPosition.
}       


// Funci�n para borrar en la posici�n actual del cursor
void borrarEnPosicion(int x, int y) {
    gotoxy(x, y);
    std::cout << ' '; // Borra el car�cter en la posici�n del cursor
    gotoxy(x, y);     // Devuelve el cursor a la misma posici�n
}                                        //  Cierra la funci�n gotoxy.

void obtenerTamanoConsola(int &ancho, int &alto) {  //  Definici�n de la funci�n obtenerTamanoConsola que recibe dos referencias a enteros.
    CONSOLE_SCREEN_BUFFER_INFO csbi;               //  Se declara una estructura CONSOLE_SCREEN_BUFFER_INFO para obtener informaci�n de la consola.
    GetConsoleScreenBufferInfo(                    //  Llama a GetConsoleScreenBufferInfo para obtener los datos de la consola.
        GetStdHandle(STD_OUTPUT_HANDLE),           //  Obtiene el handle de la consola con GetStdHandle.
        &csbi                                      //  Pasa la direcci�n de csbi para que se llene con la informaci�n de la consola.
    );                                             //  Cierra la llamada a GetConsoleScreenBufferInfo.
    ancho = csbi.srWindow.Right -                  //  Calcula el ancho de la consola como la diferencia entre los bordes derecho e izquierdo.
            csbi.srWindow.Left + 28;               //  Ajusta el ancho sumando 28 (ajuste para un tama�o espec�fico).
    alto = csbi.srWindow.Bottom -                  //  Calcula la altura como la diferencia entre los bordes inferior y superior de la consola.
           csbi.srWindow.Top + 6;                  //  Ajusta la altura sumando 6 (ajuste para un tama�o espec�fico).
}                                                  //  Cierra la funci�n obtenerTamanoConsola.

void ajusteWrap(int &x, int &y, int ancho, int alto) {  //  Definici�n de la funci�n ajusteWrap que recibe referencias a x e y, y los l�mites de ancho y alto.
    if (x < 0) x = ancho - 1;                          //  Si x es menor que 0, se ajusta a ancho - 1 para envolver el cursor.
    if (x >= ancho) x = 0;                             //  Si x es mayor o igual a ancho, se ajusta a 0 para envolver el cursor.
    if (y < 0) y = alto - 1;                           //  Si y es menor que 0, se ajusta a alto - 1 para envolver el cursor.
    if (y >= alto) y = 0;                              //  Si y es mayor o igual a alto, se ajusta a 0 para envolver el cursor.
}                  



void mostrarMenu() {
    gotoxy(0, 0);
    std::cout << "Menu de Opciones:" << std::endl;
    std::cout << "0 - Limpiar toda la pantalla" << std::endl;
    std::cout << "1 - Dibujar Triangulo" << std::endl;
    std::cout << "2 - Dibujar Cuadrado" << std::endl;
    std::cout << "3 - Dibujar Rectangulo" << std::endl;
    std::cout << "4 - Dibujar Hexagono" << std::endl;
    std::cout << "5 - Dibujar Octagono" << std::endl;
    std::cout << "6 - Dibujar Rombo" << std::endl;
    std::cout << "7 - Exportar" << std::endl;
    std::cout << "O - Ocultar menu (May)" << std::endl;
    std::cout << "L - Modo Borrador (Activar/Desactivar)" << std::endl;
    std::cout << "ESC - Salir" << std::endl;
}


std::string colorToHex(WORD color) {
    switch (color) {
        case 0: return "#000000"; // Negro
        case 1: return "#0000AA"; // Azul
        case 2: return "#00AA00"; // Verde
        case 3: return "#00AAAA"; // Celeste Agua
        case 4: return "#AA0000"; // Rojo
        case 5: return "#AA00AA"; // Purpura
        case 6: return "#AA5500"; // Amarillo
        case 7: return "#AAAAAA"; // Gris claro
        case 8: return "#555555"; // Gris oscuro
        case 9: return "#5555FF"; //  Azul calro
        case 10: return "#55FF55"; // Verde claro
        case 11: return "#55FFFF"; // Celeste claro
        case 12: return "#FF5555"; // Rojo claro
        case 13: return "#FF55FF"; //P�rpura claro
        case 14: return "#FFFF55"; // Amarillo claro
        case 15: return "#FFFFFF"; // Blanco
        default: return "#000000";
    }
}

void exportarHTML() {
    // Obtener el tama�o de la consola
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int width = csbi.dwSize.X;
    int height = csbi.dwSize.Y;

    // Buffer para almacenar el contenido de la consola
    CHAR_INFO *buffer = new CHAR_INFO[width * height];
    COORD bufferSize = { (SHORT)width, (SHORT)height };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT readRegion = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

    // Leer el contenido de la consola
    ReadConsoleOutput(hConsole, buffer, bufferSize, bufferCoord, &readRegion);

    // Abrir el archivo para guardar el contenido
    std::ofstream file("figuras.html");
    if (!file) {
        std::cerr << "Error al abrir el archivo para escribir." << std::endl;
        delete[] buffer;
        return;
    }

    // Comenzar el archivo HTML
    file << "<html><body style='font-family:Courier; white-space:pre;'>";

    // Guardar el contenido en el archivo con colores
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = x + y * width;
            char ch = buffer[index].Char.AsciiChar;
            if (ch == '\0') ch = ' ';

            // Obtener color de fondo y texto
            WORD attributes = buffer[index].Attributes;
            WORD textColor = attributes & 0x0F;
            WORD bgColor = (attributes & 0xF0) >> 4;

            // Convertir colores a c�digo hexadecimal
            std::string textColorHex = colorToHex(textColor);
            std::string bgColorHex = colorToHex(bgColor);

            // Escribir el car�cter con estilo en HTML
            file << "<span style='color:" << textColorHex << "; background-color:" << bgColorHex << ";'>" << ch << "</span>";
        }
        file << "<br>";  // Nueva l�nea para cada fila
    }

    // Cerrar el archivo HTML
    file << "</body></html>";
    file.close();
    delete[] buffer;
    gotoxy(0, 1);
    std::cout << "Exportado" << std::endl;
    limpiarLinea();  // Limpia la l�nea actual
}

void ocultarMenu() {
    for (int i = 0; i < 13; ++i) { // Asumiendo que el men� tiene 12-13 l�neas
        std::cout << "\x1B[" << (i + 1) << ";0H" <<   "                                             ";
}
}

void establecerColor(int color) {                       //  Definición de la función establecerColor que recibe un entero color.
    SetConsoleTextAttribute(                            //  Llama a SetConsoleTextAttribute para cambiar el color de texto de la consola.
        GetStdHandle(STD_OUTPUT_HANDLE),                //  Obtiene el handle de salida estándar (la consola).
        color                                           //  Pasa el valor de color para cambiar el color del texto.
    );                                                  //  Cierra la llamada a SetConsoleTextAttribute.
}                                                       //  Cierra la función establecerColor.

int seleccionarColor() {                                //  Definición de la función seleccionarColor, que no recibe parámetros.
    int color;                                          //  Declara una variable entera color.
    gotoxy(0, 13);                                       //  Mueve el cursor a la posición (0, 9).
    std::cout << "Selecciona un color (1-15): ";                           //  Muestra el texto para solicitar el color al usuario.
	std::cin >> color;  

	
	if (color < 1 || color > 15) {                      //  Verifica si el color está fuera del rango permitido (1-15).
        color = 4;                                      //  Si está fuera de rango, asigna el color 4 por defecto.
   
	}
    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
    limpiarLinea();  // Limpia la l�nea actual
    return color;                                       //  Retorna el valor de color seleccionado.
  
}                                                       //  Cierra la función seleccionarColor.
     
	 
	
                                   
void dibujarCar(char c, int x, int y, int ancho, int alto) {  //  Definición de la función dibujarCar que recibe un carácter y coordenadas.
    ajusteWrap(x, y, ancho, alto);                           //  Llama a ajusteWrap para asegurar que las coordenadas estén dentro de la consola.
    gotoxy(x, y);                                            //  Mueve el cursor a la posición (x, y).
    std::cout << c;                                          //  Imprime el carácter c en la posición actual del cursor.
}                                                            //  Cierra la función dibujarCar.

void dibujarTriangulo(int x, int y, char c, int ancho, int alto, int tamano, bool apuntandoArriba) {  // Definición de dibujarTriangulo.
    for (int i = 0; i < tamano; ++i) {                        //  Bucle exterior para iterar sobre las filas del triángulo.
        for (int j = 0; j <= i * 2; ++j) {    
		//j significa el numero de caracteres, se va multiplicando *2 para que vaya aumentando
        //a medida que vaya bajando y asi forme la base	                                                   //  Bucle interior para iterar sobre las columnas de cada fila.
            int posY = apuntandoArriba ? y + i : y - i;       //  Calcula la posición vertical según si el triángulo apunta hacia arriba o abajo.
            dibujarCar(c, x - i + j, posY, ancho, alto);      //  Llama a dibujarCar para dibujar el carácter en la posición calculada.
        }
    }
} 

void dibujarCuadrado(int x, int y, char c, int ancho, int alto, int tamano) {  // Definición de la función dibujarCuadrado.
    for (int i = 0; i < tamano; ++i) {                        //  Bucle exterior para iterar sobre las filas del cuadrado.
        for (int j = 0; j < tamano; ++j) {                    //  Bucle interior para iterar sobre las columnas de cada fila.
            dibujarCar(c, x + j, y + i, ancho, alto);         // Llama a dibujarCar para dibujar cada carácter en la posición correspondiente.
        }
    }
}                                                            // Cierra la función dibujarCuadrado.

void dibujarRectangulo(int x, int y, char c, int ancho, int alto, int tamano) {  // Definición de la función dibujarRectangulo.
    int anchoRect = tamano * 4;                         //  Calcula el ancho del rectángulo como tamano * 4.
    for (int i = 0; i < tamano; ++i) {                  // Bucle exterior para iterar sobre las filas del rectángulo.
        for (int j = 0; j < anchoRect; ++j) {           // Bucle interior para iterar sobre las columnas de cada fila.
            dibujarCar(c, x + j, y + i, ancho, alto);   //  Llama a dibujarCar para dibujar el carácter en la posición correspondiente.
        }
    }
}                                                      //  Cierra la función dibujarRectangulo.

// Función para dibujar un hexágono
void dibujarHexagono(int x, int y, char c, int ancho, int alto, int tamano) {  //  Definición de la función dibujarHexagono que recibe las coordenadas, un carácter, y el tamaño.
    for (int i = 0; i < tamano; ++i) {                              //  Bucle exterior para integrar sobre las filas del hexágono.
        for (int j = 0; j < tamano + i * 2; ++j) {                 //  Bucle interior para determinar cuántos caracteres dibujar en cada fila.
            dibujarCar(c, x - i + j, y + i, ancho, alto);         //  Llama a dibujarCar para dibujar el carácter en la posición calculada.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
    for (int i = tamano - 1; i >= 0; --i) {                        //  Bucle para dibujar la parte inferior del hexágono (desde el tamaño - 1 hasta 0).
        for (int j = 0; j < tamano + i * 2; ++j) {                 //  Bucle interior para determinar cuántos caracteres dibujar en cada fila inferior.
            dibujarCar(c, x - i + j, y + (tamano * 2 - 1 - i), ancho, alto); //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
}                                                                   //  Cierra la función dibujarHexagono.
// Función para dibujar un octágono
void dibujarOctagono(int x, int y, char c, int ancho, int alto, int tamano) {  //  Definición de la función dibujarOctagono que recibe las coordenadas, un carácter, y el tamaño.
    for (int i = 0; i < tamano / 2; ++i) {                          //  Bucle exterior para la parte superior del octágono (hasta la mitad del tamaño).
        for (int j = 0; j < tamano + i * 2; ++j) {                 //  Bucle interior para determinar cuántos caracteres dibujar en cada fila superior.
            dibujarCar(c, x - i + j, y + i, ancho, alto);         //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
    for (int i = 0; i < tamano; ++i) {                              //  Bucle para la parte central del octágono.
        for (int j = 0; j < tamano + tamano - 1; ++j) {           //  Bucle interior para dibujar la línea central del octágono.
            dibujarCar(c, x - tamano / 2 + j, y + tamano / 2 + i, ancho, alto); //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
    for (int i = tamano / 2 - 1; i >= 0; --i) {                    // Bucle para dibujar la parte inferior del octágono (desde la mitad hacia abajo).
        for (int j = 0; j < tamano + i * 2; ++j) {                 //  Bucle interior para determinar cuántos caracteres dibujar en cada fila inferior.
            dibujarCar(c, x - i + j, y + tamano + tamano / 2 + (tamano / 2 - 1 - i), ancho, alto); //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
}                                                                   //  Cierra la función dibujarOctagono.

// Función para dibujar un rombo
void dibujarRombo(int x, int y, char c, int ancho, int alto, int tamano) {  //  Definición de la función dibujarRombo que recibe las coordenadas, un carácter, y el tamaño.
    for (int i = 0; i < tamano; ++i) {                              // Bucle exterior para la parte superior del rombo.
        for (int j = 0; j <= i * 2; ++j) {                          //  Bucle interior para determinar cuántos caracteres dibujar en cada fila superior.
            dibujarCar(c, x - i + j, y + i, ancho, alto);         //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               //  Cierra el bucle exterior.
    for (int i = tamano - 2; i >= 0; --i) {                        //  Bucle para la parte inferior del rombo (desde tamano - 2 hasta 0).
        for (int j = 0; j <= i * 2; ++j) {                          //  Bucle interior para determinar cuántos caracteres dibujar en cada fila inferior.
            dibujarCar(c, x - i + j, y + tamano + (tamano - 2 - i), ancho, alto); //  Dibuja el carácter en la posición correspondiente.
        }                                                           //  Cierra el bucle interior.
    }                                                               // Cierra el bucle exterior.
}                                                                   //  Cierra la función dibujarRombo.
   
                   
// Función principal
int main() {
    int x = 20, y = 15; // Posición inicial del cursor
    int ancho, alto;
    bool menuVisible = true; // Estado del men�
    obtenerTamanoConsola(ancho, alto);
  
	mostrarMenu();
   
    while (true) {
        gotoxy(x, y);
        

        // Capturamos la tecla presionada sin esperar un Enter
        if (_kbhit()) {
            char tecla = _getch();
            
            if (tecla == 'O') {
                menuVisible = !menuVisible;
                if (menuVisible) {
                    mostrarMenu();
                } else {
                    ocultarMenu();
                }
                
            }

            switch (tecla) {
                case 72: // Flecha hacia arriba
                    y--;
                    if (y < 0) y = alto - 1;
                    break;

                case 80: // Flecha hacia abajo
                    y++;
                    if (y >= alto) y = 0;
                    break;

                case 75: // Flecha hacia la izquierda
                    x--;
                    if (x < 0) x = ancho - 1;
                    break;

                case 77: // Flecha hacia la derecha
                    x++;
                    if (x >= ancho) x = 0;
                    break;

                case 'L': // Activar/Desactivar modo borrador
                    modoBorrador = !modoBorrador; // Cambia el estado del modo borrador
                    gotoxy(100, 00);
                    if (modoBorrador) {
                        std::cout << "Modo borrador activado.   ";
                       
					} else {
                        std::cout << "Modo borrador desactivado.";
                     
				    }
                    break;
        
                

                case '0': {
                // Borrar figuras (limpia la consola)
                    system("CLS");
                    mostrarMenu();
                    break;
				}
                    

                case 27: // Tecla ESC para salir del programa
                    return 0; // Salir del programa

               
				
                case '1': // Lógica para dibujar triángulo
                    int color = seleccionarColor();         //  Llama a seleccionarColor para obtener el color deseado.
                    establecerColor(color);                 //  Cambia el color de la consola al seleccionado.

                    gotoxy(0, 13);                         //  Mueve el cursor fuera del área de dibujo para pedir el carácter.
                    std::cout << "Ingresa un caracter para el triángulo: "; // 76. Pide al usuario que ingrese un carácter para el triángulo.
					char c;    //  Declara una variable para almacenar el carácter.
                    std::cin >> c;                          //  Captura el carácter ingresado por el usuario.
                    gotoxy(0, 13);                         // Mueve el cursor fuera del área de dibujo para preguntar cómo se desea crear el triángulo.
                    std::cout << "¿Cómo deseas crear el triángulo? (1: Lados iguales, 2: Base y altura): "; // 80. Pide al usuario que elija una opción para crear el triángulo.
                    int opcion;                             // Declara una variable para almacenar la opción.
                    std::cin >> opcion;                    // Captura la opción ingresada por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                                       
				if (opcion == 1) {                                      // 83. Verifica si la opción elegida es 1 (triángulo isósceles).
                    gotoxy(0, 13);                                   // Mueve el cursor a la posición (0, 12) para pedir el tamaño.
                    std::cout << "Ingresa el tamaño de los lados del triángulo isósceles: "; // 85. Solicita al usuario el tamaño de los lados.
                    int tamano;                                      //  Declara una variable para el tamaño.
                    std::cin >> tamano;                             //  Captura el tamaño ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual                      
                    
                    gotoxy(0, 13);                                   //  Mueve el cursor a la posición (0, 13) para preguntar la dirección.
                    std::cout << "¿Apuntando hacia arriba? (s/n): "; //  Pregunta si el triángulo apunta hacia arriba.
                    char respuesta;                                  // Declara una variable para la respuesta.
                    std::cin >> respuesta;                          //  Captura la respuesta ingresada por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual                      
                   
                    dibujarTriangulo(x, y, c, ancho, alto, tamano, respuesta == 's'); // 92. Llama a dibujarTriangulo con los parámetros obtenidos.
                } else if (opcion == 2) {                             //  Verifica si la opción elegida es 2 (triángulo con base y altura).
                    gotoxy(0, 13);                                   //  Mueve el cursor a la posición (0, 12) para pedir la base.
                    std::cout << "Ingresa la base del triángulo: "; //  Solicita al usuario la base del triángulo.
                    int base;                                        //  Declara una variable para la base.
                    std::cin >> base;                               //  Captura la base ingresada por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);                                   //  Mueve el cursor a la posición (0, 13) para pedir la altura.
                    std::cout << "Ingresa la altura del triangulo: "; //  Solicita al usuario la altura del triángulo.
                    int altura;                                      //  Declara una variable para la altura.
                    std::cin >> altura;                             //  Captura la altura ingresada por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual                    
                    // Asegúrate de calcular el tamaño adecuado en función de la base y la altura
                    int tamano = altura; // Aquí puedes adaptar el cálculo del tamaño según tus necesidades //  Calcula el tamaño del triángulo basado en la altura.

                    gotoxy(0, 13);                                   //  Mueve el cursor a la posición (0, 14) para preguntar la dirección.
                    std::cout << "�Apuntando hacia arriba? (s/n): "; //  Pregunta si el triángulo apunta hacia arriba.
                    char respuesta;                                  //  Declara una variable para la respuesta.
                    std::cin >> respuesta;                          //  Captura la respuesta ingresada por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual                      
                    dibujarTriangulo(x, y, c, ancho, alto, tamano, respuesta == 's'); // 107. Llama a dibujarTriangulo con los parámetros obtenidos.
                } else {                                             //  Si ninguna de las opciones anteriores es válida.
                    gotoxy(0, 13);                                   //  Mueve el cursor a la posición (0, 12).
                    std::cout << "Opcion invalida, intenta de nuevo."; //  Muestra un mensaje de error.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual                      
				  }                                              //  Cierra el bloque if-else.
                break;                                             //  Sale del bloque switch.
                }
                 switch (tecla) {
		
				case '2' : {                                      //  Si se presiona '2' para dibujar un cuadrado.
                    int color = seleccionarColor();               //  Llama a seleccionarColor para obtener el color deseado.
                    establecerColor(color);                       //  Cambia el color de la consola al seleccionado.

                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 10) para pedir el carácter.
                    std::cout << "Ingresa un caracter para el cuadrado: "; // 117. Solicita al usuario un carácter para el cuadrado.
                    char c;                                       //  Declara una variable para el carácter.
                    std::cin >> c;                                //  Captura el carácter ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 11) para pedir el tamaño.
                    std::cout << "Ingresa el tamaño del cuadrado: "; //  Solicita al usuario el tamaño del cuadrado.
                    int tamano;                                    //  Declara una variable para el tamaño.
                    std::cin >> tamano;                           //  Captura el tamaño ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    dibujarCuadrado(x, y, c, ancho, alto, tamano); //  Llama a dibujarCuadrado con los parámetros obtenidos.
				
					break;                                         //  Sale del bloque switch.
                }
              case '3': {                                       //  Si se presiona '3' para dibujar un rectángulo.
                    
					int color = seleccionarColor();               //  Llama a seleccionarColor para obtener el color deseado.
                    establecerColor(color);                       //  Cambia el color de la consola al seleccionado.

                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 10) para pedir el carácter.
                    std::cout << "Ingresa un caracter para el rectangulo: "; // Solicita al usuario un carácter para el rectángulo.
                    char c;                                       //  Declara una variable para el carácter.
                    std::cin >> c;                                //  Captura el carácter ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 11) para pedir el tamaño.
                    std::cout << "Ingresa el tamaño del rectangulo: "; //  Solicita al usuario el tamaño del rectángulo.
                    int tamano;                                    //  Declara una variable para el tamaño.
                    std::cin >> tamano;                           //  Captura el tamaño ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    dibujarRectangulo(x, y, c, ancho, alto, tamano); //  Llama a dibujarRectangulo con los parámetros obtenidos.
				 
				    break;                                         //  Sale del bloque switch.
                }

                case '4': {                                       //  Si se presiona '4' para dibujar un hexágono.
                   
				    int color = seleccionarColor();               //  Llama a seleccionarColor para obtener el color deseado.
                    establecerColor(color);                       //  Cambia el color de la consola al seleccionado.

                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 10) para pedir el carácter.
                    std::cout << "Ingresa un caracter para el hexagono: "; //  Solicita al usuario un carácter para el hexágono.
                    char c;                                       //  Declara una variable para el carácter.
                    std::cin >> c;                                //  Captura el carácter ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);                                 //  Mueve el cursor a la posición (0, 11) para pedir el tamaño.
                    std::cout << "Ingresa el tamaño del hexagono: "; //  Solicita al usuario el tamaño del hexágono.
                    int tamano;                                    //  Declara una variable para el tamaño.
                    std::cin >> tamano;                           //  Captura el tamaño ingresado por el usuario.
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    dibujarHexagono(x, y, c, ancho, alto, tamano); //  Llama a dibujarHexagono con los parámetros obtenidos.
				 
				    break;                                         //  Sale del bloque switch.
                }
                case '5': { // 5 - Octágono
                     
					int color = seleccionarColor();
                    establecerColor(color);

                    gotoxy(0, 13); // Mueve el cursor fuera del área de dibujo para pedir el carácter
                    std::cout << "Ingresa un caracter para el octagono: ";
                    char c;
                    std::cin >> c;
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);
                    std::cout << "Ingresa el tamaño del octagono: ";
                    int tamano;
                    std::cin >> tamano;
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    dibujarOctagono(x, y, c, ancho, alto, tamano);
				   
					break;
                }

                case '6': { // 6 - Rombo
                    
					int color = seleccionarColor();
                    establecerColor(color);

                    gotoxy(0, 13); // Mueve el cursor fuera del área de dibujo para pedir el carácter
                    std::cout << "Ingresa un caracter para el rombo: ";
                    char c;
                    std::cin >> c;
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    gotoxy(0, 13);
                    std::cout << "Ingresa el tamaño del rombo: ";
                    int tamano;
                    std::cin >> tamano;
                    gotoxy(0, 13);  // Mueve el cursor al inicio de la l�nea 5
                    limpiarLinea();  // Limpia la l�nea actual
                    dibujarRombo(x, y, c, ancho, alto, tamano);
				    
				
					break;
                    
			}
               case '7': {
					exportarHTML(); 
					exportadorConsola();
               	break;
               }
       
					

   	
}                
				
                
        
            // Si el modo borrador está activado, borra en la posición actual
            if (modoBorrador) {
                borrarEnPosicion(x, y);
            }
            

            Sleep(1); // Pausa para mejorar la fluidez de movimiento
        }
    }

    return 0;
}

