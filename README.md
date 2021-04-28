# Bras_robot


Ce dépôt est le code source d'un projet mené en binôme. Le but est l'écriture d'une bibliothèque Arduino (C++) permettant de contrôler le bras robot Arduino Braccio.

Plusieurs dossiers sont présents :

temps_reel : le bras robot bouge en fonction des mouvements de l'accéléromètre
emotion : le bras bouge selon une ou plusieurs séquences (émotion) enregristrée(s)
Emission : envoie les données de l'accéléromètre sans fils depuis un Arduino nano vers l'Arduino uno de contrôle du bras robot
test_acce : fichier de test et de caractérisation de l'acceléromètre et du module sans fil