#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ctype.h>

void reverse_string(const char *input, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
}

int count_vowels(const char *s) {
    int count = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = tolower(s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int is_palindrome(const char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

static void test_reverse_string_basic(void **state) {
    (void)state; 
    
    char output[100];
    reverse_string("hello", output);
    assert_string_equal(output, "olleh");
}

static void test_reverse_string_empty(void **state) {
    (void)state;
    
    char output[100];
    reverse_string("", output);
    assert_string_equal(output, "");
}

static void test_reverse_string_palindrome(void **state) {
    (void)state;
    
    char output[100];
    reverse_string("racecar", output);
    assert_string_equal(output, "racecar");
}

static void test_count_vowels_hello(void **state) {
    (void)state;
    
    int result = count_vowels("hello");
    assert_int_equal(result, 2);
}

static void test_is_palindrome_not_palindrome(void **state) {
    (void)state;
    
    int result = is_palindrome("abcde");
    assert_int_equal(result, 0);
}

static void test_is_palindrome_with_spaces(void **state) {
    (void)state;
    
    int result = is_palindrome("ab ba");
    assert_int_equal(result, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_reverse_string_basic),
        cmocka_unit_test(test_reverse_string_empty),
        cmocka_unit_test(test_reverse_string_palindrome),
        cmocka_unit_test(test_count_vowels_hello),
        cmocka_unit_test(test_is_palindrome_not_palindrome),
        cmocka_unit_test(test_is_palindrome_with_spaces),


    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
