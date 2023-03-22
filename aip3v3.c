#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LEN (2500)
#define ZERO (super_long("0"))
#define ONE (super_long("1"))

// NEW
typedef char Token[MAX_TOKEN_LEN + 1];
typedef enum {Number, Operator, End, Error} TokenType;

//typedef int SuperLong;
typedef struct {
    char digits[MAX_TOKEN_LEN];
    char sign;
    int len;
} SuperLong;

Token token;
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
    if (token_type == Error) exit(-1);
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

// SUPER LONG ########################################################################################################

SuperLong super_long(char* s) {
    SuperLong num;
    num.sign = (*s=='0')? 0 : (*s=='-')? -1 : 1;
    if (*s == '+' || *s == '-') ++s; // skip plus or minus
    num.len = strlen(s);
    for (int i = 0; i < MAX_TOKEN_LEN; ++i)
        num.digits[i] = (i < num.len)? s[num.len - 1 - i] - '0' : 0;
    return num;
}

int equal(SuperLong num1, SuperLong num2) {
    if (num1.sign != num2.sign) return 0;
    if (num1.len != num2.len) return 0;
    for (int i = 0; i < num1.len; ++i)
        if (num1.digits[i] != num2.digits[i]) return 0;
    return 1;
}

int _max(int num1, int num2) { return (num1 > num2)? num1 : num2; }

int abs_greater(SuperLong num1, SuperLong num2) {
    if (num1.len == num2.len) {
        for (int i = num1.len - 1; i >= 0; --i)
            if (num1.digits[i] != num2.digits[i])
                return num1.digits[i] > num2.digits[i];
        return 0;
    }
    return num1.len > num2.len;
}

SuperLong _diff(SuperLong term1, SuperLong term2) {
    int max_len = _max(term1.len, term2.len);
    term1.len = max_len;
    for (int i = 0; i < max_len; ++i) {
        term1.digits[i] -= term2.digits[i];
        if (term1.digits[i] < 0) {
            --term1.digits[i + 1];
            term1.digits[i] += 10;
        }
    }
    for (int i = term1.len - 1; i > 0; --i) {
        if (term1.digits[i] != 0) break;
        --term1.len;
    }
    return term1;
}

SuperLong sum(SuperLong term1, SuperLong term2) {
    if (term1.sign != term2.sign)  // via difference
        return abs_greater(term1, term2)?  // |term1| > |term2|
            _diff(term1, term2) : _diff(term2, term1);
    int max_len = _max(term1.len, term2.len);
    term1.len = max_len;
    for (int i = 0; i < max_len; ++i) {
        term1.digits[i] += term2.digits[i];
        if (term1.digits[i] > 9) {
            term1.digits[i] -= 10;
            ++term1.digits[i + 1];
            if (i == max_len - 1) ++term1.len;
        }
    }
    return term1;
}

SuperLong inc(SuperLong num) { return sum(num, ONE); }

SuperLong opposite(SuperLong num) {
    num.sign *= -1;
    return num;
}

SuperLong diff(SuperLong term1, SuperLong term2) {  // a - b == a + -b
    return sum(term1, opposite(term2));
}

SuperLong dec(SuperLong num) { return diff(num, ONE); }

SuperLong prod(SuperLong factor1, SuperLong factor2) {  // 11 * 11
    SuperLong result = ZERO;
    int max_len = factor1.len + factor2.len - 1;  // 3
    result.len = max_len;
    for (int j = 0; j < factor2.len; ++j)
        for (int i = 0; i < factor1.len; ++i) {
            result.digits[i + j] += factor1.digits[i] * factor2.digits[j];
            if (result.digits[i + j] > 9) {
                result.digits[i + j + 1] += result.digits[i + j] / 10;
                result.digits[i + j] %= 10;
                if (i + j == max_len - 1) result.len = max_len + 1;
            }
        }
    result.sign = factor1.sign * factor2.sign;
    return result;
}

int is_zero(SuperLong num) {
    if (num.len != 1) return 0;
    return num.digits[0] == 0;
}

