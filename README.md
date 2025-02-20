# Grey Wolf Optimizer (GWO) - README

## Description

Ce projet implémente l'algorithme Grey Wolf Optimizer (GWO) en C++. L'algorithme est basé sur le comportement de chasse des loups gris et est utilisé pour résoudre des problèmes d'optimisation, en particulier pour des fonctions à plusieurs dimensions.

## Prérequis

Avant d'exécuter ce code, il est nécessaire de s'assurer que les éléments suivants sont installés sur le système :

1. **Compilateur C++ compatible avec la norme C++20 :** Ce projet a été développé en utilisant la norme C++20. Il est donc essentiel d'utiliser un compilateur compatible, tel que **g++ version 10** ou supérieur, ou un compilateur équivalent pour Windows (MinGW ou MSVC).

2. **CMake :** Il est nécessaire d'avoir CMake installé. CMake est un outil de gestion de la compilation qui permet de configurer et de compiler le code de manière portable.

3. **Terminal ou IDE :** Le code peut être exécuté soit via la ligne de commande, soit à l'aide d'un IDE tel que **CLion**, **Visual Studio Code** ou un autre IDE compatible avec CMake.

## Installation et Exécution

### Exécution sans CMake (Compilation manuelle avec g++)

Il est possible de compiler et exécuter le programme manuellement avec g++. Voici les étapes nécessaires :

1. Ouvrir un terminal et naviguer jusqu'au dossier contenant les fichiers du projet (le dossier GWO_C).

2. Compiler le code avec la commande g++ en utilisant la norme C++20 :

    ```bash
    g++ -std=c++20 -o gwo_optimizer main.cpp utils/Agent.cpp utils/Problem.cpp OriginalGWO.cpp
    ```

   Cette commande génère un fichier exécutable nommé **gwo_optimizer**.

3. Pour exécuter le programme, il suffit de saisir la commande suivante dans le terminal :

    ```bash
    ./gwo_optimizer
    ```

   Cette commande lancera le programme et effectuera l'optimisation.

### Exécution avec CMake

Il est aussi possible d'exécuter avec CMake, voici les étapes à suivre pour compiler et exécuter le programme :

1. Ouvrir un terminal et se rendre dans le dossier contenant les fichiers du projet (dossier GWO_C).

2. Créer un répertoire de build pour la compilation :

    ```bash
    mkdir build
    cd build
    ```

3. Utiliser **CMake** pour configurer le projet et générer les fichiers nécessaires à la compilation :

    ```bash
    cmake ..
    ```

   Cela génère les fichiers de construction pour **g++** (ou un autre compilateur compatible) à partir du fichier **CMakeLists.txt**.

4. Compiler le projet avec **make** :

    ```bash
    make
    ```

   Cette étape crée l'exécutable **GWO_C**.

5. Une fois la compilation terminée, il est possible d'exécuter le programme en utilisant la commande suivante :

    ```bash
    ./GWO_C
    ```

   Cela lancera le programme et effectuera l'optimisation.

### Exécution avec un IDE (par exemple, CLion)

Si un IDE comme CLion est utilisé, voici la procédure pour exécuter le programme avec CMake :

1. Ouvrir le projet dans CLion :
    * Lancer CLion.
    * Sélectionner **Open** et choisir le dossier contenant le code source (le dossier où se trouve le fichier **CMakeLists.txt**).

2. Configurer CMake dans CLion :
    * CLion détectera automatiquement le fichier **CMakeLists.txt** et proposera de configurer le projet.
    * Il est important de s'assurer que CMake et un compilateur compatible C++20 (comme **g++ version 10** ou supérieur) sont correctement configurés dans les paramètres de CLion.

3. Compiler et exécuter dans CLion :
    * Une fois la configuration terminée, il est possible de cliquer sur le bouton **Run** dans CLion pour compiler et exécuter le programme.
    * **CLion** s'occupera de la gestion de la compilation via CMake et lancera l'exécution automatiquement.

### Paramètres de Configuration

Le programme utilise une population de loups et un problème d'optimisation par défaut. Il est possible de modifier ces paramètres dans le code source, notamment :

* **La taille de la population** (`pop_size`).
* **Le nombre d'itérations** (`epoch`).
* Les détails du problème d'optimisation dans les classes **Problem** et **Agent**.

## Fonctionnalités

* **Évolution de la population :** Le programme met à jour la population des agents (loups) à chaque itération en fonction de leur position et de leur fitness, en utilisant l'algorithme GWO.
* **Sélection des meilleurs agents :** Les meilleurs agents (Alpha, Beta, Delta) influencent les autres agents (Omega) pour trouver la solution optimale.
* **Optimisation :** L'algorithme optimise une fonction cible définie dans la classe **Problem**.

## Contribution

Si des améliorations ou des suggestions sont proposées, il est possible de contacter l'auteur ou de soumettre des modifications.

Auteur : **SEBO Amil**  
Date : **20 février 2025**
