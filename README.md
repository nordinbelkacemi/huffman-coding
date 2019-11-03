# Huffman kódoló specifikáció

A program lényegében egy fájltömörítő amely két műveletet tud végezni...

## Tömörítés

**input:** 
* `example.txt` fájl (eredeti fájl)

A felhasználó operációs rendszeréhez megfelelő program futtatási parancsot beírhatjuk

|OS| command |
|--|--|
| Mac OS / Linux | `./main compress example.txt` |
| Windows | `main.exe compress example.txt` |

**output** 
* `tiny_example.txt` fájl (tömörített fájl)
* `huffman_example.txt` fájl (kódtáblázat)

## Visszaállítás

Ez a feladat a tömörítés ellentetje, azaz egy tömörített fájt visszaállítunk az eredeti alakjába.

**input:** 
* `tiny_example.txt` fájl
* `huffman_example.txt` fájl

A felhasználó operációs rendszeréhez megfelelő program futtatási parancsot beírhatjuk

|OS| command |
|--|--|
| Mac OS / Linux | `./main decompress tiny_example.txt huffman_example.txt`    |
| Windows | `main.exe decompress tiny_example.txt huffman_example.txt`    |

**output** 
* `example.txt` fájl

## Tömörített adatok (Huffman kódolás)

Minden az eredeti fájlban lévö karakterekhez hozzárendel egy egyértelmü huffman kódszót, azaz a tömörített fájlból egy huffman fával (vagy kód táblázattal) egyértelmüen ki lehet olvasni a kódszavakat és rekonstruálni az eredeti kódot.

#### Példa

`example.txt` fájl tartalma:
```
aabacdab
```

`huffman_example.txt` fájl tartalma:
```
a: 0
b: 10
c: 110
d: 111
```

`tiny_example.txt` fájl tartalma:
```
	00100110111010
```

A kódtáblázatban levő kódszavak prefix kódot alkotnak, azaz egyik kódszó sem kezdödik egy másikkal.

példa **nem** prefix kódra:
```
	a: 0
	b: 01
```
A `b` kódszó `a`-val kezdödik, vagyis ez nem prefix kód.
