# Szkolenie - Wzorce projektowe w C++

## Ankieta

* https://forms.gle/D8p92W5HiPAYpsKU9

## Docs

* https://infotraining.bitbucket.io/cpp-dp
* https://gitpitch.com/infotraining-dev/cpp-dp/develop?grs=bitbucket&p=slides_pl#/

## Maszyna wirtualna (SSH)

* link do rejestracji: https://labs.azure.com/register/rrteny9y
  - wymagania: konto Github lub Microsoft

## Biblioteki

* [Catch2](https://github.com/catchorg/Catch2)
* Boost

## Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)

### Dodatki

* C/C++
* CMakeTools
* Live Share
* Remote - SSH

## Toolchain

### Kompilator + CMake

* Dowolny kompilator C++ (gcc, clang, Visual C++) wspierający C++17

* [CMake > 3.8](https://cmake.org/)

### [Menadżer pakietów - Vcpkg (opcjonalnie)](https://github.com/microsoft/vcpkg)
  
  Instalacja vcpkg: 
  
  ```
  $cd ~
  
  $ git clone https://github.com/microsoft/vcpkg
  $ ./vcpkg/bootstrap-vcpkg.sh
  ```

  Dodać do pliku `.profile`:

  ```
  export VCPKG_ROOT=$HOME/vcpkg
  ```
  
  Instalacja bibliotek:

  ```
  $ ./vcpkg/vcpkg install [packages to install]
  ```
