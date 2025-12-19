# BAG Programmierrunde - Monorepo

Hier findest du alle relevanten Dateien und Links rund um unser ESP32 Mikrocontroller-Projekt. Im Projekt entwickeln wir
gemeinsam an einem Gerät, worauf das [Dino-Spiel](https://de.wikipedia.org/wiki/Dino-Spiel) gespielt werden können soll.


* [Hardware Komponenten](#hardware-komponenten)
* [Anleitung](#anleitung)
* [Pinout](#pinout)
* [TODOs](#todos)
* [Technische offene Fragen](#technische-offene-fragen)
* [License](#mit-license)


## Hardware Komponenten

- ESP32-S3-DevKitC-1 N16/R8
- 2.8" TFT LCD 240x320 ST7789
- ein Switch / Button


## Anleitung

Dies ist ein einfacher Weg, das Projekt bei dir einzurichten. Du kannst natürlich aber auch frei einen anderen wählen.

### Welche Programme werden benötigt:

- VS Code (Visual Studio Code https://code.visualstudio.com/)
- Git (https://git-scm.com/)

Diese müssen heruntergeladen und installiert werden.


### Schritt für Schritt Einrichtung:

1. in VS Code links auf den Reiter `Extensions` klicken und folgende Erweiterungen installieren:
   - `C/C++` von Microsoft
   - `ESP-IDF` von Espressif Systems
2. hat sich das Willkommensfenster von ESP-IDF geöffnet? dann weiter bei `3.`, falls es sich nicht geöffnet hat:
    - auf der linken Seite auf das neue ESP Icon klicken
    - in der Liste ganz unten `Advanced` aufklappen
    - auf den ersten Eintrag `Configure ESP-IDF Extension` klicken
3. im geöffnetem Willkommensfenster der ESP-IDF Extension:
   - die `Express` Installation auswählen
   - bei der Versionsauswahl die neuste Version auswählen (aktuell 5.5.1)
   - auf `Install` klicken
4. darauf warten, dass die Installation abgeschlossen ist (kann schon etwas dauern)
5. in VS Code links auf den Reiter `Source Control` klicken
   - `Clone Repository` auswählen
   - folgende URL eintragen: https://github.com/BAGProgrammierrunde/monorepo.git
   - gewünschten Zielordner fürs Klonen auswählen
   - falls du aufgefordert wirst dich bei GitHub einzuloggen, gebe deine Logindaten ein
   - wenn du noch kein GitHub Account hast, melde dich hier an: https://github.com/signup
6. nach dem erfolgreichen Öffnen des Projektes den `.vscode` Ordner löschen (wenn vorhanden)
7. in VS Code links auf den Reiter auf das ESP Icon klicken
    - in der Liste ganz unten `Advanced` aufklappen
    - auf den Eintrag `Add .vscode subdirectory files` klicken
    - damit werden alle wichtigen Einstellungen für VS Code erstellt
8. in der Leiste ganz unten in VS Code sollte folgendes eingestellt sein:
   - neben dem Feld `ESP-IDF v5.x.x` und dem Stern sollte `UART` stehen
   - wenn bei dir hier JTAG oder etwas anderes steht, klicke einmal auf den Text und wähle `UART` aus
   - im Feld daneben sollte der richtige COM-Port stehen, wo dein ESP angeschlossen ist
   - klicke auch hier einmal auf das COM Feld und wähle den richtigen Port aus der sich öffnenden Liste aus (hier sollte
   einer esp im Namen haben)
   - neben dem COM Port sollte als Gerät `esp32s3` ausgwählt sein
   - zum Ändern, einmal darauf klicken
   - nachdem ein Gerät ausgewählt wurde, wird man aufgefordert einen Chip auszuwählen - die Auswahl kann mit Escape
   übersprungen werden

Bei Fragen oder Problemen aller Art wende dich super gerne an uns. Wir helfen dir gerne weiter! :)

Viel Spaß mit deinem eingerichteten Projekt.


### Projekt kompilieren / bauen

Um das Projekt zu bauen, kannst du ganz unten in VS Code auf das Schraubenschlüssel-Icon (Build Project) klicken. Dabei
wird der Code kompiliert und ein Image (Betriebssystem + ESP Code + unser Code) erstellt. Hierbei findest du heraus, ob
der Code in eine für den ESP verständliche Sprache umgewandelt werden kann. Du brauchst hierfür *keinen* ESP - es wird
auch nichts übertragen, wenn einer angeschlossen ist.


### Projekt auf den ESP übertragen (flashen)

Um das Projekt auf den ESP zu übertragen, kannst du ganz unten in VS Code auf das Blitz-Icon (Flash Device) klicken. Vor
dem Übertragen muss das Projekt mindestens einmal gebaut worden sein. Es wird immer der zuletzt gebaute Stand übertragen
- nicht nachträglich geänderter Code.


### ESP monitoren

Um den ESP zu monitoren, kannst du ganz unten in VS Code auf das Bildschirm-Icon (Monitor Device) klicken. Monitoren
bedeutet, dass dir die Ausgaben (unter anderem Fehler) von deinem ESP auf deinem Computer angezeigt werden.

Hinweis: Während du das Monitoring aktiv hast, kannst du keinen Code auf den ESP übertragen. Schließe zuerst den Prozess
über das Mülltonnen-Icon.


### Projekt bauen, flashen und monitoren (meistens empfohlen)

Wenn du eine Änderung am Code gemacht hast und diese direkt auf den ESP übertragen möchtest, gibt es auch einen eigenen
Button der alles kombiniert. Hierfür ganz unten in VS Code auf das Flammen-Icon (ESP-IDF: Build, Flash and Monitor)
klicken. Dabei wird das Projekt kompiliert, auf den ESP übertragen und das Monitoring aktiviert.


### Zusätzlicher Tipp

Falls es aus unerklärlichen Gründen mal nicht builden und laufen sollte, lösche den `build` Ordner und versuche das
Projekt erneut zu bauen. Das hat uns öfter geholfen. :)


## Pinout

[ESP32-S3-DevKitC-1 v1.1](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html)

![ESP32-S3-DevKitC-1 Pinout](docs/media/esp32-s3_devkitc-1_pinlayout_v1.1.jpg)


## TODOs

- [x] weißen Kasten um den Dino entfernen
- [ ] Button Integration
- [ ] Springen
- [ ] Score
- [ ] Hitbox / Kollision von Kaktus und Dino ermitteln
- [ ] Game Over Bildschirm
- [ ] dynamisch wechselnder Hintergrund
- [ ] mit Button erneutes Spiel starten
- [ ] Start Bildschirm
- [ ] Sternenhimmel / Wolken
- [ ] Flugsaurier
- [ ] Ducken (voraussetzung dass wir einen 2. Button haben)
- [ ] Scoreboard
- [ ] Power-Ups
- [ ] Kombination aus Springen und Ducken
- [ ] Dino schwimmt
- [ ] Verschiedene Tiere
- [ ] was anderes als Kakteen
- [ ] Leveleditor für den PC / Windows

## Technische offene Fragen

- [ ] Bit-wise Pixel Memory -> Byte-wise Pixel Memory
- [ ] Game + Scene vs. nur Game vs. nur Scene


<br>

___

## MIT License

Copyright (c) 2024 BAGProgrammierrunde

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
