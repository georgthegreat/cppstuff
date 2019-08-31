#include "join_path.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

#include <iostream>

void assert_equals(const std::filesystem::path& first, const std::filesystem::path& second) {
    if (first != second) {
        throw std::logic_error(
            "Expected " + first.string() + " " +
            "to equal to " + second.string() +
            ", but the expectations have failed"
        );
    } else {
        std::cout << "OK: " << first << std::endl;
    }
}

int main() {
    //testing behavior
    assert_equals(join_path("foo", "bar"), "foo/bar");
    assert_equals(join_path("foo", "bar", "baz"), "foo/bar/baz");
    assert_equals(join_path("foo", "", "baz"), "foo/baz");
    assert_equals(join_path("foo/", "baz"), "foo/baz");
    assert_equals(join_path("foo", "/baz"), "/baz");

    //testing various string type support
    const std::string s1 = "foo";
    const std::string s2 = "bar";
    assert_equals(join_path(s1, s2), "foo/bar");

    const char c1[] = "foo";
    const char c2[] = "bar";
    assert_equals(join_path(c1, c2), "foo/bar");

    std::string_view v1 = s1;
    std::string_view v2 = s2;
    assert_equals(join_path(v1, v2), "foo/bar");

    const char* p1 = s1.c_str();
    const char* p2 = s2.c_str();
    assert_equals(join_path(p1, p2), "foo/bar");

    return 0;
}
