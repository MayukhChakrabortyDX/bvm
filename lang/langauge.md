# BPL Language Documentation

## 1. Overview

BPL is a **high-performance, memory-safe programming language** designed for **non-GC virtual machines**.
It is **Turing complete**, supports **generics/templates**, **polymorphism through APIs**, and **strong memory safety** via **ownership, loans, grants, and cloning**.

**Design Goals:**

* Zero-cost abstractions
* Predictable memory management
* Safe operations on arrays, strings, and objects
* Ergonomic syntax for container operations

---

## 2. Primitives

| Type                                      | Description      | Notes                                                |
| ----------------------------------------- | ---------------- | ---------------------------------------------------- |
| `int`, `float`, `double`, `short`, `byte` | Numeric types    | Copyable, no ownership rules                         |
| `bool`                                    | Boolean type     | `true` / `false`                                     |
| `char`                                    | Character type   | Used in strings (`char[]`)                           |
| `Result<T>`                               | Operation result | Must be handled with `handle { Ok(x) {} Err(e) {} }` |
| `Null<T>`                                 | Optional value   | Can be `null` or `T`; check with `if x != null`      |

---

## 3. Variables and Assignment

```bpl
var x: int = 10;        // type inference allowed
x = 20;                  // standard assignment

arr <- value             // safe insertion into container, returns Result<T>
arr[i] <- value          // safe assignment to index

arr <- 1 <- 2 <- 3       // chainable insertions
```

* `<-` is **distinct from `=`**; always returns a `Result<T>` for safe operations.
* Safe container assignment automatically checks bounds.

---

## 4. Containers

| Type        | Syntax              | Allocation | Iterable | Notes                                       |
| ----------- | ------------------- | ---------- | -------- | ------------------------------------------- |
| Heap array  | `Array<T>`          | Heap       | Yes      | Supports `<-`, iteration, and chaining      |
| Stack array | `T[]`               | Stack      | Yes      | Fixed size; safe element access via `[]`    |
| String      | `string` → `char[]` | Stack      | Yes      | Literal compiles to `char[]`; supports `<-` |

**Container Operations:**

```bpl
var arr: Array<int> = [];
arr <- 10;
arr <- [20, 30, 40];
var val: Result<int> = arr[2];  // safe access
```

**Iteration:**

```bpl
iterate arr as x {
    print(x);
}

iterate arr as x, i {
    print(i, x);
}
```

---

## 5. Functions

```bpl
fx fname(args: types) -> returnType {
    // function body
}
```

**Key Features:**

* Return type can often be inferred
* Supports **generics/templates**:

```bpl
fx add<T, K>(a: T, b: T) -> K { ... }
```

* Supports **API constraints**:

```bpl
fx func<T with API, K with API>(...) { ... }
```

* Supports **anonymous types**:

```bpl
Fx<T, K>(T, K): returnType
```

* **Recursion is allowed**, enabling Turing completeness

---

## 6. Objects and APIs

### Objects

```bpl
object Person {
    var name: string;
    var age: int;
}
```

* Objects are equivalent to **structs** in C/Rust.

### APIs

```bpl
api Printable {
    fx print(): void;
}
```

* APIs can be applied to **any type**, including primitives.

```bpl
apply Printable on string {
    fx print() { ... }
}
```

* Methods added via APIs are **statically bound** → zero runtime overhead.

---

## 7. Control Flow

```bpl
if condition { ... } elif condition { ... } else { ... }

while condition { ... }

return value;
```

* Standard operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
* Boolean operators: `&&`, `||`, `!`
* Loops and iterators support `break` and `continue`

---

## 8. Operators

| Operator   | Description                                              |   |        |
| ---------- | -------------------------------------------------------- | - | ------ |
| `=`        | Assignment                                               |   |        |
| `<-`       | Safe container insertion/assignment, returns `Result<T>` |   |        |
| `[]`       | Element access, returns `Result<T>` for safety           |   |        |
| Arithmetic | `+`, `-`, `*`, `/`                                       |   |        |
| Boolean    | `&&`, `                                                  |   | `, `!` |
| Equality   | `==`, `!=`                                               |   |        |

---

## 9. Error and Null Handling

```bpl
var x: Result<int> = arr[0];
handle x {
    Ok(v) { print(v); }
    Err(e) { print("Error:", e); }
}

var y: Null<int> = arr[0];
if y != null {
    print(y);
}
```

* `Result<T>` must be handled with `handle`.
* `Null<T>` can be safely checked for nullability.

---

## 10. Memory Safety

### Ownership

* Every value has a **single owner**.
* **Primitive#1 types** (int, float, bool, char) → copyable
* **Primitive#2 types** (Array<T>, Object, Result<T>, Null<T>, string) → move-only unless cloned

### Loans and Grants

```bpl
loan a: x { ... }   // a is &T, immutable reference
grant a: x { ... }  // a is &mut T, mutable reference
var b = clone a;    // explicit copy
```

* **Loans (`&T`)**: immutable, multiple allowed simultaneously
* **Grants (`&mut T`)**: mutable, only one at a time; no overlapping loans
* **Clone**: creates a copy without transferring ownership

### Return Semantics

* Ownership moves on return:

```bpl
fx join_str(loan a: string, loan b: string) -> string {
    var newstr: string = "";
    newstr <- a <- b;
    return newstr;  // ownership moves to caller
}

var result = join_str(loan "Hello ", loan " World");
```

* Loans/grants can be returned; **compiler assumes references live long enough** (no explicit lifetimes required).

---

## 11. Iterators

```bpl
iterate arr as x { ... }        // iterate over values
iterate arr as x, i { ... }     // iterate over values + index
```

* Iterators respect **ownership and memory safety**:

  * Reading via `loan` references
  * Modifying via `grant` references
* Supports `break` and `continue`.

---

## 12. Summary

**Turing Complete:** loops, recursion, conditionals, mutable storage
**Memory Safe:** ownership, loans, grants, clone; no GC needed
**Containers:** heap arrays, stack arrays, strings; `<-` operator for safe insertions
**Functions:** generic, templated, ownership-aware, can return loans/grants
**Objects & APIs:** polymorphism with zero-cost static binding
**Error & Null Handling:** `Result<T>` + `handle`, `Null<T>`
**Iteration & Control Flow:** ergonomic, zero-cost abstractions

---

## 13. Example – Full Program

```bpl
import std::string;

fx join_str(loan a: string, loan b: string) -> string {
    var newstr: string = "";
    newstr <- a <- b;
    return newstr;
}

fx main() {
    var result = join_str(loan "Hello ", loan "World");
    print(result);   // prints: Hello World

    var numbers: Array<int> = [];
    numbers <- [1, 2, 3, 4, 5];

    iterate numbers as x, i {
        print(i, x);
    }

    handle numbers[10] {
        Ok(v) { print(v); }
        Err(e) { print("Out of bounds"); }
    }
}
```

```

---

I can now **generate a downloadable Markdown file and a PDF** version for you.  

Do you want me to do **both**?
```