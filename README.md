# AnyFunction

## Overview

`AnyFunction::Function` is a C++14 remplacement for `std::function`, with hybrid memory allocation.
Small enough closures are stored in an *internal storage*,
while larger ones get allocated on the heap.

Contrary to `std::function` (as with C++14), `AnyFunction::Function` provides:

* Closure storage inside `AnyFunction::Function` class instances. The size of this *internal storage* is a template parameter. (Of course, the implementation always provides storage aligned following the closure requirements.)

Contrary to `std::function` (as with C++14), `AnyFunction::Function` **does not** provide:

* Use of a specific allocator (here, operators `new` and `delete` of the closure class are always used).
* Direct access to the closure instance and its *type info*.
* Non-member `std::swap` and comparison operators specializations.

## Dependencies

This header-only library is written following the C++14 standard revision.

It only depends on the C++14 standard library.

## Usage

The usage of `AnyFunction::Function` is very similar to `std::function` (as with C++14).

An instance of `AnyFunction::Function` can either store:

* Nothing (then it is not callable).
* A plain function pointer.
* A *closure*: a plain function pointer and a (possibly modifiable) state.

For instance, C++ *lambda expressions* and `std::bind` produce closures.

More generally, a valid closure for this library is any class instance that meets the following requirements:

* *CopyConstructible*
* *Destructible*
* *Callable*

## Example

```c_cpp
#include <anyfunction.hpp>
#include <functional> // For std::mem_fn

static float func_diff(float x, float y) {
    return x - y;
}

struct Adder {
    int v;
    Adder(int v): v(v) {}
    int add(int x) const { return v + x; }
};

int main(int, char**) {
    using namespace AnyFunction;
    using namespace std::placeholders; // For _1, _2, _3, ...

    // Stores a "standalone" function
    Function<float(float, float)> diff = func_diff;
    diff(2, 1); // Returns 1 = 2 - 1

    // Stores a closure (lambda)
    // Internal storage size set to 64 bytes
    Function<float(float), 64> twice = [diff](float x) mutable -> float {
        return diff(x, -x);
    };
    twice(1); // Returns 2 = 1 - (-1)

    // Copy/move to "compatible" 'Function' instances
    Function<float(float), 0>  twiceCopy = twice;
    Function<float(float), 16> twiceMove = ::std::move(twice);

    // Test "callability"
    if (twice) { // 'twice' is always invalid after move
        // Branch not taken
    }
    twice = ::std::move(twiceCopy);
    if (twice) {
        // Branch taken
    }

    // Reset function
    diff = nullptr;
    // diff(2, 1); // Would throw Exception::Empty()

    // Stores a closure (bind)
    diff = std::bind(func_diff, _2, _1);
    diff(-2, 1); // Returns 3 = 1 - (-2)

    // Use of 'std::mem_fn' for member functions/variables
    Function<int(Adder&&, int)> add = std::mem_fn(&Adder::add);
    add(Adder{2}, twice(1)); // Returns 4 = 2 + (1 - (-1))

    return 0;
}
```

## Reference

Exceptions tree in the namespace `AnyFunction`:

| Tree | Description |
| :--- | :---------- |
| `Exception::Any` | Any exception of from this library. |
| ‣&nbsp;`Exception::Empty` | When a `Function` instance is called while not holding any function/closure. |

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

> **Exception safety:** never throws (if no closure is currently stored), or same guarantee as the stored closure *destructor* and `operator delete`.

&nbsp;

Construct with a standalone function.

* `Function(Return (*func)(Args...));`

| Parameter | Description |
| :-------- | :---------- |
| `func` | Pointer to the standalone function to store. |

> **Exception safety:** never throws.

&nbsp;

Assign a standalone function.

* `Function& operator=(Return (*func)(Args...));`

| Parameter | Description |
| :-------- | :---------- |
| `func` | Pointer to the standalone function to assign. |

**Return:** current *function holder* instance.

> **Exception safety:** never throws (if no closure is currently stored), or same guarantee as the stored closure *destructor* and `operator delete`.

&nbsp;

Construct with a closure.

* `Function(Functor&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `class Functor` | [template, deducible] Any closure class. |
| `func` | Reference to the closure to copy/move. |

> **Exception safety:** same guarantee as the stored closure *constructor* and `operator new`.

> **NB:** here `Functor` is a *forwarding reference*, not necessarily a r-value reference.

&nbsp;

Assign a closure.

* `Function& operator=(Functor&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `class Functor` | [template, deducible] Any closure class. |
| `func` | Reference to the closure to copy/move. |

**Return:** current *function holder* instance.

> **Exception safety:** basic guarantee (at best), or same as **closures** selected *constructor*, *destructor*, `operator new` and `operator delete` (if worse).

> **NB:** here there is potentially two different closures, namely the one stored and the one to assign.

> **NB:** here `Functor` is a *forwarding reference*, not necessarily a r-value reference.

&nbsp;

Copy/move construction from a compatible *function holder* instance.

* `Function(Function<Return(Args...), func_size> const& func);`
* `Function(Function<Return(Args...), func_size>&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `size_t func_size` | [template, deducible] *Function holder* storage size. |
| `func` | Reference to the *function holder* instance to copy/move. |

> **Exception safety:** same guarantee as the stored closure *constructor* and `operator new`.

> **NB:** moving a *function holder* does not necessarily imply calling the *move constructor* of the moved closure (if any).

&nbsp;

Copy/move assignment from a compatible *function holder* instance.

* `Function& operator=(Function<Return(Args...), func_size> const& func);`
* `Function& operator=(Function<Return(Args...), func_size>&& func);`

| Parameter | Description |
| :-------- | :---------- |
| `size_t func_size` | [template, deducible] *Function holder* storage size. |
| `func` | Reference to the *function holder* instance to copy/move. |

**Return:** current *function holder* instance.

> **Exception safety:** basic guarantee (at best), or same as **closures** selected *constructor*, *destructor*, `operator new` and `operator delete` (if worse).

> **NB:** here there is potentially two different closures, namely the one stored and the one to assign.

> **NB:** moving a *function holder* does not necessarily imply calling the *move constructor* of the moved closure (if any).

> **NB:** *undefined behavior* if `func` is a reference to the assigned instance.

&nbsp;

Destroy a *function holder*.

* `~Function();`

> **Exception safety:** never throws (if no closure is currently stored), or same guarantee as the stored closure *destructor* and `operator delete`.

&nbsp;

Tell whether the current *function holder* is callable.

* `operator bool() const;`

**Return:** `true` if the *function holder* holds a function/closure, `false` otherwise.

> **Exception safety:** never throws.

&nbsp;

Call the held function with the given parameters.

* `Return operator()(Args... args);`

| Parameter | Description |
| :-------- | :---------- |
| `args...` | Arguments to forward to the held function. |

**Return:** return value of the held function.

> **Exception safety:** same guarantee as the held function.
