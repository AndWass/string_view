# `andwass::string_view`

This is [`string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view)
type with less undefined behaviour, and a slightly modified API.

## Some differences with `std::string_view`

`std::string_view::empty` is called `is_empty`.

`andwass::string_view::substr` will always return a valid view,
while `std::string_view::substr` throws an exception in some cases.

`remove_prefix/remove_suffix` will never cause undefined behaviour. They also return a view of what
was removed:
```c++
andwass::string_view aw_view("hello world");
assert(aw_view.remove_prefix(6) == "hello ");
assert(aw_view == "world");

aw_view = andwass::string_view("hello world");
assert(aw_view.remove_suffix(6) == " world");
assert(aw_view == "hello");
```

## FAQ

  * Is this completely API compatible? No
  * Is this completely free of UB? No, notably indexing (`some_view[index]`) can still cause UB
  * Is this completely non-throwing? No, `some_view.at(index)` can throw.
  * Does this library provide a user-defined literal? Yes, `using namespace andwass::literals` will enable
`"some string"_sv` to create views..