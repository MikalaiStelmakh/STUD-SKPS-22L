# Systemy komputerowe w sterowaniu i pomiarach (SKPS) - Lab 1

## Wstęp
- RasberryPi zostało podłączone do komputera zgodnie z instrukcją. Uruchomiony został system
ratunkowy.
- Logowanie oraz weryfikacja połączenia z internetem między urządzeniami przebiegła pomyślnie. Paczki wygenerowane komendą ping przechodzą zarówno z resberry do komputera, jak i w drugą stronę.

## Kompilacja obrazu z ramdyskiem startowym
- Uruchomiono buildroota w katalogu domowym.
- Ustawiono domyślną konfigurację pasującą do płytki z użyciem komendy: `make raspberrypi4_64_defconfig`
- Wybrano dodatkowe ustawienia, takie jak `external toolchain`, `kompresja obrazu` oraz `initramfs` używając menu otwartego komendą: `make menuconfig`. **Wyłączono** też wsparcie dla `ext2/3/4`.
- Konfigurację zapisano pod nazwą `.configlab1`.
- Wygenerowane pliki przegrano na płytkę.

Po restarcie RasberryPi z przytrzymanym przyciskiem uruchomił się wygenerowany system.

## Kompilacja obrazu bez initramfs
- Ponownie używając komendy `make menuconfig` zmieniono ustawienia. **Wyłączono** `initramfs`
oraz **włączono** wsparcie dla `ext2/3/4`.
- Konfigurację zapisano pod nazwą `.configlab1_2`.
- Wygenerowane pliki umieszczono w katalogu `user`.
- Po restarcie RasberryPi z przytrzymanym przyciskiem, uruchomił się wygenerowany linux.
- Utworzono plik tekstowy.

Po restarcie systemu plik nadal istnieje, widać więc, że linux korzysta z rzeczywistego systemu plików.
