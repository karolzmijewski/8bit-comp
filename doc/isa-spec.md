# Instruction Set Architecture

## CU signals

Control Unit Signals

| Signal | Full Name | Pin Number | Pin Per Chip |
|--------|-----------|------------|--------------|
| PCL**O** | Program Counter Low Out | 1 | 0-9 |
| PCH**O** | Program Counter High Out | 2 | 0-10 |
| PR**I** | Page Register In | 3 | 0-11 |
| MRQ | Memory Request | 4 | 0-13 |
| MRW | Memory Write | 5 | 0-14 |
| MRD | Memory Read | 6 | 0-15 |
| PCE | Program Counter Enable | 7 | 0-16 |
| PCH**I** | Program Counter High In | 8 | 0-17 |
| ALU**O** | Arithmetic Logic Unit Out | 9 | 1-9 |
| ALU**A** | Arithmetic Logic Unit Port A | 10 | 1-10 |
| ALU**B** | Arithmetic Logic Unit Port B | 11 | 1-11 |
| ALU**C** | Arithmetic Logic Unit Port C | 12 | 1-13 |
| HLT | Halt | 13 | 1-14 |
| ACU**I** | Accumulator In | 14 | 1-15 |
| MAR**I** | Memory Address Register In | 15 | 1-16 |
| IR**I** | Instruction Register In | 16 | 1-17 |
| MBR**I** | Memory Buffer Register In | 17 | 2-9 |
| IO0**I** | Input/Output Register 0 In | 18 | 2-10 |
| IO1**I** | Input/Output Register 1 In | 19 | 2-11 |
| IO2**I** | Input/Output Register 2 In | 20 | 2-13 |
| ACU**O** | Accumulator Register Out | 21 | 2-14 |
| MAR**O** | Memory Address Register Out | 22 | 2-15 |
| IR**O** | Instruction Register Out | 23 | 2-16 |
| MBR**O** | Memory Buffer Register Out | 24 | 2-17 |
| IO0**O** | Input/Output Register 0 Out | 25 | 3-9 |
| IO1**O** | Input/Output Register 1 Out | 26 | 3-10 |
| IO2**O** | Input/Output Register 2 Out | 27 | 3-11 |
| PCL**I** | Program Counter Low In | 28 | 3-13 |
| CS**I** | Chip Select In | 29 | 3-14 |
| FLAG | Flag Register In | 30 | 3-15 |


## CPU flags

Central Processing Unit flags:

| Flag | Name       | Description                                                                |
|------|------------|----------------------------------------------------------------------------|
| ZF   | Zero Flag  | Set to 1 if the result of an operation is zero, otherwise set to 0.        |
| SF   | Sign Flag  | Reflects the sign of the result, set to 1 if the result is negative.       |
| CF   | Carry Flag | Set to 1 if an operation produces a carry (overflow from MSB in unsigned). |


## ALU operational codes

Arthmetic Logic Unit operational codes decoding based on Control Unit Signals.

| OPCODE  | ALU**A** | ALU**B** | ALU**C** |
|---------|----------|----------|----------|
| ADD [ + ] | 0 | 0 | 0 |
| SUB [ - ] | 0 | 0 | 1 |
| NEG [ ~ ] | 0 | 1 | 0 |
| RSVD0     | 0 | 1 | 1 |
| OR [ \| ] | 1 | 0 | 0 |
| RSVD1     | 1 | 0 | 1 |
| AND [ & ] | 1 | 1 | 0 |
| RSVD2     | 1 | 1 | 1 |


## Instructions Set

