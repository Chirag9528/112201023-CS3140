#include "./../include/Symboltable.h"
int prime1 = 19;
int prime2 = 389;
node** hash_table=NULL; 

void init(){
    hash_table = (node**)calloc(prime1 , sizeof(node*));
    if (hash_table == NULL){
        fprintf(stderr,"Calloc Failed\n");
        fflush(stderr);
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

int insert_string(char* s , variable_type vartype , char* type , int size){
    int hash_value = rolling_hashfunction(s);
    node* entry = (node*)malloc(sizeof(node));
    if (!entry){
        fprintf(stderr , "Malloc Failed\n");
        fflush(stderr);
        return 0;
    }
    entry->name = strdup(s);
    if (vartype == INTEGER){
        entry->type = strdup(type);
        entry->vartype = INTEGER;
        entry->val = (value_struct*)malloc(sizeof(value_struct));
        entry->val->size = size;
        entry->val->value = malloc(size*sizeof(int));
    }
    else if (vartype == BOOLEAN){
        entry->type = strdup(type);
        entry->vartype = BOOLEAN;
        entry->val = (value_struct*)malloc(sizeof(value_struct));
        entry->val->size = size;
        entry->val->value = malloc(size*sizeof(bool));
    }
    else if (vartype == STRING){
        entry->type = strdup(type);
        entry->vartype = STRING;
        entry->val = (value_struct*)malloc(sizeof(value_struct));
        entry->val->size = size;
        entry->val->value = malloc(size*sizeof(char*));
    }
    else if (vartype == FLOAT){
        entry->type = strdup(type);
        entry->vartype = FLOAT;
        entry->val = (value_struct*)malloc(sizeof(value_struct));
        entry->val->size = size;
        entry->val->value = malloc(size*sizeof(float));
    }
    entry->flag = 0;
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

int update_string(char* s , void* value , int idx){
    int hash_value = rolling_hashfunction(s);
    node* temp = hash_table[hash_value];
    while (temp){
        if (strcmp(temp->name , s) == 0){
            if (temp->vartype == INTEGER){
                int val = *(int*)value;
                ((int*)temp->val->value)[idx] = val; 
            }
            else if (temp->vartype == BOOLEAN){
                bool val = *(bool*)value;
                ((bool*)temp->val->value)[idx] = val;
            }
            else if (temp->vartype == STRING){
                char* val = *(char**)value;
                ((char**)temp->val->value)[idx] = strdup(val);
            }
            else if (temp->vartype == FLOAT){
                float val = *(float*)value;
                ((float*)temp->val->value)[idx] = val;
            }
            temp->flag = 1;
        }
        temp = temp->next;
    }
    return 1;
}

void update_type_size(char* s , char* name , int size){
    int hash_value = rolling_hashfunction(s);
    node* temp = hash_table[hash_value];
    while (temp){
        if (strcmp(temp->name , s) == 0){
            temp->type = strdup(name);
            temp->val->size = size;
            if (temp->vartype == INTEGER){
                temp->val->value = malloc(size*sizeof(int));
            }
            else if (temp->vartype == BOOLEAN){
                temp->val->value = malloc(size*sizeof(bool));
            }
            else if (temp->vartype == STRING){
                temp->val->value = malloc(size*sizeof(char*));
            }
            else if (temp->vartype == FLOAT){
                temp->val->value = malloc(size*sizeof(float));
            }
        }
        temp = temp->next;
    }
}

node* search_string(char* s){
    int hash_value = rolling_hashfunction(s);
    node* bucket = hash_table[hash_value];
    while (bucket){
        if (strcmp(bucket->name , s) == 0){
            return bucket;
        }
        bucket = bucket->next;
    }
    return NULL;
}

int symbol_count = 1;
int count_length(int num){
    int temp = 0;
    if (num < 0){
        num = -num;
        temp++;
    }
    if (num == 0) return 1;
    while (num > 0){
        num/=10;
        temp++;
    }
    return temp;
}

void print_hash_table(){
    printf("\n\nSymbol Table: \n=============\n");
    printf("+---------------+---------------+------------------+-------------+------------------------------------+\n");
    printf("|   Variables   |     Type      |     VarType      | Initialized |        Values                      |\n");
    printf("+---------------+---------------+------------------+-------------+------------------------------------+\n");
    for (int i=0 ; i<prime1 ; i++){
        node* temp = hash_table[i];
        while (temp){
            int length = strlen(temp->name);
            printf("|   %s",temp->name);
            for (int j = 0 ; j<12-length ; j++) printf(" ");
            printf("|   ");
            printf("%s",temp->type);
            length = strlen(temp->type);
            for (int j=0 ; j<12-length ; j++) printf(" ");
            printf("|       ");
            if (temp->vartype == INTEGER){
                printf("int");
                for (int j=0 ; j<8 ; j++) printf(" ");
            }
            else if (temp->vartype == BOOLEAN){
                printf("bool");
                for (int j=0 ; j<7 ; j++) printf(" ");
            }
            else if (temp->vartype == STRING){
                printf("char*");
                for (int j=0 ; j<6 ; j++) printf(" ");
            }
            else if (temp->vartype == FLOAT){
                printf("float");
                for (int j=0 ; j<6 ; j++) printf(" ");
            }
            printf("|      ");
            printf("%d",temp->flag);
            for (int j=0 ; j<6 ; j++) printf(" ");
            printf("|   ");
            if (temp->vartype == INTEGER){
                if (strcmp(temp->type , "array") == 0 || strcmp(temp->type , "2darray") == 0){
                    int len = 0;
                    int arraysize = temp->val->size;
                    if (strcmp(temp->type , "2darray") == 0) arraysize*= temp->val->size2;
                    for (int idx = 0; idx < arraysize ; idx++){
                        int number = ((int*)temp->val->value)[idx];
                        if (idx == arraysize - 1){
                            printf("%d",number);
                        }
                        else{
                            printf("%d , ",number);
                            len += 3;
                        }
                        len += count_length(number);
                    }
                    for (int j=0 ; j<29-len ; j++) printf(" ");
                }
                else{
                    int number = *((int*)temp->val->value);
                    printf("%d", number);
                    int len = count_length(number);
                    for (int j=0 ; j<29-len ; j++) printf(" ");
                }

            }
            else if (temp->vartype == BOOLEAN){
                if (strcmp(temp->type , "array") == 0 || strcmp(temp->type , "2darray") == 0){
                    int len = 0;
                    int arraysize = temp->val->size;
                    if (strcmp(temp->type , "2darray") == 0) arraysize*= temp->val->size2;
                    for (int idx = 0; idx<arraysize ; idx++){
                        bool number = ((bool*)temp->val->value)[idx];
                        if (idx == arraysize - 1){
                            printf("%d",number);
                        }
                        else{
                            printf("%d , ",number);
                            len += 3;
                        }
                        len += count_length(number);
                    }
                    for (int j=0 ; j<29-len ; j++) printf(" ");
                }
                else{
                    bool number = *((bool*)temp->val->value);
                    printf("%d",number);
                    int len = count_length(number);
                    for (int j=0 ; j<29-len ; j++) printf(" ");
                }
            }
            else if (temp->vartype == STRING){
                if (strcmp(temp->type , "array") == 0 || strcmp(temp->type , "2darray") == 0){
                    int len = 0;
                    int arraysize = temp->val->size;
                    if (strcmp(temp->type , "2darray") == 0) arraysize*= temp->val->size2;
                    for (int idx = 0; idx<arraysize ; idx++){
                        char* number = ((char**)temp->val->value)[idx];
                        if (idx == arraysize - 1){
                            printf("%s",number);
                        }
                        else{
                            printf("%s , ",number);
                            len += 3;
                        }
                        if (number){
                            len += strlen(number);
                        }
                        else{
                            len += 6;
                        }
                    }
                    for (int j=0 ; j<29 - len ; j++) printf(" ");
                }
                else{
                    int len = 0;
                    char* number = *((char**)temp->val->value);
                    printf("%s",number);
                    if (number){
                        len += strlen(number);
                    }
                    else{
                        len += 6;
                    }
                    for (int j=0 ; j<29-len ; j++) printf(" ");
                }
            }
            else if (temp->vartype == FLOAT){
                if (strcmp(temp->type , "array") == 0 || strcmp(temp->type,"2darray") == 0){
                    int len = 0;
                    int arraysize = temp->val->size;
                    if (strcmp(temp->type , "2darray") == 0) arraysize*= temp->val->size2;
                    for (int idx = 0; idx < arraysize ; idx++){
                        float number = ((float*)temp->val->value)[idx];
                        if (idx == arraysize - 1){
                            printf("%f",number);
                        }
                        else{
                            printf("%f , ",number);
                            len += 3;
                        }
                        len += count_length(number)+5;
                    }
                    for (int j=0 ; j<22-len ; j++) printf(" ");
                }
                else{
                    float number = *((float*)temp->val->value);
                    printf("%f", number);
                    int len = count_length(number);
                    for (int j=0 ; j<22-len ; j++) printf(" ");
                }
            }
            printf("    |\n");
            temp = temp->next;
        }
    }
    printf("+---------------+---------------+------------------+-------------+------------------------------------+\n");
    printf("\n");
    printf("=======================================================================================================\n");
}