// x / 2 == x * 5 / 10
SuperLong half(SuperLong num) {
    if (is_zero(num)) return ZERO;
    num = prod(num, super_long("5"));
    for (int i = 1; i < num.len; ++i) num.digits[i - 1] = num.digits[i];
    --num.len;
    return num;
}

// S = (start + finish) * (finish - start + 1) * 2
SuperLong prog_sum(SuperLong start, SuperLong finish) {
    if (equal(start, finish)) return start;
    //return sum(start, prog_sum(inc(start), finish));
    /*SuperLong result = ZERO;
    for (SuperLong i = start; !equal(i, finish); i = inc(i))
        result = sum(result, i);
    return sum(result, finish);*/
    return half(prod(sum(start, finish), sum(diff(finish, start), ONE)));
}

SuperLong pow_(SuperLong base, SuperLong power) {
    if (is_zero(power)) return ONE;
    //return prod(base, pow_(base, dec(power)));
    SuperLong result = ONE;
    for (SuperLong i = ONE; !equal(i, inc(power)); i = inc(i))
        result = prod(result, base);
    return result;
}

SuperLong fact(SuperLong num) {
    if (is_zero(num)) return ONE;
    //return prod(num, fact(dec(num)));
    SuperLong result = ONE;
    for (SuperLong i = ONE; !equal(i, inc(num)); i = inc(i))
        result = prod(result, i);
    return result;
}

void print_super_long(SuperLong num) {
    if (num.sign == -1 && !is_zero(num)) printf("-");
    for (int i = num.len - 1; i >= 0; --i)
        printf("%d", num.digits[i]);
    puts("");
}

// ####################################################################################################################

SuperLong arith(SuperLong num1, char operator, SuperLong num2) {
    switch (operator) {
        case ' ': return prog_sum(num1, num2);
        case '+': return sum(num1, num2);
        case '-': return diff(num1, num2);
        case '*': return prod(num1, num2);
        case '^': return pow_(num1, num2);
        case '!': return fact(num1);
    }
    SuperLong none;
    return none;
}

SuperLong get_number() {
    get_token();
    return super_long(token);
}

SuperLong get_factorial() {
    SuperLong base;
    char operator;

    base = get_number();
    while (1) {
        get_token();
        operator = *token;
        if (!is_factorial()) {
            put_back();
            break;
        }
        base = arith(base, operator, ZERO);  // третий аргумент не нужон, так что просто 0
    }
    return base;
}

SuperLong get_sign() {  // calculate signed factorial
    SuperLong factorial;
    char operator;

    get_token();
    operator = *token;
    if (!is_plus_minus()) {
        put_back();
        factorial = get_factorial();
    } else {
        factorial = get_factorial();
        factorial = arith(ZERO, operator, factorial);
    }
    return factorial;
}

SuperLong get_factor() {  // calculate power
    SuperLong base, power;
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

SuperLong get_term() {  // calculate product
    SuperLong factor1, factor2;
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

SuperLong get_bound() {  // calculate sum
    SuperLong term1, term2;
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

SuperLong get_value() {  // calculate sum of progression
    SuperLong bound1, bound2;
    char operator;

    bound1 = get_bound();
    get_token();
    operator = *token;
    if (!is_space()) {
        put_back();
    } else {
        get_token();
        if (token_type != Number)
        bound2 = get_bound();
        bound1 = arith(bound1, operator, bound2);
    }
    return bound1;
}

int main(){
    print_super_long(get_value());
    /*
    SuperLong mOne = super_long("-1");
    SuperLong two = super_long("2");
    SuperLong mTwo = super_long("-2");
    print_super_long(sum(mOne, mOne));  // -2
    print_super_long(sum(mOne, two));  // 1 (-3)
    print_super_long(sum(two, mOne));  // 1 (3)
    print_super_long(sum(mTwo, ONE));  // -1 (-3)
    print_super_long(sum(ONE, mTwo));  // -1 (3)
    print_super_long(sum(mTwo, ONE));  // -1 (-3)
    print_super_long(sum(ONE, mOne));  // 0
    print_super_long(sum(mOne, ONE));  // 0
    */
    return 0;
}
