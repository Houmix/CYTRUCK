#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 100
#define INITIAL_CAPACITY 1000  // Taille initiale adaptative pour le tableau

typedef struct DriverNode {
    char name[MAX_NAME_LEN];
    double totalDistance;
    struct DriverNode *left, *right;
} DriverNode;

DriverNode* createNode(char *name, double distance) {
    DriverNode *newNode = (DriverNode *)malloc(sizeof(DriverNode));
    if (newNode == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0';  // Assurez-vous que la chaîne est terminée
    newNode->totalDistance = distance;
    newNode->left = newNode->right = NULL;
    return newNode;
}

DriverNode* insertOrUpdate(DriverNode *root, char *name, double distance) {
    if (root == NULL) {
        return createNode(name, distance);
    }

    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = insertOrUpdate(root->left, name, distance);
    } else if (cmp > 0) {
        root->right = insertOrUpdate(root->right, name, distance);
    } else {
        root->totalDistance += distance;
    }
    return root;
}

void processCSV(const char *filename, DriverNode **root) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[MAX_NAME_LEN * 2], name[MAX_NAME_LEN];
    double distance;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%*[^;];%*[^;];%*[^;];%*[^;];%lf;%[^;\n]", &distance, name) == 2) {
            *root = insertOrUpdate(*root, name, distance);
        }
    }
    fclose(file);
}

void collectData(DriverNode *root, DriverNode ***array, int *size, int *capacity) {
    if (root != NULL) {
        if (*size >= *capacity) {
            *capacity *= 2;
            *array = realloc(*array, (*capacity) * sizeof(DriverNode*));
            if (*array == NULL) {
                perror("Erreur lors de la redimension du tableau");
                exit(EXIT_FAILURE);
            }
        }
        collectData(root->left, array, size, capacity);
        (*array)[*size] = root;
        (*size)++;
        collectData(root->right, array, size, capacity);
    }
}

int compareDrivers(const void *a, const void *b) {
    const DriverNode *driverA = *(const DriverNode **)a;
    const DriverNode *driverB = *(const DriverNode **)b;
    return (driverB->totalDistance > driverA->totalDistance) - (driverA->totalDistance > driverB->totalDistance);
}

void freeTree(DriverNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <chemin_fichier_csv>\n", argv[0]);
        return 1;
    }

    // Récupérer le chemin du fichier CSV à partir des arguments
    char *fichier_csv = argv[1];

    clock_t start, end;
    start = clock();

    DriverNode *root = NULL;
    processCSV(fichier_csv, &root);

    int capacity = INITIAL_CAPACITY;
    int size = 0;
    DriverNode **drivers = malloc(capacity * sizeof(DriverNode*));
    if (drivers == NULL) {
        perror("Erreur lors de l'allocation de memoire");
        return EXIT_FAILURE;
    }

    collectData(root, &drivers, &size, &capacity);
    qsort(drivers, size, sizeof(DriverNode*), compareDrivers);

    for (int i = 0; i < 10 && i < size; i++) {
        printf("%.2f;%s\n",  drivers[i]->totalDistance, drivers[i]->name);
    }

    end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    freeTree(root);
    free(drivers);

    return EXIT_SUCCESS;
}