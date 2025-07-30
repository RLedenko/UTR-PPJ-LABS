In these laboratory asignments we were tasked with making a compiler for the language "PJ".

#### Example program:

```plaintext
n = 10 //Comment
    rez = 0
za i od 1 do n
    rez = rez + i*i*i
az
```

#### The syntax has 4 keywords:
za - loop varijable declaration \
az - loop end \
od - initial loop variable value \
do - loop variable max value 

The loop functions like the do-while loop in C, the comment syntax is also C-like \
Variables don't need to be declared before use. \
Loop variables, that share their name with a variable elsewhere, are used in their loop only, not even in nested loops. \
\
The compiler (Lab 4) compiles the "PJ" language to FRISC assembly code.\
FRISC emulator: https://balrog.zemris.fer.hr/friscjs 
