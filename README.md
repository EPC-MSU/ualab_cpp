# CMake Qt5 Qwt GUI for usbadc10
---
You need Qt5, Qwt6(for qt5) and CMake 3.0 or higher.
Put libusbadc10 in to usbadc10-debian for linux and usbadc10-win64 for win64 in current folder.
Not testet for windows.
Thread only for view here.

## Build instructions for linux

```
git clone https://github.com/EPC-MSU/UALab.git
cd UALab
cmake ../
make
./UALab
```
I use ninja for development.

