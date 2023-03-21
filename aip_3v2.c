#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TOKEN_LEN 2500
// NEW
typedef enum {Number, Operator, End, Error} TokenType;

char token[MAX_TOKEN_LEN + 1];
TokenType token_type;

int is_digit(char c) { return c >= '0' && c <= '9'; }

long is_operator(char c) { return (long)strchr(" +-*^!", c); }

void get_token() {
    if (token_type == End) return;
    char* s = token;
    char c = getchar();
    if (c == '\n') {
        token_type = End;
    } else if (is_operator(c)) {
        *s++ = c;
        token_type = Operator;
    } else if (is_digit(c)) {
        do {
            *s++ = c;
            c = getchar();
        } while (is_digit(c));
        ungetc(c, stdin);
        token_type = Number;
    } else token_type = Error;
    *s = '\0';
}

void put_back() {
    char* s = token;
    while (*s) ++s;  // looking for end of string
    // *s == '\0'
    while (s != token) {
        --s;
        ungetc(*s, stdin);
    }
}

void print_token() {
    printf("token = \"%s\", type = %d\n", token, token_type);
}

int is_space() {
    if (token_type != Operator) return 0;
    return *token == ' ';
}

int is_plus_minus() {
    if (token_type != Operator) return 0;
    return *token == '+' || *token == '-';
}

int is_multiply() {
    if (token_type != Operator) return 0;
    return *token == '*';
}

int is_factorial() {
    if (token_type != Operator) return 0;
    return *token == '!';
}

int is_power() {
    if (token_type != Operator) return 0;
    return *token == '^';
}

int progression_sum(int start, int finish) {
    if (start == finish) return start;
    return start + progression_sum(start + 1, finish);
}

int factorial(int num) {
    if (num == 0) return 1;
    return num * factorial(num - 1);
}

int arith(int num1, char operator, int num2) {
    switch (operator) {
        case ' ': return progression_sum(num1, num2);
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '^': return pow(num1, num2);
        case '!': return factorial(num1);
    }
    return 0;
}

int get_number() {
    get_token();
    return atoi(token);
}

int get_factorial() {
    int base;
    char operator;

    base = get_number();
    while (1) {
        get_token();
        operator = *token;
        if (!is_factorial()) {
            put_back();
            break;
        }
        base = arith(base, operator, 0);  // третий аргумент не нужон, так что просто 0
    }
    return base;
}

int get_sign() {  // calculate signed factorial
    int factorial;
    char operator;

    get_token();
    operator = *token;
    if (!is_plus_minus()) {
        put_back();
        factorial = get_factorial();
    } else {
        factorial = get_number();
        factorial = arith(0, operator, factorial);
    }
    return factorial;
}

int get_factor() {  // calculate power
    int base, power;
    char operator;

    base = get_sign();
    get_token();
    operator = *token;
    if (!is_power()) {
        put_back();
        return base;
    }
    power = get_factor();
    base = arith(base, operator, power);
    return base;
}

int get_term() {  // calculate product
    int factor1, factor2;
    char operator;

    factor1 = get_factor();
    while (1) {
        get_token();
        operator = *token;
        if (!is_multiply()) {
            put_back();
            break;
        }
        factor2 = get_factor();
        factor1 = arith(factor1, operator, factor2);
    }
    return factor1;
}

int get_bound() {  // calculate sum
    int term1, term2;
    char operator;

    term1 = get_term();
    while (1) {
        get_token();
        operator = *token;
        if (!is_plus_minus()) {
            put_back();
            break;
        }
        term2 = get_term();
        term1 = arith(term1, operator, term2);
    }
    return term1;
}

int get_value() {  // calculate sum of progression
    int bound1, bound2;
    char operator;

    bound1 = get_bound();
    get_token();
    operator = *token;
    if (!is_space()) {
        put_back();
    } else {
        bound2 = get_bound();
        bound1 = arith(bound1, operator, bound2);
    }
    return bound1;
}

int main(){
    printf("%d\n", get_value());
    return 0;
}
