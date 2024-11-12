# Macro:
A macro in C is a preprocessor directive that allows you to define code that can be reused 
throughout your program. Macros are processed by the C preprocessor before the actual compilation 
takes place. They allow you to define constants, functions, or blocks of code that are 
substituted into the program where the macro is used.

## types:
- **Object-like Macro**: similar to `const`, you use a macro to give **a name** to a **value/expression**.
- **Function-like Macro**: more like **inline functions**. You can define a macro that takes arguments, 
and when the macro is used, the arguments are substituted into the macro definition.

### Object-like example
```C
#define PI 3.14159
float circumference = 2 * PI * radius;
```

### Function-like example
```C
#define SQUARE(x) ((x) * (x))
int result = SQUARE(2+3);  // Becomes: int result = (2+3) * (2+3);
```

## notes:
- **No type checking**: Macros are simply text replacements, so there's no type checking like in functions. 
This can lead to unexpected behavior if you're not careful, especially with function-like macros.

```C
#define SQUARE(x) x * x
int result = SQUARE(2 + 3);  // This expands to 2 + (3 * 2) + 3, while you wanted (2+3) * (2+3)
```
- **Preprocessor Replacement**:
Macros are expanded by the preprocessor before the code is compiled. This means that macros are not functions, 
and they don't have a presence in the compiled code. The preprocessor simply replaces the macro name with 
its definition wherever it is used in the source code.

- **Conditional Macros**: You can use macros conditionally to include or exclude blocks of code, 
often for debugging or cross-platform development.

- **Macro Hygiene**: Since macros are just textual replacements, careless usage can result in bugs. 
For example, if a macro's body isn't properly parenthesized, or if variables inside the macro conflict 
with variables in the surrounding code.
