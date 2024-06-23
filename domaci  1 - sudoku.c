#include<stdio.h>
#include<stdlib.h>

void free_sudoku(int **sudoku, int size_of_sudoku) {
    for(int i = 0; i < size_of_sudoku; i++) free(sudoku[i]);
    free(sudoku);
}

int check(int **sudoku, int n, int test_value, int last_row, int last_col, int i_pos, int j_pos) {
    int invalid = 0;

    for(int k = 0; k < last_col; k++) {
        if(test_value == sudoku[i_pos][k]) invalid++;
    }

    for(int k = 0; k < last_row; k++) {
        if(test_value == sudoku[k][j_pos]) invalid++;
    }

    int quadrant = (j_pos / (n/2)) * (n/2);

    if(i_pos % 2 == 1) {
        for(int k = quadrant; k < quadrant + n/2; k++) {
            if(test_value == sudoku[i_pos - 1][k]) invalid++;
        }
    }
    else if(last_row == n) {
        for(int k = quadrant; k < quadrant + n/2; k++) {
            if(test_value == sudoku[i_pos + 1][k]) invalid++;
        }
    }

    return invalid;
}

int **load_sudoku(int n) {
    int **sudoku = NULL;
    sudoku = calloc(n, sizeof(int*));
    if(!sudoku) {
        printf("MEM_GRESKA\n");
        return NULL;
    }

    for(int i = 0; i < n; i++) {
        sudoku[i] = calloc(n, sizeof(int));
        if(!sudoku[i]) {
            printf("MEM_GRESKA\n");
            free_sudoku(sudoku, i + 1);
            return NULL;
        }

        for(int j = 0; j < n; j++) {
            scanf("%d", &sudoku[i][j]);

            if(sudoku[i][j] < 0 || sudoku[i][j] > n) {
                printf("NEVALIDNE VREDNOSTI\n");
                free_sudoku(sudoku, i + 1);
                return NULL;
            }

            //exclude 0 from check();
            if(sudoku[i][j] != 0) {
                if(check(sudoku, n, sudoku[i][j], i, j, i ,j) > 0) {
                    printf("NEVALIDNO STANJE\n");
                    free_sudoku(sudoku, i + 1);
                    return NULL;
                }
            }
        }
    }

    return sudoku;
}

void write_sudoku(int **sudoku, int n, int *end) {
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d", sudoku[i][j]);
            if(j < n - 1) putchar(' ');
            if(sudoku[i][j] == 0) (*end)++;
        }
        printf("\n");
    }
}

void game_loop(int **sudoku, int n) {
    int x, y, value = 0, end = 0;

    while(1) {
        scanf("%d%d%d", &x, &y, &value);
        if(x < 0 || x >= n || y < 0 || y >= n || value < 1 || value > n){
            printf("KRAJ IGRE\n");
            break;
        }

        if(sudoku[x][y] == 0) {
            if(check(sudoku, n, value, n, n, x ,y) == 0) {
                printf("VALIDNO STANJE\n");
                sudoku[x][y] = value;
            }
            else {
                printf("NEVALIDNO STANJE\n");
            }

            write_sudoku(sudoku, n, &end);

            if(end == 0) {
                printf("TACNO RESENJE\n");
                break;
            }

            end = 0;
        }
    }
}

int main(){
    int n = 0;
    scanf("%d", &n);

    if(n != 4 && n != 6){
        printf("NEVALIDNA DIMENZIJA\n");
        return 0;
    }

    int **sudoku = NULL;
    sudoku = load_sudoku(n);
    if(sudoku == NULL) {
        return 0;
    }

    game_loop(sudoku, n);
    free_sudoku(sudoku, n);
    return 0;
}
