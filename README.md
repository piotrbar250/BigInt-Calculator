# Console-Based Arithmetic and Base Conversion Application

The app is designed for performing basic arithmetic operations and numeric conversions between bases 2 to 16. The application reads input from a text file, processes queries, and saves the results in a new text file. It is compatible with both Windows and Linux systems, however you might need to pay attention to the input file fromat. Depending on the operating system, you should use either LF or CRLF as the end-of-line sequence. 

## Input File Format

The application reads queries from a text file named `input.txt`, located in the same folder as the application. Queries should follow one of the two formats below:

- For arithmetic operations:
  ```
  <operation sign> <number system>
  
  <number from the specified system>
  
  <number from the specified system>
    
  ```

- For conversions:
  ```
  <number system> <number system>
  
  <number from the first specified system>

  ```

There is no requirement to include the number of tests in the file; the program handles any number of queries.

### Notes on Compatibility

The program accounts for end-of-line character differences across operating systems, ensuring compatibility with Windows and Linux.

## Compilation and Execution

Compile the source code using GCC:

- Windows (Command Line): `gcc main.c -o main.exe`
- Linux (Terminal): `gcc main.c -o main`

To start the program:

- Windows: `main.exe`
- Linux: `./main`

Upon launching, the application displays welcome information and a reminder to ensure `input.txt` is in the correct folder. The application exits upon typing `q`; any other key triggers the processing of the input file.

## Output File

Results are saved in `output.txt` in the application's folder, following specific formats for arithmetic operations or conversions. An additional empty line is added at the end of the program's output.

## Program Structure

The application reads five lines per test from `input.txt`, with an additional two lines for arithmetic operations. It allocates necessary memory for calculations, performs error checks, and converts numbers between systems to perform operations. Memory is released at the end of each test.

### Operations

- **Conversion to Decimal**: Sums powers of the base multiplied by the digit.
- **Conversion from Decimal**: Iterative calculation of remainders and divisions by the base.
- **Arithmetic Operations**: Simulate classic under-the-line calculations.
- **Exponentiation**: Uses a fast exponentiation algorithm.
- **Modulo Operation**: Involves division, multiplication, and subtraction.

## Error Handling

The program checks for division by zero, leading zeros, out-of-range systems, and incorrect number system representations, terminating with an error message upon detection.

## Additional Information

Tested with over 20,000 cases, the application is robust against memory leaks and errors, as verified by Valgrind. More complex queries might require additional processing time.