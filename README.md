# STM32 AD8403

Eine in C implementierte Treiberbibliothek für das digitale Quad-Potentiometer **AD8403**
von Analog Devices auf STM32-Mikrocontrollern. Die Ansteuerung erfolgt über SPI, zusätzlich
werden die Pins Shutdown (SHDN) und Reset (RS) direkt über GPIO gesteuert.

**Aktuelle Version:** 1.0.0 – siehe [CHANGELOG](CHANGELOG.md)

## Beschreibung

Der AD8403 enthält vier unabhängige 256-Stufen-Potentiometer, die einzeln über eine
SPI-Schnittstelle adressiert und programmiert werden können. Jedes Potentiometer wird über
ein 2-Byte-Kommando angesprochen: Das erste Byte enthält die Adresse (Mux-Auswahl), das
zweite Byte den gewünschten Wiper-Wert im Bereich 0–255.

Neben der SPI-Programmierung verfügt der IC über zwei zusätzliche Steuerleitungen:

- **SHDN (Shutdown)**: Bei Low wird der Wiper aller Potentiometer auf Terminal B geschaltet
  (Stromsparmodus). Bei High sind alle regulären Funktionen verfügbar.
- **RS (Reset)**: Bei Low werden alle Potentiometer-Register auf die Wiper-Mittelstellung
  gesetzt. Ein positiver Puls auf RS setzt ebenfalls alle Potis auf die Mittelstellung.

Sind CS, RS und SHDN High, findet keine Operation statt.

## Dateien

| Datei          | Beschreibung                                                     |
|----------------|------------------------------------------------------------------|
| `ad8403.h`     | Version, Adressdefinitionen, SPI- und Pin-Konfiguration, API      |
| `ad8403.c`     | Implementierung der SPI-Kommunikation und Pin-Steuerung           |
| `CHANGELOG.md` | Änderungshistorie                                                |

## Adressierung der Potentiometer

| Potentiometer | Makro         | Adresse (Mux) |
|:--------------|:--------------|:-------------:|
| Poti 1        | `AD8403_MUX0` | `0b00`        |
| Poti 2        | `AD8403_MUX1` | `0b01`        |
| Poti 3        | `AD8403_MUX2` | `0b10`        |
| Poti 4        | `AD8403_MUX3` | `0b11`        |

## API

```c
void initAD8403 (void);                            // AD8403 initialisieren (SHDN, CS, RS setzen)
void setPoti (uint8_t addresse, uint8_t Data);     // Einzelnes Poti auf Wert 0–255 setzen
void setAllPoti (uint8_t Data);                    // Alle vier Potis auf denselben Wert setzen
void PotiOff (uint8_t addresse);                   // Einzelnes Poti auf Terminal B schalten (Wert 255)
void AllPotiOff (void);                            // Alle Potis ausschalten (SHDN low)
void AllPotiOn (void);                             // Alle Potis wieder einschalten (SHDN high)
void AllPotiMidscale (void);                       // Alle Potis auf Wiper-Mittelstellung setzen (RS-Puls)
```

### Funktionsdetails

| Funktion          | Beschreibung                                                                          |
|-------------------|---------------------------------------------------------------------------------------|
| `initAD8403`      | Aktiviert SHDN, wartet 20 ms (Reset-Zeit), deaktiviert CS und aktiviert RS            |
| `setPoti`         | Sendet Adresse und Datenbyte per SPI an den IC, CS wird dabei automatisch gesteuert   |
| `setAllPoti`      | Ruft `setPoti()` nacheinander für alle vier Mux-Adressen auf                          |
| `PotiOff`         | Setzt das gewählte Poti auf den Maximalwert 255 (Wiper an Terminal B)                 |
| `AllPotiOff`      | Schaltet SHDN auf Low, alle Wiper springen auf Terminal B (Stromsparmodus)            |
| `AllPotiOn`       | Schaltet SHDN auf High, reguläre Funktion wird wiederhergestellt                      |
| `AllPotiMidscale` | Erzeugt einen Low-Puls auf RS (20 ms), wodurch alle Potis auf Mittelstellung springen |

## Konfiguration

### Pins

Die Pin-Makros (`AD8403_CS_ENABLE()`, `AD8403_RS_ENABLE()`, `AD8403_SHDN_ENABLE()` usw.)
werden in `ad8403.h` automatisch erzeugt, sobald die folgenden Definitionen vorhanden sind.
Am einfachsten werden die Pins in STM32CubeMX mit den User Labels `AD8403_CS`, `AD8403_RS`
und `AD8403_SHDN` benannt – CubeMX erzeugt die Definitionen dann in `main.h`.

| Pin  | Benötigte Definitionen                       | Aktiver Zustand von `..._ENABLE()` |
|------|----------------------------------------------|------------------------------------|
| CS   | `AD8403_CS_GPIO_Port`, `AD8403_CS_Pin`       | Low                                |
| RS   | `AD8403_RS_GPIO_Port`, `AD8403_RS_Pin`       | High                               |
| SHDN | `AD8403_SHDN_GPIO_Port`, `AD8403_SHDN_Pin`   | High                               |

Fehlt eine dieser Definitionen, gibt der Compiler eine `#warning` aus
(z. B. `Kein Chip Select Pin fuer AD8403 definiert`).

### SPI-Schnittstelle

