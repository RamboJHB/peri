# Projet : Réseau de capteurs minimaliste [¶](https://www-asim.lip6.fr/trac/sesi-peri/wiki/SujetTP7-2018#Projet:R%C3%A9seaudecapteursminimaliste "Link to this section")

Compte Rendu PERI TP7
Jiang 3602103 & Li 3770906


## 1. L'objectif du projet 
L'objectif du projet est illustré par le dessin ci-dessous. 

[![2015_obj.png](https://www-asim.lip6.fr/trac/sesi-peri/chrome/site/png/2015_obj.png "2015_obj.png")](https://www-asim.lip6.fr/trac/sesi-peri/chrome/site/png/2015_obj.png)

## 2. Comportement d'Arduino( arduino_raspb_RF24.ino )
A l'Arduino, il y a 2 tâche qui communiquent par un _struct MailBox_  :
**loop_cap** : Lit la valeur de capteur de lumière et met à jour le _MailBox_ chaque 0.1 second. (Il compose un _Timer_)
**loop_send_dis** : 
-  affichage les valeur sur l'OLED et  le message reçoit de la Raspberry. ( 2 forme de texte )
-  envoie la valeur à la Raspberry chaque fois il y a un nouvelle valeur. 



## 3. Comportement de Raspberry ( NRF24L01_base.cpp )

### 3.1.  Récepteur
- Establir une base de données avec la bibliothèque de SQLite en C, il fault ajout un le bibliothèque **statique** dans la Cross-Compilator à cause de absence de libSQLite dans la carte. Donc l'écriture de chaque valeur reçoit est dans un fichier log_in dans forme de **csv** (dans répertoire : *_server-fake/server/www/_* )  ultilisant *_fopen()_*, *_fprintf()_*, et *_fflush()_*.
- Créer une date stamp avec la fonction *_time()_* .
- Il y a une autre fichier pour tester la communication entre la Raspberry et l'Arduino. Il envoie un message vers l'Arduino.
- Il y a une partie de code _select()_ qui  peut recevoir message de fifo _s2f_. On les commente par ce que c'est la seconde fonction bloquante dans ce processus. 


###  3.2. Serveur HTTP/CGI
****3.2.1. Affichage de données****

Affichage sur le poste client d'une page (*_main.py_* ):

- représentant la courbe de la dernière 360 seconds de valeurs reçues avec ****google area chart****[]([https://developers.google.com/chart/interactive/docs/gallery/areachart]) 
-  Affiche la dernière valeur reçoit avec **google gauge chart**[]([https://developers.google.com/chart/interactive/docs/gallery/gauge]) dans la même page.
- la programme lit toute la fichier *_log_in_* avec *_fileObject.readlines()_* et alors *_print_* les valeurs dans la forme de google charts arrayToDataTable.
- constructeur de javascript **New Date( time stamp )** pour créer l'axe des abscisses de temps.
- defini la range de boucle de (*_len(str)-360 , len(str)_*)
- *_main.py_* envoie la nombre de valeur reçoit vers NRF24L01_base sur un fifo *_s2f_*
- Rafraichissement de la page toutes les 5 secondes.
ajoute \<meta http-equiv="refresh" content="5"\> dans la html.

****3.2.2. Common Gateway Interface****

Au lieu d'envoyer le contenu d'un fichier (fichier HTML, image), le serveur HTTP exécute un programme , puis retourne le contenu généré.
On écrite le server en python , et dedans il faut importer les blibliothèques :




| BaseHTTPServer |CGIHTTPServer  |cgitb|
|--|--|--|
| serveur sur un port défini à la création | gestionnaire pour l'exécution des cgi |les erreurs des scripts cgi|



Trouvez ci-joints les fichiers
