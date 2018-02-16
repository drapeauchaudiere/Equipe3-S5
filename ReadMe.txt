Type de répertoire

Repository : Rassemblement de tous les fichiers de toutes les branches
	- Remote : Repository sur un serveur accessibles à tous 
	- Local : Repository personnel, accessible par vous sans connexion internet

Branch : Sous-projet qui permet de diviser les modifications et les versions pour ne 
	 pas faire les modifications directement/immediatement dans le projet principal

Working Directory : Rassemblement des fichiers de la branche actuellement modifiable


Instructions

Pull : Met à jour les fichiers du local repository avec ceux du remote repository

Fetch : Copie les fichiers du remote repository qui ne sont pas actuellement dans votre
	local repository. Les fichiers ne sont pas ajoutés à votre working directory, 
	mais seulement copiŽs dans votre local repository. C'est la première étape d'un 
	pull, la deuxième étant un merge qui permet de mettre à jour les fichiers de 
	votre working directory avec les modifications copiŽes par le fetch.

Commmit : Ajoute les modifications du working directory au local repository

Push : Met à jour les fichiers du remote repository avec la dernière sauvegarde dans 
	le local repository

Stash : Créer une copie des dossiers non commited, permet d'y revenir 

Branch : Créer un sous projet pour faire des modifications ou des versions sans affecter
	 directement ou immediatement le projet principal		

Checkout : Permet de charger les fichiers d'une autre branche dans le working directory

Merge : Applique les modifications d'une branche à une autre branche.  Créer souvent 
	des merge conflicts qui doivent être réglés avant de pouvoir continuer


Lien interessant de lecture pour Git : 
Guide de référence de Git :
https://git-scm.com/book/en/v2/Getting-Started-About-Version-Control
Survol des instructions Git : 
http://rogerdudler.github.io/git-guide/
Regard approndi des instructions Git :
https://longair.net/blog/2009/04/16/git-fetch-and-merge/