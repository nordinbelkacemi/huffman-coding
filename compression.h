// algorithm for writing the binary huffman codes of a text into a file.
//
// buffer = {_,_,_,_,_,_,_,_}, nextbuffer = {_,_,_,_,_,_,_,_}
//
// while character read from text is not EOF:
//     add huff code of character to the buffer.
//     if buffer full:
//         copy overflow into nextbuffer if needed
//         write buffer into file
//         buffer = nextbuffer
//         clear nextbuffer
//
// // end of file
// add trailing zeroes to the buffer if needed {1,1,0,1,_,_,_,_} -> {1,1,0,1,0,0,0,0}
// write buffer into file.