# stz

Standard library based on zero initialization and arenas, inspired by [null program](https://nullprogram.com/)

Initial attempt: With inheritence -> prob not worth it

Definitions:

1. types
   Like golang, `i32`, `isize`, etc.
2. range
   `RANGE(i, len)`, `RANGE(i, start, end)`, etc.
3. log
   - Pretty printing
   - Error macros
   - Testing

Fundamentals:

1. Arena

- `BufArena`
- `Available<T>`
- `Make<T>`
- `Used`

2. List

- `buf`, `len`
- `operator[i]`, `operator[i, j]`
- `Copy`

3. Slice

- `buf`, `len`, `cap`
- `operator+= T`, `operator+= List<T>`
- `Reserve`
- `Shrink`
- `Push(T)`, `Push(Arena*, List<T>)` - dynamically reallocate

4. Str (List)

- from literals
- `Split`, `Join`

5. Buf (Slice)

- `operator+= Str` - to override `List<char>`
- `Push(Arena*, Str)` - dynamically reallocate

6. Dict

- `DictChild<K,V>`
- `operator[K key, bool insert = true, Arena* = 0]`

Utilties:

1. File
2. Cmd
3. Path
4. Db
5. Gui
