# Deflate
This implementation of the deflate algoritm follows the [RFC1951](https://tools.ietf.org/html/rfc1951#section-Abstract) except for the part about the compression of the code lenght sequence (page 12). The software was written following the C99 standard.

# Features

  - huffman coding with self implemented priority queues
  - buffers to reduce file system requests
  - parameterizable block dimension

# Setup

run on the root of the project the following command:

```sh
$ make
```
it will generate an executable named `deflate`

# Usage

compress:
```sh
$ ./deflate -c <nameFileInput.ext> <nameFileOutput.ext>
```

decompress:
```sh
$ ./deflate -d <nameFileInput.ext> <nameFileOutput.ext>
```

License
----

MPL-2.0 © Samuel Martins
