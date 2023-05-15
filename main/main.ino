// Importamos librerías necesarias para el funcionamiento del programa.
#include <LiquidCrystal.h>
#define NOTE_C4 262
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
//  Matriz de bytes que contiene la representación gráfica de un carácter que se utiliza para dibujar el símbolo de grados con un pequeño círculo, para no generar conflicto en la pantalla LCD.
byte degreeChar[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Declaramos el sensor de temperatura, que apunta al pin utilizado.
OneWire ourWire(9);
// Declaramos la pantalla a utilizar, con sus respectivos pines.
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
// Asignamos el sensor de temperatura que hemos declarado antes para utilizarlo posteriormente.
DallasTemperature sensors(&ourWire);
const int potPin = A5;  // Pin analógico del potenciómetro
const int motorPin = 6; // Pin digital para controlar el motor

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mensaje de bienvenida, utilizado al iniciar el programa.
const String bienvenida = "Trabajo final de grado         <3";
void updateRow(int row);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variable que recoge el tiempo del temporizador.
bool adjustTime = false;
int timerTime;
int timerOn;
int motorOn = 0;
int transcurrentTime;
bool button9Pressed = false;
bool button10Pressed = false;
unsigned long debounceTime = 100;
unsigned long lastDebounceTime = 0;
// Variable para posteriormente, almacenar el tiempo en el que fue encendido el arduino.
unsigned long tiempoEncendido;
// Variable que recoge el tiempo actual.
unsigned long tiempoActual;
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Se realizan todas las tareas de configuración iniciales necesarias para que el programa funcione correctamente.
void setup()
{
  // Configuración de los pines
  pinMode(motorPin, OUTPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  // Asignamos los milisegundos actuales que el arduino lleva encendido.
  tiempoEncendido = millis();
  // Arrancamos el sensor de temperatura.
  sensors.begin();
  // Arrancamos la pantalla LCD y le asignamos el numero de filas y columnas.
  lcd_1.begin(16, 2);
  // Usamos la funcion yourWelcome para generar la bienvenida al usuario.
  yourWelcome();
  // Actualizamos el LCD con el programa.
  lcd_1.clear();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Funcion que se ejecuta infinitamente, aqui está el corazon del programa.
void loop()
{

  if (analogRead(3) >= 0 && analogRead(3) <= 29)
  {
    if (adjustTime == false)
    {
      adjustTime = true;
    }
    timerTime += 5;
    timerOn = 1;
    transcurrentTime = 0;
    actualizarPantallaTimer();
  }
  if (analogRead(3) >= 87 && analogRead(3) <= 90)
  {
    timerTime += 3600;
    timerOn = 1;
    transcurrentTime = 0;
    actualizarPantallaTimer();
    if (adjustTime == false)
    {
      adjustTime = true;
    }
  }
  if (analogRead(3) >= 30 && analogRead(3) <= 40)
  {
    if (adjustTime == false)
    {
      adjustTime = true;
    }
    timerTime += 60;
    timerOn = 1;
    transcurrentTime = 0;
    actualizarPantallaTimer();
  }
  if (analogRead(3) >= 330 && analogRead(3) <= 400)
  {
    adjustTime = false;
  };
  if (analogRead(3) >= 150 && analogRead(3) <= 180)
  {
    if (adjustTime == false)
    {
      adjustTime = true;
    }
    timerTime = 0;
    timerOn = 1;
    transcurrentTime = 0;

    actualizarPantallaTimer();
  }
  // Asignamos el tiempo actual a la variable tiempoActual.
  // Llamamos a la funcion que actualiza la pantalla para mantenerla actualizada constanmente.
  if (!adjustTime)
  {

    actualizarPantalla();
  }
  else
  {

    actualizarPantallaTemp();
  }
  if (tempSensor() > 29)
  {
    tone(6, NOTE_C4, 3000);
  }
  else
  {
    noTone(6);
  };
  if (timerOn == 1 && !adjustTime)
  {
    int potValue = analogRead(potPin);
    int motorSpeed = map(potValue, 0, 1023, 0, 255);
    int revoluciones = map(motorSpeed, 0, 255, 0, 200);
    analogWrite(motorPin, motorSpeed);
  }
  delay(20);
};

void actualizarPantallaTimer()
{
  int time = timerTime;
  updateRow(0);
  if (time < 59)
  {
    updateRow(0);
    lcd_1.setCursor(4, 0);
    lcd_1.print("00");
    lcd_1.print(":");
    lcd_1.print("00");
    lcd_1.print(":");
    if (time < 10)
    {
      lcd_1.print("0");
      lcd_1.print(time);
    }
    else
    {
      lcd_1.print(time);
    }
  }
  else if (time > 59 && time < 3600)
  {
    updateRow(0);
    lcd_1.setCursor(4, 0);
    lcd_1.print("00");
    lcd_1.print(":");
    if (time < 600)
    {
      lcd_1.print("0");
      lcd_1.print(time / 60);
    }
    else
    {
      lcd_1.print(time / 60);
    }
    lcd_1.print(":");
    if (time % 60 < 10)
    {
      lcd_1.print("0"); // agregar un cero delante de los segundos si es menor que 10
    }
    lcd_1.print(time % 60);
  }
  else if (time > 3599 && time < 86400)
  {
    updateRow(0);
    if (time < 36000)
    {
      lcd_1.setCursor(4, 0);
      lcd_1.print("0");
      lcd_1.print(time / 3600);
      lcd_1.print(":");
    }
    else
    {
      lcd_1.setCursor(4, 0);
      lcd_1.print(time / 3600);
      lcd_1.print(":");
    }

    int minutes = (time % 3600) / 60; // calcular los minutos restantes después de restar las horas

    if (minutes < 10)
    {
      lcd_1.print("0"); // agregar un cero delante de los minutos si es menor que 10
    }
    lcd_1.print(minutes);
    lcd_1.print(":");

    if (time % 60 < 10)
    {
      lcd_1.print("0"); // agregar un cero delante de los segundos si es menor que 10
    }
    lcd_1.print(time % 60);
  }
}

void actualizarPantallaTemp()
{
  lcd_1.createChar(0, degreeChar);
  lcd_1.setCursor(0, 2);
  lcd_1.print("Temp: ");
  lcd_1.print(tempSensor());
  lcd_1.write(char(0));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int timer()
{
  delay(980);
  transcurrentTime += 1;
  timerTime -= 1;
  int restTime = timerTime;
  return restTime;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcion que muestra el mensaje de bienvenida al usuario.
void yourWelcome()
{
  // Posicionamos el cursor en la primera fila y la primera columna, es donde empezará la pantalla a escribirse.
  lcd_1.setCursor(0, 0);
  // Recorremos el mensaje de bienvenida y lo mostramos en pantalla.
  for (int i = 0; i < bienvenida.length(); i++)
  {
    // Imprimimos en pantalla el mensaje de bienvenida, cada caracter de la cadena de caracteres.
    lcd_1.print(bienvenida[i]);
    // Asignamos delay de 100ms para que la pantalla se actualice cada 100ms.
    if (!(bienvenida[i] == ' '))
    {
      delay(180);
    }

    // Si el índice es mayor o igual a 16, establecemos el cursor en la segunda fila.
    if (i >= 16)
    { // si el índice es mayor o igual a 16
      // establecemos el cursor en la segunda fila y ademas, restamos 16 para que el cursor se mueva a la derecha.
      lcd_1.setCursor(i - 16, 1); // establecer el cursor en la línea 2
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void updateRow(int row = 3)
{
  if (row == 3 || row == 2 || row == 0)
  {
    if (row == 3)
    {
      lcd_1.setCursor(0, 0);
      lcd_1.print("                 ");
      lcd_1.setCursor(0, 2);
      lcd_1.print("                 ");
    }
    else
    {
      lcd_1.setCursor(0, row);
      lcd_1.print("                 ");
    }
  }
}
// Mostrar y actualizar datos de la pantalla a tiempo real, 100ms.
void actualizarPantalla()
{
  if (timerOn == 1)
  {
    int potValue = analogRead(potPin);
    int motorSpeed = map(potValue, 0, 1023, 0, 255);
    int revoluciones = map(motorSpeed, 0, 255, 0, 200);

    int time = timer();

    if (time < 59)
    {
      updateRow(0);
      lcd_1.setCursor(4, 0);
      lcd_1.print("00");
      lcd_1.print(":");
      lcd_1.print("00");
      lcd_1.print(":");
      if (time < 10)
      {
        lcd_1.print("0");
        lcd_1.print(time);
      }
      else
      {
        lcd_1.print(time);
      }

      lcd_1.setCursor(0, 2);
      lcd_1.print("Temp: ");
      lcd_1.print(tempSensor());
      lcd_1.write(char(0));
      lcd_1.setCursor(10, 1);
      lcd_1.print("        ");
      lcd_1.setCursor(10, 1);
      lcd_1.print(revoluciones);
      lcd_1.print("RPM");
    }
    else if (time > 59 && time < 3600)
    {
      updateRow(0);
      lcd_1.setCursor(4, 0);
      lcd_1.print("00");
      lcd_1.print(":");
      if (time < 600)
      {
        lcd_1.print("0");
        lcd_1.print(time / 60);
      }
      else
      {
        lcd_1.print(time / 60);
      }
      lcd_1.print(":");
      if (time % 60 < 10)
      {
        lcd_1.print("0"); // agregar un cero delante de los segundos si es menor que 10
      }
      lcd_1.print(time % 60);
      lcd_1.setCursor(0, 2);
      lcd_1.print("Temp: ");
      lcd_1.print(tempSensor());
      lcd_1.write(char(0));
      lcd_1.setCursor(10, 1);
      lcd_1.print("        ");
      lcd_1.setCursor(10, 1);
      lcd_1.print(revoluciones);
      lcd_1.print("RPM");
    }
    else if (time > 3599 && time < 86400)
    {
      updateRow(0);
      if (time < 36000)
      {
        lcd_1.setCursor(4, 0);
        lcd_1.print("0");
        lcd_1.print(time / 3600);
        lcd_1.print(":");
      }
      else
      {
        lcd_1.setCursor(4, 0);
        lcd_1.print(time / 3600);
        lcd_1.print(":");
      }

      int minutes = (time % 3600) / 60; // calcular los minutos restantes después de restar las horas

      if (minutes < 10)
      {
        lcd_1.print("0"); // agregar un cero delante de los minutos si es menor que 10
      }
      lcd_1.print(minutes);
      lcd_1.print(":");

      if (time % 60 < 10)
      {
        lcd_1.print("0"); // agregar un cero delante de los segundos si es menor que 10
      }
      lcd_1.print(time % 60);
      lcd_1.setCursor(0, 2);
      lcd_1.print("Temp: ");
      lcd_1.print(tempSensor());
      lcd_1.write(char(0));
      lcd_1.setCursor(10, 1);
      lcd_1.print("        ");
      lcd_1.setCursor(10, 1);
      lcd_1.print(revoluciones);
      lcd_1.print("RPM");
    }
    if (time == 0)

    {

      tone(6, NOTE_C4, 3000);
      delay(3000);
      noTone(6);
      timerTime = 0;
      timerOn = 0;
      updateRow();
    }
  }
  else
  {
    updateRow(2);
    // Limpia la pantalla y borra cualquier dato.
    // Creamos el caracter de los grados, que previamente generamos.
    lcd_1.createChar(0, degreeChar);
    // Posicionamos el cursor en la primera fila y la primera columna, es donde empezará la pantalla a escribirse.
    lcd_1.setCursor(0, 2);
    // Escribimos en pantalla el mensaje correspondiente.
    lcd_1.print("Temp: ");
    // Imprimimos en pantalla la funcion de tempSensor, ya que esta ha sido programada para que devuelva directamente el valor de la temperatura actual.
    lcd_1.print(tempSensor());

    lcd_1.write(char(0)); // Mostramos en pantalla el caracter de los grados.
    // Asignamos delay de 100ms para que la pantalla se actualice cada 100ms.
    int potValue = analogRead(potPin);
    int motorSpeed = map(potValue, 0, 1023, 0, 255);
    int revoluciones = map(motorSpeed, 0, 255, 0, 200); // Ajusta el rango de las revoluciones según tus necesidades
    lcd_1.setCursor(10, 1);
    lcd_1.print(revoluciones);
    lcd_1.print("RPM"); // Cambia el cursor a la segunda línea, sexta columna
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Funcion que devuelve el valor de la temperatura actual, usamos int para que devuelva el resultado en un entero.
int tempSensor()
{
  // Le pedimos al sensor que nos devuelva los datos de la temperatura actual.
  sensors.requestTemperatures();
  // Asiganmos a temp el valor de la temperatura actual.
  int temp = sensors.getTempCByIndex(0); // Se obtiene la temperatura en ºC
  // Retornamos el valor de temp.
  return temp;
}