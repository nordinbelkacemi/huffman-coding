// algorithm for writing the binary huffman codes of a text into a file.
//
// buffer as binary: {_,_,...,_} 8 bytes (64 bits)
//
// while character read from text is not EOF:
//     add huff code of character to the buffer.
//     while buffer_size >= 8:
//         write first byte of the buffer into file
//         shift the first byte of the buffer out of it
//
// write the first byte of the buffer into the file (writes with trailing zeroes)