# Systemy komputerowe w sterowaniu i pomiarach (SKPS) - Lab 2

## Przygotowanie OpenWRT
- Za pomocą systemu ratunkowego na karcie SD ściągnięto OpenWRT.
- Zamontowano partycje zgodnie z instrukcją.
- Zresetowano system i uruchomiono OpenWRT.
- Skonfigurowano i sprawdzono połączenie z siecią i zidentyfikowano problemy z DNS. Problem rozwiązano zgodnie ze wskazówkami w instrukcji.
- Za pomocą managera pakietów `opkg` zainstalowano `python3`, `pip` oraz `git`.
- Z pomocą `pip` zainstalowano bibliotekę `gpio4` do pythona.

## Zadanie 1: GPIO - wyjście dla LED
Napisano skrypt w pythonie zapalający i gaszący diodę 10 razy. Wykorzystano żółtą diodę na
płytce. Skrypt znajduje się w pliku `zad1.py`

## Zadanie 2: GPIO - wyjście dla LED z płynną zmianą jasności
Napisano skrypt w pythonie rozświetlający i gaszący diodę. Wykorzystano żółtą diodę na płytce.
Skrypt znajduje się się w pliku `zad2.py`.

## Zadanie 3: GPIO - wyjście PWM, buzzer pasywny
Napisano skrypt w pythonie, który za pomocą podłączonego do płytki buzzera gra dźwięki gamy c
dur w dwóch oktawach. Pomiędzy poszczególnymi dźwiękami jest chwila ciszy. Skrypt gra dźwięki
od najniższego do najwyższego. Skrypt znajduje się w pliku `zad3.py`.

## Zadanie 4: GPIO - wejście
Napisano skrypt w pythonie gaszący świecącą diodę przy naciśnięciu przycisku. Wykorzystano
czerwoną diodę i przycisk nr 1 na płytce. Skrypt znajduje się w pliku `zad4.py`.

## Zadanie 5: Servo
- Jako dodatkowy element wybrano servo.
- Narysowano schemat i pokazano go prowadzącemu.
- Zrealizowano schemat z użyciem fizycznych komponentów.
- Napisano skrypt w pythonie pozwalający ustawić serwo w określonej pozycji.

Skrypt zajmuje się w pliku `zad5.py`.

