##### Table of Contents

- [Architektur](#architektur)
- [Links](#links)
- [Hilfen \& Tutorials](#hilfen--tutorials)
    - [Platformio Tutorials](#platformio-tutorials)

## Architektur

Unter src befindet sich alles an Source Code. Der Einstiegspunkt ist unter main/main.cpp zu finden.

Die Hardwarekomponenten liegen unter src/components. Die Components sind unsere Abstraktionsschicht zur Hardware. Sie
werden in src/components/structure/Device für einen einfachen Zugriff zusammengefasst. Wir greifen aus main nie direkt
auf die Hardware zu.

## Links

- [Miro Board](https://miro.com/app/board/o9J_loXnq8o=/)
- [GitHub Repository](https://github.com/BAGProgrammierrunde/monorepo)

## Hilfen & Tutorials

### Platformio Tutorials

[Youtube - Platformio Tutorial (deutsch)](https://www.youtube.com/watch?v=0lCem8XTKMs)








