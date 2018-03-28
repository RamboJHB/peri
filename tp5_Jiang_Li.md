# Communication sans  fil

## Comportement de  fake_gateway

Un faux programme fake_gateway.c qui simule le comportement du vrai
gateway.c.

- Ecriture chaque seconde dans un fichier log_in (dans
*server-fake/server/www/*) d'une ligne ultilisant *fopen()*,
*fprintf()*, et *fflush()*.
- Ecriture :
- une date de la fonction *time()* .
- une valeur aléatoire séparées par un ' , ' ultilisant
*fprintf()* et *rand()*
- avec *fgets()* pour compter les numéros des lignes déja
existes (N). Donc N=execution_time
- Lecture sur une stream d'une valeur que fake_gateway affiche sur
le terminal et dans un fichier log_out ultilisant command line:
*$./gate_way > log_out*

## Comportement du  serveur

- Affichage sur le poste client d'une page (*main.py* ):
- représentant la courbe de la dernière 60 de valeurs reçues
avec **google area  chart**[]([https://developers.google.com/chart/interactive/docs/gallery/areachart])
- la programme lit toute la fichier *log_in* avec
*fileObject.readlines()* et alors *print* les valeurs dans la forme de
google charts arrayToDataTable.
- defini la range de boucle de (*len(str)-60 , len(str)*)
- les numéro des lignes correspent au temps d'éxecution (second)
- *main.py* envoie la valeur de ligne N vers fake_gateway sur fifo *s2f*
- gate_way recevoie la valeur avec *select()* et *read()* et
l'affichera sur le terminal
- Rafraichissement de la page toutes les 5 secondes.
- ajoute \<meta http-equiv="refresh" content="5"\> dans la html.
- donc chaque dans 5 seconds , gate_way affiche :
>from sever ---------gate_way execution time =N seconds

