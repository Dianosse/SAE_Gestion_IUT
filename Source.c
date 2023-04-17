#include <stdio.h>
#include <string.h>
#include <math.h>

#pragma warning(disable:4996)

enum {
	NB_SEMESTRES = 2,
	MIN_UE = 3,
	MAX_UE = 6,
	MAX_MATIERES = 10,
	MAX_EPREUVES = 5,
	MAX_ETUDIANTS = 100,
	MAX_CHAR = 30
};
const float MIN_NOTE = 0.f, MAX_NOTE = 20.f;

typedef char CH30[MAX_CHAR + 1];

typedef struct {
	unsigned int nbNotes;
	CH30 nom;
	float notes[NB_SEMESTRES][MAX_MATIERES][MAX_EPREUVES];
} Etudiant;

typedef struct {
	CH30 nom;
	float coef[MAX_UE];
} Epreuve;

typedef struct {
	CH30 nom;
	unsigned int nbEpreuves;
	Epreuve epreuves[MAX_EPREUVES];
} Matiere;

typedef struct {
	unsigned int nbMatieres;
	Matiere matieres[MAX_MATIERES];
} Semestre;

typedef struct {
	unsigned int nbUE; // nombre de coef, commun à toutes les épreuves
	unsigned int nbEtudiants;
	Etudiant etudiants[MAX_ETUDIANTS];
	Semestre semestres[NB_SEMESTRES];
} Formation;

void formation(Formation* formationInit);
void epreuve(Formation* formationInit);
void coefficients(const Formation* formationInit);
void note(Formation* formationInit);
void initTabNote(Formation* formationInit); //on met tous les tableaux d'etudiant a -1
void notes(const Formation* formationInit);
void releve(const Formation* formationInit);
void decision(const Formation* formationInit);

int main() {
	char cde[31] = "";
	Formation maForm;
	maForm.nbUE = 0;
	maForm.nbEtudiants = 0;
	maForm.semestres[0].nbMatieres = 0;
	maForm.semestres[1].nbMatieres = 0;
	initTabNote(&maForm);


	do {
		scanf("%s", cde);
		if (strcmp(cde, "formation") == 0) // C2
		{
			formation(&maForm);
		}
		else if (strcmp(cde, "epreuve") == 0) // C3
		{
			epreuve(&maForm);
		}
		else if (strcmp(cde, "coefficients") == 0) // C4
		{
			coefficients(&maForm);
		}
		else if (strcmp(cde, "note") == 0) // C5
		{
			note(&maForm);
		}
		else if (strcmp(cde, "notes") == 0) // C6
		{
			notes(&maForm);
		}
		else if (strcmp(cde, "releve") == 0) // C7
		{
			releve(&maForm);
		}
		else if (strcmp(cde, "decision") == 0) // C8
		{
			decision(&maForm);
		}
	} while (strcmp(cde, "exit") != 0); // C1

	return 0;
}


/*
Le rôle de cette fonction est de définir le nombre d'UE dans la formation
Elle prend en paramètre un in out vu que que la formation est donné et modifiée
*/
void formation(Formation* formationInit) {
	unsigned int nombreUE;
	scanf("%u", &nombreUE);
	if (nombreUE >= MIN_UE && nombreUE <= MAX_UE && formationInit->nbUE == 0) {
		printf("Le nombre d'UE est defini\n");
		formationInit->nbUE = nombreUE;
	}
	else if (formationInit->nbUE != 0) {
		printf("Le nombre d'UE est deja defini\n");
	}
	else {
		printf("Le nombre d'UE est incorrect\n");
	}
}

