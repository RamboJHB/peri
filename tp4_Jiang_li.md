# TP4 : Serveur WEB minimaliste pour la commande des LEDs et la lecture du bouton poussoir
Jiang	3602103
Li 		3770906

### 1. Communication par FIFO
**Dans quel répertoire est créée la fifo ?**
Ils sont créés dans /tmp(temporary directory) par open() ou mkfifo.
**Quelle différence mkfifo et open ?**


|open|mkfifo|
|--|--|
| open or create a file for R/W  |create a new fifo file(pipe)|
| return a file descriptor if succeed|  return 0 if succeed|
| for multiple type of files  |  just for making named pipe|
| int open(const char *path, int oflag, ...); |   int mkfifo(const char *path, mode_t mode);|


**Pourquoi tester que la fifo existe ?**

Si le nom de fifo déjà existe:

C : mkfifo() génère l'erreur : [EEXIST]The named file exists.

Python : os.mkfifo() génère : OSError : [Errno 17] File exists.

**À quoi sert flush ?**

The function flush forces a write of all buffered data for the given output or update stream via the stream's underlying write function.  The open status of the stream is unaffected. 

 **Pourquoi ne ferme-t-on pas la fifo ?**
 
 La taille de un fifo est toujours 0B. Et l'OS vide le /tmp de temps en temps automatiquement.
 
**Que fait readline() ?**

**fileObject.readline( size )** : Il est dans blibliothèque python OS. Sa fonction est de rendre une ligne de fichier.( termine à '\n' ). Donc quand on doit envoyer un '\n' pour chaque fois.


**Pour quoi  lorsque le vous lancer un écrivain (en C ou en Pyhton) rien ne se passe tant que vous n'avez pas lancé un lecteur?**

En c , on execute writer , et alors reader. Parce que quand on lance write le process block au **second** write() , le fifo est plein à ce moment. Apres on lance reader , read() vide le fifo ,et les 2 process continuent.


**P.S.** On modifie write.c comme ci-dessous:

```C
//write(fd, "end\n", sizeof("end")); //change to
write(fd, "end\n", sizeof("end\n"));
```

## 2. Création d'un serveur fake
### 2.1. Client fake.c
**La fonction select()**
4 macros sont fourni pour manipuler les sets. 
|FD_ZERO()  | FD_SET() / FD_CLR() | FD_ISSET() |
|--|--|--|
| clears  aset | add/remove a given file descriptor from a set |test if a file descriptor is part of the set (after select)|

### 2.2. server server.py
```C
while (str != 'end\n') : 
    str = f2s.readline()
    print '%s' % str,
    f2s.flush()
```

## 3. Création d'un serveur web
### 3.1. Connection entre Raspberry & Navigateur Web
On ajoute 132.227.107.36 dans la tableau de LocalHost. L'IP http://132.227.102.36:8022/  ( porte 8022 ) est redirigé pour accéder à la raspberry carde 22 par le routeur.
### 3.2. Client fake.c
On modifie le fake.c pour le execute sur Raspberry et controller les LEDs et le bouton. On ajoute :
  - GPIO operations : pour configurer les branches et mapping ( comme lcd_user.c ).
  - fonction différent par la valeur valeur reçue ver le Web : 
    * 0 : tous éteint
    * 1 : allumer LED1
    * 2 : allumer LED2
    * 3 : allumer les 2
 - lire le valeur de bouton et l'envoyer à le server.
### 3.3. Serveur HTTP/CGI
**Common Gateway Interface**
Au lieu d'envoyer le contenu d'un fichier (fichier HTML, image), le serveur HTTP exécute un programme , puis retourne le contenu généré.
On écrite le server en python , et dedans il faut importer les blibliothèques :


| BaseHTTPServer |CGIHTTPServer  |cgitb|
|--|--|--|
| serveur sur un port défini à la création | gestionnaire pour l'exécution des cgi |les erreurs des scripts cgi|


```python
cgitb.enable()
server = BaseHTTPServer.HTTPServer #Création du serveur HTTP 
handler = CGIHTTPServer.CGIHTTPRequestHandler #Création du Gestionnaire de CGI

server_address = ("", 8000) #l'adresse ethernet et du port d'écoute
handler.cgi_directories = ["/cgi"] #répertoire contenant les scripts CGI

httpd = server(server_address, handler)
httpd.serve_forever()
```
Et on modifie le led.py pour affichage de la valeur de bouton en html.
