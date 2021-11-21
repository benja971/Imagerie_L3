#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// structure d'entete du fichier

typedef struct entete_fichier
{
	char idformat[2];
	int taille_fichier;
	int reserve;
	int debut;
} entete_fichier;

// structure d'entete de l'image
typedef struct entete_bmp
{
	int taille_image;
	int largeur;
	int alignement;
	int hauteur;
	short plans;
	short profondeur;
	int compression;
	int taille_image_totale;
	int resolutionh;
	int resolutionv;
	int nbrcouleur;
	int nbrcouleuri;
} entete_bmp;

// structure d'un pixel
typedef struct pixels
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} pixels;

// structure du fichier
typedef struct fichierimage
{
	struct entete_fichier entetefichier;
	struct entete_bmp entetebmp;
	struct pixels image[5000][5000];
} fichierimage;

typedef struct TSL
{
	float t;
	float s;
	float l;
} TSL;

fichierimage *charger(char *nom)
{
	FILE *f;
	int i, j, k;
	unsigned char zero;
	unsigned char *buffer;
	int position = 0;
	int longueur = 0;
	struct fichierimage *fichier = (struct fichierimage *)malloc(sizeof(struct fichierimage));

	f = fopen(nom, "rb");
	fseek(f, 0, SEEK_END);
	longueur = ftell(f);
	fseek(f, 0, SEEK_SET);
	buffer = (unsigned char *)malloc(longueur * sizeof(unsigned char));
	fread((unsigned char *)buffer, 1, longueur, f);
	fclose(f);

	// lecture de l'entete du fichier

	memcpy(&fichier->entetefichier.idformat, (buffer + position), 2);
	position += 2;
	memcpy(&fichier->entetefichier.taille_fichier, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetefichier.reserve, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetefichier.debut, (buffer + position), 4);
	position += 4;

	/*
		printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
		printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
		printf("reserve                  :%d\n",fichier->entetefichier.reserve);
		printf("debut                    :%d\n",fichier->entetefichier.debut);
	*/

	// lecture de l'entete de l'image

	memcpy(&fichier->entetebmp.taille_image, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.largeur, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.hauteur, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.plans, (buffer + position), 2);
	position += 2;
	memcpy(&fichier->entetebmp.profondeur, (buffer + position), 2);
	position += 2;
	memcpy(&fichier->entetebmp.compression, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.taille_image_totale, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.resolutionh, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.resolutionv, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.nbrcouleur, (buffer + position), 4);
	position += 4;
	memcpy(&fichier->entetebmp.nbrcouleuri, (buffer + position), 4);
	position += 4;

	/*
		printf("taille image             :%d\n",fichier->entetebmp.taille_image);
		printf("largeur                  :%d\n",fichier->entetebmp.largeur);
		printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
		printf("plans                    :%d\n",fichier->entetebmp.plans);
		printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
		printf("compression              :%d\n",fichier->entetebmp.compression);
		printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
		printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
		printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
		printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
		printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
	*/

	if (fichier->entetebmp.plans != 1 || fichier->entetebmp.profondeur != 24 || fichier->entetebmp.compression != 0 || fichier->entetebmp.nbrcouleur != 0 || fichier->entetebmp.nbrcouleuri != 0)
	{
		printf("Impossible de charger l'image\n");
		free(fichier);
		free(buffer);
		return 0;
	}

	// mise de l'image en memoire

	for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (j = 0; j < fichier->entetebmp.largeur; j++)
		{
			memcpy(&fichier->image[i][j].b, (buffer + position), 1);
			position += 1;
			memcpy(&fichier->image[i][j].g, (buffer + position), 1);
			position += 1;
			memcpy(&fichier->image[i][j].r, (buffer + position), 1);
			position += 1;
		}
		for (k = 0; k < fichier->entetebmp.largeur % 4; k++)
		{
			memcpy(&zero, (buffer + position), 1);
			position += 1;
		}
	}

	free(buffer);
	return fichier;
}

