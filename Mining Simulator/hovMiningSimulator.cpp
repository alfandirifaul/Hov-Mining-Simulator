#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
    int index;
    int gold;
    int depth;
    Node *left;
    Node *right;
};

Node *createNewNode(int index, int gold, int depth){
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->index = index;
    newNode->gold = gold;
    newNode->depth = depth;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *insert(Node *node, int index, int gold, int depth){
    if(node == NULL){
        return createNewNode(index, gold, depth);
    }

    if(index < node->index){
        node->left = insert(node->left, index, gold, depth + 1);
    }else if(index > node->index){
        node->right = insert(node->right, index, gold, depth + 1);
    }else{
        node->gold += gold;
    }
    return node;
}

Node *searchIndex(Node *node, int index){
    if (node == NULL || node->index == index){
        return node;
    }else if(index < node->index){
        return searchIndex(node->left, index);
    }else{
        return searchIndex(node->right, index);
    }
}

void insertMiningData(Node **node){
    int index;
    printf("Input cave index [1-999]     : ");
    scanf("%d", &index);
    if(index < 1 || index > 999){
        puts("Invalid cave index. Please enter a value between 1-999.");
        return;
    }

    int gold;
    printf("Input gold production [1-100]: ");
    scanf("%d", &gold);
    if(gold < 1 || gold > 100){
        puts("Invalid gold value. Please enter a value between 1-999.");
        return;
    }

    Node *existingNode = searchIndex(*node, index);
    if(existingNode == NULL){
        *node = insert(*node, index, gold, 1);
        printf("New index cave detected.\nTotal gold production for Cave %d is %d\n",
        index, gold);
    }else{
        existingNode->gold += gold;
        printf("Cave already exists.\nNew total gold production for Cave %d is %d\n",
        index, existingNode->gold);
    }
}

void display(Node *node){
        if(node == NULL)return;

        display(node->left);
        printf(">> Cave %d   (depth %d   ), total production gold: %d\n",
        node->index, node->depth, node->gold);
        display(node->right);
}

void displayAllCaveData(Node *node){
    if(node == NULL){
        puts("Cave data is empty, nothing to display.");
        return;
    }
    display(node);
}

int calculateMaxDepth(Node *node){
    if(node == NULL){
        return 0;
    }

    int leftGold = calculateMaxDepth(node->left);
    int rightGold = calculateMaxDepth(node->right);
    return (leftGold > rightGold) ? leftGold : rightGold;
}

int calculateTotalGold(Node *node, int depth){
    if(node == NULL){
        return 0;
    }

    if(node->depth == depth){
        return node->gold + calculateTotalGold(node->left, depth) + calculateTotalGold(node->right, depth);
    }else{
        return calculateTotalGold(node->left, depth) + calculateTotalGold(node->right, depth);
    }
}

void displayMiningReports(Node *node){
    if(node == NULL){
        puts("Cave data is empty, nothing to display");
        return;
    }

    int maxDepth = 0;
    Node *current = node;
    while (current != NULL){
        maxDepth++;
        current = current->right;
    }

    for(int i = 1; i < maxDepth; i++){
        int totalGold = calculateTotalGold(node, i);
        if(totalGold > 0){
            printf(">> Total gold production for depth %d    is %d\n",
            i, totalGold);
        }
    }
}

void freeTree(Node *node){
    if(node != NULL){
        return;
    }

    freeTree(node->right);
    freeTree(node->left);

    free(node);
}

void mainMenu(){
    puts("Hov Mining Simulator");
    puts("====================");
    puts("[1] Insert Mining Data");
    puts("[2] Display All Cave Data");
    puts("[3] Display Mining Reports");
    puts("[4] Exit");
    printf(">> ");
}

void pressAnyKey(){
    puts("Press enter to continue.");
    getchar();
    getchar();
}

int main(){
    Node *root = NULL;
    
    while(true){
        mainMenu();
        int choice; scanf("%d", &choice);
        switch(choice){
            case 1:
                insertMiningData(&root);
                pressAnyKey();
                break;
            case 2:
                displayAllCaveData(root);
                pressAnyKey();
                break;
            case 3:
                displayMiningReports(root);
                pressAnyKey();
                break;
            case 4:
                freeTree(root);
                puts("All data has been removed from memory.");
                puts("Thank you for playing, see you again...");
                return 0;
            default:
                puts("Invalid choice. Please input key 1-4");
        }   
    }
    return 0;
}