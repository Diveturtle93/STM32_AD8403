# STM32 AD8403
 
Eine in C implementierte Treiberbibliothek für das digitale Quad-Potentiometer **AD8403**
von Analog Devices auf STM32-Mikrocontrollern. Die Ansteuerung erfolgt über SPI, zusätzlich
werden die Pins Shutdown (SHDN) und Reset (RS) direkt über GPIO gesteuert.
 
## Beschreibung
 
Der AD8403 enthält vier unabhängige 256-Stufen-Potentiometer, die einzeln über eine
SPI-Schnittstelle adressiert und programmiert werden können. Jedes Potentiometer wird über
ein 2-Byte-Kommando angesprochen: das erste Byte enthält die Adresse (Mux-Auswahl), das
zweite Byte den gewünschten Wiper-Wert im Bereich 0–255.
 
Neben der SPI-Programmierung verfügt der IC über zwei zusätzliche Steuerleitungen:
 
- **SHDN (Shutdown)**: Bei Low wird der Wiper aller Potentiometer auf Terminal B geschaltet
  (Stromsparmodus). Bei High sind alle regulären Funktionen verfügbar.
- **RS (Reset)**: Bei Low werden alle Potentiometer-Register auf die Wiper-Mittelstellung
  gesetzt. Ein positiver Pulse auf RS setzt ebenfalls alle Potis auf die Mittelstellung.
  
## Dateien
 
| Datei       | Beschreibung                                                        |
|-------------|----------------------------------------------------------------------|
| `AD8403.h`  | Pin-Verhalten, Widerstandstabellen, Adressdefinitionen, API          |
| `AD8403.c`  | Implementierung der SPI-Kommunikation und Pin-Steuerung               |
 
## Adressierung der Potentiometer
 
| Potentiometer | Adresse (Mux) |
|:--------------|:-------------:|
| Poti 1        | `0b00`        |
| Poti 2        | `0b01`        |
| Poti 3        | `0b10`        |
| Poti 4        | `0b11`        |
 
## API
 
```c
void initAD8403 (void);                            // AD8403 initialisieren (SHDN, CS, RS setzen)
void setPoti (uint8_t addresse, uint8_t Data);      // Einzelnes Poti auf Wert 0–255 setzen
void setAllPoti (uint8_t Data);                     // Alle vier Potis auf denselben Wert setzen
void PotiOff (uint8_t addresse);                    // Einzelnes Poti auf Terminal B schalten (Wert 255)
void AllPotiOff (void);                             // Alle Potis ausschalten (SHDN low)
void AllPotiOn (void);                               // Alle Potis wieder einschalten (SHDN high)
void AllPotiMidscale (void);                        // Alle Potis auf Wiper-Mittelstellung setzen (RS-Puls)
```
 
### Funktionsdetails
 
| Funktion           | Beschreibung                                                                    |
|---------------------|----------------------------------------------------------------------------------|
| `initAD8403`       | Aktiviert SHDN, wartet 20 ms (Reset-Zeit), deaktiviert CS und aktiviert RS       |
| `setPoti`           | Sendet Adresse und Datenbyte per SPI an den IC, CS wird dabei automatisch gesteuert |
| `setAllPoti`        | Ruft `setPoti()` nacheinander für alle vier Mux-Adressen auf                    |
| `PotiOff`           | Setzt das gewählte Poti auf den Maximalwert 255 (Wiper an Terminal B)           |
| `AllPotiOff`        | Schaltet SHDN auf Low, alle Wiper springen auf Terminal B (Stromsparmodus)      |
| `AllPotiOn`         | Schaltet SHDN auf High, reguläre Funktion wird wiederhergestellt                |
| `AllPotiMidscale`   | Erzeugt einen Low-Puls auf RS (20 ms), wodurch alle Potis auf Mittelstellung springen |
 
## Widerstandswerte
 
Der AD8403 hat einen Gesamtwiderstand von ca. 1 kΩ je Potentiometer. Je nach gewählter
Brückenbeschaltung (Terminal A oder Terminal B kurzgeschlossen) ergeben sich unterschiedliche
Widerstandskurven zwischen digitalem Registerwert und tatsächlichem Messwert.
 
### Brücke an Terminal A kurzgeschlossen, Terminal B offen
 
| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:----------------------:|:-----------------------:|
| 0            | 347 Ω                  | –                       |
| 35           | 309 Ω                  | 312 Ω                   |
| 56           | 298 Ω                  | 291 Ω                   |
| 98           | 259 Ω                  | 249 Ω                   |
| 128          | 228 Ω                  | 219 Ω                   |
| 156          | 196 Ω                  | 191 Ω                   |
| 197          | 148 Ω                  | 150 Ω                   |
| 213          | 128 Ω                  | 134 Ω                   |
| 237          | 95 Ω                   | 110 Ω                   |
| 255          | 65 Ω                   | 92 Ω                    |
| RS-Pulse     | 128 Ω                  | 128 Ω                   |
| SHDN Low     | –                      | 128 Ω                   |
 
