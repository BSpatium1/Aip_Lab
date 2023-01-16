#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

void gen_Array(int num[],int n);
void print_Array(FILE* output,int num[],int n);
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
    int *Array,*Array_cpy;

    if (choice == 1) {
        printf("Введите размер массива : ");
        scanf("%d", &n);
        //n = 10;
        Array = (int *) malloc(n * sizeof (int));
        Array_cpy = (int *) malloc(500000 * sizeof (int));
        gen_Array(Array, n);

    } else {
        char name_file[256];// = "file.txt";
        scanf("%s",name_file);
        FILE *input = fopen(name_file, "r");
        Array = (int *) malloc(500000 * sizeof (int));
        Array_cpy = (int *) malloc(500000 * sizeof (int));
        n = 0;
        while (!feof(input)) {
            fscanf(input, "%d", &Array[n]);
            fgetc(input);
            n += 1;
        }
        fclose(input);
    }

    make_sort(output, Array, n, bubble_sort, "BUBBLE SORT");
    make_sort(output, Array, n, shekerSort, "SHEKER SORT");
    make_sort(output, Array, n, ShellSort, "SHELL SORT");

    fclose(output);

    free(Array);
    free(Array_cpy);
    return 0;
}

void gen_Array(int num[],int n){
    srand(time(NULL));
    for(int i = 0;i<n;i++){
        num[i] = rand() % 100;
    }
}

void print_Array(FILE* output,int num[],int n){
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
    int *Array_cpy = (int *) malloc(n * sizeof(int));
    memcpy(Array_cpy, num, n * sizeof(int));
    printf("%s\n", title);
    fprintf(output, "%s\n", title);
    //print_Array(output, Array_cpy, n);
    sort_func(output, Array_cpy, n);
    //print_Array(output, Array_cpy, n);
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
               // fprintf(output,"%d\n", num_of_swaps);
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
                //fprintf(output,"%d\n", num_of_swaps);
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
}
// - - - - - -- - - - -- - - - - - - -- - - - - - - - - - - - - -  -




void ShellSort(FILE* output, int num[],int n)
{
    clock_t start = clock();
    double time_spent = 0.0;
    long num_of_swaps = 0;
    long num_of_com = 0;
    int i, j, step;
    int tmp;
    for (step = n / 2; step > 0; step /= 2)
        for (i = step; i < n; i++)
        {
            tmp = num[i];
            for (j = i; j >= step; j -= step)
            {
                num_of_com++;
                if (tmp < num[j - step])
                    num[j] = num[j - step];
                else
                    break;

            }
            num[j] = tmp;
            num_of_swaps++;
        }
    fprintf(output,"num of swap - %ld\n", num_of_swaps);
    fprintf(output,"num of compare - %ld\n", num_of_com);
    clock_t end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(output,"time of work %4lf\n",time_spent);
}





void qsortRecursive(FILE* output, int num[], int n) {
    int num_of_swaps = 0;
    int num_of_com = 0;
    clock_t start = clock();
    double time_spent =0.0;
    //Указатели в начало и в конец массива
    int i = 0;
    int j = n - 1;

    //Центральный элемент массива
    int mid = num[n / 2];

    //Делим массив
    do {
        //Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        //В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
        while(num[i] < mid) {
            i++;
            num_of_com++;
        }
        //В правой части пропускаем элементы, которые больше центрального
        while(num[j] > mid) {
            j--;
            num_of_com++;
        }

        //Меняем элементы местами
        if (i <= j) {
            int tmp = num[i];
            num[i] = num[j];
            num[j] = tmp;
            num_of_swaps++;

            i++;
            j--;
        }
    } while (i <= j);


    //Рекурсивные вызовы, если осталось, что сортировать
    if(j > 0) {
        //"Левый кусок"
        qsortRecursive(output,num, j + 1);
    }
    if (i < n) {
        //"Првый кусок"
        qsortRecursive(output,&num[i], n - i);
    }
    fprintf(output,"num of swap - %d\n", num_of_swaps);
    fprintf(output,"num of compare - %d\n", num_of_com);
    clock_t end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(output,"time of work %4lf\n",time_spent);

}