Standardmäßig wird `hspi1` verwendet. Soll ein anderes SPI-Handle genutzt werden, muss
`AD8403_SPI_PORT` vor dem Einbinden von `ad8403.h` definiert werden, z. B. global über die
Compiler-Defines:

```c
#define AD8403_SPI_PORT    hspi2
```

### Debug

Ist das Symbol `DEBUG` gesetzt, wird automatisch `DEBUG_AD8403` definiert.

### Version

Die Version der Bibliothek ist in `ad8403.h` hinterlegt:

```c
#define AD8403_MAJOR    1
#define AD8403_MINOR    0
#define AD8403_PATCH    0
#define AD8403_DEV      0
```

## Verwendung

### 1. Dateien einbinden

`ad8403.h` und `ad8403.c` in das STM32-Projekt kopieren und den Header einbinden. Da
`ad8403.h` die HAL-Typen (`SPI_HandleTypeDef`, `uint8_t`) und die Pin-Definitionen aus
`main.h` benötigt, muss vorher `spi.h` (bindet `main.h` ein) eingebunden werden:

```c
#include "spi.h"
#include "ad8403.h"
```

### 2. Initialisierung

```c
MX_SPI1_Init();
initAD8403();
```

### 3. Potentiometer ansteuern

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

## Widerstandswerte

Je nach gewählter Brückenbeschaltung (Terminal A oder Terminal B kurzgeschlossen) ergeben
sich unterschiedliche Widerstandskurven zwischen digitalem Registerwert und tatsächlichem
Messwert.

### Brücke an Terminal A kurzgeschlossen, Terminal B offen

| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:---------------------:|:----------------------:|
| 0            | 347 Ω                 | –                      |
| 35           | 309 Ω                 | 312 Ω                  |
| 56           | 298 Ω                 | 291 Ω                  |
| 98           | 259 Ω                 | 249 Ω                  |
| 128          | 228 Ω                 | 219 Ω                  |
| 156          | 196 Ω                 | 191 Ω                  |
| 197          | 148 Ω                 | 150 Ω                  |
| 213          | 128 Ω                 | 134 Ω                  |
| 237          | 95 Ω                  | 110 Ω                  |
| 255          | 65 Ω                  | 92 Ω                   |
| RS-Puls      | 128 Ω                 | 128 Ω                  |
| SHDN Low     | –                     | 128 Ω                  |

### Brücke an Terminal B kurzgeschlossen, Terminal A offen

| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:---------------------:|:----------------------:|
| 0            | 65 Ω                  | 92 Ω                   |
| 35           | 95 Ω                  | 110 Ω                  |
| 56           | 128 Ω                 | 134 Ω                  |
| 98           | 148 Ω                 | 150 Ω                  |
| 128          | 196 Ω                 | 191 Ω                  |
| 156          | 228 Ω                 | 219 Ω                  |
| 197          | 259 Ω                 | 249 Ω                  |
| 213          | 298 Ω                 | 291 Ω                  |
| 237          | 309 Ω                 | 312 Ω                  |
| 255          | 347 Ω                 | –                      |
| RS-Puls      | 128 Ω                 | 128 Ω                  |
| SHDN Low     | –                     | 128 Ω                  |

### Brücke an Terminal A und Terminal B kurzgeschlossen

| Registerwert | Gemessener Widerstand | Berechneter Widerstand |
|:------------:|:---------------------:|:----------------------:|
| 0            | 65 Ω                  | 92 Ω                   |
| 35           | 95 Ω                  | 110 Ω                  |
| 56           | 128 Ω                 | 134 Ω                  |
| 98           | 148 Ω                 | 150 Ω                  |
| 128          | 196 Ω                 | 191 Ω                  |
| 156          | 228 Ω                 | 219 Ω                  |
| 197          | 259 Ω                 | 249 Ω                  |
| 213          | 298 Ω                 | 291 Ω                  |
| 237          | 309 Ω                 | 312 Ω                  |
| 255          | 65 Ω                  | 92 Ω                   |
| RS-Puls      | 128 Ω                 | 128 Ω                  |
| SHDN Low     | 65 Ω                  | 92 Ω                   |

Diese Werte sind messtechnisch ermittelt worden und können je nach verbauter Beschaltung
und Bauteiltoleranzen abweichen.

## Hinweise

- Die SPI-Konfiguration (Modus, Taktrate, SPI-Handle) muss vor dem Aufruf von
  `initAD8403()` bereits über die STM32 HAL initialisiert sein.
- `setPoti()` aktiviert und deaktiviert den Chip-Select-Pin automatisch um die SPI-Übertragung.
- Die SPI-Übertragung erfolgt blockierend (`HAL_SPI_Transmit` mit `HAL_MAX_DELAY`),
  `initAD8403()` und `AllPotiMidscale()` blockieren jeweils 20 ms (`HAL_Delay`).
- Diese Bibliothek wird unter anderem im [EAuto_BMS](https://github.com/Diveturtle93/EAuto_BMS)
  zur Ansteuerung der Tankanzeige im Kombiinstrument verwendet.

## Abhängigkeiten

- STM32 HAL (GPIO, SPI, `HAL_Delay`)
- `spi.h` – von STM32CubeMX generierter SPI-Treiber (stellt das SPI-Handle bereit)
- `main.h` – von STM32CubeMX generiert (Pin-Definitionen)

## Lizenz

Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).
