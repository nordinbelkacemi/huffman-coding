# File compression program using Huffman coding

The program can compress text files, and restore them.

## Compression

**input:** 
* `example.txt` file (original file)

Run the following command to compress the file

|OS| command |
|--|--|
| Mac OS / Linux | `./main compress example.txt` |
| Windows | `main.exe compress example.txt` |

**output** 
* `tiny_example.dat` file (compressed file)
* `example_helper.dat` file (helper file containing the huffman table);

## Decompression

**input:** 
* `tiny_example.dat` file (compressed file)
* `huffman_example.txt` file (helper file)

Run the following command to restore the compressed file.

|OS| command |
|--|--|
| Mac OS / Linux | `./main restore tiny_example.dat`    |
| Windows | `main.exe restore tiny_example.dat`    |

**output** 
* `example_restored.txt` file (restored file)

## Result

Compressing a text file of the bible, the following huffman tree was generated:

![Alt text](./bible_htree.svg)
<img src="./bible_htree.svg">