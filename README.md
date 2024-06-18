# sh
# SHITPRESS

Tiny suckless compression library. 


Features

- single header library, no linking bullcrap:
  - fewer than 200 lines, extremely small and nonbloated
  - both compression and decompression are streaming! no file buffers needed
  - constant memory complexity, super low RAM requirements (a few bytes)
  - linear time complexity, super fast
  - ABSOLUTELY ZERO dependencies, not even standard library is used
- two executable utilities using the library also included:
  - sh: Super simple, fast, streaming, max. compression ratio 33%.
  - sushh: Non-streaming, repeatedly compresses as long as file size
    reduces, potentially compresses more (even to less than 1%!) but is slower,
    eats more RAM and uses malloc. Guarantees to never enlarge a file by more
    than 1 byte (unlike normal shitpress)!
- generally usable for any type of data, not assuming any specific use (such as
  file compression), i.e. can be used in any wild hacking style (e.g. to
  compress stuff in RAM on low-RAM embedded computers etc.)


Here are some measured stats for sh , sushh and a commonly used
lz4 utility (sizes in bytes):

| input                   | orig. size | compressed (shitpress) | compressed (supershit.) | compr. r. (shitpr.) |  compr. r. (supersh.) | compressed (lz4) | compr. r. (lz4) |
| ----------------------- | ---------- | ---------------------- | ----------------------- | ------------------- | --------------------- | ---------------- | --------------- |
| 100000 "A"s txt         | 100002     | 33339                  | 134                     | 33.3%               | 0.1% !!!              | 422              | 42.0%           |
| WM Commons screens. raw | 3072000    | 1522151                | 1018285                 | 49.5%               | 33.1%                 | 721531           | 23.4%           |
| sh binary        | 30888      | 16995                  | 16896                   | 55.0%               | 54.7%                 | 12859            | 41.6%           |
| sh source        | 4886       | 3834                   | 3836                    | 78.4%               | 78.5%                 | 580              | 75.3%           |
| Wikip. main page HTML   | 96085      | 78957                  | 78959                   | 82.1%               | 82.1%                 | 32459            | 33.7%           |
| Flatland (txt book)     | 219615     | 197874                 | 197876                  | 90.1%               | 90.1%                 | 135782           | 61.8%           |
| 100000 pi digits txt    | 100002     | 94254                  | 94256                   | 94.2%               | 94.2%                 | 95354            | 95.3%           |
| Lenna raw image         | 1048576    | 1023261                | 1023263                 | 97.5%               | 97.5%                 | 1039481          | 99.1%           |
| Willhelm scr. raw audio | 155858     | 152170                 | 152172                  | 97.6%               | 97.6%                 | 152434           | 97.8%           |
| 100% random data        | 1024       | 1027                   | 1025                    | 100.2%              | 100.0%                | 1043             | 101.8%          |
| forged malicious data   | 100        | 200                    | 101                     | 200.0%              | 101.0%                | 30               | 30.0%           |