/*
Cette fonction a pour rôle d'ajouter une épreuve à la formation
Elle prend en paramètre un in out vu que que la formation est donné et modifiée
*/
void epreuve(Formation* formationInit) {
	if (formationInit->nbUE == 0) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}
	int numSem;
	int verifMat = 1;
	int nbsCoefSup = 0; //compteur le nombre de coef supérieur à 0
	float tmpCoef[MAX_UE]; //qui stock le coef pour savoir si il est au bon format
	CH30 nomMatiere;
	CH30 nomEp;

	scanf("%u %s %s", &numSem, nomMatiere, nomEp);
	for (int i = 0; i < formationInit->nbUE; ++i) {
		scanf("%f", &tmpCoef[i]);
	}
	if (numSem > NB_SEMESTRES || numSem < 1) {
		printf("Le numero de semestre est incorrect\n");
		return;
	}

	unsigned int nbMat = formationInit->semestres[numSem - 1].nbMatieres; //nombre de matieres accuellement
	int indiceMat = 0; //indice qui nous permet de connaitre la position de la matiere si elle existe déjà
	//on determine l'indice de la matiere
	if (nbMat > 0) { // si il y a plusieurs matieres
		for (int i = 0; i < nbMat; ++i) {
			if (strcmp(nomMatiere, formationInit->semestres[numSem - 1].matieres[i].nom) == 0) {
				indiceMat = i;//elle existe a l'indice i
				verifMat = 0;
				break;
			}
		}
		if (verifMat == 1) {                     //si elle existe pas dans les matieres deja existante et nbMat > 1 car
			indiceMat = nbMat;
			formationInit->semestres[numSem - 1].matieres[indiceMat].nbEpreuves = 0;
		}
	}
	if (verifMat) { //si la matiere n'existe pas
		//on verifie les coef
		for (int i = 0; i < formationInit->nbUE; ++i) {
			if (tmpCoef[i] < 0.) {
				printf("Au moins un des coefficients est incorrect\n");
				return;
			}
			else if (tmpCoef[i] > 0.) {
				nbsCoefSup++;
			}
		}
		if (nbsCoefSup == 0) {
			printf("Au moins un des coefficients est incorrect\n");
			return;
		}
		strcpy(formationInit->semestres[numSem - 1].matieres[indiceMat].nom, nomMatiere);
		formationInit->semestres[numSem - 1].matieres[indiceMat].nbEpreuves = 0; //il y a 0 epreuve dans une matiere qui vient d'etre ajouteé;
		formationInit->semestres[numSem - 1].nbMatieres += 1;
		printf("Matiere ajoutee a la formation\n");
	}
	int nbEp = formationInit->semestres[numSem - 1].matieres[indiceMat].nbEpreuves;  //nombre d'epreuve actuellement
	//vérification qu'il n'existe pas d'epeuve qui porte deja meme nom que dans nomEp
	for (int i = 0; i < nbEp; ++i) {
		if (strcmp(nomEp, formationInit->semestres[numSem - 1].matieres[indiceMat].epreuves[i].nom) == 0) {
			printf("Une meme epreuve existe deja\n");
			return;
		}
	}
	// l'epreuve n'existe pas
	//on verifie les coef au cas où la matiere existait deja
	for (int i = 0; i < formationInit->nbUE; ++i) {
		if (tmpCoef[i] < 0.) {
			printf("Au moins un des coefficients est incorrect\n");
			return;
		}
		else if (tmpCoef[i] > 0.) {
			nbsCoefSup++;
		}
	}
	if (nbsCoefSup == 0) {
		printf("Au moins un des coefficients est incorrect\n");
		return;
	}
	//on ajoute l'epreuve
	strcpy(formationInit->semestres[numSem - 1].matieres[indiceMat].epreuves[nbEp].nom, nomEp);
	formationInit->semestres[numSem - 1].matieres[indiceMat].nbEpreuves += 1;
	printf("Epreuve ajoutee a la formation\n");
	//on ajoute les coef
	for (int i = 0; i < formationInit->nbUE; ++i) {
		formationInit->semestres[numSem - 1].matieres[indiceMat].epreuves[nbEp].coef[i] = tmpCoef[i];
	}
}

