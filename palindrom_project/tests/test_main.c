#include "../src/palindrom.h"
#include "../src/logger.h"
#include <assert.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>

void test_preprocessing() {
    setlocale(LC_ALL, "");
    TextData data;
    const char* test_str = "А роза упала, на лапу Азора!";
    
    preprocess_text(test_str, &data, 2);
    assert(wcscmp(data.clean_text, L"арозаупаланалапуазора") == 0);
    
    free_text_data(&data);
    printf("Test Preprocessing: PASSED\n");
}

int main() {
    log_init(NULL, "ERROR");
    test_preprocessing();
    return 0;
}
