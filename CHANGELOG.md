# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden hier dokumentiert.
Format nach [Keep a Changelog](https://keepachangelog.com/de/1.1.0/),
Versionierung nach [Semantic Versioning](https://semver.org/lang/de/).

## [Unveröffentlicht]

## [1.0.0] - 2026-10-18

Erste veröffentlichte Version.

### Hinzugefügt

- Treiber `ad8403.c` / `ad8403.h` für das digitale Quad-Potentiometer AD8403 mit den
  Funktionen `initAD8403`, `setPoti`, `setAllPoti`, `PotiOff`, `AllPotiOff`,
  `AllPotiOn` und `AllPotiMidscale`.
- Adressmakros `AD8403_MUX0` bis `AD8403_MUX3` für die vier Potentiometer.
- Versionsmakros `AD8403_MAJOR`, `AD8403_MINOR`, `AD8403_PATCH` und `AD8403_DEV`.
- Konfigurierbares SPI-Handle über `AD8403_SPI_PORT` (Standard: `hspi1`).
- Automatische Pin-Makros für CS, RS und SHDN aus `AD8403_<PIN>_GPIO_Port` /
  `AD8403_<PIN>_Pin`, inklusive `#warning`, wenn ein Pin nicht definiert ist.
- Debug-Symbol `DEBUG_AD8403`, das bei gesetztem `DEBUG` definiert wird.
- README mit Beschreibung, API, Konfiguration, Verwendung und gemessenen Widerstandswerten.
- CHANGELOG.

[Unveröffentlicht]: https://github.com/Diveturtle93/STM32_AD8403/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Diveturtle93/STM32_AD8403/releases/tag/v1.0.0