### Brücke an Terminal B kurzgeschlossen, Terminal A offen
 
| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:----------------------:|:-----------------------:|
| 0            | 65 Ω                   | 92 Ω                    |
| 35           | 95 Ω                   | 110 Ω                   |
| 56           | 128 Ω                  | 134 Ω                   |
| 98           | 148 Ω                  | 150 Ω                   |
| 128          | 196 Ω                  | 191 Ω                   |
| 156          | 228 Ω                  | 219 Ω                   |
| 197          | 259 Ω                  | 249 Ω                   |
| 213          | 298 Ω                  | 291 Ω                   |
| 237          | 309 Ω                  | 312 Ω                   |
| 255          | 347 Ω                  | –                       |
| RS-Pulse     | 128 Ω                  | 128 Ω                   |
| SHDN Low     | –                      | 128 Ω                   |
 
### Brücke an Terminal A und Terminal B kurzgeschlossen
 
| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:----------------------:|:-----------------------:|
| 0            | 65 Ω                   | 92 Ω                    |
| 35           | 95 Ω                   | 110 Ω                   |
| 56           | 128 Ω                  | 134 Ω                   |
| 98           | 148 Ω                  | 150 Ω                   |
| 128          | 196 Ω                  | 191 Ω                   |
| 156          | 228 Ω                  | 219 Ω                   |
| 197          | 259 Ω                  | 249 Ω                   |
| 213          | 298 Ω                  | 291 Ω                   |
| 237          | 309 Ω                  | 312 Ω                   |
| 255          | 65 Ω                   | 92 Ω                    |
| RS-Pulse     | 128 Ω                  | 128 Ω                   |
| SHDN Low     | 65 Ω                   | 92 Ω                    |
 
Diese Werte sind messtechnisch ermittelt worden und können je nach verbauter Beschaltung
und Bauteiltoleranzen abweichen.
 
## Verwendung
 
### 1. Dateien einbinden
 
`AD8403.h` und `AD8403.c` in das STM32-Projekt kopieren und den Header einbinden:
 
```c
#include "AD8403.h"
```
 
### 2. Pin-Makros definieren
 
Die Bibliothek erwartet projektspezifische Makros für die Steuerung von Shutdown-, Chip-
Select- und Reset-Pin. Diese müssen im Projekt (üblicherweise in `main.h`) definiert werden:
 
```c
#define AD8403_SHDN_ENABLE()    HAL_GPIO_WritePin(SHDN_GPIO_Port, SHDN_Pin, GPIO_PIN_SET)
#define AD8403_SHDN_DISABLE()   HAL_GPIO_WritePin(SHDN_GPIO_Port, SHDN_Pin, GPIO_PIN_RESET)
#define AD8403_CS_ENABLE()      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define AD8403_CS_DISABLE()     HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)
#define AD8403_RS_ENABLE()      HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET)
#define AD8403_RS_DISABLE()     HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET)
```
 
### 3. Initialisierung
 
```c
initAD8403();
```
 
### 4. Potentiometer ansteuern
 
```c
// Poti 1 auf Wert 128 (Mittelstellung) setzen
setPoti(AD8403_MUX0, 128);
 
// Alle vier Potis auf denselben Wert setzen
setAllPoti(200);
 
// Poti 3 ausschalten (auf Terminal B, Wert 255)
PotiOff(AD8403_MUX2);
 
// Alle Potis in den Stromsparmodus versetzen
AllPotiOff();
 
// Alle Potis wieder aktivieren
AllPotiOn();
 
// Alle Potis auf Mittelstellung zurücksetzen
AllPotiMidscale();
```
 
## Hinweise
 
- Die SPI-Konfiguration (Modus, Taktrate, `hspi1`-Handle) muss vor dem Aufruf von
  `initAD8403()` bereits über die STM32 HAL initialisiert sein.
- `setPoti()` aktiviert und deaktiviert den Chip-Select-Pin automatisch um die SPI-Übertragung.
- Diese Bibliothek wird unter anderem im [EAuto_BMS](https://github.com/Diveturtle93/EAuto_BMS)
  zur Ansteuerung der Tankanzeige im Kombiinstrument verwendet.
  
## Abhängigkeiten
 
- `spi.h` – STM32 HAL SPI-Treiber (`hspi1`)
- `BatteriemanagementSystem.h` – projektspezifischer Header (Pin-Makros)

## Lizenz
 
Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).
