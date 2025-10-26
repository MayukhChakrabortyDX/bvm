# Bliss Programming Language (BPL) – Version 0.1

Bliss is a low-level programming language for a custom virtual machine. It allows **memory manipulation, arithmetic, control flow, and system calls**, similar to assembly.

---

## **1. Program Structure**

A Bliss program has **two main sections**:

1. **Data Section** (`::data`) – Define variables and arrays.
2. **Code Section** (`::code`) – Instructions executed by the VM.

### **Example Program**

```bpl
::data
str char[] "Hello World"
a int 10
b int 20

::code
load R1 a
load R2 b
iADD         ; R1 + R2 -> ROUT
load RSYS str
syscall "printf"
end
```

---

## **2. Data Section**

Defines variables as:

```
<name> <type> <value>
```

* `<name>` – Alphabetic identifier
* `<type>` – `int`, `char`, `byte`, `short`, `long`, `float`, `double` (arrays use `[]`)
* `<value>` – Actual value (`10`, `'A'`, `['A','B']`)

**Example:**

```bpl
a int 10
arr int[] [1,2,3,4]
msg char[] "Hi"
```

---

## **3. Code Section**

### **3.1 Arithmetic Instructions**

Format: `xADD`, `xSUB`, `xMUL`, `xDIV`
`x` = `b` (byte), `s` (short), `i` (int), `l` (long), `f` (float), `d` (double)

* `xADD` → `ROUT = R1 + R2`
* `xSUB` → `ROUT = R1 - R2`
* `xMUL` → `ROUT = R1 * R2`
* `xDIV` → `ROUT = R1 / R2` (sets `RERR = DIV_BY_ZERO` if dividing by 0)

---

### **3.2 Memory Instructions**

| Instruction           | Description                                                                           | Notes                                        |
| --------------------- | ------------------------------------------------------------------------------------- | -------------------------------------------- |
| `LOAD <reg> <data>`   | Load a variable from the data section into a register                                 | `reg = R1, R2, ROUT, GR1-GR6, RSYS`          |
| `MEMSTORE <size>`     | Store the value from `R1` to heap address in `R2`; register address is stored in `R1` | `size` = 1–8 bytes                           |
| `REGLOAD <size>`      | Load value from heap address in `R2` into register `R1`                               | `size` = 1–8 bytes                           |
| `ALLOC`               | Allocate memory on heap; size stored in `R1`                                          | Returns heap address in `R1`                 |
| `FREE`                | Free memory at address in `R1`                                                        |                                              |
| `RESIZE <size>`       | Resize value in `R1` to 8,16,32 bits                                                  | Size expansion has no effect beyond max size |
| `CONVERT <from> <to>` | Convert types (int↔float, long↔double)                                                | Only valid conversions allowed               |

**Memory Flow Notes:**

* For `MEMSTORE`:

  * `R1` → register holding value
  * `R2` → heap address
  * `size` → 1–8 bytes
* For `REGLOAD`:

  * `R2` → heap address
  * `R1` → register to store value
  * `size` → 1–8 bytes

---

### **3.3 Control Flow**

* `JUMP <label>` – Unconditional jump
* `CALL <label>` – Call function
* `RETURN` – Return from function
* Conditional instructions (`xEQ`, `xNEQ`, `xLT`, `xLTEQ`) – Execute next instruction based on `R1` vs `R2`

**Labels:**

* `label:` → For jumps
* `label::` → For function calls

---

### **3.4 Registers**

| Register           | Purpose                                |
| ------------------ | -------------------------------------- |
| `R1`, `R2`, `ROUT` | General-purpose                        |
| `GR1`–`GR6`        | Temporary storage                      |
| `RERR`             | Stores errors like `DIV_BY_ZERO`       |
| `RFX`              | Stores the function heap address |
| `RSYS`             | Holds system call arguments            |

**Operations:**

* `MOV <from> <to>` – Copy between registers
* `CLEAR <reg>` – Reset register to zero

---

### **3.5 System Calls**

* `SYSCALL <lib>` – Calls a runtime library (e.g., `"printf"`) using `RSYS` as argument.

---

### **3.6 Program End**

* `END` – Marks the end of execution.

---