#include <stdio.h>
#define STR_MAX_LEN 30
#define STR_MAX_MAX_LEN 3000

int len = 0;
int belong[STR_MAX_LEN] = {0};
int belong_max[STR_MAX_LEN] = {0};
int palindrome_len_max;

void get_palindrome(char s[],int start,int end);

int main(){
    char s[STR_MAX_MAX_LEN + 1];
    scanf("%100[^\n]s",s);
    s[STR_MAX_LEN] = '\0';  // cut to STR_MAX_LEN

    for(len = 0;s[len]!= '\0';len++);
    palindrome_len_max = -1;  // any smallest number
    get_palindrome(s,0,len);
    for(int i=0;i<len;i++){
        if(belong_max[i] == 1){
            printf("%c",s[i]);
        }
    }
    printf("\n");

    int is_first = 1;
    for(int i=0;i<len;i++){
        if(belong_max[i] == 0){
            if (is_first) {
                is_first = 0;
            } else {
                printf(", ");
            }
            printf("%d - %c",i+1,s[i]);
        }
    }
    printf("\n"); //
}
//1q2wr3t5a3gw2plmq1
void get_palindrome(char s[],int start, int end){
    // get len of current palindrome
    int palindrome_len = 0;
    for(int i = 0;i<len;i++){
        if(belong[i]== 1){
            palindrome_len++;
        }
    }
    // return condition
    if(palindrome_len + (end-start+1) <= palindrome_len_max) {
        return;
    }
    // base of recursion
    if(start>end) {
        if(palindrome_len>palindrome_len_max){
            // update max
            palindrome_len_max = palindrome_len;
            for(int i = 0;i<len;i++){
                belong_max[i] = belong[i];
            }
        }
        return;
    }
    if(s[start] == s[end]){
        belong[start] = 1;
        belong[end] = 1;
        get_palindrome(s,start+1,end-1);
        belong[start] = 0;
        belong[end] = 0;
    }else{
        get_palindrome(s,start,end-1);
        get_palindrome(s,start+1,end);
    }
}
