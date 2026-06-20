//----------------------------------------------------------------------
// Titel	:	AD8403.h
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	27.08.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	AD8403
//----------------------------------------------------------------------
// Shutdown Pin				= Low, Wiper wird an Terminal B angeschlossen
// Shutdown Pin				= High, Alle anderen Funktionen gegeben
// RS Pin					= Low, Alle Poti Register werden auf Wiper Center gesetzt
// RS Pin Pulse				= positiv Pulse, Alle Potis werden auf Wiper Center gesetzt
// RS Pin					= High, Alle anderen Funktionen gegeben
// CS, RS und Shutdown		= High, No Operation
//----------------------------------------------------------------------

// Sicherheitssymbol
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------

// Dateiheader definieren
//----------------------------------------------------------------------
#ifndef INC_AD8403_H_
#define INC_AD8403_H_
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Version definieren
//----------------------------------------------------------------------
#define AD8403_MAJOR					0
#define AD8403_MINOR					0
#define AD8403_PATCH					0
#define AD8403_DEV						0
//----------------------------------------------------------------------

// Addressen
//----------------------------------------------------------------------
#define AD8403_MUX0						0b00								// Poti 1
#define AD8403_MUX1						0b01								// Poti 2
#define AD8403_MUX2						0b10								// Poti 3
#define AD8403_MUX3						0b11								// Poti 4
//----------------------------------------------------------------------

// Neudefinieren des SPI wenn notwendig
//----------------------------------------------------------------------
#ifndef AD8403_SPI_PORT
	#define AD8403_SPI_PORT hspi1
#endif

extern SPI_HandleTypeDef AD8403_SPI_PORT;
//----------------------------------------------------------------------

// Pins definieren
//----------------------------------------------------------------------
// Chip Select Pin
//----------------------------------------------------------------------
#ifdef AD8403_CS_GPIO_Port
	#define AD8403_CS_ENABLE() (HAL_GPIO_WritePin(AD8403_CS_GPIO_Port, AD8403_CS_Pin, GPIO_PIN_RESET))			// Chip-Select Leitung enable
	#define AD8403_CS_DISABLE() (HAL_GPIO_WritePin(AD8403_CS_GPIO_Port, AD8403_CS_Pin, GPIO_PIN_SET))			// Chip-Select Leitung disable
#else
	#warning "Kein Chip Select Pin fuer AD8403 definiert"
#endif
//----------------------------------------------------------------------
// RS Pin
//----------------------------------------------------------------------
#ifdef AD8403_RS_GPIO_Port
	#define AD8403_RS_ENABLE() (HAL_GPIO_WritePin(AD8403_RS_GPIO_Port, AD8403_RS_Pin, GPIO_PIN_SET))			// Reset-Select Leitung enable
	#define AD8403_RS_DISABLE() (HAL_GPIO_WritePin(AD8403_RS_GPIO_Port, AD8403_RS_Pin, GPIO_PIN_RESET))			// Reset-Select Leitung disable
#else
	#warning "Kein RS Pin fuer AD8403 definiert"
#endif
//----------------------------------------------------------------------
// Shutdown Pin
//----------------------------------------------------------------------
#ifdef AD8403_SHDN_GPIO_Port
	#define AD8403_SHDN_ENABLE() (HAL_GPIO_WritePin(AD8403_SHDN_GPIO_Port, AD8403_SHDN_Pin, GPIO_PIN_SET))		// Shutdown Leitung enable
	#define AD8403_SHDN_DISABLE() (HAL_GPIO_WritePin(AD8403_SHDN_GPIO_Port, AD8403_SHDN_Pin, GPIO_PIN_RESET))	// Shutdown Leitung disable
#else
	#warning "Kein Shutdown Pin fuer AD8403 definiert"
#endif
//----------------------------------------------------------------------

// Funktionen definieren
//----------------------------------------------------------------------
void initAD8403 (void);														// Initialisiere AD8403
void setPoti (uint8_t addresse, uint8_t Data);								// Poti mit Daten fuellen
void setAllPoti (uint8_t Data);												// Alle Potis mit gleichen Daten fuellen
void PotiOff (uint8_t addresse);											// Poti ausschalten
void AllPotiOff (void);														// Alle Potis ausschalten
void AllPotiOn (void);														// Alle Potis einschalten
void AllPotiMidscale (void);												// Alle Potis auf halbe Stellung
//----------------------------------------------------------------------

#endif /* INC_AD8403_H_ */
//----------------------------------------------------------------------
