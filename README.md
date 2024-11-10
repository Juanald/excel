# An Excel Engine

This is a project that simulates an Excel engine. It is capable of reading from a CSV, and evaluating arithmetical expressions referencing cells on the fly. In this way, it allows for basic equations, much like Excel allows with the =B1+B2... syntax.

## Inspiration

I was inspired by reading Bill Gates: Hard Drive. This book followed Bill Gates' rise to prominance, and I was always curious about how Excel was written, and how it came to such prominance. I found out that Excel was originally written in C++, and did this program in C++ to both learn it, and to experience what original Microsoft programmers might have experienced in the 1980s.

## Running

To run the engine, first formulate your data.csv file. The first row are usually the headers, and then follows data. For any cell, you can have an arithemtical expression referencing other cells: eg. =A2+B2\*-(B3). Note that the unary negation operator requires brackets.

Clone the project by running `git clone https://github.com/Juanald/excel.git`.
Then navigate to the src directory: `cd excel/src`.
Build the project by running `make`.
Run the program by running `./excel.exe`.

Improvements are welcome!
