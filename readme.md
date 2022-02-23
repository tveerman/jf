# jf - A trivial JSON formatter

jf is a trivial JSON formatter. It's basically a wrapper around the marvelous "[JSON for Modern C++](https://github.com/nlohmann/json)" by Niels Lohmann. Output is written to stdout.

## Usage
```bash
$ jf -h
jf [-h] [-i n] [file]
        -h      Shows this help message.
        -i      Number of spaces to indent. Default is 2. Use -1 for no indentation at all.
        file    Path to file to indent. When no path is provided the data is read from stdin.
```

## Diagnostics
The jf utility exits 0 on success, and >0 if an error occurs. On success the output written to stdout. JSON parse errors are written to stderr.


