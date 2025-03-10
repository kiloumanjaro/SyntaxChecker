# Syntax Checker for C Code

## General Description
Before a compiler converts a source file into machine-readable code, the syntax of the source file is checked first. For this MP, you are going to simulate this syntax checking for source code written in C.

## Scope
The MP will require you to check the syntax of the following expressions in C:
- Variable declaration
- Function declaration

Pointers and arrays are not in scope for this MP.

### Variable Declaration
The MP will stick with primitive types. The types are:
- `int`
- `char`
- `float`
- `double`

Multiple declarations, including those with initializations, are in scope. This also means that identifiers have to be checked. They should follow the naming rule of C, i.e., they can start with `_` or any letter from the English alphabet, followed by 0 or more alphanumeric characters, including `_`.

#### Samples
```c
int x;
char c, h;
float pi = 3.1416;
double x, y, z = 6.022140857;
```

### Function Declaration
In a similar fashion, what will be in scope are functions returning primitive type values and primitive type arguments only. In addition, functions that do not return any value, i.e., declared as `void`, are in scope. Functions with and without any argument list are in scope. And like in variable declarations, multiple function declarations are in scope too.

#### Samples
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
The input is entirely from standard I/O (console). It will contain a number of lines. The first input is a positive number. This positive number, say `c`, represents the number of test cases there are. The actual test cases will follow in the next lines. Each test case starts with either `1` or `2`. `1` indicates that the test is supposed to be a variable declaration, `2` a function declaration.

## Output
Print all output on the console (standard output). It should have `c` lines, `c` being the number of test cases. If the test case is valid, print:
```
VALID <type>.
```
Where `<type>` is either:
- `VARIABLE DECLARATION`
- `FUNCTION DECLARATION`

## Constraints and Other Instructions
- Implement this using **C, C++, Java, or Python**.
- You are to include all the references you have used in solving this MP. The references may be your old notes, online resources, etc. Include them in your source file as comments or documentation.
- If you have any questions about the MP, post them in the appropriate forum discussion board.
- As an additional reference, please check the sample test cases provided for this MP.

## Sample Test Case
### Input
```
4
1 int x, y, z = 10;
1 double a int function();
2 INT function2(void);
2 int solve(int,char,double x,float,int,int);
```

### Output
```
VALID VARIABLE DECLARATION
INVALID VARIABLE DECLARATION
INVALID FUNCTION DECLARATION
VALID FUNCTION DECLARATION