int enregistrer(char *nom, struct fichierimage *fichier)
{
	FILE *f;
	int i, j, k;
	unsigned char zero = (unsigned char)0;
	unsigned char *buffer;
	int position = 0;
	int longueur = 14 + fichier->entetefichier.taille_fichier;
	buffer = (unsigned char *)malloc(longueur * sizeof(unsigned char));

	// ecriture de l'entete du fichier

	/*
		printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
		printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
		printf("reserve                  :%d\n",fichier->entetefichier.reserve);
		printf("debut                    :%d\n",fichier->entetefichier.debut);
	*/

	memcpy((buffer + position), &fichier->entetefichier.idformat, 2);
	position += 2;
	memcpy((buffer + position), &fichier->entetefichier.taille_fichier, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetefichier.reserve, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetefichier.debut, 4);
	position += 4;

	// ecriture de l'entete de l'image
	/*
	printf("taille image             :%d\n", fichier->entetebmp.taille_image);
	printf("largeur                  :%d\n", fichier->entetebmp.largeur);
	printf("hauteur                  :%d\n", fichier->entetebmp.hauteur);
	printf("plans                    :%d\n", fichier->entetebmp.plans);
	printf("profondeur               :%d\n", fichier->entetebmp.profondeur);
	printf("compression              :%d\n", fichier->entetebmp.compression);
	printf("taille image totale      :%d\n", fichier->entetebmp.taille_image_totale);
	printf("resolution h             :%d\n", fichier->entetebmp.resolutionh);
	printf("resulution v             :%d\n", fichier->entetebmp.resolutionv);
	printf("nbr couleurs             :%d\n", fichier->entetebmp.nbrcouleur);
	printf("nbr couleurs importantes :%d\n", fichier->entetebmp.nbrcouleuri);
	*/
	memcpy((buffer + position), &fichier->entetebmp.taille_image, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.largeur, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.hauteur, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.plans, 2);
	position += 2;
	memcpy((buffer + position), &fichier->entetebmp.profondeur, 2);
	position += 2;
	memcpy((buffer + position), &fichier->entetebmp.compression, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.taille_image_totale, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.resolutionh, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.resolutionv, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.nbrcouleur, 4);
	position += 4;
	memcpy((buffer + position), &fichier->entetebmp.nbrcouleuri, 4);
	position += 4;

	if (fichier->entetebmp.plans != 1 || fichier->entetebmp.profondeur != 24 || fichier->entetebmp.compression != 0 || fichier->entetebmp.nbrcouleur != 0 || fichier->entetebmp.nbrcouleuri != 0)
	{
		printf("Impossible d'enregistrer l'image\n");
		free(buffer);
		return 0;
	}

	for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (j = 0; j < fichier->entetebmp.largeur; j++)
		{
			memcpy((buffer + position), &fichier->image[i][j].b, 1);
			position += 1;
			memcpy((buffer + position), &fichier->image[i][j].g, 1);
			position += 1;
			memcpy((buffer + position), &fichier->image[i][j].r, 1);
			position += 1;
		}
		for (k = 0; k < fichier->entetebmp.largeur % 4; k++)
		{
			memcpy((buffer + position), &zero, 1);
			position += 1;
		}
	}

	f = fopen(nom, "wb");
	fwrite((unsigned char *)buffer, 1, longueur, f);
	fclose(f);
	free(buffer);
	return 1;
}

