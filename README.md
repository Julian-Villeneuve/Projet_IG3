# Projet_IG3
Projet de l'UE IG3 en rendu en temps réel et animation.

## Installation

Toutes les dépendances sont déjà installées, il suffit de faire dans le dossier général une première fois :

```console
cmake
```
puis à chaque fois que l'on veut compiler le projet :

```console
cd build/
cmake ..
make
```
et enfin pour lancer le projet:

```console
../bin/IG_project
```

Pour rajouter des textures, c'est dans le dossier ```src/textures``` et les shaders dans le dossier ```src/shaders```, le cmake mettra tout cela dans le dossier bin.

Ce qui a été fait : 
- Charger et visualiser des objets/mesh
- Blinn-Phong
- Shadowmapping (malgré le soucis non corrigé du sol sur lequel il ne fonctionne pas)
- Un cylindre et une classe Bone pour l'animation, avec un calcul de poids 

Ce qui était dans mes objectifs et qui n'a pas été fait :
- Faire fonctionner les calculs de poids en affichant avec un dégradé de couleur entre deux Bones
- Animer ces Bones en pliant le cylindre
- Microfacettes
