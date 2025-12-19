# Tools

## Font Generation

Im Ordner `font_generation` befindet sich ein Python Script, mit welchem Fonts für unser ESP Projekt erstellen werden
können.


## ESP-IDF in der Konsole ausführen

Um die ESP-IDF in der Konsole zum Laufen zu bringen, kannst du folgenden Befehl nutzen:

### Windows:

```cmd
./docs/tools/idf.bat
```

### Mac:

```
./docs/tools/idf.sh
```

Bei Problemen mit Permissions einmalig `chmod +x idf.sh`

### Build Flash Monitor

```cmd
idf.py build flash monitor
```