fichierimage *nouveau(int largeur, int hauteur)
{
	int i, j;
	struct fichierimage *fichier;
	int longueur;

	if (largeur > 5000 || hauteur > 5000 || largeur <= 0 || hauteur <= 0)
	{
		return (NULL);
	}

	longueur = 14 + 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
	fichier = (struct fichierimage *)malloc(sizeof(struct fichierimage));

	// ecriture de l'entete du fichier

	fichier->entetefichier.idformat[0] = 'B';
	fichier->entetefichier.idformat[1] = 'M';
	fichier->entetefichier.taille_fichier = 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
	fichier->entetefichier.reserve = 0;
	fichier->entetefichier.debut = 54;

	// ecriture de l'entete de l'image

	fichier->entetebmp.taille_image = 40;
	fichier->entetebmp.largeur = largeur;
	fichier->entetebmp.hauteur = hauteur;
	fichier->entetebmp.plans = 1;
	fichier->entetebmp.profondeur = 24;
	fichier->entetebmp.compression = 0;
	fichier->entetebmp.taille_image_totale = 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
	fichier->entetebmp.resolutionh = 1;
	fichier->entetebmp.resolutionv = 1;
	fichier->entetebmp.nbrcouleur = 0;
	fichier->entetebmp.nbrcouleuri = 0;

	// lecture de l'ente du fichier

	/*
		printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
		printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
		printf("reserve                  :%d\n",fichier->entetefichier.reserve);
		printf("debut                    :%d\n",fichier->entetefichier.debut);
	*/

	// lecture de l'ente de l'image

	/*
		printf("taille image             :%d\n",fichier->entetebmp.taille_image);
		printf("largeur                  :%d\n",fichier->entetebmp.largeur);
		printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
		printf("plans                    :%d\n",fichier->entetebmp.plans);
		printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
		printf("compression              :%d\n",fichier->entetebmp.compression);
		printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
		printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
		printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
		printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
		printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
	*/

	// mise de l'image en memoire

	for (i = fichier->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (j = 0; j < fichier->entetebmp.largeur; j++)
		{
			fichier->image[i][j].b = 0;
			fichier->image[i][j].g = 0;
			fichier->image[i][j].r = 0;
		}
	}

	return fichier;
};

pixels nouveauPixel(char r, char g, char b)
{
	pixels pixel;
	pixel.r = r;
	pixel.g = g;
	pixel.b = b;
	return pixel;
}

void nuancesDeGris(fichierimage *model, fichierimage *gray)
{
	for (int i = model->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			char gr = (model->image[i][j].b + model->image[i][j].g + model->image[i][j].r) / 3;
			gray->image[i][j] = nouveauPixel(gr, gr, gr);
		}
	}
}

void imageMiroir(fichierimage *model, fichierimage *miroir)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			int k = model->entetebmp.largeur - j;
			miroir->image[i][k] = nouveauPixel(model->image[i][j].r, model->image[i][j].g, model->image[i][j].b);
		}
	}
}

void imageSymetrique(fichierimage *model, fichierimage *symetrique)
{
	for (int i = model->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (int j = 0; j <= (model->entetebmp.largeur) / 2; j++)
		{
			int k = model->entetebmp.largeur - j;
			symetrique->image[i][k] = nouveauPixel(model->image[i][j].r, model->image[i][j].g, model->image[i][j].b);
			symetrique->image[i][j] = nouveauPixel(model->image[i][j].r, model->image[i][j].g, model->image[i][j].b);
		}
	}
}

void imageRotationGauche(fichierimage *model, fichierimage *rotationGauche)
{
	for (int i = 0; i < model->entetebmp.largeur; i++)
	{
		for (int j = 0; j <= model->entetebmp.hauteur; j++)
		{
			rotationGauche->image[i][j] = nouveauPixel(model->image[model->entetebmp.hauteur - j][i].r, model->image[model->entetebmp.hauteur - j][i].g, model->image[model->entetebmp.hauteur - j][i].b);
		}
	}
}

void imageRotationDroite(fichierimage *model, fichierimage *rotationDroite)
{
	for (int i = 0; i < model->entetebmp.largeur; i++)
	{
		for (int j = 0; j <= model->entetebmp.hauteur; j++)
		{
			rotationDroite->image[i][j] = nouveauPixel(model->image[j][model->entetebmp.largeur - i].r, model->image[j][model->entetebmp.largeur - i].g, model->image[j][model->entetebmp.largeur - i].b);
		}
	}
}

