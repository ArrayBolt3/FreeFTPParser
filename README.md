# FreeFTPParser - a (now obsolete) drop-in replacement for Professor Bernstein's ftpparse library

DON'T USE THIS - this has gone from being a FOSS replacement for non-free software to being an interesting coding exercise, as [the original ftpparse is now public-domain where possible, and FOSS elsewhere.](https://cr.yp.to/distributors.html)

This library exists for one reason - providing the functionality of the ftpparse library in a truly free-and-open-source package.

FreeFTPParser's job is to parse the output of FTP LIST responces. These are (or at least were) very poorly standardized, making it difficult to extract information from them in applications. FreeFTPParser makes it easy to parse these responces by simply feeding them in line-by-line to the parser's `ftpparse()` function. This function extracts information from the LIST responce lines and places it into a struct for easy use. The API is fully compatible with ftpparse. Everything is licensed under the 0BSD license, which is approximately equivalent to Bernstein's original ftpparse license minus the commercial use restriction and with a warranty disclaimer.

Currently FreeFTPParser supports UNIX-style FTP LIST output only. Further formats may be added at a later date - just open a new issue and provide a sample of LIST output from an incompatible server and I'll try to add support for it. (I wanted to add support for more formats but literally could not find any of them documented anywhere except in code I couldn't use!)

# License and Copyright

See the LICENSE file for detailed license information. FreeFTPParser is licensed under the 0BSD license.

Copyright 2023 Aaron Rainbolt.

# TODO

The documentation is an absolute mess.

A lot more robustness checks need to be done - the testing I've done is significantly less than thorough.

Perhaps support for something more than just the one format of LIST output should be added?

What happens if you try to compile this with something other than GCC?

Normalize line length to 80 characters everywhere.
