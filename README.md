# Standard lib for C++

## From 'Speculations on arenas and custom strings in C++' April 14, 2024 

- Existing practice is unimportant. I’ve seen where that goes. Like the C
  standard library, the C++ standard library offers me little. Its concepts
  regarding ownership and memory management are irreconcilable (move
  semantics, smart pointers, etc.), so I have to build from scratch anyway. So
  absolutely no including C++ headers. The most valuable features are built
  right into the language, so I won’t need to include library definitions.


- No public or private. Still no const beyond what is required to access
  certain features. This means I can toss out a bunch of keywords like class,
  friend, etc. It eliminates noisy, repetitive code and interfaces — getters,
  setters, separate const and non-const — which in my experience means fewer
  defects.


- No references beyond mandatory cases. References hide addresses being taken
  — or merely implies it, when it’s actually an expensive copy — which is an
  annoying experience when reading unfamiliar C++. After all, for arenas the
  explicit address-taking (permanent) or copying (scratch) is a critical part
  of communicating the interfaces.


- In theory constexpr could be useful, but it keeps falling short when I try
  it out, so I’m ignoring it. I’ll elaborate in a moment.

- Minimal template use. They blow up compile times and code size, they’re
  noisy, and in practice they make debug builds (i.e. -O0) much slower
  (typically ~10x) because there’s no optimization to clean up the mess. I’ll
  only use them for a few foundational purposes, such as allocation. (Though
  this article is about the fundamental stuff.)


- No methods aside from limited use of operator overloads. I want to keep a C
  style, plus methods just look ugly without references: obj->func() vs.
  func(obj). (Why are we still writing -> in the 21st century?) Function
  overloading can instead differentiate “methods.” Overloads are acceptable in
  moderation, especially because I’m paying for it (symbol decoration) whether
  or not I take advantage.

- Finally, no exceptions of course. I assume -fno-exceptions, or the local
  equivalent, is active.

## Summary

- no standard lib
- no classes, const
- no references (caveats: Str constructor from literal)
- no constexpr (caveats: needs to be tested)
- minimal templates (caveats: avoiding macros)
- no methods (caveats: operator overloads) (not sure why??)
- no exceptions