float MAX(float a, float b) { return a > b ? a : b; }
float MIN(float a, float b) { return a < b ? a : b; }

TSL RVB2TSL(pixels rgb)
{
	TSL tsl;

	float red = rgb.r / 255.0;
	float green = rgb.g / 255.0;
	float blue = rgb.b / 255.0;

	float max = MAX(MAX(red, green), blue);
	float min = MIN(MIN(red, green), blue);

	tsl.l = (max + min) / 2.0;

	if (max == min)
	{
		tsl.t = 0.0;
		tsl.s = 0.0;
	}
	else
	{
		if (tsl.l < 0.5)
		{
			tsl.s = (max - min) / (max + min);
		}
		else
		{
			tsl.s = (max - min) / (2.0 - max - min);
		}

		if (red == max)
		{
			tsl.t = (green - blue) / (max - min);
		}
		else if (green == max)
		{
			tsl.t = 2.0 + (blue - red) / (max - min);
		}
		else
		{
			tsl.t = 4.0 + (red - green) / (max - min);
		}
	}
	return tsl;
}

void imageRVBversTSL(fichierimage *model, fichierimage *imageT, fichierimage *imageS, fichierimage *imageL)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			TSL tsl = RVB2TSL(model->image[i][j]);
			imageT->image[i][j] = nouveauPixel((int)(tsl.t * 255), (int)(tsl.t * 255), (int)(tsl.t * 255));
			imageS->image[i][j] = nouveauPixel((int)(tsl.s * 255), (int)(tsl.s * 255), (int)(tsl.s * 255));
			imageL->image[i][j] = nouveauPixel((int)(tsl.l * 255), (int)(tsl.l * 255), (int)(tsl.l * 255));
		}
	}
}

pixels TLSversRGB(TSL tsl)
{
	pixels rgb;
	unsigned char region, remainder, p, q, t;

	if (tsl.s == 0)
		return nouveauPixel(tsl.l, tsl.l, tsl.l);

	region = tsl.t / 43;
	remainder = (tsl.t - (region * 43)) * 6;

	p = (tsl.l * (255 - tsl.s));
	q = (tsl.l * (255 - ((tsl.s * remainder))));
	t = (tsl.l * (255 - ((tsl.s * (255 - remainder)))));

	switch (region)
	{
	case 0:
		rgb.r = tsl.l;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = tsl.l;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = tsl.l;
		rgb.b = t;
		break;
	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = tsl.l;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = tsl.l;
		break;
	default:
		rgb.r = tsl.l;
		rgb.g = p;
		rgb.b = q;
		break;
	}

	return rgb;
}

void imageTSLversRGB(fichierimage *T, fichierimage *S, fichierimage *L, fichierimage *rgb)
{
	float tmax = 0;

	for (int i = 0; i < T->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < T->entetebmp.largeur; j++)
		{
			TSL tsl = {(T->image[i][j].r), S->image[i][j].r, L->image[i][j].r};
			rgb->image[i][j] = TLSversRGB(tsl);
		}
	}
}

float pourcentageCouleur(char couleur, fichierimage *model)
{
	int nb = 0;
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			if (couleur == 'r')
				nb += model->image[i][j].r;
			else if (couleur == 'g')
				nb += model->image[i][j].g;
			else if (couleur == 'b')
				nb += model->image[i][j].b;
		}
	}
	return ((float)nb / ((model->entetebmp.hauteur * model->entetebmp.largeur) * 255)) * 100;
}

void inversiondescanneaux(fichierimage *model, fichierimage *inversion)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			inversion->image[i][j] = nouveauPixel(model->image[i][j].b, model->image[i][j].r, model->image[i][j].g);
		}
	}
}

void imageNegative(fichierimage *model, fichierimage *negatif)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			negatif->image[i][j] = nouveauPixel(255 - model->image[i][j].r, 255 - model->image[i][j].g, 255 - model->image[i][j].b);
		}
	}
}

