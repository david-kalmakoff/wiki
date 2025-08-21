# RISC-V Assembly Language

[Book - Amazon](https://www.amazon.ca/RISC-V-Assembly-Language-Anthony-Reis/dp/1088462006)

## Motivation

I bought this book in 2019 and it has sat on the shelf since then. After reading the 'But How Do It Know?' book I was motivated to crack this one open and dive deeper into assembly language.

I see RISC-V as having an exciting future, being an open alternative to the current architectures.

## Chapter 1: Numbering Systems

To represent a negative number in a two's complement system, flip its bits and add 1.

```
0011 (3)

  1100 (flipped)
+ 0001 (1)
------
 1111
  1101 (-3)
+ 0011 (3)
------
  0000 (0)
```
