/**************************************************************************/
/*!
    @file     MB85RS1MT.ino
    @author   Christophe Persoz for SPI version
    @license  BSD (see license.txt)

    This sketch connects the FRAM chips.
    Once ok, it read/write Byte, Short, Long at different addresses.
    Dump the full memory
    Erase the complete chip (if ERASE_FRAM is defined)
 
    All the operations are visible on Serial if SER_OUT is defined
    and the execution time of all of them is indicated

    @section  HISTORY

    v1.0.0 - First release
 
 */
/**************************************************************************/

#include <SPI.h>
#include <FRAM_MB85RS_SPI.h>

// Defines
//#define ERASE_FRAM  // Erase FRAM at the end of setup if defined
//#define DUMP_FRAM   // Full dump of the memory

// Prototypes
elapsedMicros _tExe;

// Example code for Fujitsu F-RAM chip
uint8_t FRAM_CS = 21  ;
static FRAM_MB85RS_SPI FRAM(FRAM_CS);

// Vars
uint16_t addr = 0;



// Functions
void printTime()
{
  char buf[11] = "";
  if(_tExe > 10000)
    sprintf(buf,"%4.6f", _tExe*0.000001);
    
  Serial.print("   Execution time: "); (_tExe > 10000) ? Serial.print(buf) : Serial.print(_tExe); Serial.println((_tExe > 100000) ? "s" : " us");
}




void setup()
{
 
  Serial.begin(115200);
  while (!Serial) {}

  _tExe = 0;
  FRAM.init();
  printTime();


  //** BYTE / uint8_t
  
  uint8_t byteVal = 0;
  Serial.print("\n\n** READ/WRITE BYTE (uint8_t)\n");
  
  // Read Byte
  Serial.println("Read Byte in memory, ");
  _tExe = 0;
  if (FRAM.read(0x3F86, &byteVal))
  {
    printTime();
    Serial.print("Restart = ");
    Serial.print(byteVal); Serial.println(" times\n");
    byteVal++;
  } else
    Serial.println("ERROR");
  
  // Write Byte
  Serial.println("Write Byte (8Bit val): Restart");
  _tExe = 0;
  if (FRAM.write(0x3F86, byteVal))
    printTime();
  else
    Serial.println("ERROR");

  // Read the updated Byte for check
  Serial.println("Read after update: ");
  _tExe = 0;
  if (FRAM.read(0x3F86, &byteVal))
  {
    printTime();
    Serial.print("Restarted = "); Serial.print(byteVal); Serial.println(" times\n");
  } else
    Serial.println("ERROR");

  
  //** SHORT / uint16_t

  uint16_t shortVal = 12224;
  Serial.print("\n** READ/WRITE SHORT (uint16_t)\n");
  
  // Test write Byte
  Serial.print("Write Short (16bit val): IndexS = "); Serial.println(shortVal);
  _tExe = 0;
  if (FRAM.write(addr, shortVal))
    printTime();
  else
    Serial.println("ERROR");

  // Read the updated short for check
  Serial.println("Read after update, ");
  _tExe = 0;
  if (FRAM.read(addr, &shortVal))
  {
    printTime();
    Serial.print("IndexS = "); Serial.println(shortVal);
  } else
    Serial.println("ERROR");


  //** LONG / uint32_t

  uint32_t longVal = 15485224;
  Serial.print("\n** READ/WRITE LONG (uint32_t)\n");
  
  // Test write Byte
  Serial.print("Write Short (32bit val): IndexL = "); Serial.println(longVal);
  _tExe = 0;
  if (FRAM.write(addr, longVal))
    printTime();
  else
    Serial.println("ERROR");

  // Read the updated short for check
  Serial.println("Read after update, ");
  _tExe = 0;
  if (FRAM.read(addr, &longVal))
  {
    printTime();
    Serial.print("IndexL = "); Serial.println(longVal);
  } else
    Serial.println("ERROR");


  //** Dump the entire memory!
#ifdef DUMP_FRAM
  uint8_t value;
  _tExe = 0;
  
  for (uint32_t a = 0; a < FRAM.getMaxMemAdr(); a++) {
    FRAM.read(a, &byteVal);
    if ((a % 32) == 0) {
      Serial.print("\nADDRESS 0x"); Serial.print(a, HEX); Serial.println(": ");
    }
    Serial.print("0x"); 
    if (byteVal < 0x1) 
      Serial.print('0');
    Serial.print(byteVal, HEX); Serial.print(" ");
  }
  Serial.print("\n");
  printTime();
#endif

  //** ERASE FRAM
  
#ifdef ERASE_FRAM
  //** ERASE CHIP
  Serial.println("\n\n** ERASE CHIP");
  _tExe = 0;
  if (FRAM.eraseChip())
    printTime();
  else
    Serial.println("ERROR");
#endif
}

void loop()
{

}