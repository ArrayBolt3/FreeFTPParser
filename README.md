# FreeFTPParser - a drop-in replacement for Professor Bernstein's ftpparse library

This library exists for one reason - providing the functionality of the ftpparse library in a truly free-and-open-source package.

FreeFTPParser's job is to parse the output of FTP LIST responces. These are (or at least were) very poorly standardized, making it difficult to extract information from them in applications. FreeFTPParser makes it easy to parse these responces by simply feeding them in line-by-line to the parser's `ftpparse()` function. This function extracts information from the LIST responce lines and places it into a struct for easy use. The API is fully compatible with ftpparse. Everything is licensed under the 0BSD license, which is approximately equivalent to Bernstein's original ftpparse license minus the commercial use restriction and with a warranty disclaimer.

Currently FreeFTPParser supports UNIX-style FTP LIST output only. Further formats may be added at a later date - just open a new issue and provide a sample of LIST output from an incompatible server and I'll try to add support for it. (I wanted to add support for more formats but literally could not find any of them documented anywhere except in code I couldn't use!)

# Guidelines for developing FreeFTPParser

ftpparse's documentation is sparse and partially mixed into the source code. Obviously we can't study how ftpparse does things in order to provide a free-and-open-source alternative, but that makes it hard to get the info we need to make a perfect drop-in replacement. For this reason, we have adopted the following policy:

1. No FreeFTPParser contributor can closely study ftpparse.c from the ftpparse library.
2. No one who has closely studied ftpparse.c from the ftpparse library in the past can contribute to FreeFTPParser.
3. Studying ftpparse.h from the ftpparse library is permissible, as it consists solely of documentation and an API definition.
4. No copy-pasting of code from the ftpparse library into FreeFTPParser is permitted, not even if it comes from ftpparse.h. (FreeFTPParser's ftpparse.h was written from scratch using ftpparse's ftpparse.h as a guideline but without copying code.)

Additionally, it is not permissible to make any API breaking changes to FreeFTPParser, as it is designed to be simply dropped into existing projects that use ftpparse and work without issues.

# License and Copyright

See the LICENSE file for detailed license information. FreeFTPParser is licensed under the 0BSD license.

Copyright 2023 Aaron Rainbolt.

# TODO

The documentation is an absolute mess.

A lot more robustness checks need to be done - the testing I've done is significantly less than thorough.

Perhaps support for something more than just the one format of LIST output should be added?

What happens if you try to compile this with something other than GCC?

Normalize line length to 80 characters everywhere.
