# Syntax Checker for C

## General Description
Before a compiler converts a source file into machine-readable code, the syntax of the source file is checked first. This program simulates syntax checking for source code written in C.

## Scope
This program checks the syntax of the following expressions in C:

- Variable declarations
- Function declarations

Pointers and arrays are not included.

### Variable Declarations
The program verifies variable declarations using primitive types:

- `int`
- `char`
- `float`
- `double`

It supports multiple declarations, including those with initializations. Identifiers are checked to ensure they follow C's naming rules: they must start with an underscore (`_`) or a letter, followed by zero or more alphanumeric characters (including underscores).

#### Examples
```c
int x;
char c, h;
float pi = 3.1416;
double x, y, z = 6.022140857;
```

### Function Declarations
The program supports function declarations returning primitive type values and accepting primitive type arguments. Functions that do not return a value (`void`) are also supported. Both function declarations with and without argument lists are checked. Multiple function declarations in a single statement are also supported.

#### Examples
```c
void display();
void compute(void);
int square(int);
int power(int, int);
char toLower(char);
double squareRoot(float);
int gcd(int, int), lcm(int, int);
int isPrime(int n);
```

## Input
The program reads input from standard I/O (console). The first input is a positive number representing the number of test cases. The actual test cases follow in subsequent lines. Each test case starts with either `1` or `2`:

- `1` indicates a variable declaration
- `2` indicates a function declaration

## Output
For each test case, the program prints a single line:
- If the test case is valid: `VALID VARIABLE DECLARATION.` or `VALID FUNCTION DECLARATION.`
- If the test case is invalid: `INVALID VARIABLE DECLARATION.` or `INVALID FUNCTION DECLARATION.`

## Constraints and Additional Instructions
- The program is implemented in C, C++, Java, or Python.
- References used in developing this program should be included as comments or documentation in the source file.
- If any questions arise, refer to relevant programming resources.

## Sample Test Cases
### Input
```
4
1 int x, y, z = 10;
1 double a int function();
2 INT function2(void);
2 int solve(int, char, double x, float, int, int);
```

### Output
```
VALID VARIABLE DECLARATION.
INVALID VARIABLE DECLARATION.
INVALID FUNCTION DECLARATION.
VALID FUNCTION DECLARATION.
```
