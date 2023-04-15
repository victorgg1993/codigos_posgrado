// Pines del aparato:
// -     MISO - pin 14
// -     MOSI - pin 16
// -      CLK - pin 15
// -       CS - pin 10
// -      LED - pin 9
// - Pulsador - pin 8

// Comandos implementados:
// STRING ( es como un print )
// Enter
// Delay
// GUI solo
// GUI con símbolos
// Shift + Fx
// Alt + Tab
// Left (flecha izquierda)
// Down (flecha abajo)
// Esc

#include <Keyboard.h>
#include "Keyboard_es_ES.h"
#include <SPI.h>
#include <SD.h>

#define NOMBRE_FICHERO "/payload.txt"

File archivo;

const byte CS_PIN = 10;
const byte LED_PIN = 9;
const byte PULS_PIN = 8;

bool ledStatus = false;

void setup()
{
  // Activar únicamente para debug
  // Serial.begin(9600);
  // Como tiene usb nativo, es necesario esperar hasta que esté el serial listo
  // while (!Serial)
  //   ;

  Keyboard.begin(KeyboardLayout_es_ES);

  pinMode(LED_PIN, OUTPUT);
  pinMode(PULS_PIN, INPUT);
}

void loop()
{
  if (pulsado(PULS_PIN))
  {
    initAtaque();
  }
}

bool pulsado(byte pin)
{
  if (digitalRead(pin))
  {
    // pulsado, esperar a que se estabilice
    while (digitalRead(pin))
    {
      delay(50);
    }

    // Cuando llegue aquí, significa que se soltó el pulsador
    // y por lo tanto, se considera un click válido
    return true;
  }

  return false;
}

void initAtaque()
{
  if (initMicroSD())
  {
    archivo = SD.open(NOMBRE_FICHERO);

    if (archivo)
    {
      digitalWrite(LED_PIN, HIGH); // Avisamos que empezamos a ejecutar el payload

      while (archivo.available())
      {
        String linea = archivo.readStringUntil('\n'); // leer la siguiente línea del archivo

        // Comandos implementados:
        if (linea.startsWith("STRING "))
        {
          Keyboard.print(linea.substring(7));
        }

        else if (linea.equals("ENTER"))
        {
          Keyboard.press(KEY_RETURN);
          Keyboard.release(KEY_RETURN);
        }

        else if (linea.startsWith("DELAY "))
        {
          int tiempo = linea.substring(6).toInt();
          delay(tiempo);
        }

        // tecla GUI sola
        else if (linea.equals("WINDOWS") || linea.startsWith("GUI"))
        {
          Keyboard.press(KEY_LEFT_GUI);
          Keyboard.releaseAll();
        }

        // tecla GUI con más símbolos
        else if (linea.startsWith("WINDOWS ") || linea.startsWith("GUI "))
        {
          Keyboard.press(KEY_LEFT_GUI);
          if (linea.startsWith("WINDOWS "))
          {
            linea = linea.substring(8);
          }
          else if (linea.startsWith("GUI "))
          {
            linea = linea.substring(4);
          }
          Keyboard.print(linea);
          Keyboard.releaseAll();
        }

        else if (linea.startsWith("SHIFT "))
        {
          if (linea.endsWith("F10"))
          {
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(KEY_F10);
            Keyboard.releaseAll();
          }
        }

        else if (linea.equals("DOWN"))
        {
          Keyboard.press(KEY_DOWN_ARROW);
          Keyboard.releaseAll();
        }

        else if (linea.equals("LEFT"))
        {
          Keyboard.press(KEY_LEFT_ARROW);
          Keyboard.releaseAll();
        }

        else if (linea.equals("ALT TAB"))
        {
          Keyboard.press(KEY_LEFT_ALT);
          Keyboard.press(KEY_TAB);
          Keyboard.releaseAll();
        }

        else if (linea.equals("ESCAPE"))
        {
          Keyboard.press(KEY_ESC);
          Keyboard.release(KEY_ESC);
        }

        else if (linea.startsWith("REM"))
        {
          // línea de comentario, no hacemos nada
        }
      }

      archivo.close();
      digitalWrite(LED_PIN, LOW); // Avisamos que el payload terminó
    }
  }
}

bool initMicroSD()
{
  if (!SD.begin(CS_PIN))
  {
    return false;
  }
  else
  {
    return true;
  }
}
