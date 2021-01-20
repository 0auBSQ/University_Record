## Exécution de cette partie du TP:
    Chaque fichier python contient un exercice, il suffit de lancer le fichier pour que toutes 
    les questions de l'exercice soient éxécutées.

Les explications du code.

## Deuxième partie: Etude et manipulation de lois de probabilité

### 2.1 Loi Binomiale
Dans cette première question on aborde la loi Binomiale.
Pour cela on part de x=0:100 et on utilise la fonction stats.binom.ppf de scipy

### 2.2 Loi Normale univariée
Dans cette seconde question on parle de la loi Normale univariée.
Toujours comme pour la première question on va utiliser une fonction de scipy (stats.norm.pdf)

### 2.3 Simulation de données à partir d'une loi
### 2.3.1 Cas de la loi Normale
Pour générer un échantillon de taille n selon la loi Normale on utilise une fonction (stats.norm.rvs)
Pour l'histogramme il suffit d'utiliser hist(comme pour matlab), on obtient bien une forme de cloche


### 2.4 Estimation de densité
### 2.4.1 Cas de la loi Normale
Pour générer l'échantillon on utilise la même fonction que pour la question précédente (stats.norm.rvs)
Pour afficher la vraie densité nous avons utilisé KernelDensity disponible avec
sklearn.neighbors, cette fonction permet d'obtenir la vraie densité.

### 2.4.2 Cas de la loi exponentielle
Pour générer l'échantillon on utilise la même fonction que pour la question précédente (stats.expon.rvs)
Pour afficher la vraie densité on utilise la même méthode que pour la question précédente.



## Troisième partie: Intervalles de confiance

### Problème 1:
Pour les deux premières questions on calcule simplement la moyenne empirique et on affiche l'histogramme
    des fréquences.
Pour cette 3e question on va utiliser les fonctions python suivante:
    "mean, sigma = np.mean(vals), np.std(vals)".
    ou np.mean() donne la moyenne et np.std() donne l'écart type, ces deux fonctions sont disponibles
    avec Numpy.
    Ensuite on utilise la fonction "interval" (disponible avec stats.norm) en lui donnant mean et
    sigma ainsi que le pourcentage (0.95 ou 0.99).
    On obtient ensuite l'intervalle de confiance.

### Problème 2:
Même chose que pour la question du Problème 1, on génere un échantillon suivant les données de la question.
Ensuite on calcule l'intervalle de comfiance de la même maniere que pour le Problème 1

### Problème 3:
Même procédé que pour les deux Problèmes précédent.