/*
Cette fonction a pour rôle de vérifier les coefficients d'un semestre
Elle prend en in la fonction et renvoie seulement du texte via des printf
*/
void coefficients(const Formation* formationInit) {    //C4
	if (formationInit->nbUE == 0) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}
	int numSem;
	int compteur;
	scanf("%d", &numSem);
	if (numSem == 1 || numSem == 2) {
		int nbMatiere = formationInit->semestres[numSem - 1].nbMatieres;
		if (nbMatiere == 0) {
			printf("Le semestre ne contient aucune epreuve\n");
			return;
		}
		for (int i = 0; i < formationInit->nbUE; i++) {//indice coef i indice pour vérifier les coef des chaque UE
			compteur = 0;
			for (int j = 0; j < nbMatiere; j++) {					//indice pour parcourir les matières
				int nbepreuve = formationInit->semestres[numSem - 1].matieres[j].nbEpreuves;
				for (int k = 0; k < nbepreuve; k++) {				//indide pour parcourir chaque épreuve
					if (formationInit->semestres[numSem - 1].matieres[j].epreuves[k].coef[i] > 0.) {
						compteur++;
					}
				}
			}
			if (compteur == 0) {
				printf("Les coefficients d'au moins une UE de ce semestre sont tous nuls\n");
				return;
			}
		}
		printf("Coefficients corrects\n");
	}
	else {
		printf("Le numero de semestre est incorrect\n");
	}
}

/*
Cette fonction a pour rôle d'ajouter une note à un étudiant pour une épreuve donnée
Elle prend en paramètre un in out vu que que la formation est donnée et modifiée
*/
void note(Formation* formationInit) {
	unsigned int numSem;
	float noteA;
	int indiceMat;
	int indiceEp;
	int indiceEtu = 0;
	unsigned int verifMat = 0;
	unsigned int verifEp = 0;
	unsigned int verifEtu = 0;
	CH30 nomEtu, nomMatiere, nomEpreuve;
	scanf("%d %s %s %s %f", &numSem, nomEtu, nomMatiere, nomEpreuve, &noteA);

	if (numSem > NB_SEMESTRES || numSem < 1) {
		printf("Le numero de semestre est incorrect\n");
		return;
	}

	//on determine l'indice de la matiere
	for (int i = 0; i < formationInit->semestres[numSem - 1].nbMatieres; ++i) {
		if (strcmp(nomMatiere, formationInit->semestres[numSem - 1].matieres[i].nom) == 0) {
			indiceMat = i;
			verifMat = 1; //la matiere existe
			break;
		}
	}
	if (verifMat == 0) {
		printf("Matiere inconnue\n");
		return;
	}
	//on determine l'indice de l'epreuve
	for (int i = 0; i < formationInit->semestres[numSem - 1].matieres[indiceMat].nbEpreuves; ++i) {
		if (strcmp(nomEpreuve, formationInit->semestres[numSem - 1].matieres[indiceMat].epreuves[i].nom) == 0) {
			indiceEp = i;
			verifEp = 1; //l'epreuve existe
			break;
		}
	}
	if (verifEp == 0) {
		printf("Epreuve inconnue\n");
		return;
	}
	//on teste la note rentre
	if (noteA < MIN_NOTE || noteA > MAX_NOTE) {
		printf("Note incorrecte\n");
		return;
	}

	//on determine l'indice de l'etudiant
	for (int i = 0; i < formationInit->nbEtudiants; ++i) {
		if (strcmp(nomEtu, formationInit->etudiants[i].nom) == 0) {
			indiceEtu = i;
			verifEtu = 1;
		}
	}
	if (verifEtu == 0 && formationInit->nbEtudiants > 0) {
		indiceEtu = formationInit->nbEtudiants;
		strcpy(formationInit->etudiants[indiceEtu].nom, nomEtu);
		formationInit->nbEtudiants += 1;
		printf("Etudiant ajoute a la formation\n");
	}
	else if (verifEtu == 0 && formationInit->nbEtudiants == 0) {
		strcpy(formationInit->etudiants[indiceEtu].nom, nomEtu);
		formationInit->nbEtudiants += 1;
		printf("Etudiant ajoute a la formation\n");
	}
	if (formationInit->etudiants[indiceEtu].notes[numSem - 1][indiceMat][indiceEp] != -1) {
		printf("Une note est deja definie pour cet etudiant\n");
		return;
	}
	else {
		formationInit->etudiants[indiceEtu].notes[numSem - 1][indiceMat][indiceEp] = noteA;
		formationInit->etudiants[indiceEtu].nbNotes += 1;
		printf("Note ajoutee a l'etudiant\n");
	}

}

