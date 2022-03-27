#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>

#include <andwass/string_view.hpp>

TEST(StringView, Construction) {
    auto fn = [](andwass::string_view sv) {
        return sv;
    };

    const char *str = "hello world";
    EXPECT_EQ(andwass::string_view().size(), 0);
    EXPECT_EQ(fn(str).data(), str);
    EXPECT_EQ(fn(str).size(), strlen(str));

    EXPECT_EQ(fn({str, 4}).data(), str);
    EXPECT_EQ(fn({str, 4}).size(), 4);
    EXPECT_FALSE(fn({str, 4}).is_empty());

    EXPECT_EQ(fn("").size(), 0);
    EXPECT_TRUE(fn("").is_empty());
}

TEST(StringView, Equality) {
    EXPECT_TRUE(andwass::string_view("hello") == andwass::string_view("hello"));
    EXPECT_TRUE(andwass::string_view("hello") == "hello");
    EXPECT_TRUE(andwass::string_view("hello world", 5) == "hello");

    EXPECT_TRUE(andwass::string_view("hello") != andwass::string_view("world"));
}

TEST(StringView, Indexing) {
    EXPECT_TRUE(andwass::string_view("hello")[0] == 'h');
    EXPECT_TRUE(andwass::string_view("hello")[2] == 'l');
}

TEST(StringView, RemovePrefix) {
    auto data = []() {
        return andwass::string_view("hello world");
    };
    auto do_test = [&](size_t n, const char* expected_remaining, const char* expected_return_value)
    {
        auto x = data();
        auto ret = x.remove_prefix(n);
        EXPECT_EQ(x, expected_remaining);
        EXPECT_EQ(ret, expected_return_value);
    };

    do_test(6, "world", "hello ");
    do_test(0, "hello world", "");
    do_test(10000, "", "hello world");

    auto x = data();
    auto y = x;
    EXPECT_EQ(y.remove_prefix(10000).begin(), x.begin());
}

TEST(StringView, RemoveSuffix) {
    auto data = []() {
        return andwass::string_view("hello world");
    };
    auto do_test = [&](size_t n, const char* expected_remaining, const char* expected_return_value)
    {
        auto x = data();
        auto ret = x.remove_suffix(n);
        EXPECT_EQ(x, expected_remaining);
        EXPECT_EQ(ret, expected_return_value);
    };
    do_test(6, "hello", " world");
    do_test(0, "hello world", "");
    do_test(10000, "", "hello world");

    auto x = data();
    auto y = x;
    EXPECT_EQ(y.remove_suffix(10000).begin(), x.begin());
}

TEST(StringView, substr) {
    andwass::string_view data("hello world");
    EXPECT_EQ(data.substr(0, 5), "hello");
    EXPECT_EQ(data.substr(0), "hello world");
    EXPECT_EQ(data.substr(6), "world");
    EXPECT_EQ(data.substr(1000), "");
    EXPECT_EQ(data.substr(1000).begin(), data.end());
}

TEST(StringView, StartsEndsWith) {
    andwass::string_view data("hello world");
    EXPECT_TRUE(data.starts_with("hello world"));
    EXPECT_TRUE(data.starts_with("hello "));
    EXPECT_TRUE(data.starts_with(""));

    EXPECT_TRUE(data.ends_with("hello world"));
    EXPECT_TRUE(data.ends_with(" world"));
    EXPECT_TRUE(data.ends_with(""));

    EXPECT_FALSE(data.starts_with("world"));
    EXPECT_FALSE(data.ends_with("hello"));

    EXPECT_FALSE(data.starts_with("hello world "));
    EXPECT_FALSE(data.ends_with("hello world "));
}

TEST(StringView, FindContains) {
    andwass::string_view data("hello world");
    EXPECT_EQ(data.find(""), 0);
    EXPECT_EQ(data.find("hello"), 0);
    EXPECT_EQ(data.find("world"), 6);
    EXPECT_EQ(data.find("abc"), andwass::string_view::npos);

    EXPECT_TRUE(data.contains("hello"));
    EXPECT_TRUE(data.contains("world"));
    EXPECT_FALSE(data.contains("helloworld"));
}

TEST(StringView, FindNth) {
    andwass::string_view data("ab ab ab ab ab");
    EXPECT_EQ(data.find_nth("ab", 0), 0);
    EXPECT_EQ(data.find_nth("ab", 1), 3);
    EXPECT_EQ(data.find_nth("ab", 4), 12);
    EXPECT_EQ(data.find_nth("ab", 5), andwass::string_view::npos);

    EXPECT_EQ(data.find_nth("abc", 0), andwass::string_view::npos);
    EXPECT_EQ(andwass::string_view("aaaaaaaaaa").find_nth("a", 5), 5);
}

TEST(StringView, SubStringStartingWith) {
    andwass::string_view data("ab cde f gh ij");
    EXPECT_EQ(data.substr_starting_with("cde"), "cde f gh ij");
    EXPECT_EQ(data.substr_starting_with("ab"), "ab cde f gh ij");
    EXPECT_EQ(data.substr_starting_with("klj"), "");
}

TEST(StringView, ReverseFind) {
    andwass::string_view data("hello world");
    EXPECT_EQ(data.rfind('d'), 10);
    EXPECT_EQ(data.rfind('z'), andwass::string_view::npos);
    EXPECT_EQ(data.rfind('l'), 9);
    EXPECT_EQ(data.rfind('h'), 0);

    EXPECT_EQ(data.rfind("world"), 6);
    EXPECT_EQ(data.rfind("hello"), 0);
    EXPECT_EQ(data.rfind("abc"), andwass::string_view::npos);
    EXPECT_EQ(data.rfind("d"), 10);
    EXPECT_EQ(data.rfind(""), data.size());
}

TEST(StringView, Compare) {
    using andwass::operator""_sv;

    EXPECT_TRUE( "abc"_sv.compare("abcd"_sv) < 0 );
    EXPECT_TRUE( "abcd"_sv.compare("abc"_sv) > 0 );
    EXPECT_TRUE( "abc"_sv.compare("abc"_sv) == 0 );
    EXPECT_TRUE( ""_sv.compare(""_sv) == 0 );
}

#pragma clang diagnostic pop