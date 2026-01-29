
# 1.1 Basics

## 1.1.1 Values

- All values are immutable
- Unbound values are automatically printed
- Statements end with `;`
- Parenthesis can wrap any expression, call arguments and function arguments

```
"Hello world";
```

## 1.1.2 Variables

Variables are uniquely bound to values, and this in all downstream scopes

```
let x = 1;
```

## 1.1.3 Comments

```
# This is a comment
```

# 1.2 Operators

- Logical Operators `and, or, not, ==, !=, <=, >=, <, >`
- Math operators: `+, -, /, *, **, %`
- Assignment: `=`
- Type: `:`
- Function def, switch: `->`
- bitwise operators: `|, &, ~, ^`
- pipes, function composition: `|>`

# 1.3 Flow of control

## 1.3.1 if else

if statements that return values are encouraged in functional programming

```
let x = if y > 20 then 0 else 1;
```

multi-line if statements:
`elif` keyword replaces `else if`

```
let x = if y > 20 {
    1 # <-- Notice no ';'
} elif y == 0 {
    2
} else {
    3
};
```

## 1.3.2 switch

### 1.3.2.1 Match

```
let x = match y with 0 -> "good", else -> "else";
```

Multiline:
```
let x = match y with {
    0 -> "good",
    1 | 2 -> "{ ... },
    else -> "bad"
};
```

### 1.3.2.2 Guards
```
let x = match y with y > 20 -> "good", else -> "else";
```

Multiline:
```
let x = match y with {
    y > 20 -> 0,
    y == 0 -> { ... },
    else -> { ... }
};
```

## 1.3.3 Loops

There is no such things as loops. You can use alternatively high order functions (`range`) or tail recursion optimization. 

Example:
```
let factorial = n -> {
    1 if n < 1 else n * factorial n - 1
}
```
# 1.4 Scopes & Entry point

There is no entry point, one file is just one big block (if block, function block) and returns one value. Scripts are just files that return functions. Programs on the other hand are functions in the sdtlib

file.txt
```
let main = x -> {
    # doSomething
} # return
```

Then you can call
```
$ eval(file.txt) x="hello"; # evaluates file.txt to a function
```

Then you can turn the file into an executable, with the name of the function, like in bash `./script`

```
$ script x="hello"
```


# 1.5 Namespaces

Namespaces are marked using `.` notation. Side effects in modules are executed when the module is defined, but not re-executed on import. Return values (expressions without ;) inside modules are ignored - only let, struct, Enum, ... bindings are exported

```
module Math {
    let add = x, y -> x + y;
    let mul = x, y -> x * y;
};

```
Files are implicit namspaces, and are imported using `use`

```
use math.Math; # import Math from file math

Math.add 1 2;
```

