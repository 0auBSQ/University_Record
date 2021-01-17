/* Exercice 3 */

float qx[1..3][1..3];
float mat[1..3][1..3];
float matb[1..3][1..3];
int durA;
int durB;

/* Résumer sur un tableau les gains par produit selon les stratégies définies en 1) */
execute RESUME_PRODUCTS {
  for (var i = 1; i <= 3; i++) {
    for (var j = 1; j <= 3; j++) {
    	if (i == 1)
    		durA = 12;
    	if (i == 2)
    		durA = 10;
    	if (i == 3)
    		durA = 19;
    	if (j == 1)
    		durB = 12;
    	if (j == 2)
    		durB = 9;
    	if (j == 3)
    		durB = 18;
    	if (durA == durB)
    		qx[i][j] = 0.08;
    	if (durA == durB + 1)
    		qx[i][j] = -0.04;
    	if (durA == durB + 3)
    		qx[i][j] = -0.10;
    	if (durA == durB + 7)
    		qx[i][j] = -0.12;
    	if (durA == durB + 10)
    		qx[i][j] = -0.14;
    	if (durA + 2 == durB)
    		qx[i][j] = 0.20;
    	if (durA + 6 == durB)
    		qx[i][j] = 0.30;
    	if (durA + 8 == durB)
    		qx[i][j] = 0.40;
  	}
  }
}

/* Formuler le problème comme un jeu à somme nulle dont on donnera la matrice des gains */
execute BUILD_MAT {
  for (var i = 1; i <= 3; i++) {
    for (var j = 1; j <= 3; j++) {
    	if (i == 1)
    		durA = 12;
    	if (i == 2)
    		durA = 19;
    	if (i == 3)
    		durA = 10;
    	if (j == 1)
    		durB = 12;
    	if (j == 2)
    		durB = 18;
    	if (j == 3)
    		durB = 9;
    	if (durA == durB)
    		mat[i][j] = qx[i][j] + 0.08;
    	if (durA == durB + 1)
    		mat[i][j] = qx[i][j] + -0.04;
    	if (durA == durB + 3)
    		mat[i][j] = qx[i][j] + -0.10;
    	if (durA == durB + 7)
    		mat[i][j] = qx[i][j] + -0.12;
    	if (durA == durB + 10)
    		mat[i][j] = qx[i][j] + -0.14;
    	if (durA + 2 == durB)
    		mat[i][j] = qx[i][j] + 0.20;
    	if (durA + 6 == durB)
    		mat[i][j] = qx[i][j] + 0.30;
    	if (durA + 8 == durB)
    		mat[i][j] = qx[i][j] + 0.40;
  	}
  }
}

/* On ajoute ici les gains/pertes de l'objet 2 (En additionnant les deux matrices) */

/* On résoud a nouveau le probleme avec Q1 = Q2/2 */
execute BUILD_MATB {
  for (var i = 1; i <= 3; i++) {
    for (var j = 1; j <= 3; j++) {
    	if (i == 1)
    		durA = 12;
    	if (i == 2)
    		durA = 19;
    	if (i == 3)
    		durA = 10;
    	if (j == 1)
    		durB = 12;
    	if (j == 2)
    		durB = 18;
    	if (j == 3)
    		durB = 9;
    	if (durA == durB)
    		matb[i][j] = (qx[i][j] / 2) + 0.08;
    	if (durA == durB + 1)
    		matb[i][j] = (qx[i][j] / 2) + -0.04;
    	if (durA == durB + 3)
    		matb[i][j] = (qx[i][j] / 2) + -0.10;
    	if (durA == durB + 7)
    		matb[i][j] = (qx[i][j] / 2) + -0.12;
    	if (durA == durB + 10)
    		matb[i][j] = (qx[i][j] / 2) + -0.14;
    	if (durA + 2 == durB)
    		matb[i][j] = (qx[i][j] / 2) + 0.20;
    	if (durA + 6 == durB)
    		matb[i][j] = (qx[i][j] / 2) + 0.30;
    	if (durA + 8 == durB)
    		matb[i][j] = (qx[i][j] / 2) + 0.40;
  	}
  }
}

