# Ideas on what i want to do

## comments
-- single line comment
-[ multiline comment ]-


## scope
define a scope
a:{}

-- copy a to b
b:$a

## values

NULL -- nothing, only exists if a scope does not return
[0] -- equivalent to [00000000]
[1] -- equivalent to [11111111]
[01010101] -- define a value like that

]-
## grouping
grouping prevents immediate execution

()


## operations
IF (<arg>) (<arg>) -- checks first block, if [1] executes the second
EQ (<arg>) (<arg>) -- check equality of two values, returns [1] if so, [0] if not
NEQ (<arg>) (<arg>) -- opposite of EQ
TAKE -- takes value from current scope's stack
PUT -- puts value on current scope's stack
PEEK -- gets value but does not take from stack
POW (<arg>) -- checks if a value is [0] returns [1] if so, [0] if not
EMPTY? -- checks if the scopes stack is empty returns [1] if so, [0] if not
MAP -- applies a operation to every element on the stack
REVERSE -- reverses the stack on the scope
NULL? <arg> -- checks if the given thing is NULL

! <arg> -- bitwise not on a a value
& <arg> <arg> -- bitwise and on values
| <arg> <arg> -- bitwise or on values
XOR <arg> <arg> -- bitwise xor on a

RETURN <arg> -- exits scope execution, returns the value
EXEC <arg> -- executes the scope

OUTNUM <arg> -- prints number equivalent of a byte
OUTCHAR <arg> -- prints char equivalent of a byte
OUT <arg> -- just prints the byte e.g. OUT [0] prints 00000000

## what it shold look like:

a:{
    IF (EQ([0], TAKE)) ()
}
