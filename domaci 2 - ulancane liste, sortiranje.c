#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COUNTRY_LEN 4
#define NAME_LEN 255

typedef struct runner {
    int index;
    char *name;
    char country[COUNTRY_LEN];
    int born;
    int begin;
    int start;
    int delta;
} Runner;

typedef struct node {
    struct runner *runner;
    struct node *next;
} Node;

typedef struct leaderboard {
    char country[COUNTRY_LEN];
    int place;
} Leaderboard;

void free_list(Node *head) {
    while(head) {
        free(head->runner->name);
        Node *old = head;
        head = head->next;
        free(old->runner);
        free(old);
    }
}

Node *form_list() {

    //input file
    FILE *input = fopen("results.csv", "r");

    if(!input) {
        printf("DAT_GRESKA\n");
        return NULL;
    }

    Node *head = NULL, *tail, *new_node = NULL;
    Runner *new_runner = NULL;

    int index, born;
    int begin_h, begin_m, begin_s;
    int start_h, start_m, start_s;
    char tmp_name[NAME_LEN], tmp_country[COUNTRY_LEN];

    //load list element
    while(fscanf(input, "%d,%[^,],%[^,],%d,%d:%d:%d,%d:%d:%d\n", &index, tmp_name, tmp_country, &born, &begin_h, &begin_m, &begin_s, &start_h, &start_m, &start_s) != EOF) {
        new_runner = malloc(sizeof(Runner));
        if(!new_runner) {
            printf("MEM_GRESKA\n");
            free_list(head);
            fclose(input);
            return NULL;
        }

        new_runner->index = index;
        new_runner->born = born;
        strcpy(new_runner->country, tmp_country);

        int name_len = (int) strlen(tmp_name);
        new_runner->name = calloc(name_len + 1, sizeof(char));
        if(!new_runner->name) {
            printf("MEM_GRESKA\n");
            free(new_runner);
            free_list(head);
            fclose(input);
            return NULL;
        }
        strcpy(new_runner->name, tmp_name);

        int tmp_begin = 3600 * begin_h + 60 * begin_m + begin_s;
        int tmp_start = 3600 * start_h + 60 * start_m + start_s;

        new_runner->begin = tmp_begin;
        new_runner->start = tmp_start;
        new_runner->delta = tmp_begin - tmp_start;

        new_node = malloc(sizeof(Node));
        if(!new_node) {
            printf("MEM_GRESKA\n");
            free(new_runner->name);
            free(new_runner);
            free_list(head);
            fclose(input);
            return NULL;
        }

        new_node->runner = new_runner;
        new_node->next = NULL;

        if(!head) head = new_node;
        else tail->next = new_node;

        tail = new_node;
    }

    fclose(input);
    return head;
}

Leaderboard *bubble_sort(Leaderboard *ranking, int n, int limit) {
    Leaderboard tmp;
    for(int i = 0; i < limit; i++) {
        for(int j = i; j < n; j++) {
            if(ranking[i].place < ranking[j].place) {
                tmp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = tmp;
            }
        }
    }

    return ranking;
}

