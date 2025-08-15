# But How Do It Know? - The Basic Principles of Computers for Everyone

## Logic Gates

Logic gates are electrical components that make up a computer, they exist in the silicon chips and other components.

The simplest of the gates is the NAND gate. It has 2 inputs and one output. The output is always on, unless both inputs are on, then it is off.

![NAND Gate](https://en.wikipedia.org/wiki/File:NAND_ANSI_Labelled.svg)

The AND gate is a combination of a NAND gate and a NOT gate. I is always off, unless both inputs are on, then it is on.

![AND Gate](https://en.wikipedia.org/wiki/File:AND_ANSI.svg)

## Bit

Bits are built from NAND gates. There are 2 inputs and one output. The first input is the state we want to capture. The second input is the selector.

When the selector is switched on, the state of the first input is captured. When the second input is switched off the state of the first input is stored. The output will stay the stored value until the second input is switched on and the first input is in a different state.

![Bit](https://i.sstatic.net/7dbEM.png)

## Bits, Nibbles, Bytes

A byte is a collection of 8 ordered bits, a nibble is 4.

Bytes can be used to represent data such as [ASCII](https://en.wikipedia.org/wiki/ASCII) characters.
