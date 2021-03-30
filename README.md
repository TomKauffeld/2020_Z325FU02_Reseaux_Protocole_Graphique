# 2020_Z325FU02_Reseaux_Protocole_Graphique
This is a project done for the module Reseaux 2 (networking) in the 3rd year for my Bachlor degee.

For this project, we need to create a new protocol to encode binary data into an image with error correction (think QR code for example). And to implement the encoding part of the protocol.

## Credits
For image loading, writing and resizing, using the headers from [stb](https://github.com/nothings/stb) by [Sean Barrett](https://github.com/nothings) and [Jorge Rodriguez](https://github.com/BSVino).  

## Protocol
The data will be split into different blocks of 4 data bits and encoded using Hamming(7,4) with odd parity and using secded.  
On the image, the indexes are labels from the bottom right to the top right, and then the next column :
|   | c1| c2| c3|
|---|---|---|---|
| r1|  8|  6|  3|
| r2|  7|  5|  2|
| r3|  6|  4|  1|

The encoded data will then be inserted in an alternating pattern by putting down one bit from each block repeated 8 times.  
Exemple : using 4 blocks : `a`, `b`, `c` and `d`

|index|block| bit |
|----:|----:|----:|
|    1|    a|    1|
|    2|    b|    1|
|    3|    c|    1|
|    4|    d|    1|
|    5|    a|    2|
|    6|    b|    2|
|    7|    c|    2|
|    8|    d|    2|
|    9|    a|    3|
|   10|    b|    3|
|   11|    c|    3|
|   12|    d|    3|
| ... | ... | ... |

In the top left, bottom left and top right corners, special markers will be placed to detect the presence and size/position of the data image.  
In these markers will also be placed a version number of this protocol in 8 bits (will always set to 1 in this implementation) and the number of blocks inside the image in 8 bits

Top left corner :
|    | c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8 | c9 |
|----|----|----|----|----|----|----|----|----|----|
| r1 |  # |  # |  # |  # |  # |  # |  # |  # | v1 |
| r2 |  # |    |    |    |    |    |    |  # | v2 |
| r3 |  # |    |  # |  # |  # |  # |    |  # | v3 |
| r4 |  # |    |  # |  # |  # |  # |    |  # | v4 |
| r5 |  # |    |  # |  # |    |    |    |  # | v5 |
| r6 |  # |    |  # |  # |    |    |    |  # | v6 |
| r7 |  # |    |    |    |    |    |    |  # | v7 |
| r8 |  # |  # |  # |  # |  # |  # |  # |  # | v8 |
| r9 | s1 | s2 | s3 | s4 | s5 | s6 | s7 | s8 |    |

The rest of the corners will be rotated so the internal white space is oriented to the center of the image.

## Command line interface
In the project `pgtk` a command line interface executable will be created.  
### Usage :  
#### Encoding :
* read from the arguments : `./pgtk encode -o "output_image.bmp" -c "input string"`
* read from a file : `./pgtk encode -o "output_image.bmp" -i "input_file"`
#### Decoding :
* write to the console : `./pgtk decode -i "input_image.bmp" -c`
* write to a file : `./pgtk decode -i "input_image.bmp" -o "output_file"`
