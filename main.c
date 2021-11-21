#include "image.h"
#include <time.h>
//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont d�finit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donn� en arg2, cette
// image est contenue dans une pointeur de type : struct fichierimage * fournit en arg1
//
// fonction struct fichierimage * nouveau(int,int)
// permet de creer une image en memoire de largeur arg1 et de hauteur arg2, la fonction
// retourne un pointeur de type : struct fichierimage *
//------------------------------------------------------------------------------

int main()
{
	/*
	// exemple de déclaration d'un pointeur image
	fichierimage *fichier = charger("NIDDAM_base.bmp");

	fichierimage *gray = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	nuancesDeGris(fichier, gray);
	enregistrer("resultats/NIDDAM_gris.bmp", gray);

	fichierimage *miroir = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	imageMiroir(fichier, miroir);
	enregistrer("resultats/NIDDAM_miroir.bmp", miroir);

	fichierimage *symetrique = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	imageSymetrique(fichier, symetrique);
	enregistrer("resultats/NIDDAM_symetrique.bmp", symetrique);

	fichierimage *rotationg = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
	imageRotationGauche(fichier, rotationg);
	enregistrer("resultats/NIDDAM_rotationGauche.bmp", rotationg);

	fichierimage *rotationd = nouveau(fichier->entetebmp.hauteur, fichier->entetebmp.largeur);
	imageRotationDroite(fichier, rotationd);
	enregistrer("resultats/NIDDAM_rotationDroite.bmp", rotationd);

	fichierimage *T = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	fichierimage *S = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	fichierimage *L = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	imageRVBversTSL(fichier, T, S, L);
	enregistrer("resultats/NIDDAM_imageT.bmp", T);
	enregistrer("resultats/NIDDAM_imageS.bmp", S);
	enregistrer("resultats/NIDDAM_imageL.bmp", L);

	printf("pourcentage de rouge: %f\n", pourcentageCouleur('r', fichier));
	printf("pourcentage de vert: %f\n", pourcentageCouleur('g', fichier));
	printf("pourcentage de bleu: %f\n", pourcentageCouleur('b', fichier));

	fichierimage *inversion = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	inversiondescanneaux(fichier, inversion);
	enregistrer("resultats/NIDDAM_inversion.bmp", inversion);

	fichierimage *negatif = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	imageNegative(fichier, negatif);
	enregistrer("resultats/NIDDAM_negatif.bmp", negatif);

	fichierimage *rgb = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	imageTSLversRGB(charger("./resultats/NIDDAM_imageT.bmp"), charger("./resultats/NIDDAM_imageS.bmp"), charger("./resultats/NIDDAM_imageL.bmp"), rgb);
	enregistrer("resultats/NIDDAM_TSLversRGB.bmp", rgb);

	fichierimage *Imonochrome = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	monochrome(fichier, 'g', Imonochrome);
	enregistrer("resultats/NIDDAM_monochrome.bmp", Imonochrome);

	fichierimage *binaire = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);
	seuillage(fichier, 128.0, binaire);
	enregistrer("resultats/NIDDAM_binaire.bmp", binaire);
	printf("Veuillez entrer le facteur d'agrandissement: ");
	int facteur;
	scanf("%f", &facteur);
	fichierimage *agrandi = nouveau(fichier->entetebmp.largeur * facteur, fichier->entetebmp.hauteur * facteur);
	printf("test");
	agrandissement(fichier, facteur, agrandi);
	enregistrer("resultats/NIDDAM_agrandi.bmp", agrandi);
	*/

	int continuer = 1;
	while (continuer == 1)
	{
		char choix;
		menu();
		printf("Voulez vous continuer ?\nVotre choix (o/n): ");
		scanf("%c", &choix);
		getchar();

		while (choix != 'o' && choix != 'n')
		{
			printf("Veuillez entrer un choix valide (o/n): ");
			scanf("%c", &choix);
			getchar();
		}
		if (choix == 'o')
			continuer = 1;
		else if (choix == 'n')
			continuer = 0;
	}
}