void monochrome(fichierimage *model, char couleur, fichierimage *mono)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			if (couleur == 'r')
			{
				mono->image[i][j] = nouveauPixel(model->image[i][j].r, 0, 0);
			}
			else if (couleur == 'g')
			{
				mono->image[i][j] = nouveauPixel(0, model->image[i][j].g, 0);
			}
			else
			{
				mono->image[i][j] = nouveauPixel(0, 0, model->image[i][j].b);
			}
		}
}

void seuillage(fichierimage *model, float seuil, fichierimage *seuillage)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			float gr = (model->image[i][j].r + model->image[i][j].g + model->image[i][j].b) / 3.0;

			if (gr < seuil)
				seuillage->image[i][j] = nouveauPixel(0, 0, 0);
			else
				seuillage->image[i][j] = nouveauPixel(255, 255, 255);
		}
	}
}

void agrandissement(fichierimage *model, int facteur, fichierimage *agrandi)
{
	for (int i = 0; i < agrandi->entetebmp.hauteur; i++)
		for (int j = 0; j < agrandi->entetebmp.largeur; j++)
		{
			agrandi->image[i][j] = model->image[i / facteur][j / facteur];
		}
}

void histogramme(fichierimage *model, int *histo_r, int *histo_g, int *histo_b)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{

			histo_r[model->image[i][j].r]++;
			histo_g[model->image[i][j].g]++;
			histo_b[model->image[i][j].b]++;
		}
}

void histogrammeCumule(int *histo_r, int *histo_g, int *histo_b, int *histo_cumule_r, int *histo_cumule_g, int *histo_cumule_b)
{
	histo_cumule_r[0] = histo_r[0];
	histo_cumule_g[0] = histo_g[0];
	histo_cumule_b[0] = histo_b[0];

	for (int i = 1; i < 256; i++)
	{
		histo_cumule_r[i] = histo_cumule_r[i - 1] + histo_r[i];
		histo_cumule_g[i] = histo_cumule_g[i - 1] + histo_g[i];
		histo_cumule_b[i] = histo_cumule_b[i - 1] + histo_b[i];
	}
}

void egalisationhisto(fichierimage *model, fichierimage *egalise)
{
	int histo_r[256];
	int histo_g[256];
	int histo_b[256];
	int histo_cumule_r[256];
	int histo_cumule_g[256];
	int histo_cumule_b[256];

	for (int i = 0; i < 256; i++)
	{
		histo_r[i] = 0;
		histo_g[i] = 0;
		histo_b[i] = 0;
		histo_cumule_r[i] = 0;
		histo_cumule_g[i] = 0;
		histo_cumule_b[i] = 0;
	}

	histogramme(model, histo_r, histo_g, histo_b);
	histogrammeCumule(histo_r, histo_g, histo_b, histo_cumule_r, histo_cumule_g, histo_cumule_b);

	int nbr_pixels = model->entetebmp.hauteur * model->entetebmp.largeur;

	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
			egalise->image[i][j] = nouveauPixel(histo_cumule_r[model->image[i][j].r] * 255 / nbr_pixels, histo_cumule_g[model->image[i][j].g] * 255 / nbr_pixels, histo_cumule_b[model->image[i][j].b] * 255 / nbr_pixels);
}

void reduire(fichierimage *model, int facteur, fichierimage *reduit)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
			if (i % facteur == 0 && j % facteur == 0)
				reduit->image[i / 2][j / 2] = model->image[i][j];
}

