/* Exercice 2 */

int board[1..7][1..6] = [[1, 0, 0, 1, 0, 0],[0, 0, 1, 0, 0, 1],[0, 1, 0, 0, 1, 0],[0, 1, 1, 0, 0, 0],[1, 1, 0, 0, 0, 0],[0, 0, 0, 0, 1, 1],[0, 0, 0, 1, 1, 0]];
int mat[1..7][1..6];

/* Construction de la matrice du jeu */
execute COMPUTE_MATRIX {
	for (var i = 1; i <= 7; i++) {
		  for (var j = 1; j <= 6; j++) {
		   	if (board[i][j] == 0) {
		   	  mat[i][j] = 1;
		   	}
		   	else {
		   	  mat[i][j] = -1;
		   	}
		  }
	}
}

/* Le jeu n'admet pas de valeur
 * v- = -1 et v+ = 1, v+ != v- donc le jeu n'admet pas de valeur v.
 */

int s_X[1..7];
int s_Y[1..6];

/* Calcule le poids de chaque stratégie */
execute PROCESS_STRATS {
  for (var i = 1; i <= 7; i++) {
    for (var j = 1; j <= 6; j++) {
      s_X[i] += mat[i][j];
      s_Y[j] -= mat[i][j];
    }
  }
}

/* Les stratégies optimales sont celles ayant les valeurs les plus élevées (respectivement s_X pour le joueur X et s_Y pour le joueur Y)
 * Ici toutes les stratégies sont optimales pour le joueur X, mais seules les stratégies 2 et 4 (Choisir une des deux case du centre) sont optimales pour le joueur Y.
 * Le joueur X est ici fortement avantagé, ayant un gain moyen positif dans tout les cas.
 */