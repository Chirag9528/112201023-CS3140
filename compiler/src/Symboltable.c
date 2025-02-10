#include "Symboltable.h"
int prime1 = 19;
int prime2 = 389;
node** hash_table; 

void init(){
    hash_table = (node**)calloc(prime1 , sizeof(node*));
    if (hash_table == NULL){
        fprintf(stderr,"Calloc Failed\n");
        exit(0);
    }
}

int rolling_hashfunction(char* s){
    unsigned long long hash = 0;
    while (*s){
        hash = (hash * prime2 + (*s++))%mod;
    }
    return hash%prime1;
}

int insert_string(char* s , int flag , int value){
    int hash_value = rolling_hashfunction(s);
    node* entry = (node*)malloc(sizeof(node));
    if (!entry){
        fprintf(stderr , "Malloc Failed\n");
        return 0;
    }
    entry->name = strdup(s);
    if (flag){
        entry->flag = 1;
        entry->value = value;
    }
    else{
        entry->flag = 0;
    }
    if (hash_table[hash_value] == NULL){
        entry->next = NULL;
        hash_table[hash_value] = entry;
    }
    else{
        entry->next = hash_table[hash_value];
        hash_table[hash_value] = entry;   
    }
    return 1;
}

int update_string(char* s , int value){
    int hash_value = rolling_hashfunction(s);
    node* temp = hash_table[hash_value];
    while (temp){
        if (strcmp(temp->name , s) == 0){
            temp->value =  value;
            temp->flag = 1;
        }
        temp = temp->next;
    }
    return 1;
}

int search_string(char* s){
    int hash_value = rolling_hashfunction(s);
    node* bucket = hash_table[hash_value];
    while (bucket){
        if (strcmp(bucket->name , s) == 0){
            if (bucket->flag) return bucket->value;
            return -9999999;
        }
        bucket = bucket->next;
    }
    return -8888888;
}

int symbol_count = 1;
void print_hash_table(){
    printf("Symbol Table: \n");
    printf("----------------\n");
    for (int i=0 ; i<prime1 ; i++){
        node* temp = hash_table[i];
        while (temp){
            printf("%s ---> %d\n",temp->name ,  temp->value);
            temp = temp->next;
        }
    }
    printf("\n");
    printf("===========================================================================\n");
}