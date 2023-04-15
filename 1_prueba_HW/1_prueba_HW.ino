// Pines:
// -     MISO - pin 14
// -     MOSI - pin 16
// -      CLK - pin 15
// -       CS - pin 10
// -      LED - pin 9
// - Pulsador - pin 8

#include <SPI.h>
#include <SD.h>

File root;

const byte CS_PIN = 10;
const byte LED_PIN = 9;
const byte PULS_PIN = 8;

bool ledStatus = false;

void setup()
{
  Serial.begin(9600);
  // Como tiene usb nativo, es necesario esperar hasta que esté el serial listo
  while (!Serial)
    ;

  Serial.println("[Info] Monitor Serial funcionando");
  pinMode(LED_PIN, OUTPUT);
  pinMode(PULS_PIN, INPUT);

  testMicroSD();
}

void loop()
{
  if (pulsado(PULS_PIN))
  {
    Serial.println("[Info] Botón pulsado");

    ledStatus = !ledStatus;
    digitalWrite(LED_PIN, ledStatus);    
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

void testMicroSD()
{
  Serial.println("[Debug] Inicializando la microSD...");
  if (!SD.begin(CS_PIN))
  {
    Serial.println("[Error] No se pudo inicializar!");
  }
  else
  {
    Serial.println("[Debug] Inicialización ok");
    root = SD.open("/");
    leerContenidoMicroSD(root, 0);
  }
}

void leerContenidoMicroSD(File dir, int numEspacios) {
  while (true) {

    File archivo = dir.openNextFile();
    if (!archivo) {
      // no hay más archivos
      break;
    }

    for (int i = 0; i < numEspacios; i++) {
      Serial.print(" ");
    }

    Serial.println(archivo.name());

    if (archivo.isDirectory()) {
      // si es un directorio, imprimir sus archivos
      leerContenidoMicroSD(archivo, numEspacios + 2);
    }

    archivo.close();
  }
}

