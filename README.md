# Obliczanie liczby PI

Niniejszy program realizuje równoległe obliczenia liczby PI metodą Monte-Carlo w języku C z wykorzystaniem biblioteki MPI.

## Instalacja

### Windows
- Wymaga WSL
- Na WSL należy wywołać komendę `sudo apt-get update` by zaktualizować paczki
- Następnie należy zainstalować mpi za pomocą komendy `sudo apt install mpich`
- W przypadku problemów z paczkami należy w pliku `/etc/resolv.conf` ustawić `nameserver 8.8.8.8` (połączenie z DNSem Googla) i spróbować powyższych komend ponownie

### Linux (Ubuntu)
- Wykonujemy te same kroki co na WSLu

## Użycie

### Kompilacja wszystkich plików
- wywołanie komendy `make all`
- po zmianie plików źródłowych należy przed przystąpieniem do kompilacji wyczyścić pliki wykonywalne komendą `make clean`

### Sekwencyjne obliczanie liczby PI metodą Monte-Carlo

```
./monte-carlo-seq <number_of_iteration>
```

- argument `<number_of_iteration>` jest typu `int` i jest opcjonalny - domyślnie zostanie wykonany milion iteracji
- losowość liczb została ustawiona przez ustawienie ziarna przy każdym uruchomieniu w oparciu o punkt w czasie

### Równoległe obliczanie liczby PI metodą Monte-Carlo

```
mpirun -np <num_procs> ./monte-carlo
```

Po wyświetleniu liczby komend podajemy oczekiwaną liczbę losowań.


