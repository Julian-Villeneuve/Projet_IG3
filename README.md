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

## Rapport

### Contexte

Je suis parti sur certaines bases du projet de M1 [BezierCurve]([docs/CONTRIBUTING.md](https://github.com/Julian-Villeneuve/BezierCurve)), bien que je n'aie au final pas utilisé une grosse partie de ce projet. Tout d'abord j'utilisais Visual Studio sous Windows, avec tous les builds automatisés et Qt. Alors que pour ce projet là, j'ai préféré être sous Linux et VsCode sans utiliser Qt avec lequel j'avais eu pas mal de difficultés. Je me disais que j'allais utiliser ImGui mais au final je n'en ai pas vu l'utilité puisque ce projet est plutôt simple.

Comme nous l'avons observé à la première séance de TP avec David Vanderhaeghe, je n'avais pas une très bonne base pour les objets et je n'avais pas non plus de lumières. Ce que j'ai réutilisé au final en n'ayant qu'à traduire les fonctions Qt, c'était une petite partie du programme principal et la caméra. Ou un autre exemple avec la classe Mesh et la classe Shader que j'avais codées et que vous pouvez aller consulter avec le lien plus haut, j'ai totalement pris les classes Mesh et Shader de LearnOpenGL puisque je pense que c'était ici qu'était le problème que j'avais l'an dernier, mais que je n'ai pas vraiment réussi à situer (je pense plutôt à Mesh, j'aurais pu reprendre ma classe Shader mais sait-on jamais je préfère être sûr de ma base)

- Le premier objectif était donc de traduire la partie de code que j'allais réutiliser puis faire en sorte d'afficher des objets. 
- Cette étape n'était pas précisée puisqu'évidente pour la suite mais je la dissocie tout de même : des lights.
- Au début j'hésitais entre du HDR+Bloom et du Shadowmapping, mais en voyant que tout le monde faisait le premier, je me suis dit que j'allais replonger dans les cours de M1 avec du shadowmapping
- Les microfacettes
- Animer un petit cylindre (classe Bone + calcul des poids + animer l'articulation du milieu)

### Lights

Après avoir fini de créer une bonne base, j'ai fait une scène toute simple avec 5 cubes et un tout petit cube qui servira de lumière par la suite :

![cubes_sans_light](https://user-images.githubusercontent.com/59332180/225334577-e1b515dd-150d-4e45-96e2-0541b5e0c6cc.png)

En affichant juste les polygones puisqu'on ne voit pas grand chose sans lumière :

![cubes_sans_light_polygon](https://user-images.githubusercontent.com/59332180/225334642-668b7286-73f2-489d-a5da-b60d8a8cd268.png)

En ajoutant à cela la méthode de Phong (plus tard Blinn-Phong) pour éclairer cette scène depuis le petit cube de lumière, cela donne :

![cubes_light_specular](https://user-images.githubusercontent.com/59332180/225337052-21ed3511-496a-4a2f-aa99-b393c61268be.png)

A noter la position du cube qui est différent puisque je le fais tourner sur deux axes autour/au milieu des cubes pour vraiment voir les différentes positions sans avoir à modifier à la main pour les tests, ce qui m'avais très vite lassé.

### Shadowmapping

Avant de commencer le shadowmapping, je me suis dit qu'il fallait ajouter un sol comme dans le tuto de LearnOpenGL pour voir au mieux les ombres s'étendre plutôt que d'observer les ombres juste entre les cubes. Ironique au vu du bug que j'ai vite eu tout au long du développement du shadowmapping, et qui persiste jusqu'au rendu du projet. Le sol ne prend tout simplement pas en compte le shadowmapping, les ombres ne sont pas affichées dessus. J'ai essayé plusieurs méthodes pour régler cela. Les coordonnées de textures sont bonnes, la texture elle même aussi (j'ai fini par prendre celle de LearnOpenGL), 

Petite vidéo pour montrer le bug de texture au sol, qui ne prend pas en compte le shadowmapping : 
https://youtu.be/o3bEpthMxlM

### Animation

## Conclusion

## TLDR

Ce qui a été fait : 
- Charger et visualiser des objets/mesh
- Blinn-Phong
- Shadowmapping (malgré le soucis non corrigé du sol sur lequel il ne fonctionne pas)
- Un cylindre et une classe Bone pour l'animation, avec un calcul de poids 

Ce qui était dans mes objectifs et qui n'a pas été fait :
- Faire fonctionner les calculs de poids en affichant avec un dégradé de couleur entre deux Bones
- Animer ces Bones en pliant le cylindre
- Microfacettes