Node *filter_list(Node *head, int *decade, int count, Leaderboard **ranking) {
    Node *save = head;
    int born;
    int were_born = 0;

    //count the number of runners born in the specified decades
    for(int i = 0; i < count; i++) {
        decade[i] /= 10;
        for(; head; head = head->next) {
            born = (head->runner->born / 10) % 10;
            if(decade[i] == born) were_born++;
        }
        head = save;
    }

    if(were_born == 0) return NULL;

    int *keep_index = calloc(were_born, sizeof(int));
    if(!keep_index) {
        printf("MEM_GRESKA\n");
        return NULL;
    }

    //store the indices of the runners to keep
    for(int i = 0, j = 0; i < count; i++) {
        for(; head; head = head->next) {
            born = (head->runner->born / 10) % 10;
            if(decade[i] == born) keep_index[j++] = head->runner->index;
        }
        head = save;
    }

    Node *old, *prev = NULL, *first = NULL;
    int saved = 0;
    int country_count = 0, country_exist = 0;

    //filter the linked list
    while(head) {
        int x = 0;
        for(int i = 0; i < were_born; i++) { //check all indexes
            if(head->runner->index == keep_index[i]) x++;
        }

        if(x == 0) { //free unneeded runners
            free(head->runner->name);
            free(head->runner);
            if(prev != NULL) prev->next = head->next;
            old = head;
            head = head->next;
            free(old);
        }
        else { //update runners to keep
            //calculate leaderboards for stdout
            for(int i = 0; i < country_count; i++) { //country already preset in the leaderboard
                if(strcmp((*ranking)[i].country, head->runner->country) == 0) {
                    (*ranking)[i].place++;
                    country_exist++;
                }
            }

            if(country_exist == 0) { //add a country to the leaderboard
                Leaderboard *tmp = realloc(*ranking, (country_count + 1) * sizeof(Leaderboard));
                if(!tmp) {
                    printf("MEM_GRESKA\n");
                    free(keep_index);
                    free(*ranking);
                    return NULL;
                }
                *ranking = tmp;

                strcpy((*ranking)[country_count].country, head->runner->country);
                (*ranking)[country_count].place = 1;
                country_count++;
            }
            else country_exist = 0;

            if(saved == 0) { //save the first found element as the list head
                first = head; saved++;
            }

            prev = head;
            head = head->next;
        }
    }

    free(keep_index);

    //in some cases the limit might be below 3
    int limit = country_count < 3 ? country_count : 3;
    *ranking = bubble_sort(*ranking, country_count, limit);

    //print leaderboard
    for(int i = 0; i < limit; i++) {
        printf("%s %d", (*ranking)[i].country, (*ranking)[i].place);
        if(i < limit - 1) putchar('\n');
    }

    return first;
}

Node* insertion_sort(Node *save, Node *end) {
    Node *head = save;
    Runner *tmp;

    for(; head != end; head = head->next) {
        if((head->runner->delta > head->next->runner->delta) || ((head->runner->delta == head->next->runner->delta) && (head->runner->index > head->next->runner->index))) {
            tmp = head->runner;
            head->runner = head->next->runner;
            head->next->runner = tmp;
            save = insertion_sort(save, head);
            break;
        }
    }

    return save;
}

Node* sort_list(Node *head) { //prepare list for insertion_sort();
    Node *save = head;
    Node *pause = NULL;
    Runner *tmp;

    for(; head->next; head = head->next) {
        if((head->runner->delta > head->next->runner->delta) || ((head->runner->delta == head->next->runner->delta) && (head->runner->index > head->next->runner->index))) {
            tmp = head->runner;
            head->runner = head->next->runner;
            head->next->runner = tmp;
            pause = head;
            save = insertion_sort(save, pause);
        }
    }

    return save;
}

void print_file(Node *head) {
    FILE* output = fopen("lineup.txt", "w");
    for(; head; head = head->next) {
        fprintf(output, "%04d %s %d %2d", head->runner->index, head->runner->country, head->runner->born, head->runner->delta);
        if(head->next) fprintf(output,"\n");
    }
    fclose(output);
}

int main(int argc, char *argv[]) {
    if(argc == 1) return 0;

    //form list
    Node *list = form_list();
    if(!list) {
        return 0;
    }

    //get decades to filter by
    int *decade = calloc(argc - 1, sizeof(int));
    if(!decade) {
        printf("MEM_GRESKA\n");
        free_list(list);
        return 0;
    }

    char year[3];
    for(int i = 0; i < argc - 1; i++) {
        for(int j = 0; j < 2; j++) {
            year[j] = argv[i + 1][j + 1];
        }
        decade[i] = atoi(year);
    }

    //leaderboards for stdout
    Leaderboard *ranking = malloc(sizeof(Leaderboard));
    if(!ranking) {
        printf("MEM_GRESKA\n");
        free(decade);
        free_list(list);
        return 0;
    }

    //filter by decade
    list = filter_list(list, decade, argc - 1, &ranking);
    if(list == NULL) {
        free(decade);
        free(ranking);
        return 0;
    }

    //sort and write output file
    list = sort_list(list);
    print_file(list);

    free_list(list);
    free(decade);
    free(ranking);

    return 0;
}