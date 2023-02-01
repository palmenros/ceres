# Ceres

## Lexical structure

### Source encoding

All source code must be encoded in ASCII.

### Keywords

#### General

| Keyword    | Meaning                             |
|------------|-------------------------------------|
| fn         | Beginning of function declaration   |
| var        | Beginning of variable declaration   |
| const      | Beginning of variable declaration   |
| struct     | Beginning of struct declaration     |
| enum       | Beginning of enum declaration       |
| constraint | Beginning of constraint declaration |
| pub | Export as public |
| use | Import object to namespace |
| mod | Beginning of module (namespace) declaration |

#### Functions

| Keyword | Meaning                           |
|---------|-----------------------------------|
| return  | Return from function with value   |

#### Control flow

| Keyword | Meaning      |
|---------|--------------|
| goto    | Control flow |
| break   | Control flow |
| if      | Control flow |
| else    | Control flow |
| for     | Control flow |
| while   | Control flow |
| switch  | Control flow |
| default | Control flow |
| continue |  Control flow |

### Identifiers

Identifiers must start with a letter or `_`, and can contain letters, `_` and numbers.

Identifiers and have no limit on their length.

### Comments

Inline comments start with `//`, and block comments begin with `/*` and end with `*/`, comments can be nested.

### Whitespace

Whitespace is defined as per the ASCII specification, whitespace in source code is ignored, except for token separation.

### Literals

#### Characters and strings

| | Explanation | Example |
|-|-|-|
| Character | Characters are enclosed in `'_'`| `'H'` |
| String | Strings are enclosed in `"_"`| `"H"` |

#### ASCII escapes

| | Name |
|-|-|
| `\n` | Newline |
| `\r` | Carriage return |
| `\t` | Tab |
| `\\` | Backslash |
| `\0` | Null |

#### Quote escapes

| | Name |
|-|-|
| `\'` | Single quote |
| `\"` | Double quote |

#### Number literals

| | Explanation | Example |
|-|-|-|
| Decimal integer | Integer constant separated by `_`| `999_324_214` |
| Hex integer | Hex integer beginning with `0x` | `0xAF567` |
| Octal integer | Octal integer beginning with `0o` | `0o567` |
| Binary integer | Binary integer beginning with `0b` | `0b1111_0101` |
| Floating point | Floating point contains `.` | `342.2` |

### Operators

|Operator  | Name |
|-|-|
| `+` | Plus |
| `-` | Minus |
| `*` | Star |
| `/` | Slash |
|`%`| Percent |
|`^`| Caret |
|`!`| Not |
|`&`| And |
|`|`| Or |
|`&&`| Andand |
|`||`| Oror |
| `^^`| Caretcaret |
|`>>`| Shl |
|`<<`| Shr |
|`=`| Eq |
|`==`| Eqeq |
|`!=`| Ne |
| `>` | Gt |
| `<`| Lt |
|`>=`| Ge |
|`<=`| Le |
|`->`| Rarrow |
|  `.`| Dot |
|`,`| Comma |
|`;`| Semi |
|`:`| Colon |
|`::`| Coloncolon |

### Delimiters

| Delimiter | Name |
|-|-|
| `{}` | Braces |
| `[]` | Brackets |
|`()`| Parens |
