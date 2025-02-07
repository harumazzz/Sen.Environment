# Kernel API

-   In this documentation, I will instruct you how to register custom C++ function to JS function.

-   Types table:

|                                     C++ Type                                     |   JS Type   |
| :------------------------------------------------------------------------------: | :---------: |
| int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, size_t |   bigint    |
|                                   std::string                                    |   string    |
|                                       bool                                       |   boolean   |
|                                  float, double                                   |   number    |
|                                struct ArrayBuffer                                | ArrayBuffer |
|    List<T> (only primitives type, other please write your own implementation)    |  Array<T>   |

-   Please notice that you do not want to assign `std::string_view` to JS function, otherwise memory
    issues will happens.

## Variable types

-   All variable types supported by `Kernel` are defined in `value_adapter.hpp`

-   All variable types relate to `struct` or `class` need to be wrapped in `std::shared_ptr`

-   All variable that is not cheap to copy in C++ should be wrapped in reference to reduce the
    amount of copy it makes

## Assigning custom functions

### Simple primitype type function

-   In `kernel/interface/api/method.hpp`:

```cpp
inline static auto print(
	std::string& data
) -> void
{
	std::cout << data << std::flush << '\n';
}
```

-   In `kernel/interface/context.hpp`:

```cpp
// the proxy need to take return type and the rest are the required arguments type
runtime.add_proxy(FunctionProxy<void, std::string&>::as_function<Interface::API::print>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "print_test"_sv);
```

-   You need to add a custom define, example:

```ts
namespace Sen.Kernel {
	export function print_test(data: string): void;
}
```

-   After compile, you can now call in TS:

```ts
namespace Sen.Script {
	export function main() {
		Kernel.print_test('Hello World');
	}
}
Sen.Script.main();
```

### Complex type function

-   In `kernel/interface/api/method.hpp`:

```cpp
struct Point
{
	int x{};
	int y{};
};
```

```cpp
template <>
auto from_value<std::shared_ptr<Point>>(
	JSContext* context,
	JSValue val
) -> std::shared_ptr<Point>
{
	// make the shared pointer based on the function make_shared
	auto destination = std::make_shared<Point>();
	// Make a new reference count, if you don't make a new reference count, the quickjs engine will make an undefined behavior
	auto value = Value::as_new_reference(context, val);
	// test if value is object
	assert_conditional(value.is_object(), "Value must be object", "from_value");
	// accessing x property inside object and cast it's value to int64
	destination->x = value.get_property("x").get_bigint64();
	// accessing y property inside object and cast it's value to int64
	destination->y = value.get_property("y").get_bigint64();
	return destination;
}
```

```cpp
// This is an user defined type, please use std::shared_ptr to wrap it
inline static auto print_point(
	std::shared_ptr<Point>& data
) -> void
{
	std::cout << data->x << '\t' << data->y << std::flush << '\n';
}
```

-   In `kernel/interface/context.hpp`:

```cpp
// the proxy need to take return type and the rest are the required arguments type
runtime.add_proxy(Proxy<void, std::shared_ptr<Point>&>::as_function<Interface::API::print_point>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "print_point"_sv);
```

-   You need to add a custom define, example:

```ts
namespace Sen.Kernel {
	export interface Point {
		x: bigint;
		y: bigint;
	}

	export function print_point(data: Point): void;
}
```

-   After compile, you can now call in TS:

```ts
namespace Sen.Script {
	export function main() {
		Kernel.print_point({
			x: 10n,
			y: 10n,
		});
	}
}
Sen.Script.main();
```

-   In order to receive the user defined in JS, you need to implement the `to_value` method.

```cpp
template <>
static auto to_value<std::shared_ptr<Point>>(
	JSContext* context,
	const std::shared_ptr<Point>& value
) -> JSValue {
	// Create a new instance of JS Value
	auto destination = Value::as_new_instance(context, JS_UNITIALIZED);
	// Set the destination is an object first
	destination.set_object();
	// Defint it's custom property
	destination.set_property("x", value->x);
	// Defint it's custom property
	destination.set_property("y", value->y);
	// Release the value, so that the engine is holding the value instead of inside this global scope
	return destination.release();
}
```

```cpp
// This is an user defined type, please use std::shared_ptr to wrap it
inline static auto return_point(

) -> std::shared_ptr<Point>
{
	// for custom type that has pointer, I suggest you make a custom deleter
	return std::make_shared<Point>(1, 2);
}
```

-   In `kernel/interface/context.hpp`:

```cpp
// the proxy need to take return type and the rest are the required arguments type
runtime.add_proxy(FunctionProxy<std::shared_ptr<Point>>::as_function<Interface::API::return_point>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "return_point"_sv);
```

-   You need to add a custom define, example:

```ts
namespace Sen.Kernel {
	export interface Point {
		x: bigint;
		y: bigint;
	}

	export function return_point(): Point;
}
```

-   After compile, you can now call in TS:

```ts
namespace Sen.Script {
	export function main() {
		// Use the method
		let point: Kernel.Point = Kernel.return_point();
		// Accessing object
		let x: bigint = point.x;
		let y: bigint = point.y;
		// Use it
		// ... Other codes
	}
}
Sen.Script.main();
```