CPU instruction set mapping. Commands are set via the `IR` (Instruction Register).
From there, the following codes are decoded by the `CU` (Control Unit) and
translated into proper [CU signals](#cu-signals).

| Mnemonic | Code    |
|----------|---------|
| NOP      | 0 0 0 0 |
| LOAD     | 0 0 0 1 |
| STORE    | 0 0 1 0 |
| ADD      | 0 0 1 1 |
| SUBT     | 0 1 0 0 |
| INPUT    | 0 1 0 1 |
| OUTPUT   | 0 1 1 0 |
| HALT     | 0 1 1 1 |
| SKIPCOND | 1 0 0 0 |
| JUMP     | 1 0 0 1 |

### Instruction encoding and alignment

- The instruction word size is 32 bits (4 bytes).
- The program counter (PC) is byte-addressed. Each assertion of `PCE` advances the PC by 1 byte.
- Instructions are aligned to 4-byte boundaries. If an instruction uses fewer than 4 bytes, the remaining bytes MUST be filled with `NOP` so that the next real instruction starts at the next 4-byte boundary.

In other words: instruction start addresses satisfy $PC \bmod 4 = 0$.

#### NOP

`NOP` performs no operation (no register/memory side effects; flags unchanged). It exists to pad instruction words to 4 bytes.

#### SKIPCOND

`SKIPCOND` conditionally skips the next 32-bit instruction word.

- If the condition is met, the control unit asserts `PCE` 4 times to advance the PC by 4 bytes.
- If the condition is not met, execution continues with the next byte.

Note: programs must be padded/aligned as described above for “skip 4 bytes” to reliably land at the next instruction.

### STORE

CU signals decoding table for `Store` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 0 1 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 0 1 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 0 1 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 0 1 0     | 0 0 1 1 | PCL**O** | MAR**I** |          |          |
| 4   | 1     | X  | X  | X  | 0 0 1 0     | 0 1 0 0 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 5   | 2     | X  | X  | X  | 0 0 1 0     | 0 1 0 1 | MRQ      | MRD      | MBR**I** | PCE      |
| 6   | 3     | X  | X  | X  | 0 0 1 0     | 0 1 1 0 | PCL**O** | MAR**I** |          |          |
| 7   | 4     | X  | X  | X  | 0 0 1 0     | 0 1 1 1 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 8   | 5     | X  | X  | X  | 0 0 1 0     | 1 0 0 0 | MRQ      | MRD      | PR**I**  | PCE      |
| 9   | 6     | X  | X  | X  | 0 0 1 0     | 1 0 0 1 | MBR**O** | MAR**I** |          |          |
| 10  | 7     | X  | X  | X  | 0 0 1 0     | 1 0 1 0 | ACU**O** | MRQ      | MRW      | PCE      |

### ADD

CU signals decoding table for `Add` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 0 1 1     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 0 1 1     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 0 1 1     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 0 1 1     | 0 0 1 1 | PCL**O** | MAR**I** |          |          |
| 4   | 1     | X  | X  | X  | 0 0 1 1     | 0 1 0 0 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 5   | 2     | X  | X  | X  | 0 0 1 1     | 0 1 0 1 | MRQ      | MRD      | MBR**I** | PCE      |
| 6   | 3     | X  | X  | X  | 0 0 1 1     | 0 1 1 0 | ALU**O** | ACU**I** |          |          |

### LOAD

CU signals decoding table for `Load` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     |  X |  X |  X | 0 0 0 1     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     |  X |  X |  X | 0 0 0 1     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     |  X |  X |  X | 0 0 0 1     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     |  X |  X |  X | 0 0 0 1     | 0 0 1 1 | PCL**O** | MAR**I** |          |          |
| 4   | 1     |  X |  X |  X | 0 0 0 1     | 0 1 0 0 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 5   | 2     |  X |  X |  X | 0 0 0 1     | 0 1 0 1 | MRQ      | MRD      | MBR**I** | PCE      |
| 6   | 3     |  X |  X |  X | 0 0 0 1     | 0 1 1 0 | PCL**O** | MAR**I** |          |          |
| 7   | 4     |  X |  X |  X | 0 0 0 1     | 0 1 1 1 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 8   | 5     |  X |  X |  X | 0 0 0 1     | 1 0 0 0 | MRQ      | MRD      | PR**I**  | PCE      |
| 9   | 6     |  X |  X |  X | 0 0 0 1     | 1 0 0 1 | MBR**O** | MAR**I** |          |          |
| 10  | 7     |  X |  X |  X | 0 0 0 1     | 1 0 1 0 | ACU**I** | MRQ      | MRD      | PCE      |

### OUTPUT

CU signals decoding table for `Output` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 1 1 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 1 1 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 1 1 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 1 1 0     | 0 0 1 1 | ACU**O** | IO0**I** |          |          |

### SUBST

CU signals decoding table for `Subst` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 1 0 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 1 0 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 1 0 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 1 0 0     | 0 0 1 1 | PCL**O** | MAR**I** |          |          |
| 4   | 1     | X  | X  | X  | 0 1 0 0     | 0 1 0 0 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 5   | 2     | X  | X  | X  | 0 1 0 0     | 0 1 0 1 | MRQ      | MRD      | MBR**I** | PCE      |
| 6   | 3     | X  | X  | X  | 0 1 0 0     | 0 1 1 0 | ALU**C** | ALU**O** | ACU**I** |          |

### JUMP

CU signals decoding table for `Jump` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 1 0 0 1     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 1 0 0 1     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 1 0 0 1     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 1 0 0 1     | 0 0 1 1 | PCL**O** | MAR**I** |          |          |
| 4   | 1     | X  | X  | X  | 1 0 0 1     | 0 1 0 0 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 5   | 2     | X  | X  | X  | 1 0 0 1     | 0 1 0 1 | MRQ      | MRD      | MBR**I** | PCE      |
| 6   | 3     | X  | X  | X  | 1 0 0 1     | 0 1 1 0 | PCL**O** | MAR**I** |          |          |
| 7   | 4     | X  | X  | X  | 1 0 0 1     | 0 1 1 1 | PCH**O** | PR**I**  | IR**O**  | CS**I**  |
| 8   | 5     | X  | X  | X  | 1 0 0 1     | 1 0 0 0 | MRQ      | MRD      | MAR**I** |          |
| 9   | 6     | X  | X  | X  | 1 0 0 1     | 1 0 0 1 | MAR**O** | PCH**I** | CS**I**  |          |
| 10  | 7     | X  | X  | X  | 1 0 0 1     | 1 0 1 0 | MBR**O** | PCL**I** |          |          |

### SKIPCOND

CU signals decoding table for `Skipcond` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | 0  | 0  | 0  | 1 0 0 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | 0  | 0  | 0  | 1 0 0 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | 0  | 0  | 0  | 1 0 0 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 0   | F     | 0  | 0  | 1  | 1 0 0 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | 0  | 0  | 1  | 1 0 0 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | 0  | 0  | 1  | 1 0 0 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | 0  | 0  | 1  | 1 0 0 0     | 0 0 1 1 | PCE      |          |          |          |
| 4   | 1     | 0  | 0  | 1  | 1 0 0 0     | 0 1 0 0 | PCE      |          |          |          |
| 5   | 2     | 0  | 0  | 1  | 1 0 0 0     | 0 1 0 1 | PCE      |          |          |          |
| 6   | 3     | 0  | 0  | 1  | 1 0 0 0     | 0 1 1 0 | PCE      |          |          |          |
| 0   | F     | 0  | 1  | 0  | 1 0 0 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | 0  | 1  | 0  | 1 0 0 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | 0  | 1  | 0  | 1 0 0 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | 0  | 1  | 0  | 1 0 0 0     | 0 0 1 1 | PCE      |          |          |          |
| 4   | 1     | 0  | 1  | 0  | 1 0 0 0     | 0 1 0 0 | PCE      |          |          |          |
| 5   | 2     | 0  | 1  | 0  | 1 0 0 0     | 0 1 0 1 | PCE      |          |          |          |
| 6   | 3     | 0  | 1  | 0  | 1 0 0 0     | 0 1 1 0 | PCE      |          |          |          |
| 0   | F     | 1  | 0  | 0  | 1 0 0 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | 1  | 0  | 0  | 1 0 0 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | 1  | 0  | 0  | 1 0 0 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | 1  | 0  | 0  | 1 0 0 0     | 0 0 1 1 | PCE      |          |          |          |
| 4   | 1     | 1  | 0  | 0  | 1 0 0 0     | 0 1 0 0 | PCE      |          |          |          |
| 5   | 2     | 1  | 0  | 0  | 1 0 0 0     | 0 1 0 1 | PCE      |          |          |          |
| 6   | 3     | 1  | 0  | 0  | 1 0 0 0     | 0 1 1 0 | PCE      |          |          |          |

### HALT

CU signals decoding table for `Halt` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 1 1 1     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 1 1 1     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 1 1 1     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 1 1 1     | 0 0 1 1 | HLT      |          |          |          |

### INPUT

CU signals decoding table for `Input` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 1 0 1     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 1 0 1     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 1 0 1     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 1 0 1     | 0 0 1 1 | ACU**I** | IO0**O** |          |          |
