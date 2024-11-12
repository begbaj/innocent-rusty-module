# Designated Initializers
Designated initializers allow you to assign values to specific fields of a structure by name, 
rather than initializing fields in order. This is especially useful when working with large structures 
or when only initializing a subset of fields.

## Struct Designated Initializer

```C
struct syscall_hook {
    const char *name;
    void *function;
    void *original;
};

struct syscall_hook hooks[] = {
    {
        .name = "open",          // Initialize the 'name' field
        .function = my_open_hook, // Initialize the 'function' field
        .original = orig_open,   // Initialize the 'original' field
    }
};
```

The dot `.` allows you to explicitly specify which field of the structure you are initializing. 
This makes the code clearer and less prone to errors, especially when structures have many fields 
or when you don’t want to initialize them in order.

Without the dot (designated initializer), you would need to initialize the fields in the order 
they are declared in the structure, like this:

```C
struct syscall_hook hooks[] = {
    {"open", my_open_hook, orig_open}
};
```

### Nested Structure Initialization
For structures within structures (nested structures), you can use designated initializers for 
inner members as well.

```C
struct inner {
    int x;
    int y;
};

struct outer {
    struct inner in;
    int z;
};

struct outer obj = {
    .in = { .x = 5, .y = 10 },   // Nested initializer for struct 'inner'
    .z = 15
};
```

## Array Designated Initliazier
You can use designated initializers for arrays to initialize specific elements by their index.

```C
int arr[5] = { [0] = 10, [3] = 20 };
```

In this example:
- arr[0] is initialized to 10
- arr[3] is initialized to 20
- The remaining elements are set to 0 by default.

## Partial and Mixing initialization
If you only want to initialize specific fields in a structure or an array, designated initializers 
allow you to do that, leaving the rest of the fields with their default values (usually 0 for numbers).

```C
struct example {
    int a;
    int b;
    int c;
};

struct example obj = {
    .a = 10   // Only 'a' is initialized, 'b' and 'c' are set to 0 by default
};
```

You can also mix designated and positional initializers in the same initialization block, although 
it's not always recommended for readability reasons.

```C
struct example {
    int a;
    int b;
    int c;
};

struct example obj = { 10, .b = 20 }; // 'a' is set to 10, 'b' to 20, 'c' is default 0
```

