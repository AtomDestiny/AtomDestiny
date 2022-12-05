# Atom destiny C++ coding style guide

Glance coding style guide description.<br />
For contribution you should use this codig rules.<br />
This style guide specially differs from Unreal Engine core coding standards.

### Base concepts

- C++20 standard
- 4 spaces, do not use Tabs
---

### Brackets and spaces

- put brackets on a different lines

>```cpp
>
>void BracketsExample()
>{
>     Foo();
>}

- 1 space after if/else switch instructions
- else operator use on the next line after closing bracket
- if/else operator with one code line must be used without brackets

>```cpp
>if (transactions.empty())
>     Foo();
>else
>{
>     Foo();
>     Bar();
>}

### Types

- reference and pointer symbols add to right part of type
>```cpp
>int i = 0;
>int& ref = i;
>int* pointer = &ref;

- variable instantiation must starts on new line
>```cpp
>int first = 0;
>int second = 0;
>int* third = &second;

### Naming convention

- local variables starts with lower case

>```cpp
>auto exampleValue = 100;

- class/struct names start with upper case and has an PascalCase style

>```cpp
>class ExampleClass
>{
>};

- private fields starts with prefix m_ and then should have camelCase style

>```cpp
>struct A
>{
>private:
>     int m_exampleField;
>};

- public fields should have camelCase style

>```cpp
>struct A
>{
>     int demoField;
>};

- methods and functions should have PascalCase style

>```cpp
>void RunTransport()
>{
>     object->Call();
>}

- namespace names should have PascalCase style

>```cpp
>namespace AtomDestiny::Utils
>{
>    class A
>    {
>    };
>}

- constants should have  PascalCase or should be used as enum

>```cpp
>constexpr double TransactionFee = 0.001;
>
>class Transport
>{
>public:
>     enum Options
>     {
>         Capacity = 100
>     };
>};

- global variable starts with 'g' symbol

>```cpp
>static int gPlatformStatus = 0;
