# Instruction Set Architecture

## CU signals

Control Unit Signals

| Signal | Full Name | Pin Number |
|--------|-----------|------------|
| PCL**O** | Program Counter Low Out | 1 |
| PCH**O** | Program Counter High Out | 2 |
| IR**I** | Instruction Register In | 3 |
| MRQ | Memory Request | 4 |
| MRW | Memory Write | 5 |
| MRD | Memory Read | 6 |
| PCE | Program Counter Enable | 7 |
| PCH**I** | Program Counter High In | 8 |
| ALU**O** | Arithmetic Logic Unit Out | 9 |
| ALU**A** | Arithmetic Logic Unit Port A | 10 |
| ALU**B** | Arithmetic Logic Unit Port B | 11 |
| ALU**C** | Arithmetic Logic Unit Port C | 12 |
| HLT | Halt | 13 |
| ACU**I** | Accumulator In | 14 |
| MAR**I** | Memory Address Register In | 15 |
| IR**I** | Instruction Register In | 16 |
| MBR**I** | Memory Buffer Register In | 17 |
| IO0**I** | Input/Output Register 0 In | 18 |
| IO1**I** | Input/Output Register 1 In | 19 |
| IO2**I** | Input/Output Register 2 In | 20 |
| ACU**O** | Accumulator Register Out | 21 |
| MAR**O** | Memory Address Register Out | 22 |
| IR**O** | Instruction Register Out | 23 |
| MBR**O** | Memory Buffer Register Out | 24 |
| IO0**O** | Input/Output Register 0 Out | 25 |
| IO1**O** | Input/Output Register 1 Out | 26 |
| IO2**O** | Input/Output Register 2 Out | 27 |
| PCL**I** | Program Counter Low In | 28 |
| CS**I** | Chip Select In | 29 |


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
| LOAD     | 0 0 0 0 |
| STORE    | 0 0 1 0 |
| ADD      | 0 0 1 1 |
| SUBT     | 0 1 0 0 |
| INPUT    | 0 1 0 1 |
| OUTPUT   | 0 1 1 0 |
| HALT     | 0 1 1 1 |
| SKIPCOND | 1 0 0 0 |
| JUMP     | 1 0 0 1 |

### STORE

CU signals mapping for `Store` instruction.

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

CU signals mapping for `Add` instruction.

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

CU signals mapping for `Load` instruction.

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

CU signals mapping for `Output` instruction.

| No. | Notes | SF | CF | ZF | Instruction | CU step | CU sigs. |          |          |          |
|-----|-------|----|----|----|-------------|---------|----------|----------|----------|----------|
| 0   | F     | X  | X  | X  | 0 1 1 0     | 0 0 0 0 | PCL**O** | MAR**I** |          |          |
| 1   | F     | X  | X  | X  | 0 1 1 0     | 0 0 0 1 | PCH**O** | PR**I**  |          |          |
| 2   | F     | X  | X  | X  | 0 1 1 0     | 0 0 1 0 | MRQ      | MRD      | IR**I**  | PCE      |
| 3   | 0     | X  | X  | X  | 0 1 1 0     | 0 0 1 1 | ACU**O** | IO0**I** |          |          |

### SUBST

CU signals mapping for `Subst` instruction.

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

CU signals mapping for `Jump` instruction.

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
