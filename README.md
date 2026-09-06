# ChatGaiya++ Compiler 🚀

**ChatGaiya++** is a custom programming language and compiler written in **C++17**. It is designed around **Chittagonian (Chatgaiya) dialect-inspired keywords** and transpiles ChatGaiya++ source files (`.cg`) into executable **Python 3** code.

The compiler demonstrates several core concepts of compiler design, including:

* 🔤 Lexical Analysis
* 🧩 Syntax Parsing
* 🧠 Static Type Checking
* ⚠️ Syntax & Type Error Detection
* 🔄 Source-to-Source Translation
* 🐍 Python 3 Code Generation

---

## ✨ Features

* Custom programming language with Chatgaiya-inspired syntax
* Integer, floating-point, string, and boolean data types
* Variable declaration and assignment
* Arithmetic and logical expressions
* Operator precedence
* `if`, `else if`, and `else` statements
* `while` loops
* `break` and `continue`
* Console input and output
* Automatic input type conversion
* Increment and decrement operators
* String concatenation
* String indexing
* Boolean input handling
* Static type checking
* Syntax error reporting
* Python 3 code generation

---

## 🛠️ Technologies

| Technology               | Purpose                                             |
| ------------------------ | --------------------------------------------------- |
| **C++17**                | Compiler implementation                             |
| **Python 3**             | Generated target language                           |
| **g++**                  | Compiler used to build ChatGaiya++                  |
| **Standard C++ Library** | Lexing, parsing, type checking, and code generation |

---

## 📁 Project Structure

```text
ChatGaiya++/
│
├── main.cpp          # ChatGaiya++ compiler source code
├── example.cg        # Example ChatGaiya++ program
├── example.py        # Generated Python program
└── README.md         # Project documentation
```

---

# 🛠️ Build & Installation

## Requirements

Make sure the following are installed:

* **g++** with C++17 support
* **Python 3**

You can verify the installations using:

```bash
g++ --version
python3 --version
```

### Compile the Compiler

```bash
g++ -std=c++17 -O2 main.cpp -o chatgaiya
```

On Windows, you can use:

```bash
g++ -std=c++17 -O2 main.cpp -o chatgaiya.exe
```

---

# 🚀 Usage

## 1. Compile a ChatGaiya++ Program

Suppose you have:

```text
example.cg
```

Run:

```bash
./chatgaiya example.cg
```

The compiler will generate:

```text
example.py
```

On Windows:

```powershell
.\chatgaiya.exe example.cg
```

---

## 2. Specify an Output File

You can specify a custom Python output file:

```bash
./chatgaiya example.cg -o output.py
```

On Windows:

```powershell
.\chatgaiya.exe example.cg -o output.py
```

---

## 3. Run the Generated Python Program

```bash
python3 output.py
```

On Windows:

```powershell
python output.py
```

---

# 📖 Language Reference

## 1. Data Types

ChatGaiya++ provides four basic data types.

| ChatGaiya++ | Equivalent | Description           |
| ----------- | ---------- | --------------------- |
| `ongko`     | `int`      | Integer               |
| `dhoshomik` | `float`    | Floating-point number |
| `kotha`     | `string`   | String                |
| `ho`        | `bool`     | Boolean               |

### Example

```cpp
ongko age = 20;
dhoshomik height = 5.8;
kotha name = "Nafi";
ho student = hasa;
```

---

## 2. Boolean Values

| ChatGaiya++ | Python  |
| ----------- | ------- |
| `hasa`      | `True`  |
| `misa`      | `False` |

Example:

```cpp
ho isStudent = hasa;

zodi (isStudent) {
    ko("Student");
}
```

---

# 🔀 Control Flow

## If Statement

ChatGaiya++ uses `zodi` for conditional statements.

```cpp
zodi (age >= 18) {
    ko("Adult");
}
```

Generated Python:

```python
if (age >= 18):
    print("Adult")
```

---

## If-Else Statement

```cpp
zodi (age >= 18) {
    ko("Adult");
} noile {
    ko("Minor");
}
```

Generated Python:

```python
if (age >= 18):
    print("Adult")
else:
    print("Minor")
```

---

## Else-If Statement

Use `noile zodi` for an `else if` branch.

```cpp
zodi (marks >= 80) {
    ko("A+");
} noile zodi (marks >= 70) {
    ko("A");
} noile {
    ko("Below A");
}
```

Generated Python:

```python
if (marks >= 80):
    print("A+")
elif (marks >= 70):
    print("A")
else:
    print("Below A")
```

---

# 🔁 While Loop

The `zotokkhon` keyword represents a `while` loop.

```cpp
ongko count = 0;

zotokkhon (count < 5) {
    ko(count);
    count++;
}
```

Generated Python:

```python
count = 0

while (count < 5):
    print(count)
    count += 1
```

---

## Break

Use `tham` to exit a loop.

```cpp
zotokkhon (hasa) {
    ko("Running");
    tham;
}
```

Equivalent Python:

```python
while True:
    print("Running")
    break
```

---

## Continue

Use `chol` to skip the current iteration.

```cpp
zotokkhon (count < 10) {
    count++;

    zodi (count == 5) {
        chol;
    }

    ko(count);
}
```

Equivalent Python:

```python
while count < 10:
    count += 1

    if count == 5:
        continue

    print(count)
```

---

# 🖥️ Input & Output

## Output

Use `ko()` to print an expression.

```cpp
ko("Hello ChatGaiya++");
```

Generated Python:

```python
print("Hello ChatGaiya++")
```

Variables can also be printed:

```cpp
ongko age = 20;
ko(age);
```

---

## Input

Use `lo()` to read input from the console.

```cpp
kotha name = lo();
```

Generated Python:

```python
name = input()
```

---

# 🔄 Automatic Input Conversion

ChatGaiya++ automatically converts input according to the declared variable type.

### Integer

```cpp
ongko age = lo();
```

Generated Python:

```python
age = int(input())
```

### Floating Point

```cpp
dhoshomik height = lo();
```

Generated Python:

```python
height = float(input())
```

### String

```cpp
kotha name = lo();
```

Generated Python:

```python
name = input()
```

### Boolean

```cpp
ho status = lo();
```

Generated Python:

```python
status = (input().strip().lower() in ('hasa', 'true', '1'))
```

---

# ➕ Operators & Expressions

## Arithmetic Operators

ChatGaiya++ supports common arithmetic operators:

| Operator | Operation      |
| -------- | -------------- |
| `+`      | Addition       |
| `-`      | Subtraction    |
| `*`      | Multiplication |
| `/`      | Division       |
| `%`      | Modulo         |

Example:

```cpp
ongko a = 10;
ongko b = 3;

ko(a + b);
ko(a - b);
ko(a * b);
ko(a / b);
ko(a % b);
```

---

## Increment & Decrement

ChatGaiya++ supports:

```cpp
x++;
x--;
```

They are translated into:

```python
x += 1
x -= 1
```

Example:

```cpp
ongko x = 10;

x++;
ko(x);

x--;
ko(x);
```

---

# 🧠 Comparison Operators

| Operator | Meaning               |
| -------- | --------------------- |
| `==`     | Equal                 |
| `!=`     | Not equal             |
| `>`      | Greater than          |
| `<`      | Less than             |
| `>=`     | Greater than or equal |
| `<=`     | Less than or equal    |

Example:

```cpp
zodi (age >= 18) {
    ko("You can vote");
}
```

---

# 🔗 Logical Operators

ChatGaiya++ supports:

| ChatGaiya++ | Python | Meaning     |
| ----------- | ------ | ----------- |
| `&&`        | `and`  | Logical AND |
| `\|\|`      | `or`   | Logical OR  |
| `!`         | `not`  | Logical NOT |

Example:

```cpp
zodi (age >= 18 && student == hasa) {
    ko("Adult student");
}
```

Generated Python:

```python
if (age >= 18 and student == True):
    print("Adult student")
```

---

# 🔤 Strings

Strings are represented using the `kotha` type.

```cpp
kotha name = "Nafi";
kotha university = "Leading University";

ko(name);
ko(university);
```

---

## String Concatenation

Strings can be combined using `+`.

```cpp
kotha name = "Nafi";

ko("Hello " + name);
```

Generated Python:

```python
print("Hello " + str(name))
```

ChatGaiya++ automatically converts non-string values when they are concatenated with strings.

Example:

```cpp
ongko age = 21;

ko("Age: " + age);
```

Generated Python:

```python
print("Age: " + str(age))
```

---

# 🔢 String Indexing

String characters can be accessed using an index.

```cpp
kotha name = "Nafi";

ko(name[0]);
```

ChatGaiya++ translates string indexing through its internal character-access mechanism.

Conceptually:

```python
_cg_charat(name, 0)
```

This provides controlled character access for the generated Python program.

