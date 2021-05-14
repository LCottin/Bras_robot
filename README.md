# Bras_robot


Ce dépôt est le code source d'un projet de deuxième année mené en binôme à l'ENSEIRB-MATMECA. Le but est l'écriture d'une bibliothèque Arduino (C++) permettant de contrôler le bras robot Arduino Braccio.

Plusieurs dossiers sont présents :
  - temps_reel : le bras robot bouge en fonction des mouvements de l'accéléromètre.
  - emotion : le bras bouge selon une ou plusieurs séquences (émotion) enregristrée(s).
  - Emission : envoie les données de l'accéléromètre sans fils depuis un Arduino nano vers l'Arduino uno de contrôle du bras robot, 1e bracelet.
  - Emission2 : envoie les données de l'accéléromètre sans fils depuis un Arduino nano vers l'Arduino uno de contrôle du bras robot, 2e bracelet.
  - test_acce : fichier de test et de caractérisation de l'acceléromètre et du module sans fil.
  - Reception : recupère les données envoyées par l'acceléromètre sur un Arduino nano avant de les transmettre au bras via une conversion analogique-numérique.

Il est à noter que les fichiers Braccio.h et Braccio.cpp ne sont pas synchronisés entre les dossiers : toutes modifications de ces fichiers dans l'un des deux dossiers doit également être faite dans l'autre dossier !