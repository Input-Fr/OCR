#include <stdio.h>
#include <string.h>

short sudoku[9][9] =
        {
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
        };

int possible(short n, size_t x, size_t y){
    for (size_t val = 0; val < 9; val+=1){
        //test sur la ligne
        if (sudoku[val][y] == n){
            return 0;
        }
        //test sur la colonne
        if (sudoku[x][val] == n){
            return 0;
        }
    }
    for (size_t Y = 0; Y < 3; Y += 1){
//calcul de la position de Ro et Co les premières valeurs
//dans le petit carré trois*trois dans laquelle
        //la valeur n se trouve
        size_t Ro = 6;
        size_t Co = 6;
        if(x < 3)
            Ro = 0;
        else
        if(x < 6)
            Ro = 3;
        if(y < 3)
            Co = 0;
        else
        if(y < 6)
            Co = 3;

        for(size_t X = 0; X < 3; X+=1){
            //test sur le carre de trois
            if (sudoku[Ro+Y][Co+X] == n){
                return 0;
            }
        }
    }
    return 1;
}



int solve(){
    for(size_t i = 0; i < 9; i++){       //parcour toutes les cases du sudoku
        for(size_t j = 0; j < 9; j++){

            if(sudoku[i][j] == 0){
                for(short num = 1; num <= 9; num++){
                    if (possible(num,i,j)){       //test de la case avec le chiffre num
                        sudoku[i][j] = num;
                        if(solve()){
                            return 1; //appel recursif, si le sudoku est resolu
                            //alors le num est bon sinon le num n'est pas bon
                        }
                        sudoku[i][j] = 0; //la valeur ne marche pas donc
                        // on reniatilise a zero et on continue avec les autres nums
                    }
                }
                return 0;
            }

        }
    }
    return 1;
}


void read(const char *file) {
    FILE *fichier = fopen(file, "r");
    if (fichier == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    int caractere;
    size_t i = 0;
    size_t j = 0;
    while ((caractere = fgetc(fichier)) != EOF) {
        if(i == 9){
            i = -1;
            j+=1;
        }
        if(caractere != ' '){
            if(caractere >= '1' && caractere <= '9'){
                sudoku[j][i] = caractere - 48;    //on converti les valeurs char en int
            }
            i++;
        }
    }
    fclose(fichier);
}

void write(char *file){
    char message[] = ".result";
    char resultat[20];
    strcpy(resultat, file);
    strcat(resultat, message);     //creer un fichier avec le nom du fichier original + .result
    FILE *nouveauFichier = fopen(resultat, "w");
    if (nouveauFichier == NULL) {
        return;
    }

    for (size_t row = 0; row < 9; row++)
    {
        for (size_t col = 0; col < 9; col++)
        {
            if(col % 3 == 0 && col != 0){
                fprintf(nouveauFichier," ");
            }
            fprintf(nouveauFichier,"%d", sudoku[row][col]); //print les valeurs
        }
        if((row + 1) % 3 == 0){
            fprintf(nouveauFichier,"\n");
        }
        if(row!= 8)
            fprintf(nouveauFichier,"\n");
    }
    fclose(nouveauFichier);
}

int main(int argc, char * argv[])
{
    if (argc != 2) {
        return 1;
    }

    read(argv[1]);  	//lire le fichier avec la grile la pour remplir dans la matrice

    int a = solve();  	//on resoud la grille

    if(!a){     		//on verifie que la grille a ete resolu
        printf("sudoku non valide");
        return 1;
    }

    write(argv[1]);     	//sauvegarde de la grille dans un nouveau fichier

    return 0;
}
