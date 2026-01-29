
# 2.1 Types
The language is statically typed: types cannot be implicitely casted. The language enforces type safety. Although, the types never need to be explicitely written in the syntax. You can write the type explicitely with the following syntax:

```
let x: int = 1;
```

# 2.2 Primitives

The primitives types are: int, string, float, bytes, bool, functions

Functions are first class citizens

```
let x: string = "Hello world";
```

# 2.3 OS types

The language also comes with OS types: path, timestamp, duration, schedule. A timestamp is a fixed point in time. It follows ISO 8601 (YYYY-MM-DDTHH:MM:SS.sssZ or +HH:MM). A duration is some elapsed time duration (seconds, minutes, hours, ...). Finally, a schedule is some recurring time, ex: `weekly`

```
let myfile: path = hello.txt;
```

# 2.4 Data structures

The default data structures are: Object<A, B, ...> and List<T>. An Object is a named tuple (struct) with hashmap capabilities.

## 2.4.1 Objects

Object declaration:

```
let o = {
    name = "jack",
    age = 33
};
```

Objects don't enforce their type when declared, only when passed around in functions for example. This allows for nested objects:

```
let x = {
    name = "jack",
    pet = {
        name = "dogo",
        age = 1
    }
};
```

## 2.4.2 Lists

List declaration:
```
let l = [1, 2, 3];
```

# 2.5 Custom Types

You can create custom types, which are essentially only labels over object:

```
struct Person {
    name: string,
    age: int
};
```

```
let o: Person = { name = "jack", age = 33 };
```

This makes it easier to enforce type constraints on objects for functions or lists

# 2.6 Combining lists and objects

Combining lists and objects allow to create powerful datastructures such as Databse records:

```
let db: List<Person> = [
    { name = "Kai", age = 40 },
    { name = "Kjetil", age = 56 }
];
```

These allow for powerful operators on them

# 2.7 Enums

The language also have enums:

```
enum Weekday {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};
```

You can combine them with bitwise operations

```
let weekend: Weekday = Saturday | Sunday;
```
This means that weekend will both match Saturday and/or Sunday.

Finally, enums resolved to strings when printed:
```
Saturday;
```

```
Weekday<Saturday>
```

# 2.8 Monads

The language has two monads that allow for powerful type safety. Their values have to be extracted with a match statement to be used.

## 2.8.1 Option<T>

Options is either Some(x) or None

## 2.8.2 Result<T,E>

Result is either Ok(x) or Error

```
let y: Result<int> = somefunction;
let x: int = match y with {
    Ok _ -> 1,
    Err _ -> 0
};
```

## 2.8.3 Async<T>

Functions run as background processes return Async<T> values. These needs to be extracted with "await" statements, but they make the execution of the program block.

The await function has the following definition:
```
let await = (input: Async<T>, condition: Option<function>,
                timeout: Option<duration>, default: Option<T>) -> T {...}
```
