# BAG Programmierrunde - Monorepo

Hier findest du alle relevanten Dateien und Links rund um unser ESP32 Mikrocontroller-Projekt. Im Projekt entwickeln wir
gemeinsam an einem Gerät, worauf als kleines Projekt zum Anfangen
das [Dino-Spiel](https://de.wikipedia.org/wiki/Dino-Spiel) gespielt werden können soll.

* [Über uns](#über-uns)
* [Hardware Komponenten](#hardware-komponenten)
* [Anleitung](#anleitung)
* [Pinout](#pinout)
* [TODOs](#todos)
* [Technische offene Fragen](#technische-offene-fragen)
* [License](#mit-license)

## Über uns

Wir sind eine offene und freie Programmierrunde. Wir treffen uns jeden Mittwoch um 18 Uhr auf unserem Discord-Server.
Öfters auch spontan zwischendurch in den Voice Channels. Komm einfach mal vorbei und tritt uns bei!

[BAG Programmierrunde](https://discord.gg/dzDScWAK)

Keine Verpflichtungen – alles locker und entspannt. Die Runde soll dazu dienen, sich auszutauschen, voneinander zu
lernen und gemeinsam Spaß zu haben.

## Hardware Komponenten

- ESP32-S3-DevKitC-1 N16R8
- 2.8" TFT LCD 240x320 ST7789
- ein Switch / Button

## Anleitung

Das ist ein einfacher Weg, das Projekt bei dir einzurichten. Wir machen es hauptsächlich so, du kannst natürlich aber
auch frei einen anderen wählen.

### Benötigte Programme

Für diesen Weg müsstest du dir diese Programme einmal herunterladen und installieren:

- Visual Studio Code: https://code.visualstudio.com/
- Git: https://git-scm.com/

### Schritt-für-Schritt Einrichtung

1. Nachdem du VS Code gestartet hast, klicke links auf den Reiter `Extensions`, suche nach folgenden Erweiterungen und
   installiere sie:
    - `C/C++` von Microsoft
    - `ESP-IDF` von Espressif Systems
2. Öffne die ESP-IDF Extension über das Espressif-Symbol in der linken Seitenleiste:
    - Klappe in der Liste ganz unten `Advanced` auf
    - Klicke hier auf den ersten Eintrag `Open ESP-IDF Installation Manager`
    - Wähle im oben in der Mitte geöffnetem Popup `Github` aus
3. Im ESP-IDF Installation Manager:
    - Klicke auf den Button `Start Installation`
    - Wähle `Custom Installation` aus (`Start Configuration Wizard`)
    - Bei Step 2: Klicke auf `Install Python` (falls das Fenster keine Rückmeldung mehr gibt, einfach warten)
    - Bei Step 3: Klicke auf `esp32s3` (oder lasse den Haken bei `All` drin) und dann auf
      `Continue with Selected Targets`
    - Bei Step 4: Klicke bei Stable Releases auf `Latest` (aktuell: `v5.5.3`) und dann unten auf `Continue Installation`
    - Bei Step 5: Klicke auf `Continue with Selected Mirrors` (die schnellsten Server werden automatisch ausgewählt)
    - Bei Step 6: Wähle zusätzlich zu `core` mindestens auch `ide` aus und klicke dann auf `Continue to Tools selection`
    - Bei Step 7: Klicke auf `Continue`
    - Bei Step 8: Klicke auf `Continue` (hier kannst du den Installationspfad ändern, Empfehlung ist es, den Standard zu
      lassen)
    - Bei Step 9: Klicke auf `Start Installation`
    - Warte, bis die Installation abgeschlossen ist
    - Klicke auf `Complete Installation`
    - Klicke auf `Exit Installer`
4. Klicke in VS Code links auf den Reiter `Source Control`
    - Wähle hier `Clone Repository` aus
    - Trage im aufkommenden Dialogfeld dann folgende URL ein: https://github.com/BAGProgrammierrunde/monorepo.git
    - Wähle den gewünschten Zielordner fürs Projekt aus und öffne das Projekt.
    - Logge dich bei GitHub ein, falls du dazu aufgefordert wirst (wenn du noch keinen GitHub Account hast, kannst du
      dich hier anmelden: https://github.com/signup)
5. Nach dem erfolgreichen Öffnen des Projektes, wieder im Explorer, den `.vscode` Ordner löschen (wenn vorhanden)
6. Klicke in der linken Seitenleiste wieder auf die Espressif-Extension:
    - In der Liste ganz unten den Reiter `Advanced` aufklappen
    - Klicke hier auf den Eintrag `Add .vscode subdirectory files`
7. In der Leiste ganz unten in VS Code muss Folgendes eingestellt werden:
    - Klicke auf `ESP-IDF vx.x` und wähle oben im Popup die eben installierte Version aus
    - Klicke daneben auf den Stern und wähle oben im Popup `UART` aus
    - Im Feld daneben sollte `Detect` oder der richtige COM-Port stehen, an dem dein ESP angeschlossen ist
    - Neben dem COM-Port sollte als Gerät `esp32s3` ausgewählt werden
    - Nachdem du das Gerät ausgewählt hast, wirst du aufgefordert einen Chip auszuwählen. Diese Auswahl mit einem Escape
      oder Klick außerhalb des Dialoges schließen

Bei Fragen oder Problemen aller Art melde dich super gerne bei uns. Wir helfen dir gerne weiter! :D
Und nun viel Spaß mit deinem eingerichteten Projekt!

### Das Projekt auf den ESP laden und laufen lassen

#### [Build] Projekt kompilieren / bauen

Um das Projekt zu bauen, kannst du ganz unten in VS Code auf den Schraubenschlüssel Icon (`Build Project`) klicken.
Dabei wird der Code kompiliert und ein Image (Betriebssystem + ESP Bibliothek Code + unser Code) erstellt. Hierbei
findest du heraus, ob dein Code noch jegliche Fehler hat und ob er in eine für den ESP verständliche Sprache umgewandelt
werden kann. Du brauchst hierfür **keinen** ESP. Es würde hier auch nichts übertragen werden, selbst wenn einer
angeschlossen wäre.

#### [Flash] Projekt auf den ESP übertragen

Um das Projekt auf den ESP zu übertragen, kannst du ganz unten in VS Code auf das Blitz-Icon (`Flash Device`) klicken.
Vor dem Übertragen muss das Projekt mindestens einmal wie im vorherigen Schritt gebaut worden sein. Es wird immer der
zuletzt gebaute Stand übertragen. Nicht etwa nachträglich geänderter Code.

#### [Monitor] ESP Ausgaben erhalten

Um Textausgaben, die im Code veranlasst werden (durch das ausprinten von z.B. Variablen etc.) und andere hilfreiche
Informationen wie Laufzeitfehler vom ESP zu sehen, kannst du ganz unten in VS Code auf das Bildschirm-Icon
(`Monitor Device`) klicken.

Hinweis: Während du das Monitoring aktiv hast, kannst du keinen Code auf den ESP übertragen. Schließe zuerst den Prozess
über das Mülltonnen-Icon.

#### [Build | Flash | Monitor] Kombination aller drei (meist empfohlen)

Wenn du eine Änderung am Code gemacht hast, diese direkt auf den ESP übertragen und die Ausgabe starten möchtest,
gibt es auch einen eigenen Button, der alles oben Aufgeführte kombiniert. Hierfür ganz unten in VS Code einfach auf das
Flammen-Icon (`ESP-IDF: Build, Flash and Monitor`) klicken. Dabei wird das Projekt kompiliert, auf den ESP übertragen
und das Monitoring aktiviert.

### Tipps

Falls es aus unerklärlichen Gründen mal nicht builden und laufen sollte, lösche den `build` Ordner (auch ein paar Mal)
und versuche das Projekt erneut zu bauen. Das hat uns öfter geholfen :).

## Pinout

[ESP32-S3-DevKitC-1 v1.1](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html)
![ESP32-S3-DevKitC-1 Pinout](docs/media/esp32-s3_devkitc-1_pinlayout_v1.1.jpg)

## TODOs

TODO Check includes

- [x] weißen Kasten um den Dino entfernen
- [x] Button Integration
- [ ] Springen
- [ ] Score
- [ ] Hitbox / Kollision von Kaktus und Dino ermitteln
- [ ] Game Over Bildschirm
- [ ] dynamisch wechselnder Hintergrund
- [ ] mit Button erneutes Spiel starten
- [ ] Start Bildschirm
- [ ] Sternenhimmel / Wolken
- [ ] Flugsaurier
- [ ] Ducken (Voraussetzung: wir haben einen 2. Button)
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
