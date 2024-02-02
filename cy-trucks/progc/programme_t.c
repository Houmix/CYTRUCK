#include<stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct AVL{
    char* city;
    int crossed;
    int d_city;
    int eq;
    struct AVL* pLeft;
    struct AVL* pRight;
}AVL_Tree;

typedef AVL_Tree* pTree;

pTree createNodeAVL(const char* city, int crossed, int d_city){
    pTree new = malloc(sizeof(AVL_Tree));
    if (new == NULL) {
        printf("Malloc à échoué\n");
        exit(1);
    }
    new->city = strdup(city); 
    new->crossed = crossed;
    new->d_city = d_city;
    new->eq = 0;
    new->pLeft = NULL;
    new->pRight = NULL;

    return new;
}


int max2(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}


int min2(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int max3(int a, int b, int c) {
    if (a > b) {
        if (a > c) {
            return a;
        } else {
            return c;
        }
    } else {
        if (b > c) {
            return b;
        } else {
            return c;
        }
    }
}
int min3(int a, int b, int c) {
    if (a < b && a < c) {
        return a;
    } else if (b < c) {
        return b;
    } else {
        return c;
    }
}


pTree leftRotation(pTree avl){
    if(avl == NULL){
        printf("L'arbe AVL vaut NULL donc vide\n");
        exit(2);
    }
    pTree pivot;
    int eq_a, eq_p;

    pivot = avl->pRight;
    avl->pRight = pivot->pLeft;
    pivot->pLeft = avl;
   
    eq_a = avl->eq;
    eq_p = pivot->eq;
    avl->eq = eq_a - max2(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p-1);

    avl = pivot;
    return avl;
}
pTree rightRotation(pTree avl){
    if(avl == NULL){
        printf("L'arbe AVL vaut NULL donc vide\n");
        exit(3);
    }
    pTree pivot;
    int eq_a, eq_p;

    pivot = avl->pLeft;
    avl->pLeft = pivot->pRight;
    pivot->pRight = avl;
   
    eq_a = avl->eq;
    eq_p = pivot->eq;
    avl->eq = eq_a - min2(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    avl = pivot;
    return avl;
}

pTree doubleLeftRotation(pTree avl){
    if(avl == NULL){
        printf("L'arbe AVL vaut NULL donc vide\n");
        exit(4);
    }
    avl->pRight = rightRotation(avl->pRight);
    return leftRotation(avl);
}

pTree doubleRightRotation(pTree avl){
        if(avl == NULL){
        printf("L'arbe AVL vaut NULL donc vide\n");
        exit(5);
    }
    avl->pLeft = leftRotation(avl->pLeft);
    return rightRotation(avl);
}

pTree equilibrageAVL(pTree avl){
        if(avl == NULL){
        printf("L'arbe AVL vaut NULL donc vide\n");
        exit(6);
    }
    if(avl->eq > 1){
        if(avl->pRight->eq >= 0){
            return leftRotation(avl);
        }
        else{
            return doubleLeftRotation(avl);
        }
    }
    else if(avl->eq < -1){
        if(avl->pLeft->eq <= 0){
            return rightRotation(avl);
        }
        else{
            return doubleRightRotation(avl);
        }
    }
    return avl;
}

pTree insertAVL(pTree avl, int* h, const char* city, int crossed, int d_city){
    pTree new = createNodeAVL(city, crossed, d_city);
    if(new == NULL){
        printf("Malloc à échoué\n");
        exit(7);
    }
    if(avl ==  NULL){
        *h = 1;
        return new;
    }
    else if(new->crossed <= avl->crossed){
        avl->pLeft = insertAVL(avl->pLeft, h,city, crossed, d_city);
        *h = -(*h);
    }
    else if(new->crossed > avl->crossed){
        avl->pRight = insertAVL(avl->pRight, h, city, crossed, d_city);
    }
    else{
        h = 0;
        return avl;
    }
    if(h != 0){
        avl->eq = avl->eq + *h;
        avl = equilibrageAVL(avl);
        if(avl->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return avl;
}


pTree fillAVL(const char* data, pTree avl) {
    FILE* file = fopen(data, "r");
    if (file == NULL) {
        perror("Error opening the file... \n");
        exit(EXIT_FAILURE);
    }

    char city[50];
    int h = 0;
    int crossed = 0, d_city = 0;

    // The loop retrieves the csv data line by line, then adds them to the tree
    while (fscanf(file, "%49[^;];%d;%d", city, &crossed, &d_city) == 3) {
        avl = insertAVL(avl, &h, city, crossed, d_city);
    }

    fclose(file);
    return avl;
}

void infixreverse(pTree avl, FILE* file) {
    if (avl != NULL) {
        infixreverse(avl->pRight, file);
        fprintf(file, "%s;%d;%d", avl->city, avl->crossed, avl->d_city);
        infixreverse(avl->pLeft, file);
    }
}


void freeAVL(pTree avl){
    if (avl != NULL) {
        
    freeAVL(avl->pLeft);
    freeAVL(avl->pRight);
    free(avl);
    }
}
    

int main(int argc, char *argv[]){
   
    if (argc != 2) {
        printf("Le nombre d'argumentdu programme c n'est pas respecté\n");
        exit (8);
    }

    pTree avl = NULL;
    avl = fillAVL(argv[1], avl);

    FILE *file = fopen("temp/firsttemp.csv", "w");
    if (file == NULL) {
        perror("Error d'ouverture du fichier data...\n");
        exit(EXIT_FAILURE);
    }
    infixreverse(avl, file);
    fclose(file);

    
    freeAVL(avl);
    return 0 ;
}



