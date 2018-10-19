# ARM

## Dépendances

- OpenGL 3.3
- Qt 5.7

## Compiler le projet

```sh
mkdir build
cd build
qmake -qt=5 ../qmakefile
make
./arm <model>
```

L'exécutable prend en paramètre le fichier de modèle à charger. Les
formats supportés sont PGM3D et OBJ. Deux fichiers d'exemple sont
disponible dans le dossier data (`shep_logan.pgm3d` et `cube.obj`)

## Problèmes connus

- Le projet a l'air de ne fonctionner uniquement sur cartes graphiques
NVidia. Après recherches, ce problème est lié à un context OpenGL
incorrect du à un bug de Qt. Nous n'avons pas trouvé de solution.

- Sur certains PC (notamment les PC du CREMI), la lecture des coordonnées
dans les fichiers est incorrecte, nous n'avons pas réussi à déterminer
l'origine du problème. La conversion du nombre lu tronque les décimales.
