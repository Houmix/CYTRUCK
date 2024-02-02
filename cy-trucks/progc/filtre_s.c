#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096  

struct Stat {
    int id;
    float min;
    float max;
    float somme;
    int count;
    float moyenne;
    float difference;
};

int main() {
    FILE *input = fopen("temp/s_intermediaire_calcul.csv", "r");
    if (input == NULL) { 
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée s_intermediaire_calcul.csv\n");
        return 1;
    }

    FILE *output = fopen("temp/s_filtre.csv", "w+");
    if (output == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sortie s_filtre.csv\n");
        fclose(input);
        return 1;
    }

    // Variables pour gérer dynamiquement la mémoire
    int max_id = -1;
    struct Stat *s = NULL;

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input)) {
        int id; 
        float value;
        if (sscanf(line, "%d;%f", &id, &value) != 2) {
            fprintf(stderr, "Erreur lors de la lecture de la ligne : %s", line);
            continue; 
        }

        // Vérifier si l'identifiant est dans la plage attendue
        if (id >= 0) {
            // Mettre à jour le plus grand identifiant rencontré
            if (id > max_id) {
                max_id = id;
            }

            // Réallouer dynamiquement la mémoire si nécessaire
            s = realloc(s, (max_id + 1) * sizeof(struct Stat));

            // Initialiser à zéro si l'élément est realloué
            if (s[id].count == 0) {
                s[id].id = id;
                s[id].min = value;
                s[id].max = value;
                s[id].somme = value;
                s[id].count = 1;
            } else {
                // Mettre à jour les s
                if (value < s[id].min) {
                    s[id].min = value;
                }
                if (value > s[id].max) {
                    s[id].max = value;
                }
                s[id].somme += value;
                s[id].count++;
            }
        }
    }

    for (int i = 0; i <= max_id; i++) {
        if (s[i].count > 0) {
            s[i].moyenne = (float)s[i].somme / s[i].count;
            s[i].difference = (float)s[i].max - (float)s[i].min;
            fprintf(output, "%d;%.3f;%.2f;%.3f;%.3f\n", s[i].id, s[i].min, s[i].moyenne, s[i].max, s[i].difference);
        }
    }

    free(s);



    fclose(input);
    fclose(output);

    if (remove("temp/s_intermediaire_calcul.csv") != 0) {
        printf("Erreur lors de la suppression du fichier %s\n", "temp/s_intermediaire_calcul.csv");
        return 1;
    }
    printf("\nLe fichier %s a été supprimé avec succès.\n", "temp/s_intermediaire_calcul.csv");
   

    return 0;
}