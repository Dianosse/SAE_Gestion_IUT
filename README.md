# SAE_Gestion_IUT

Ce projet était notre premier projet durant ma première année de BUT informatique.

Ce projet a pour but de nous faire gérer une formation. La formation d’un BUT. Et plus précisément dans notre cas une année de BUT donc 2 semestres. Nous avons développé un interpréteur de commande pour gérer la formation.  
Nous devons plus précisément gérer les notes pour chaque étudiant. Nous avons à gérer maximum 100 étudiants. Notre application est capable de gérer un ou plusieurs étudiants, et leur notes.  
Elle est capable d’évaluer leur année et de trancher sur leur passage dans l’année suivante  ou dans le cas contraire un redoublement.  
L’utilisateur peut donc grâce à notre application, créer une formation. C’est-à-dire qu’il peut ajouter à sa formation des matières avec des épreuves qui leurs sont propres, des épreuves avec leurs coefficients. Et bien sûr des étudiants avec leurs notes.  
Notre application est capable d’afficher sous forme de tableau le relevé de notes d’un étudiant en particulier. Et la décision du jury par rapport aux notes obtenues par l’étudiant pendant l’année.  
Tout se passe via l'invite de commande où l’utilisateur tape une chaîne de caractère puis les paramètres qu'il veut rentrer pour la formation. L’utilisateur dispose de 8 commandes différentes : exit, formation, epreuve, coefficients, note, notes, releve, decision. Chaque commande s’accompagne de un ou plusieurs paramètres qui sont aussi gérés par notre interpréteur.  
Chaque paramètre est séparé par un espace pour permettre une interprétation correcte à la machine. Par exemple pour créer une formation de 3 UE il faut taper “formation 3”.  
Le programme renvoie les éléments prévue dans chaque commande ou des messages d’erreur. Par exemple, si nous rentrons “formation 7” cela nous renvoie en sortie “le nombre d’UE est incorrect”.  
Nous devons donc gérer les potentielles erreurs rentrées par l’utilisateur. Mais ce sont des erreurs que nous connaissons à l’avance car nous connaissons les limites numériques de la formation.  
Et pour chaque commande/fonction prévue dans ce programme il y a un but global à avoir en sortie ou des messages d’erreur en cas d’erreur.  
