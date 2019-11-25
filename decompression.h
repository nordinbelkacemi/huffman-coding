// decompression algorithm:
//
// buffer = {_,_,_,_,_,_,_,_}
// p = pointer to the root of the huffman tree
// n = number of characters to be written
// k = number of characters already written
//
// while a byte can succesfully be read from the file:
//     while we haven't reached the end of the buffer and k != n:
//         if the bit is zero, p = p->left
//         if the bit is one, p = p->right
//         if p points to a leaf node, print the character into the file
//         take p back to the root of the huffman tree
//         