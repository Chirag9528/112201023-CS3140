#include "SyntaxTree.h"

void inorder(tree* root){
    if (root == NULL) return;
    inorder(root->left);
    if (root->name){
        printf("%s  ",root->name);
    }
    else{
        printf("%d  ",root->value);
    }
    inorder(root->right);
    return;
}

void preorder(tree* root){
    if (root == NULL) return;
    if (root->name){
        printf("%s  ",root->name);
    }
    else{
        printf("%d  ",root->value);
    }
    preorder(root->left);
    preorder(root->right);
    return;
}


int depth(tree* root){
    if (root == NULL) return 0;
    else if (!root->left && !root->right) return 0;
    int left = depth(root->left);
    int right = depth(root->right);
    if (left >= right) return 1 + left;
    return 1 + right;
}

tree* createnode(char* s , int value , tree* left , tree* right){
    tree* newnode = (tree*)malloc(sizeof(tree));
    if (newnode == NULL) return newnode;
    if (s){
        newnode->name = strdup(s);
    }
    if (left){
        newnode->left = (tree*)malloc(sizeof(tree));
        if (newnode->left == NULL) return NULL;
        newnode->left = left;
    }
    if (right){
        newnode->right = (tree*)malloc(sizeof(tree));
        if (newnode->right == NULL) return NULL;
        newnode->right = right;
    }
    newnode->value = value;
    return newnode;
}


void print_tree(tree* root){
    char* space = "      ";
    int height = depth(root);
    int maxheight = height;
    int maxnodes = 1<<height;
    queue* qu = init_queue();
    push(qu , root , 0);
    int leftmostcal = 0;
    while (qu->size > 0){
        int n = qu->size;
        int initialspace = 1<<height;
        while (initialspace > 1){
            printf("%s",space);
            initialspace--;
        }
        int diff = maxheight - height;
        int prevtrack = -leftmostcal;
        int total = n;
        while (n--){
            queuenode* node = pop(qu);

            // for printing
            int gap = ((node->number - prevtrack)/(1<<(height+1)))-1;
            while (gap > 0){
                int gaptime = (1<<height)*2;
                while (gaptime > 0){
                    printf("%s",space);
                    gaptime--;
                }
                gap--;
            }
            
            prevtrack = node->number;

            if (total == n+1 && node->number != (-leftmostcal)){
                int firstnodenotpresent = (1<<height)*2;
                while (firstnodenotpresent > 1){
                    printf("%s",space);
                    firstnodenotpresent--;
                }
            }

            int temp = (1<<height)*2;
            if (node->treenode->name){
                printf("%s",node->treenode->name);
            }
            else{
                printf("%d",node->treenode->value);
            }
            while (temp > 1){
                printf("%s",space);
                temp--;
            }
            // ends

            if (node->treenode->left){
                push(qu , node->treenode->left, node->number-(1<<(height-1)));
            }
            if (node->treenode->right){
                push(qu , node->treenode->right , node->number+(1<<(height-1)));
            }
        }
        printf("\n\n");
        height--;
        leftmostcal+= (1<<height);
    }
    printf("\n\n");
}

void print_info(tree* root){
    printf("Preorder Traversal: \n");
    printf("--------------------\n");
    preorder(root);
    printf("\n\n");
    printf("Inorder Traversal: \n");
    printf("--------------------\n");
    inorder(root);
    printf("\n\n");
    printf("Abstract Syntax Tree: \n");
    printf("----------------------\n");
    printf("\n");
    print_tree(root);
    printf("===========================================================================\n\n");
}


void print_decl(tree* root){
    char* space = "        ";
    printf("%s%s",space , space);
    printf("%s\n",root->name);
    printf("\n");
    printf("%s",root->left->name);
    printf("%s%s%s%s",space,space,space,space);
    queue* qu = init_queue();
    push(qu , root->right , 0);
    while (qu->size > 0){
        tree* node = pop(qu)->treenode;
        printf("%s%s",node->name,space);
        if (node->right){
            push(qu , node->right , 0);
        }
    }
    printf("\n\n\n");
}

void print_decl_info(tree* root){
    printf("Preorder Traversal: \n");
    printf("--------------------\n");
    preorder(root);
    printf("\n\n");
    printf("Inorder Traversal: \n");
    printf("--------------------\n");
    inorder(root);
    printf("\n\n");
    printf("Abstract Syntax Tree: \n");
    printf("----------------------\n");
    printf("\n");
    print_decl(root);
    printf("===========================================================================\n\n");
}