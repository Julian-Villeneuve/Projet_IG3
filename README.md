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
![cubes_texture](https://user-images.githubusercontent.com/59332180/225344420-dccb86e5-715a-49ed-90c9-86b2cfd742aa.png)
![cubes_texture2](https://user-images.githubusercontent.com/59332180/225344436-1470cc0b-27f2-404a-846c-757366ee5612.png)

Avant de commencer le shadowmapping, je me suis dit qu'il fallait ajouter un sol comme dans le tuto de LearnOpenGL pour voir au mieux les ombres s'étendre plutôt que d'observer les ombres juste entre les cubes. Ironique au vu du bug que j'ai vite eu tout au long du développement du shadowmapping, et qui persiste jusqu'au rendu du projet. Le sol ne prend tout simplement pas en compte le shadowmapping, les ombres ne sont pas affichées dessus. J'ai essayé plusieurs méthodes pour régler cela. Les coordonnées de textures sont bonnes, la texture elle même aussi (j'ai fini par prendre celle de LearnOpenGL), tout ce qui est en rapport avec la depthmap et les calculs dans les shaders aussi puisque cela fonctionne sur les cubes. Je pense aussi que le sol est bien codé dans la boucle de rendering, donc je ne vois pas d'où vient le soucis et j'ai décidé de laisser cela de côté pour faire l'animation puisqu'à ce stade, il ne restait qu'une semaine à partir du cours sur le skinning pour coder ce dernier. J'ai même copié collé certaines partie du tuto LearnOpenGL pour écraser des parties de mon code qui auraient selon moi pu provoquer ce bug, mais rien n'y fait je n'ai pas réussi à débug cela.

Petite vidéo pour montrer mon shadowmapping mais surtout le bug de texture au sol, qui ne prend pas en compte les ombres : 
https://youtu.be/o3bEpthMxlM

### Animation

Depuis le début je voulais faire du skinning donc à partir du cours que l'on a eu mercredi 8 mars, je n'ai travaillé que sur cela dans ce projet en parallèle des projets des autres UEs.

Dans un premier temps, j'ai codé un boudin (cylindre) que j'ai placé dans ma scène puisque je me suis dit que plutôt de tout séparé, autant les mettre ensemble pour avoir un beau shadowmapping sur un boudin animé.

J'ai pour cela suivi les étapes décrites par Loïc Barthe, notamment sur Moodle que voici :

    - définir une classe Bone contenant une transformation
    - ajouter les données de poids au Vertex
    - calculer les poids pour chaque Vertex
    - définir 2 Bones succéssifs dont les articlulations se trouvent respectivement à la base et au milieu du cylindre
    - animer l'articulation du milieu (vous pouvez ajouter des primitives visuelles pour afficher avec deux segments les deux bones)
    - ajouter au maillage une donnée "pose de référence"
    - animer les points du maillage en CPU (et mettre à jour les données sur le GPU)
    - effectuer le même calcul sur GPU (en envoyant les poids et les matrices de transformation au shader).

Les 4 premières étapes ont été faites mais puisque l'animation ne marche pas rien de visuel ne peut être montré donc c'est dans le code directement qu'il faut regarder mon travail.

Je pense que le problème est situé dans mon calcul de poids, en modifiant les fonctions aux noms explicites createBoudinMesh et drawBoudinMesh afin de prendre en compte les poids, je n'ai réussi qu'à casser mon boudin et faire n'importe quoi. Avec le manque de temps au final et mon incompréhension j'ai juste fait un rollback pour avoir mes boudins sans les poids (donc même si je décidais de les afficher avec des dégradés de couleurs comme le fait Loïc Barthe sur Moodle cela ne fonctionnerait pas).

Voilà ci-dessous le boudin sur lequel marche le shadowmapping mais qui ne peut pas être animé.

![cubes_et_boudin](https://user-images.githubusercontent.com/59332180/225348434-b53c5d63-a91e-41eb-a1f9-a0dec8d90c24.png)
![cubes_et_boudin2](https://user-images.githubusercontent.com/59332180/225348443-c15ecd6e-0fad-45fe-8f97-0774219f985b.png)

Pour les microfacettes j'ai tenté de les faire sans réellement me plonger sur les matériaux, HDR, Bloom, etc. Et visiblement j'aurais dû puisque je n'arrivais pas à les faire fonctionner, je les ai simplement supprimées du projet. J'avais toujours des résultats décevant, par exemple ce genre de shader:

```glsl
// Distribution de microfacettes
float D(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ajout des microfacettes
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 H = normalize(lightDir + viewDir);
    float microfacet = D(norm, H, roughness);
    float NdotH = max(dot(norm, H), 0.0);
    float F = F0 + (1.0 - F0) * pow(1.0 - NdotH, 5.0);
    vec3 specular = (microfacet * vec3(F)) / (4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, lightDir), 0.0));

    vec3 finalLight = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(finalLight, 1.0);
}
```
Donnait ce genre de résultats où l'on dirait que plus rien n'est pris en compte à part la lumière ambiante et la position du cube lumineux (enlevant carrément la couleur de tout objet ne faisant pas face à la lumière) :


![capture microfacettes nulles 2](https://user-images.githubusercontent.com/59332180/225353874-56106b2c-fb77-486c-ac15-4c9f71c02c21.png)

Cela ne sert pas à grand chose mais je souhaitais juste mentionner la partie microfacettes qui étaient le premier objectif et le seul imposé, juste pour mentionner le fait que je n'ai pas ignoré cela j'ai juste passé pas mal de temps pour au final ne pas y arriver.


## Conclusion

Sur tous les objectifs que je m'étais fixés, deux n'ont pas marché (microfacettes et animation) et un a un bug (shadowmapping) donc je suis globalement déçu du résultat et de ce que je peux montrer. Je suis cependant très satisfait de ce que j'ai appris, il n'y a que la partie sur les framebuffer que je n'ai pas totalement compris ainsi que les microfacettes où j'ai beaucoup pataugé. Tout le reste a bien été assimilé selon moi et la liberté offerte par ce projet m'a permi d'explorer ce qui me plaisait selon mes envies sur le moment.

Comme souvent je suis aussi très déçu de ma gestion du temps, je me suis mis assez vite sur ce projet en sachant que j'allais manquer de temps au mois de mars avec les projets d'IC et VO (ainsi que le Chef-d'oeuvre), et pourtant j'ai un peu trop pris mon temps et trop souvent dévié sur ce que je voulais faire au début. Je dirais que la partie rendu m'a pris 30-35h (même si les résulats n'en donnent pas cette impression) et autour de seulement 7-8h pour la partie animation au moment où j'étais débordé par les fin de projets et oraux à préparer, pour cette partie j'aurais dû m'y mettre bien avant le cours de skinning, et étudier cela de mon côté.

Selon moi j'ai réussi environ 60% de mes objectifs. Mais sur les 40% restant il y a une petite partie que j'ai tout de même compris sur le papier. Par exemple j'avais étudié les BRDF au cours du projet Mitsuba au semestre dernier, et là en travaillant un peu sur du PBR et microfacettes, j'ai pu compléter certaines choses que je n'avais pas compris auparavant. Ou au contraire lier des choses que j'avais appris avec d'autres notions de cours.



## TLDR

Ce qui a été fait : 
- Traduire/adapter une partie du projet M1
- Charger et visualiser des objets/mesh correctement
- Blinn-Phong
- Shadowmapping (malgré le soucis non corrigé du sol sur lequel il ne fonctionne pas)
- Un cylindre et une classe Bone pour l'animation, avec un calcul de poids 

Ce qui était dans mes objectifs et qui n'a pas été fait :
- Faire fonctionner les calculs de poids en affichant avec un dégradé de couleur entre deux Bones
- Animer ces Bones en pliant le cylindre
- Microfacettes