/*
Cette fonction a pour rôle de vérifier les notes d'un étudiant dans un semestre donné
Elle prend en in la fonction et renvoie seulement du texte via des printf
*/
void notes(const Formation* formationInit) {
	unsigned int numSem;
	unsigned int verifEtu = 0;
	unsigned int indiceEtu = 0;
	CH30 nomEtu;
	scanf("%u %s", &numSem, nomEtu);
	if (numSem > NB_SEMESTRES || numSem < 1) {
		printf("Le numero de semestre est incorrect\n");
		return;
	}
	for (int i = 0; i < formationInit->nbEtudiants; ++i) {
		if (strcmp(nomEtu, formationInit->etudiants[i].nom) == 0) {
			verifEtu = 1;
			indiceEtu = i;
			break;
		}
	}
	if (verifEtu == 0) {
		printf("Etudiant inconnu\n");
		return;
	}
	for (int i = 0; i < formationInit->semestres[numSem - 1].nbMatieres; ++i) {
		for (int j = 0; j < formationInit->semestres[numSem - 1].matieres[i].nbEpreuves; ++j) {
			if (formationInit->etudiants[indiceEtu].notes[numSem - 1][i][j] == -1) {
				printf("Il manque au moins une note pour cet etudiant\n");
				return;
			}
		}
	}
	printf("Notes correctes\n");
}

/*
Cette fonction a pour rôle d'afficher un releve de note pour un étudiant pour un semestre donné
Elle prend en in la fonction et renvoie seulement du texte via des printf
*/
void releve(const Formation* formationInit) {
	unsigned int numSem;
	unsigned int indiceEtu;
	unsigned int verifEtu = 0;
	CH30 nomEtu;
	scanf("%u %s", &numSem, nomEtu);
	if (numSem > NB_SEMESTRES || numSem < 1) {
		printf("Le numero de semestre est incorrect\n");
		return;
	}
	for (int i = 0; i < formationInit->nbEtudiants; ++i) {
		if (strcmp(nomEtu, formationInit->etudiants[i].nom) == 0) {
			verifEtu = 1;
			indiceEtu = i;
			break;
		}
	}
	if (verifEtu == 0) {
		printf("Etudiant inconnu\n");
		return;
	}

	int compteur;
	int nbMatiere = formationInit->semestres[numSem - 1].nbMatieres;
	for (int i = 0; i < formationInit->nbUE; i++) {				//indice coef i indice pour vérifier les coef des chaque UE
		compteur = 0;
		for (int j = 0; j < nbMatiere; j++) {					//indice pour parcourir les matières
			int nbepreuve = formationInit->semestres[numSem - 1].matieres[j].nbEpreuves;
			for (int k = 0; k < nbepreuve; k++) {				//indide pour parcourir chaque épreuve
				if (formationInit->semestres[numSem - 1].matieres[j].epreuves[k].coef[i] > 0.) {
					compteur++;
				}
			}
		}
		if (compteur == 0) {
			printf("Les coefficients de ce semestre sont incorrects\n");
			return;
		}
	}

	for (int i = 0; i < formationInit->semestres[numSem - 1].nbMatieres; ++i) {
		for (int j = 0; j < formationInit->semestres[numSem - 1].matieres[i].nbEpreuves; ++j) {
			if (formationInit->etudiants[indiceEtu].notes[numSem - 1][i][j] == -1) {
				printf("Il manque au moins une note pour cet etudiant\n");
				return;
			}
		}
	}

	//affichage du releve
	//premiere ligne avec les UE
	int nbEspace = 10;
	for (int i = 0; i < nbEspace; ++i) {
		printf(" ");
	}
	for (int i = 0; i < formationInit->nbUE; ++i) {
		printf(" UE%d ", i + 1);
	}
	printf("\n");
	//on affiche chaque matiere et sa moyenne par UE
	for (int i = 0; i < formationInit->semestres[numSem - 1].nbMatieres; ++i) {
		printf("%s", formationInit->semestres[numSem - 1].matieres[i].nom);
		for (int j = 0; j < nbEspace - strlen(formationInit->semestres[numSem - 1].matieres[i].nom); ++j) {
			printf(" ");
		}
		float sommeNote = 0.;
		float sommeCoef = 0.;
		float moyenne;
		for (int j = 0; j < formationInit->nbUE; ++j) {
			for (int k = 0; k < formationInit->semestres[numSem - 1].matieres[i].nbEpreuves; ++k) {
				sommeNote += formationInit->etudiants[indiceEtu].notes[numSem - 1][i][k] * formationInit->semestres[numSem - 1].matieres[i].epreuves[k].coef[j];
				sommeCoef += formationInit->semestres[numSem - 1].matieres[i].epreuves[k].coef[j];
			}
			if (sommeCoef == 0.) {
				printf("  ND ");
			}
			else {
				moyenne = floorf(10 * (sommeNote / sommeCoef)) / 10;
				if (moyenne < 10.) {
					printf(" ");
				}
				printf("%.1f ", floorf(10 * (sommeNote / sommeCoef)) / 10);
			}
			sommeNote = 0.;
			sommeCoef = 0.;
		}
		printf("\n");
	}
	printf("--\n");
	printf("Moyennes");
	for (int i = 0; i < nbEspace - strlen("Moyennes"); ++i) {
		printf(" ");
	}
	//on affiche la moyenne par UE
	float sommeNote = 0.;
	float sommeCoef = 0.;
	for (int i = 0; i < formationInit->nbUE; ++i) {
		for (int j = 0; j < formationInit->semestres[numSem - 1].nbMatieres; ++j) {
			for (int k = 0; k < formationInit->semestres[numSem - 1].matieres[j].nbEpreuves; ++k) {
				sommeNote += formationInit->etudiants[indiceEtu].notes[numSem - 1][j][k] * formationInit->semestres[numSem - 1].matieres[j].epreuves[k].coef[i];
				sommeCoef += formationInit->semestres[numSem - 1].matieres[j].epreuves[k].coef[i];
			}
		}
		if (floorf(10 * (sommeNote / sommeCoef)) / 10 < 10.) {
			printf(" ");
		}
		printf("%.1f ", floorf(10 * (sommeNote / sommeCoef)) / 10);
		sommeNote = 0.;
		sommeCoef = 0.;
	}
	printf("\n");
}

