# Algorithmes de regression linéaire

(This README is in french, since it's an university project)

## 1-1-1 : Moindre carrés - Modèle scalaire

- Le calcul des coefficients de régression est représenté par ces lignes :

```
# Compute (a)x + b
theta[0] = numpy.cov(x, y, ddof = 0)[0][1] / numpy.var(x)
# Compute ax + (b)
theta[1] = statistics.mean(y) - theta[0] * statistics.mean(x)
print(theta)
```

thata[0] représente Beta1 et theta[1] Beta0 (Les indices ont été inversés lors de mes calculs).

La formule des moindres carrés ici est :
- Beta1 = Covariance(X, Y) / Variance(X)
- Beta0 = Moyenne(Y) - (Beta1 * Moyenne(X))

- Les données et la droite de régression sont représentées graphiquement utilisant le module matplotlib.

- Les résultats obtenus via la fonction polyfit de numpy (Python) sont équivalents.

## 1-1-2 : Moindre carrés - Modèle vectoriel

- Le calcul des coefficients de régression (vecteur Beta) est représenté par ces lignes :

```
# Compute the matrix formula (A.T * A)^-1 * A.T * y to get the Beta (here called theta) values
X = numpy.array([numpy.ones(len(x)), x])
Y = numpy.array([y])
theta = numpy.linalg.pinv(X.T.dot(X)).dot(X.T).T.dot(Y.T)
print(theta)
```

Cette fois ci les indices sont dans le bon ordre.

La formule utilisée est celle donnée dans le sujet, c'est à dire :
- Beta = (A.T * A)^-1 * A.T * y

Avec A matrice de regression (nommée X dans le code) auquel à été apposée une ligne supplémentaire composée de 1.

- Les données et la droite de régression sont représentées graphiquement utilisant le module matplotlib.

- Il n'est pas possible de tester la normalité des erreurs en utilisant qqplot de Matlab car la licence du logiciel ne nous est pas fournie.

- Les résultats obtenus via la fonction polyfit de numpy (Python) sont équivalents.

## 1-2 : Descente de gradiant

- Pour cet exercice j'ai repris le code d'un de mes anciens programmes sur mon Github (https://github.com/0auBSQ/ft_linear_regression-)

- Les données sont normalisées en entrée, et "dénormalisées" à chaque update du graphique, afin de pouvoir conserver le même taux d'apprentissage peu importe le jeu de données.

- Les valeurs Beta sont, encore une fois représentées par "theta" dans le programme.

- Les valeurs sont similaires, avec un petit taux d'erreur pour le programme utilisant la méthode des gradiants (L'erreur est minimisée à chaque étape, mais jamais nulle)

- Les données et la droite de régression sont représentées graphiquement utilisant le module matplotlib.