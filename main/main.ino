//Importamos librerías necesarias para el funcionamiento del programa.
#include <LiquidCrystal.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
//Matriz de bytes que contiene la representación gráfica de un carácter que se utiliza para dibujar el símbolo de grados con un pequeño círculo, para no generar conflicto en la pantalla LCD.
byte degreeChar[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Declaramos el sensor de temperatura, que apunta al pin utilizado.
OneWire ourWire(13); 
//Declaramos la pantalla a utilizar, con sus respectivos pines.
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
//Asignamos el sensor de temperatura que hemos declarado antes para utilizarlo posteriormente.
DallasTemperature sensors(&ourWire);

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//Mensaje de bienvenida, utilizado al iniciar el programa.
const String bienvenida = "Trabajo final de grado         <3";







/////////////////////////////////////////////////////////////////////////////////////////////////////////










/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Se realizan todas las tareas de configuración iniciales necesarias para que el programa funcione correctamente.
void setup(){
  //Arrancamos el sensor de temperatura.
  sensors.begin();
  //Arrancamos la pantalla LCD y le asignamos el numero de filas y columnas.
  lcd_1.begin(16, 2);
  //Usamos la funcion yourWelcome para generar la bienvenida al usuario.
  yourWelcome();
  //Actualizamos el LCD con el programa.
  actualizarPantalla();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Funcion que se ejecuta infinitamente, aqui está el corazon del programa.
void loop(){
  //Llamamos a la funcion que actualiza la pantalla para mantenerla actualizada constanmente.
  actualizarPantalla();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Funcion que muestra el mensaje de bienvenida al usuario.
void yourWelcome(){
  //Posicionamos el cursor en la primera fila y la primera columna, es donde empezará la pantalla a escribirse.
  lcd_1.setCursor(0, 0);
  //Recorremos el mensaje de bienvenida y lo mostramos en pantalla.
  for(int i = 0; i<welength; i++){
    //Imprimimos en pantalla el mensaje de bienvenida, cada caracter de la cadena de caracteres.
    lcd_1.print(bienvenida[i]);
    //Asignamos delay de 100ms para que la pantalla se actualice cada 100ms.
    delay(100);
    //Si el índice es mayor o igual a 16, establecemos el cursor en la segunda fila.
    if (i >= 16) { // si el índice es mayor o igual a 16
    //establecemos el cursor en la segunda fila y ademas, restamos 16 para que el cursor se mueva a la derecha.
    lcd_1.setCursor(i - 16, 1); // establecer el cursor en la línea 2
  }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Mostrar y actualizar datos de la pantalla a tiempo real, 100ms.
void actualizarPantalla() {
  //Limpia la pantalla y borra cualquier dato.
  lcd_1.clear();
  //Creamos el caracter de los grados, que previamente generamos.
  lcd_1.createChar(0, degreeChar);
  //Posicionamos el cursor en la primera fila y la primera columna, es donde empezará la pantalla a escribirse.
  lcd_1.setCursor(0, 0);
  //Escribimos en pantalla el mensaje correspondiente.
  lcd_1.print("Temp: ");
  //Imprimimos en pantalla la funcion de tempSensor, ya que esta ha sido programada para que devuelva directamente el valor de la temperatura actual.
  lcd_1.print(tempSensor());
  //Mostramos en pantalla el caracter de los grados.  
  lcd_1.write(char(0));
  //Asignamos delay de 100ms para que la pantalla se actualice cada 100ms.
  delay(100);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
  
//Funcion que devuelve el valor de la temperatura actual, usamos int para que devuelva el resultado en un entero.
int tempSensor() {
  //Le pedimos al sensor que nos devuelva los datos de la temperatura actual.
  sensors.requestTemperatures();
  //Asiganmos a temp el valor de la temperatura actual.
  int temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC
  //Retornamos el valor de temp.
  return temp;
}