# AnyFunction

## Overview

`AnyFunction::Function` is a C++14 remplacement for `std::function`, with hybrid memory allocation.
Small enough closures are stored in an internal storage,
while larger ones get allocated on the heap.

Contrary to `std::function` (as with C++14), `AnyFunction::Function` provides:

* Closure storage inside `AnyFunction::Function` class instances. The size of this internal storage is a template parameter. (Of course, the implementation always provides storage aligned according to the functor requirements.)

Contrary to `std::function` (as with C++14), `AnyFunction::Function` **does not** provides:

* Use of a specific allocator (here, operators `new` and `delete` of the functor class are always used).
* Direct access to the functor instance and its *type info*.
* Non-member `std::swap` and comparison operators specializations.

## Usage

This header-only library is written for the C++14 standard revision.

The usage of `AnyFunction::Function` is very similar to `std::function` (as with C++14).

The functor used with `AnyFunction::Function` must meet the requirements of:

* *CopyConstructible*
* *Destructible*
* *Callable*

## Reference

Exception tree in namespace `AnyFunction`:

| Tree | Description |
| :--- | :---------- |
| `Exception::Any` | Any exception of from this library. |
| ‣&nbsp;`Exception::None` | When a `Function` instance is called while not holding any functor. |

&nbsp;

### template class `AnyFunction::Function`

| Parameter | Description |
| :-------- | :---------- |
| `Return(Args...)` | Expected function signature. |
| `size_t`  | [optional] Size of the internal buffer, in bytes. |

> **NB:** template class instances with the same function signature but different internal buffer sizes are compatibles, meaning that copy/move operations are allowed between them.

> **NB:** methods from `AnyFunction::Function` instances are not *thread-safe*.

&nbsp;

### class `AnyFunction::Function<Return(Args...), size>`

#### Public member methods:

&nbsp;

Construct an empty *function holder*.

* `Function();`
* `Function(std::nullptr_t);`

| Parameter | Description |
| :-------- | :---------- |
| *not bound* | [optional] `nullptr` value. |

> **Exception safety:** never throws.

&nbsp;

Clear a *function holder*.

* `void clear();`
* `Function& operator=(std::nullptr_t);`

| Parameter | Description |
| :-------- | :---------- |
| *not bound* | `nullptr` value. |

**Return:** current *function holder* instance.

> **Exception safety:** strong guarantee at best, or same as the stored functor (if any) *destructor* and `operator delete` if worst.

&nbsp;

Construct with a standalone function (no closure).

* `Function(Return (*func)(Args...));`

| Parameter | Description |
| :-------- | :---------- |
| `func` | Pointer to the standalone function to store. |

> **Exception safety:** never throws.

&nbsp;

Assign a standalone function (no closure).

* `Function& operator=(Return (*func)(Args...));`

| Parameter | Description |
| :-------- | :---------- |
| `func` | Pointer to the standalone function to assign. |

**Return:** current *function holder* instance.

> **Exception safety:** strong guarantee at best, or same as the stored functor (if any) *destructor* and `operator delete` if worst.

&nbsp;

Construct with a functor (function and closure).

* `Function(Functor&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `class Functor` | [deducible] Any functor class. |
| `func` | Reference to the functor to copy/move. |

> **Exception safety:** same as functor selected *constructor* and `operator new`.

> **NB:** here `Functor` is a *forwarding reference*, not necessarily a r-value reference.

&nbsp;

Assign a functor (function and closure).

* `Function& operator=(Functor&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `class Functor` | [deducible] Any functor class. |
| `func` | Reference to the functor to copy/move. |

**Return:** current *function holder* instance.

> **Exception safety:** basic guarantee at best, or same as **functors** selected *constructor*, *destructor*, *operators* `new` and `delete` if worst.

> **NB:** here there is potentially two different functors, namely the one stored and the one to assign.

> **NB:** here `Functor` is a *forwarding reference*, not necessarily a r-value reference.

&nbsp;

Copy/move construction from a compatible *function holder* instance.

* `Function(Function<Return(Args...), func_size> const& func);`
* `Function(Function<Return(Args...), func_size>&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `size_t func_size` | [deducible] Compatible *function holder* storage size. |
| `func` | Reference to the *function holder* instance to copy/move. |

> **Exception safety:** same as functor selected *constructor* and `operator new`.

&nbsp;

Copy/move assignment from a compatible *function holder* instance.

* `Function& operator=(Function<Return(Args...), func_size> const& func);`
* `Function& operator=(Function<Return(Args...), func_size>&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `size_t func_size` | [deducible] Compatible *function holder* storage size. |
| `func` | Reference to the *function holder* instance to copy/move. |

**Return:** current *function holder* instance.

> **Exception safety:** basic guarantee at best, or same as **functors** selected *constructor*, *destructor*, *operators* `new` and `delete` if worst.

> **NB:** here there is potentially two different functors, namely the one stored and the one to assign.

> **NB:** *undefined behavior* if `func` is a reference to the assigned instance.

&nbsp;

Destroy a *function holder*.

* `~Function();`

> **Exception safety:** never throws, or same guarantee as the stored functor (if any) *destructor* and `operator delete` if at least one do.

&nbsp;

Tell whether the current *function holder* is callable.

* `operator bool() const;`

**Return:** `true` if the current *function holder* is callable, `false` otherwise.

> **Exception safety:** never throws.

&nbsp;

Call the held function with the given parameters.

* `Return operator()(Args... args);`

| Parameter | Description |
| :-------- | :---------- |
| `args...` | Arguments to forward to the held function. |

**Return:** return value of the held function.

> **Exception safety:** same guarantee as the held function.