void menu()
{
	system("cls");

	printf("Veuillez choisir l'action %c effectuer:\n", 133);
	printf("\tNuances de gris (n)\n");
	printf("\tImage miroir (m)\n");
	printf("\tImage sym%ctrique (s)\n", 130);
	printf("\tRotation gauche (g)\n");
	printf("\tRotation droite (d)\n");
	printf("\tPourcentage d'une couleur (p)\n");
	printf("\tInversion des canneaux (i)\n");
	printf("\tImage en n%cgatif (-)\n", 130);
	printf("\tImage monochrome (=)\n");
	printf("\tSeuillage de l'image (l)\n");
	printf("\tAgrandissement de l'image (a)\n");

	printf("\tQuitter (q)\n");

	char choix;
	printf("\n\nVotre choix: ");
	scanf("%c", &choix);
	getchar();

	// printf("\nVeuillez entrer le chemin du fichier %c modifier: ", 133);

	char nom[100];

	// gets(nom);
	// fichierimage *model = charger(nom);

	fichierimage *model = charger("NIDDAM_base.bmp");

	if (choix == 'n')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		nuancesDeGris(model, nouvelle);
		enregistrer("./resultats/NIDDAM_gris.bmp", nouvelle);
		system("start ./resultats/NIDDAM_gris.bmp");
	}

	else if (choix == 'm')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageMiroir(model, nouvelle);
		enregistrer("./resultats/NIDDAM_miroir.bmp", nouvelle);
		system("start ./resultats/NIDDAM_miroir.bmp");
	}

	else if (choix == 's')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageSymetrique(model, nouvelle);
		enregistrer("./resultats/NIDDAM_symetrique.bmp", nouvelle);
		system("start ./resultats/NIDDAM_symetrique.bmp");
	}

	else if (choix == 'g')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);
		imageRotationGauche(model, nouvelle);
		enregistrer("./resultats/NIDDAM_rotationGauche.bmp", nouvelle);
		system("start ./resultats/NIDDAM_rotationGauche.bmp");
	}

	else if (choix == 'd')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);
		imageRotationDroite(model, nouvelle);
		enregistrer("./resultats/NIDDAM_rotationDroite.bmp", nouvelle);
		system("start ./resultats/NIDDAM_rotationDroite.bmp");
	}

	else if (choix == 'p')
		printf("Il y a %lf pourcent de rouge dans cette image.\n", pourcentageCouleur('r', model));

	else if (choix == 'i')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		inversiondescanneaux(model, nouvelle);
		enregistrer("./resultats/NIDDAM_inversionCanaux.bmp", nouvelle);
		system("start ./resultats/NIDDAM_inversionCanaux.bmp");
	}

	else if (choix == 'v')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageNegative(model, nouvelle);
		enregistrer("./resultats/NIDDAM_negatif.bmp", nouvelle);
		system("start ./resultats/NIDDAM_negatif.bmp");
	}

	else if (choix == '=')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		monochrome(model, 'g', nouvelle);
		enregistrer("./resultats/NIDDAM_monochrome.bmp", nouvelle);
		system("start ./resultats/NIDDAM_monochrome.bmp");
	}

	else if (choix == 'l')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		seuillage(model, 128, nouvelle);
		enregistrer("./resultats/NIDDAM_seuillage.bmp", nouvelle);
		system("start ./resultats/NIDDAM_seuillage.bmp");
	}

	else if (choix == '+')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur * 2, model->entetebmp.hauteur * 2);
		agrandissement(model, 2, nouvelle);
		enregistrer("./resultats/NIDDAM_agrandissement.bmp", nouvelle);
		system("start ./resultats/NIDDAM_agrandissement.bmp");
	}

	else if (choix == '-')
	{
		fichierimage *reduite = nouveau(model->entetebmp.largeur / 2, model->entetebmp.hauteur / 2);
		reduire(model, 2, reduite);
		enregistrer("./resultats/NIDDAM_reduite.bmp", reduite);
		system("start ./resultats/NIDDAM_reduite.bmp");
	}

	else if (choix == 'e')
	{
		fichierimage *nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		egalisationhisto(model, nouvelle);
		enregistrer("./resultats/NIDDAM_egalisation.bmp", nouvelle);
		system("start ./resultats/NIDDAM_egalisation.bmp");
	}

	system("start ./NIDDAM_base.bmp");
}