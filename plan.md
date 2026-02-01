
# Drop AST 

Too much boilerplate code, hard to maintain, never satisfied with implementation

# New pipeline:

1. lexe to token
2. parse to bytecode. Save (lineno, offset) somehow.
3. semantic analysis on bytecode.
4. execute bytecode


# Calling convention (cdecl)

Caller does:
- Push all arguments to the stack, from right to left 
- Call function
- Cleanup call args

Meanwhile, called function does:
- push ebp
- mov ebp, esp
- // do something. Can access pushed args with offset from ebp
- write return value to EAX
- mov esp, ebp
- pop ebp


# Pipe example
```
ls ALL | filter x -> x.size > 30; 
```

```
main:
    mv $ALL %eax
    call ls // eax -> eax
    push $lambda
    call filter // eax, lambda -> eax
    call print // eax

lambda:
    push ebp
    mov ebp, esp

    push 30
    call gt // eax, 30 -> eax

    mov esp, ebp
    pop ebp
```


# Design

- Eveything is a function call, getting their values from the stack. Even '+', '-', ...
- Return values are saved inside EAX
- The first argument of a function is always passed from EAX
- Most datatypes are just pointers: strings, List, Objects, functions. A function is a pointer to a function definition
