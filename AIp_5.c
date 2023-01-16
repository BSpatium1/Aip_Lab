#include "stdio.h"
#include "stdlib.h"
#include "time.h"


void gen_Array(int num[],int n);
void bubble_sort(FILE* output,int num[],int n);
void print_Array(FILE* output,int num[],int n);

int main() {
    FILE* output = fopen("output.txt","w");
    int choice; // выбор пользователя
    int n; // размер сгенирированного массива
    printf("Сгенирировать массив - нажмите 1\n");
    printf("Загрузить файл - нажмите 0\n");
    scanf("%d", &choice);
    //choice = 0;
    int *Array;
    if (choice == 1) {
        printf("Введите размер массива : ");
        scanf("%d", &n);
        //n = 10;
        Array = (int *) malloc(n * sizeof(int));
        gen_Array(Array, n);

    } else {
        char name_file[256];// = "file.txt";
        scanf("%s",name_file);
        FILE *input = fopen(name_file, "r");
        Array = (int *) malloc(500000 * sizeof(int));
        n = 0;
        while (!feof(input)) {
            fscanf(input, "%d", &Array[n]);
            fgetc(input);
            n += 1;
        }
        fclose(input);

    }

    print_Array(output,Array,n);
    bubble_sort(output,Array,n);
    print_Array(output,Array,n);
    fclose(output);

    free(Array);
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
void bubble_sort(FILE* output, int num[],int n){
    int tmp;
    int noSwap;
    int num_of_swaps = 0,num_of_compares = 0;

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
                fprintf(output,"%d. %d <-> %d\n", num_of_swaps, num[j], num[j+1]);
                noSwap = 0;
            }
        }
        if (noSwap == 1)
            break;
    }
12 > 10
10 > 12
11 > 9
11 > 40

}