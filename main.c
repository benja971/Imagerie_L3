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