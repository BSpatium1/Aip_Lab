#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

void gen_num(int num[],int n);
void print_num(FILE* output,int num[],int n);
void make_sort(
        FILE* output,
        int num[],
        int n,
        void (*sort_func)(FILE*, int[], int),
        char title[]
);
void bubble_sort(FILE* output,int num[],int n);
void shekerSort(FILE* output,int num[], int n);
void qsortRecursive(FILE* output, int num[], int n);
void ShellSort(FILE* output, int num[],int n);

int main() {
    FILE* output = fopen("output.txt","w");
    int choice; // выбор пользователя
    int n; // размер сгенирированного массива
    printf("Сгенирировать массив - нажмите 1\n");
    printf("Загрузить файл - нажмите 0\n");
    scanf("%d", &choice);
    //choice = 0;
    int *num,*num_cpy;

    if (choice == 1) {
        printf("Введите размер массива : ");
        scanf("%d", &n);
        //n = 10;
        num = (int *) malloc(n * sizeof (int));
        num_cpy = (int *) malloc(500000 * sizeof (int));
        gen_num(num, n);

    } else {
        char name_file[256];// = "file.txt";
        scanf("%s",name_file);
        FILE *input = fopen(name_file, "r");
        num = (int *) malloc(500000 * sizeof (int));
        num_cpy = (int *) malloc(500000 * sizeof (int));
        n = 0;
        while (!feof(input)) {
            fscanf(input, "%d", &num[n]);
            fgetc(input);
            n += 1;
        }
        fclose(input);
    }

    make_sort(output, num, n, bubble_sort, "BUBBLE SORT");
    make_sort(output, num, n, shekerSort, "SHEKER SORT");
    make_sort(output, num, n, ShellSort, "SHELL SORT");

    fclose(output);

    free(num);
    free(num_cpy);
    return 0;
}

void gen_num(int num[],int n){
    srand(time(NULL));
    for(int i = 0;i<n;i++){
        num[i] = rand() % 100;
    }
}

void print_num(FILE* output,int num[],int n){
    for (int i = 0; i < n; i++) {
        printf("%d,", num[i]);
        fprintf(output,"%d,",num[i]);

    }
    printf("\n");
    fprintf(output,"\n");

}

void make_sort(
        FILE* output,
        int num[],
        int n,
        void (*sort_func)(FILE*, int[], int),
        char title[]
) {
    int *num_cpy = (int *) malloc(n * sizeof(int));
    memcpy(num_cpy, num, n * sizeof(int));
    printf("%s\n", title);
    fprintf(output, "%s\n", title);
    //print_num(output, num_cpy, n);
    sort_func(output, num_cpy, n);
    //print_num(output, num_cpy, n);
    printf("\n");
    fprintf(output, "\n");
}

void bubble_sort(FILE* output, int num[],int n){
    clock_t start = clock();
    double time_spent = 0.0;
    int tmp;
    int noSwap;
    long num_of_swaps = 0;
    long num_of_com = 0;

    for (int i = n - 1; i >= 0; i--)
    {
        noSwap = 1;
        for (int j = 0; j < i; j++)
        {
            if (num[j] > num[j + 1])
            {
                tmp = num[j];
                num[j] = num[j + 1];
                num[j + 1] = tmp;
                num_of_swaps++;
                noSwap = 0;
            }
            num_of_com++;
        }
        if (noSwap == 1)
            break;
    }
    clock_t end = clock();
    fprintf(output,"num of swap - %ld\n", num_of_swaps);
    fprintf(output,"num of compare - %ld\n",num_of_com);
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(output,"time of work %4lf\n",time_spent);
    printf("Num of swap : %ld\n",num_of_swaps);
    printf("Num of compare : %ld\n", num_of_com);
    printf("Time :%f\n",time_spent);


}
//  - - - --  - - - - - - - - -- - - - - - -- - - - - - -- - - - - -
void shekerSort(FILE* output, int num[], int n){
    clock_t start = clock();
    double time_spent =0.0;
    long num_of_swaps = 0;
    long num_of_com = 0;
    int left = 0, right = n - 1;
    int flag = 1;
    while ((left < right) && flag > 0)
    {

        flag = 0;
        for (int i = left; i<right; i++)
        {
            if (num[i]>num[i + 1])
            {
                double t = num[i];
                num[i] = num[i + 1];
                num[i + 1] = t;
                num_of_swaps++;
                flag = 1;
            }
            num_of_com++;
        }
        right--;
        for (int i = right; i>left; i--)
        {
            if (num[i - 1]>num[i])
            {
                double t = num[i];
                num[i] = num[i - 1];
                num[i - 1] = t;
                num_of_swaps++;
                flag = 1;
            }
            num_of_com++;
        }
        left++;

    }
    fprintf(output,"num of swap - %ld\n", num_of_swaps);
    fprintf(output,"num of compare - %ld\n", num_of_com);
    clock_t end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(output,"time of work %4lf\n",time_spent);
    printf("Num of swap : %ld\n",num_of_swaps);
    printf("Num of compare : %ld\n", num_of_com);
    printf("Time :%f\n",time_spent);
}
// - - - - - -- - - - -- - - - - - - -- - - - - - - - - - - - - -  -




void ShellSort(FILE* output, int num[],int n) {
    clock_t start = clock();
    double time_spent = 0.0;
    long num_of_swaps = 0;
    long num_of_com = 0;

    for (int step = n / 2; step > 0; step /= 2)
        for (int i = step; i < n; ++i)
            for (int j = i - step; j >= 0; j -= step) {
                if (num[j] > num[j + step]) {
                    int t = num[j];
                    num[j] = num[j + step];
                    num[j + step] = t;
                    num_of_swaps += 3;

                    ++num_of_com;
                } else{
                    break;
                }
            }

    fprintf(output, "num of swap - %ld\n", num_of_swaps);
    fprintf(output, "num of compare - %ld\n", num_of_com);
    clock_t end = clock();
    time_spent += (double) (end - start) / CLOCKS_PER_SEC;
    fprintf(output, "time of work %4lf\n", time_spent);
    printf("Num of swap : %ld\n", num_of_swaps);
    printf("Num of compare : %ld\n", num_of_com);
    printf("Time :%f\n", time_spent);
}









