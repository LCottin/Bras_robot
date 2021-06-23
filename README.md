# Bras_robot


Ce dépôt est le code source d'un projet de deuxième année mené en binôme à l'ENSEIRB-MATMECA. Le but est l'écriture d'une bibliothèque Arduino (C++) permettant de contrôler le bras robot Arduino Braccio.

Plusieurs dossiers sont présents :
  - Controle : permet de controler le bras soit en temps réel (accéléromètre) soit avec des émotions (séquences enregistrées).
  - Emission : envoie les données de l'accéléromètre sans fils depuis un Arduino nano vers l'Arduino uno de contrôle du bras robot
      - 1e bracelet : ID = 1
      - 2e bracelet : ID = 2 
      - 3e bracelet : ID = 3
  - Reception : recupère les données envoyées par l'acceléromètre sur un Arduino nano avant de les transmettre au bras via une conversion analogique-numérique.
  - batterie : permet de savoir si la batterie est encore chargée ou non
  - Telecommande : pilote le bras à distance, offre un menu déroulant pour changer de mode et permet de mettre en pause/stop les mouvements
