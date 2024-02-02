#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_BUFFER 4096

typedef struct AVL {
    int id;
    float min;
    float max;
    float moy;
    float diff;
    struct AVL *pLeft;
    struct AVL *pRight;
    int h;
} Arbre, *pArbre;


int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

pArbre rotationGauche(pArbre a) {
    if (a == NULL || a->pRight == NULL) {
        return a;
    }

    pArbre pivot = a->pRight;
    a->pRight = pivot->pLeft;
    pivot->pLeft = a;
    int eq_a = a->h;
    int eq_p = pivot->h;
    a->h = eq_a - max(eq_p, 0) - 1;
    pivot->h = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));
    a = pivot;
    return a;
}

pArbre rotationDroite(pArbre a) {
    if (a == NULL || a->pLeft == NULL) {
        return a;
    }

    pArbre pivot = a->pLeft;
    a->pLeft = pivot->pRight;
    pivot->pRight = a;
    int eq_a = a->h;
    int eq_p = pivot->h;
    a->h = eq_a - min(eq_p, 0) + 1;
    pivot->h = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
    a = pivot;
    return a;
}

pArbre doubleRotationGauche(pArbre a) {
    if (a == NULL) {
        return a;
    }

    a->pRight = rotationDroite(a->pRight);
    a = rotationGauche(a);
    return a;
}

pArbre doubleRotationDroite(pArbre a) {
    if (a == NULL) {
        return a;
    }

    a->pLeft = rotationGauche(a->pLeft);
    a = rotationDroite(a);
    return a;
}

pArbre equilibrerAVL(pArbre a) {
    if (a == NULL) {
        return a;
    }

    if (a->h >= 2) {
        if (a->pRight != NULL && a->pRight->h >= 0) {
            return rotationGauche(a);
        } else {
            return doubleRotationGauche(a);
        }
    } else if (a->h <= -2) {
        if (a->pLeft != NULL && a->pLeft->h <= 0) {
            return rotationDroite(a);
        } else {
            return doubleRotationDroite(a);
        }
    }
    return a;
}

pArbre creerNoeud(int id, float diff, float max, float min, float moy) {
    Arbre *new = malloc(sizeof(*new));
    if (new != NULL) {
        new->id = id;
        new->max = max;
        new->min = min;
        new->moy = moy;
        new->diff = diff;
        new->h = 0; 
        new->pLeft = NULL;
        new->pRight = NULL;
    } else {
        fprintf(stderr, "Erreur d'allocation dynamique.\n");
    }
    return new;
}

pArbre insertionAVL(pArbre x, int id, float diff, float max, float min, float moy, int *h) {
    if (x == NULL) {
        *h = 1;
        return creerNoeud(id, diff, max, min, moy);
    } else if (diff < x->diff) {
        x->pLeft = insertionAVL(x->pLeft, id, diff, max, min, moy, h);
        *h = -*h;
    } else if (diff > x->diff) {
        x->pRight = insertionAVL(x->pRight, id, diff, max, min, moy, h);
    } else if (diff == 0) {
        return x;
    }

    if (*h != 0) {
        x->h = x->h + *h;
        x = equilibrerAVL(x);

        if (x->h == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }

    return x;
}

void extrairecol5(pArbre *a, FILE *fichier) {
    if (a == NULL || fichier == NULL) {
        fprintf(stderr, "Erreur, tous est à NULL\n");
        return;
    }

    char ligne[TAILLE_BUFFER];
    int h;
    int id;
    float mini, moy, maxi;
    int i = 0;

    while (pLeftets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, "\n");
        int colonne = 1;

        while (token != NULL) {
            char *token2 = strtok(token, ";");

            while (token2 != NULL) {
                if (colonne == 1) {
                    id = atoi(token2);
                } else if (colonne == 2) {
                    mini = atof(token2);
                } else if (colonne == 3) {
                    moy = atof(token2);
                } else if (colonne == 4) {
                    maxi = atof(token2);
                } else if (colonne == 5) {
                    float diff = atof(token2);
                    i++;
                    *a = insertionAVL(*a, id, diff, maxi, mini, moy, &h);
                }

                token2 = strtok(NULL, ";");
                colonne++;
            }

            token = strtok(NULL, "\n");
        }
    }
}

void parcoursDecroissant(pArbre a, FILE *fichierSortie, int *i, const int lim) {
    if (a == NULL || fichierSortie == NULL || i == NULL) {
        return;
    }

    if (*i <= lim) {
        parcoursDecroissant(a->pRight, fichierSortie, i, lim);
        if (*i <= lim) {
            fprintf(fichierSortie, "%d;%d;%f;%f;%f;%f\n", *i, a->id, a->min, a->moy, a->max, a->diff);
            (*i)++;
        }
        parcoursDecroissant(a->pLeft, fichierSortie, i, lim);
    }
}

void libererAVL(pArbre a) {
    if (a != NULL) {
        libererAVL(a->pLeft);
        libererAVL(a->pRight);
        free(a);
    }
}

int main() {
    FILE *fichier1, *fichier2;
    pArbre a = NULL;

    fichier1 = fopen("temp/s_filtre.csv", "r");
    if (fichier1 == NULL) {
        perror("Erreur lors de l'ouverture du fichier en lecture");
        return 1;
    }

    fichier2 = fopen("temp/fichier_traite_opt_s.csv", "w");
    if (fichier2 == NULL) {
        printf("Erreur lors de l'ouverture du fichier en écriture");
        fclose(fichier1);
        return 1;
    }

    extrairecol5(&a, fichier1);

    int compteur = 1;
    parcoursDecroissant(a, fichier2, &compteur, 50);

    fclose(fichier1);
    fclose(fichier2);

    libererAVL(a);

    return 0;
}