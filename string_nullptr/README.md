_Document number: P2166R0_

_Project: Programming Language C++_

_Audience: LEWG-I, LEWG, LWG_

_Yuriy Chernyshov \<georgthegreat@gmail.com\>, \<thegeorg@yandex-team.ru\>_

_Date: 2020-05-06_

# A Proposal to Prohibit std::basic_string and std::basic_string_view construction from nullptr.

## Introduction and Motivation

According to the C++ Standard, the behavior of `std::basic_string::basic_string(const CharT* s)` constructor _is undefined if [s, s + Traits::length(s)) is not a valid range (for example, if s is a null pointer)_ (citation is taken [from cppreference.com](https://en.cppreference.com/w/cpp/string/basic_string/basic_string), the standard have slighty different wording in [21.3.2.2 [string.cons]](https://wg21.link/string.cons#12)). Same applies to `std::basic_string_view::basic_string_view(const CharT* s)` constructor.

Existing implementations (i. e. [libc++](https://github.com/llvm/llvm-project/blob/1b678ee8a6cc7510801b7c5be2bcde08ff8bbd6e/libcxx/include/string#L822)) might add a runtime assertion to forbid such behavior. Certain OpenSource projects would trigger this assertion. The list includes, but not limited to:

* [poco](https://github.com/pocoproject/poco/blob/3fc3e5f5b8462f7666952b43381383a79b8b5d92/Data/ODBC/include/Poco/Data/ODBC/Extractor.h#L465),
* [hdf5](https://bitbucket.hdfgroup.org/projects/HDFFV/repos/hdf5/browse/c%2B%2B/src/H5PropList.cpp#558),
* [llvm](https://github.com/llvm/llvm-project/blob/ca09dab303f4fd72343be10dbd362b60a5f91c45/llvm/lib/Target/NVPTX/NVPTXAsmPrinter.cpp#L1319) project itself, though the code is marked as unreachable.

On a large private monorepo applying proposed changes and running an automatic CI-check helped to find 7 problematic projects (the number includes projects listed above), one of which would actually segfault if the code was reached (and the code was really easy reachable).

This proposal attempts to improve the diagnostics by explicitly deleting the problematic constructors, thus moving these assertions to compile time.

## Impact on the Standard

This proposal changes `<string>` and `<string_view>` headers only and does not affect the language core.

## Proposed Wording

The wording is relative to [N4861](https://wg21.link/n4861).

1. Modify 21.3.2 [[basic.string]](https://wg21.link/basic.string) as follows:

```cpp
[...]
namespace std {
    template<class charT, class traits = char_traits<charT>,
        class Allocator = allocator<charT>>
    class basic_string {
    public:
        // types
        [...]
        // [string.cons], construct/copy/destroy
        [...]
        constexpr basic_string(const charT* s, size_type n, const Allocator& a = Allocator());
        constexpr basic_string(const charT* s, const Allocator& a = Allocator());
    +   constexpr basic_string(nullptr_t) = delete;
        [...]
        template<class T>
        constexpr basic_string& operator=(const T& t);
        constexpr basic_string& operator=(const charT* s);
    +   constexpr basic_string& operator=(nullptr_t) = delete;
        [...]
    };
    [...]
}
```

2. Modify 21.4.1 [[string.view.synop]](https://wg21.link/string.view.synop) as indicated:

```cpp
[...]
template<class charT, class traits = char_traits<charT>>
class basic_string_view {
public:
// types
    [...]
    constexpr basic_string_view(const charT* str);
+   constexpr basic_string_view(nullptr_t) = delete;
    [...]
};
```

## Further Discourse

These changes would not allow to remove runtime check, the following code is still will compile and trigger it:

```cpp
const char *p = nullptr; // or more likely, p = functionThatCanReturnNull()
string s(p, 3);
```

The another part of the proposal suggests to remove sized counterpart of nullptr constructors,  as _the behavior is undefined if [s, s + count) is not a valid range_ (citation source same). That is, the following statements are suggested where appropriate:

```cpp
basic_string(nullptr_t, size_t) == delete;
constexpr basic_string_view(nullptr_t, size_t) == delete;
```

These changes will break the legal, yet not legitimate case of constructing `std::string` using `basic_string(nullptr, 0);` and `std::string_view` using `basic_string_view(nullptr, 0);`.

## Acknowledgements

The author would like to thank Antony Poloukhin, Marshall Clow and Eric Fiselier for a thorough review and suggestions.
