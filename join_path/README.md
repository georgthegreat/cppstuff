_Document number: D????R0_

_Project: Programming Language C++_

_Audience: LWG_

_Yuriy Chernyshov \<georgthegreat@gmail.com\>, \<thegeorg@yandex-team.ru\>_

_Date: 2019-09-01_

# A Proposal to Add std::filesystem::join_path Variadic Function

## Introduction and Motivation

C++17 introduced `std::filesystem` namespace and, particularly, `std::filesystem::path` class, both providing a convenient way to operate with local file systems.

`std::filesystem::path` supports `append()` member method, which allows to join two path objects with suitable path separator. However, there is no convenient way to join multiple path objects, which is supported by the standard libraries of other program languages (e. g. [os.path.join](https://docs.python.org/3/library/os.path.html#os.path.join) from Python).

As I write this proposal, the only way of joining multiple path objects is to sequentially apply `operator/` to them, which requires the client to convert the first argument to `std::filesystem::path` object explicitly, thus making the code look like:

```
const std::filesystem::path home_dir = std::filesystem::path("/home") / std::getenv("USER");
std::filesystem::is_dir(std::filesystem::path(home) / "thegeorg" / "repo" / "cppstuff");
```

It also requires both left and right arguments of `operator/` to be `std::filesystem::path` objects, which requires memory allocation and makes the multiple appendings ineffective.

This is an attempt to add a more convenient and effective way to join multiple paths with a single statement.

## Impact on the Standard

This proposal changes `<filesystem>` header only and does not affect the language core. The proposal might benefit from `path_view` type which might be introduced in [P1030](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1030r2.pdf).

## Design Decisions

The author supposes that `join_path` would require at least two path components to be joined. The variadic variant of the method was chosen in order to avoid putting all input components into some container (thus, saving at least one allocation) and in order to match function signature against existing variants.

## Proposed Wording

//TODO: add anchor from the working draft.

```cpp
template<typename... Parts>
path join_path(path origin, string_view part, const Parts&... parts);
```

_Returns: the result of sequential application of `std::filesystem::path::append` to origin._

_Note: at least two arguments are needed in order to invoke this method._

_Notes to the reviewers:_

* `string_view` might be changed to `path_view` which might be introduced in [P1030](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1030r2.pdf).
* There is no proposal which suggests adding `reserve` method to `std::filesystem::path` objects, yet implementation might benefit from its addition.

## Possible Implementation

```
template<typename... Parts>
path join_path(path origin, string_view part, const Parts&... Parts) {
    path.append(second);
    (path.append(rest), ...);
    return path;
}
```

An implementation can be found in `join_path.h` file in the repo. See attached `join_path_test.cpp` and `Makefile` for usage examples.

## Feature-Testing Macro

For the purposes of SG10 we recommend the feature-testing macro name `__cpp_lib_filesystem_join_path`.

## Acknowledgements

The author would like to thank Andrew Andreev, Konstantin Shalnev and Nikolay Fedorov who have helped to shape out the proposed method.
