# teatoo

## how to use

I'll explain starting with the hello world program:
```
hello_world:{
    IF (EMPTY?) (
        PUT [01100100] -- d
        PUT [01101100] -- l
        PUT [01110010] -- r
        PUT [01101111] -- o
        PUT [01110111] -- w
        PUT [00100000] -- \s
        PUT [01101000] -- h
        PUT [01100101] -- e
        PUT [01101100] -- l
        PUT [01101100] -- l
        PUT [01101111] -- o
        RETURN [1]
    )
    RETURN (TAKE)
}


EXEC $hello_world;
```

the building blocks of the langauge are scopes and operations.
you use these to create everything.
to define a scope, write and identifier and followed by ':' and then the body of the scope surrounded by '{}'.
a scope always has internal memory: a stack of bytes. this is classic stack memory, first in - first out.
inside the brackets you can list operations that should be executed when calling the 'EXEC' operation on scope.
in this example we define a scope called 'hello_world'. we then use the 'IF' operation with an 'EMPTY?' operation as condition.
'EMPTY?' will check if the stack of the scope its called in contains any values and returns something like "true" or "false".
the stuff in '()' is called a sequence. this is the second argument for the 'IF'. it will only be executed if the stuff in the first sequence (the condition)
returns "true" (i.e. 255).
then there is a lot of 'PUT' operations which simply put the given byte on the stack (thats why its reversed).
inside the sequence there is a 'RETURN' operation. this will cause the execution of the scope to stop here and yield the result given next.
in this case its 255 in binary, but the shortcut version of this.
after the scope definition we see the 'EXEC' opeation with a $-reference on the scope.
a $-reference instructs the interpreter to make a copy of the stack and then executes it.
if you dont use a copy the original stack is permanently modified.

since nothing else is done, the program ends without andy output.

if we want to see the output, we can do it like that:
```
hello_world:{

    IF (EMPTY?) (
        PUT [01100100] -- d
        PUT [01101100] -- l
        PUT [01110010] -- r
        PUT [01101111] -- o
        PUT [01110111] -- w
        PUT [00100000] -- \s
        PUT [01101000] -- h
        PUT [01100101] -- e
        PUT [01101100] -- l
        PUT [01101100] -- l
        PUT [01101111] -- o
        RETURN [1]
    )
    RETURN (TAKE)
}

run_hello_world:{
    EXEC hello_world
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
    OUTCHAR (EXEC hello_world)
}

EXEC $run_hello_world;
```
this defines a second scope that executes the first scope several times (does not copy).
the first execution will just fill the scopes stack with the values "hello world".
all consecutive exec's will trigger 'RETURN (TAKE)', which takes the top value from the stack, removes it and returns it.
then this is passed to 'OUTCHAR' which prints the value interpreted as a character.


### module

the whole file is a module.
on the 'module-level' you can only define scopes and call 'EXEC' ONCE!

### scopes

a scope is defined by
```
name:{...}
```
it has an internal stack memory (first in - first out).

### scope references

a scope can be referenced in two ways: a copy reference and a reference.
the copy ref is done by writing ```$name```. this will cause the scope to be copied and all operations done when executing will not modify the original scope.
the regular ref is done by just writing the name  ```name```. all operations done when executing it will permanently modify the scopes stack. all future calls to the scope will
have a scope with possibly preset stack memory.

### bytes

a byte is written like that: ```[00000000]```.
its a binary representation of number basically. from right to left we have the powers of 2^0, 2^1, 2^2,... up to 2^7.
this means for example this ```[00000001]``` is 1, ```[00010011]``` is 19 etc.
you can write shorthands if you want to have 0 or 255: ```[1]``` is 255 (it basically sets all to 1) and ```[0]``` is 0, it sets all to 0.

### 'boolean'

IF operations require 'boolean' values. since they do not exist in this language (only bytes) we help ourselfs with the 0 and 255 values.
IF will execute its second argument if the first is 255, and does not in any other case.
Some operations are used to produce these boolean values, like e.g. EMPTY?, NULL?, POW and so on.
you can therefor use these safely as first argument for an IF

### operations

all operations and what they do are listed here:

#### IF
```IF <condition> (<operations>)```
IF takes a sequence or a byte.
if given a sequence it executes it and expects the result to be a byte (otherwise error).
when it has the byte (either direclty or by executing the sequence) it checks if it is 255 i.e. ```[1]``` or ```[11111111]```,
and if so it executes the second argument, returning its result.

#### EQ
EQ takes two arguments that must be a byte or result in a byte. if they are equal, it returns 255 and 0 otherwise.

#### NEQ
yeah.

#### | aka OR and the other bitwise opeations
| is a bitwise or operation on the arguments which, again, must be bytes or result in bytes.
its called, e.g., like that ```| [1] [0]``` (like in lisp, operation is always first).
similar for AND(&) and XOR and NOT.
only OR (|), AND (&) and NOT (!) have the one-character symbols. XOR must be written out.

#### TAKE
TAKE takes the top value from the stack of the scope its executed in and returns it. it resuts a special 'NULL RESULT' if the stack is empty.

#### PUT
PUT puts the byte argument given on top of the stack and returns 'NULL'.

#### PEEK
similar to TAKE, it gets the first value from the stack, but does NOT remove it.

#### RETURN
returns the given arugment. This CAN return 'NULL' without throwing an error. there is simply no way the create a 'NULL' by youself. like writing it down or somehting.

#### EXEC
EXEC expects a scope ref argument and executes it, returning its result.

#### STACK
STACK expects two arguments, the scope on which to operate first (copy or ref), and a byte value.
it puts the value given on the stack of the given scope and then returns the scope itself, i.e. a reference to the scope that is now modified (globally), or the copy of the scope.

#### OUT
prints the byte argument given as a binary representation.
```OUT [1]``` prints "11111111"

#### OUTCHAR
prints the byte argument given as a decimal number.

#### OUTCHAR
prints the byte argument given as a decimal number.

#### POW
POW checks it the given byte argument is 'powered'. it checks if it is ```[0]``` and returns 255 if so, otherwise it returns 0.

#### EMPTY?
takes no argument, checks if the given stack is empty (has no values) returns 255 if so, 0 otherwise

#### NULL?
takes one byte argument and checks if it is the special 'NULL' result. returns 255 if so, 0 otherwise


## Future plans
* 'REVERSE' - reverses the stack of the scope its executed in
* IO - some form of IO. either read file or read input or both. also the option to write to file.
* more than one module allowed 'module references' or otherwise maybe known as 'import' or something.
