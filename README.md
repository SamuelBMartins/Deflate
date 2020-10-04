# Deflate
This implementation of the deflate algoritm follows the [RFC1951](https://tools.ietf.org/html/rfc1951#section-Abstract) except for the part about the compression of the code lenght sequence (page 12). The software was written following the C99 standard.


## Setup

Run on the root of the project the following command:

```sh
$ make
```
It will generate an executable named `deflate`


## Usage

How to compress:
```sh
$ ./deflate -c <nameFileInput.ext> <nameFileOutput.ext>
```

How to decompress:
```sh
$ ./deflate -d <nameFileInput.ext> <nameFileOutput.ext>
```


## Features

  - huffman coding with self implemented priority queues
  - buffers to reduce file system requests
  - parameterizable block dimension


License
----

MPL-2.0 © Samuel Martins
