# JavaScript Standard Library

- [Standard objects by category](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects)

## Value properties

These global properties return a simple value. They have no properties or methods.

1.  globalThis
2.  Infinity
3.  NaN
4.  undefined

## Function properties

These global functions—functions which are called globally, rather than on an object—directly return their results to the caller.

1.  eval()
2.  isFinite()
3.  isNaN()
4.  parseFloat()
5.  parseInt()
6.  decodeURI()
7.  decodeURIComponent()
8.  encodeURI()
9.  encodeURIComponent()
10. escape() Deprecated
11. unescape() Deprecated

## Fundamental objects

These objects represent fundamental language constructs.

1.  Object
2.  Function
3.  Boolean
4.  Symbol

## Error objects

Error objects are a special type of fundamental object. They include the basic Error type, as well as several specialized error types.

1.  Error
2.  AggregateError
3.  EvalError
4.  RangeError
5.  ReferenceError
6.  SyntaxError
7.  TypeError
8.  URIError
9.  InternalError Non-standard

## Numbers and dates

These are the base objects representing numbers, dates, and mathematical calculations.

1.  Number
2.  BigInt
3.  Math
4.  Date
5.  Temporal

## Text processing

These objects represent strings and support manipulating them.

1.  String
2.  RegExp

## Indexed collections

These objects represent collections of data which are ordered by an index value. This includes (typed) arrays and array-like constructs.

1.  Array
2.  TypedArray
3.  Int8Array
4.  Uint8Array
5.  Uint8ClampedArray
6.  Int16Array
7.  Uint16Array
8.  Int32Array
9.  Uint32Array
10. BigInt64Array
11. BigUint64Array
12. Float16Array
13. Float32Array
14. Float64Array

## Keyed collections

These objects represent collections which use keys. The iterable collections (Map and Set) contain elements which are easily iterated in the order of insertion.

1.  Map
2.  Set
3.  WeakMap
4.  WeakSet

## Structured data

These objects represent and interact with structured data buffers and data coded using JavaScript Object Notation (JSON).

1.  ArrayBuffer
2.  SharedArrayBuffer
3.  DataView
4.  Atomics
5.  JSON

## Managing memory

These objects interact with the garbage collection mechanism.

1.  WeakRef
2.  FinalizationRegistry

## Control abstraction objects

Control abstractions can help to structure code, especially async code (without using deeply nested callbacks, for example).

1.  Iterator
2.  AsyncIterator
3.  Promise
4.  GeneratorFunction
5.  AsyncGeneratorFunction
6.  Generator
7.  AsyncGenerator
8.  AsyncFunction

## Reflection

1.  Reflect
2.  Proxy

## Internationalization

Additions to the ECMAScript core for language-sensitive functionalities.

1.  Intl
2.  Intl.Collator
3.  Intl.DateTimeFormat
4.  Intl.DisplayNames
5.  Intl.DurationFormat
6.  Intl.ListFormat
7.  Intl.Locale
8.  Intl.NumberFormat
9.  Intl.PluralRules
10. Intl.RelativeTimeFormat
11. Intl.Segmenter