/*
Cette fonction a pour rôle d'afficher un releve de note pour un étudiant pour une année scolaire et afficher si il peut passer à l'année d'après
Elle prend en in la fonction et renvoie seulement du texte via des printf
*/
void decision(const Formation* formationInit) {
	unsigned int indiceEtu;
	unsigned int verifEtu = 0;
	unsigned int vir = 0;
	CH30 nomEtu;
	scanf("%s", nomEtu);
	for (int i = 0; i < formationInit->nbEtudiants; ++i) {
		if (strcmp(nomEtu, formationInit->etudiants[i].nom) == 0) {
			verifEtu = 1;
			indiceEtu = i;
			break;
		}
	}
	if (verifEtu == 0) {
		printf("Etudiant inconnu\n");
		return;
	}


	for (int Sem = 0; Sem < NB_SEMESTRES; Sem++)
		for (int i = 0; i < formationInit->semestres[Sem].nbMatieres; ++i) {
			for (int j = 0; j < formationInit->semestres[Sem].matieres[i].nbEpreuves; ++j) {
				if (formationInit->etudiants[indiceEtu].notes[Sem][i][j] == -1) {
					printf("Il manque au moins une note pour cet etudiant\n");
					return;
				}
			}
		}

	int compteur;
	int nbMatiere;
	for (int Sem = 0; Sem < NB_SEMESTRES; ++Sem) {
		nbMatiere = formationInit->semestres[Sem].nbMatieres;
		for (int i = 0; i < formationInit->nbUE; i++) {				//indice coef i indice pour vérifier les coef des chaque UE
			compteur = 0;
			for (int j = 0; j < nbMatiere; j++) {					//indice pour parcourir les matières
				int nbepreuve = formationInit->semestres[Sem].matieres[j].nbEpreuves;
				for (int k = 0; k < nbepreuve; k++) {				//indide pour parcourir chaque épreuve
					if (formationInit->semestres[Sem].matieres[j].epreuves[k].coef[i] > 0.) {
						compteur++;
					}
				}
			}
			if (compteur == 0) {
				printf("Les coefficients de ce semestre sont incorrects\n");
				return;
			}
		}
	}

	//affichage du releve
	//premiere ligne avec les UE
	int nbespaceUE = 19;
	for (int i = 0; i < nbespaceUE; ++i) {                //ou un/deux printf de \t
		printf(" ");
	}
	for (int i = 0; i < formationInit->nbUE; ++i) {
		printf(" UE%d ", i + 1);
	}
	printf("\n");
	//on affiche la moyenne par UE
	float tab[NB_SEMESTRES][MAX_UE];                    //tableau qui va stocker toutes les moyennes
	for (int Sem = 0; Sem < NB_SEMESTRES; Sem++) {
		printf("S%d                 ", Sem + 1);
		float sommeNote = 0.;
		float sommeCoef = 0.;
		for (int i = 0; i < formationInit->nbUE; ++i) {
			for (int j = 0; j < formationInit->semestres[Sem].nbMatieres; ++j) {
				for (int k = 0; k < formationInit->semestres[Sem].matieres[j].nbEpreuves; ++k) {
					sommeNote += formationInit->etudiants[indiceEtu].notes[Sem][j][k] * formationInit->semestres[Sem].matieres[j].epreuves[k].coef[i];
					sommeCoef += formationInit->semestres[Sem].matieres[j].epreuves[k].coef[i];
				}     //stock de la moyenne
			}
			if ((floorf(10 * (sommeNote / sommeCoef)) / 10) < 10.) {
				printf(" ");
			}
			printf("%.1f ", floorf(10 * (sommeNote / sommeCoef)) / 10);
			tab[Sem][i] = sommeNote / sommeCoef;          //récup des moyennes dans un tab à 2 dimensions
			sommeNote = 0.;
			sommeCoef = 0.;
		}
		printf("\n");
	}
	printf("--\n");
	printf("Moyennes annuelles ");
	float MoyAnnue[MAX_UE];
	float somme = 0.;
	for (int j = 0; j < formationInit->nbUE; j++) {
		float somme = tab[0][j] + tab[1][j];				//addittionne les notes du sem1 et sem2 pour la même UE
		if ((floorf(10 * (somme / 2)) / 10) < 10.) {
			printf(" ");
		}
		printf("%.1f ", floorf(10 * (somme / 2)) / 10);
		MoyAnnue[j] = floorf(10 * (somme / 2)) / 10;			//récupère ces moyennes annuelles pour chaque UE
	}
	printf("\n");
	printf("Acquisition\t   ");
	int verifDeve = 0;
	int cpt = 0;
	for (int i = 0; i < formationInit->nbUE; i++) {
		if (MoyAnnue[i] >= 10.) {
			cpt++;
		}
	}
	if (cpt == 0) {
		printf("Aucune");
	}

	for (int i = 0; i < formationInit->nbUE; i++) {
		if (MoyAnnue[i] >= 10) {
			if (vir == 1) {				//Permet d'afficher les virgules bien comme des les exemples
				printf(", ");
			}
			printf("UE%d", i + 1);
			verifDeve++;
			vir = 1;
		}
	}

	printf("\n");
	printf("Devenir         ");
	if (verifDeve > (formationInit->nbUE / 2)) {
		printf("   Passage\n");
	}
	else {
		printf("   Redoublement\n");
	}
}


void initTabNote(Formation* formationInit) {
	for (int i = 0; i < MAX_ETUDIANTS; ++i) {
		for (int j = 0; j < NB_SEMESTRES; ++j) {
			for (int k = 0; k < MAX_MATIERES; ++k) {
				for (int l = 0; l < MAX_EPREUVES; ++l) {
					formationInit->etudiants[i].notes[j][k][l] = -1;
				}
			}
		}
	}
}