---

# 🧪 Static Type Checking

ChatGaiya++ performs static type checking during compilation.

For example:

```cpp
ongko x = "Hello";
```

will result in a compilation error because `ongko` expects an integer.

Similarly:

```cpp
dhoshomik x = "ChatGaiya";
```

is invalid because a string cannot be assigned directly to a floating-point variable.

This allows many type-related errors to be detected **before the generated Python program is executed**.

---

# 🔍 Type Inspection

ChatGaiya++ provides:

```cpp
ki_type(expr)
```

to inspect the compile-time type of an expression.

Possible results include:

```text
ongko
dhoshomik
kotha
ho
```

Example:

```cpp
ongko age = 20;

ko(ki_type(age));
```

---

# 📦 Include / Header Syntax

ChatGaiya++ supports header/include-style lines such as:

```cpp
shuru_kor
```

and:

```cpp
#include
```

These directives are currently ignored by the compiler and do not generate Python code.

---

# 📝 Complete Example

### `example.cg`

```cpp
ongko num = 10;
kotha name;

ko("Enter your name:");
name = lo();

ko("Hello " + name);

zodi (num > 5) {
    ko("Number is greater than 5");
} noile {
    ko("Number is 5 or less");
}

ongko count = 0;

zotokkhon (count < 3) {
    ko(count);
    count++;
}
```

### Generated `example.py`

```python
#!/usr/bin/env python3
# Generated by ChatGaiya++ compiler

num = 10
name = ""

print("Enter your name:")
name = input()

print("Hello " + str(name))

if (num > 5):
    print("Number is greater than 5")
else:
    print("Number is 5 or less")

count = 0

while (count < 3):
    print(count)
    count += 1
```

### Example Output

```text
Enter your name:
Nafi
Hello Nafi
Number is greater than 5
0
1
2
```

---

# 🏗️ Compilation Pipeline

ChatGaiya++ follows a simplified compiler pipeline:

```text
        ChatGaiya++ Source
              (.cg)
                │
                ▼
        ┌─────────────────┐
        │ Lexical Analysis│
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │ Syntax Parsing  │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │ Static Type     │
        │ Checking        │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │ Python Code     │
        │ Generation      │
        └────────┬────────┘
                 │
                 ▼
          Python 3 Program
               (.py)
```

---

# 🎯 Project Goals

The main goal of ChatGaiya++ is to demonstrate how a programming language and compiler can be designed from scratch using C++.

The project focuses on:

1. Designing a custom programming language
2. Creating a lexer/tokenizer
3. Parsing language syntax
4. Implementing static type checking
5. Handling syntax and semantic errors
6. Translating the custom language into another programming language
7. Generating executable Python 3 code

---

# 📚 Example Keywords

| Keyword           | Purpose                  |
| ----------------- | ------------------------ |
| `ongko`           | Integer                  |
| `dhoshomik`       | Floating point           |
| `kotha`           | String                   |
| `ho`              | Boolean                  |
| `hasa`            | True                     |
| `misa`            | False                    |
| `zodi`            | If                       |
| `noile zodi`      | Else-if                  |
| `noile` / `tokon` | Else                     |
| `zotokkhon`       | While                    |
| `tham`            | Break                    |
| `chol`            | Continue                 |
| `lo()`            | Input                    |
| `ko()`            | Output                   |
| `ki_type()`       | Type inspection          |
| `shuru_kor`       | Include/header directive |

---

# 🚧 Future Improvements

Possible future additions include:

* `for` loops
* Functions and return values
* Arrays/lists
* User-defined data structures
* More advanced type checking
* Better error recovery
* Compiler optimization
* Additional built-in functions
* Module/import system
* Improved Python code generation
* More comprehensive standard library
* Interactive REPL
* VS Code syntax highlighting
* Better compiler diagnostics

---

# 🤝 Contributing

Contributions, suggestions, and improvements are welcome.

If you would like to contribute:

```bash
git clone <repository-url>
cd ChatGaiya++
```

Create a new branch:

```bash
git checkout -b feature/your-feature
```

Make your changes, test them, and submit a pull request.

---

# 👨‍💻 Author

**Md. Fahmidur Rahman Nafi**

ChatGaiya++ was developed as a compiler-design project to explore the fundamentals of programming language implementation and source-to-source compilation.

---

# 📄 License

This project is available for educational and learning purposes.

Add an appropriate open-source license such as **MIT License** if you intend to allow unrestricted reuse and modification.
