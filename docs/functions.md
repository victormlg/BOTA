
# 3.1 Functions

## 3.1.1 Declaration

Return statements are marked by not ending the line with `;`

short:
```
let func = x, y -> x + y;
```

long:
```
let func = x, y -> {
    x + y
};
```

## 3.1.2 Call

To call a function, do:
```
func 1 2
```

## 3.1.3 Recursion

Since all variables are uniquely bound in all the downstream scopes, we can use the function variable name in recursion:

```
let recfunc = x -> {
    if x > 10 then x else recfunc x + 1
};
```

## 3.1.4 keyword arguments

We can distinguish between two types of arguments: positional and keyword arguments. All positional arguments can be called as keyword arguments by default. However, when calling a function, keyword arguments must come after positional arguments.

```
let func = a -> a + 2;

func a=3;
```

Keywords arguments allow to give default values to arguments, that can be overridden if needed.

```
let otherfunc = x, y z = 10 -> z + y * z;
otherfunc 1 2; # valid
otherfunc 1 2 3; # valid
otherfunc x=1 y=2 z=3 # valid
```

## 3.1.5 flags

Flags are very important for UNIX shell. This language doesn't have a different syntax for them but we can make use of Enums to support them quite elegantly.

```
enum CommandFlag {
    All,
    Long
}

let unix_command = flag: CommandFlag -> int {
    # check flag
};

unix_command flag = All | Long;
```


# 3.2 High Order Functions

## 3.2.1 High order functions as SQL functions

You can manipulate lists of structs with SQL operations: project (SELECT), filter (WHERE), join (JOIN), but also map and much more

## 3.2.2 Other helpers

We need utils such as 
- `range`: returns a list from x to y with step z

# 3.3 Function composition

## 3.3.1 Pipes

The return value of functions can be fed directly into the first argument of a next functions, without intermediary variables using pipes. You can also pipe values

```
let a = [ 1, 2, 3] |> filter x -> x > 2;
a;
```

```
[1,2]
```

## 3.3.2 Function compositions


Functions can be combined with the same pipe operator `|>`

```
let func1 = x -> x + 1;
let func2 = y -> y * 2;

let func3 = func1 |> func2 # x -> (x + 1)*2
```

If a function has several positional arguments, it can be curried into a function with one positional argument:
```
let twoargs = x,y -> x + y;
let func4 = twoargs 3;

let func5 = func4 |> func2 # x -> (3 + y)*2
```

usecase: combine "background process" with function to make it run in the background. Combine "schedule" with a function to make it run in a regular schedular, like a cron job

