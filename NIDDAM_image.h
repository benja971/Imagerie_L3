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
	 */

	if (fichier->entetebmp.plans != 1 || fichier->entetebmp.profondeur != 24 || fichier->entetebmp.compression != 0 || fichier->entetebmp.nbrcouleur != 0 || fichier->entetebmp.nbrcouleuri != 0)
	{
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

int moyenne(int r, int g, int b)
{
	return (r + g + b) / 3;
}

void nuancesDeGris(fichierimage *model, fichierimage *gray)
{
	for (int i = model->entetebmp.hauteur - 1; i >= 0; i--)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			char gr = moyenne(model->image[i][j].b, model->image[i][j].g, model->image[i][j].r);
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

	tsl.l = (max + min) * 50;

	if (max == min)
	{
		tsl.t = 0.0;
		tsl.s = 0.0;
	}
	else
	{
		if (tsl.l < 50)
		{
			tsl.s = (max - min) / (max + min) * 100.0;
		}
		else
		{
			tsl.s = (max - min) / (2.0 - max - min) * 100.0;
		}

		if (red == max)
		{
			tsl.t = 60 * (green - blue) / (max - min);
		}
		else if (green == max)
		{
			tsl.t = 60 * (blue - red) / (max - min) + 120;
		}
		else
		{
			tsl.t = 60 * (red - green) / (max - min) + 240;
		}
	}

	// printf("tsl : %f %f %f\n", tsl.t, tsl.s, tsl.l);

	return tsl;
}

void imageRVBversTSL(fichierimage *model, fichierimage *imageT, fichierimage *imageS, fichierimage *imageL)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
	{
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			TSL tsl = RVB2TSL(model->image[i][j]);
			imageT->image[i][j] = nouveauPixel((int)(tsl.t), (int)(tsl.t), (int)(tsl.t));
			imageS->image[i][j] = nouveauPixel((int)(tsl.s), (int)(tsl.s), (int)(tsl.s));
			imageL->image[i][j] = nouveauPixel((int)(tsl.l), (int)(tsl.l), (int)(tsl.l));
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
			else if (couleur == 'v')
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
	// ne fonctionne qu'avec des facteurs entiers
	for (int i = 0; i < agrandi->entetebmp.hauteur; i++)
		for (int j = 0; j < agrandi->entetebmp.largeur; j++)
			agrandi->image[i][j] = model->image[i / facteur][j / facteur];
}

void reduction(fichierimage *model, int facteur, fichierimage *reduite)
{
	// ne fonctionne qu'avec des facteurs entiers
	for (int i = 0; i < reduite->entetebmp.hauteur; i++)
		for (int j = 0; j < reduite->entetebmp.largeur; j++)
			reduite->image[i][j] = model->image[i * facteur][j * facteur];
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

void convolution(fichierimage *model, int masque[3][3], fichierimage *conv, int div)
{
	int moy_r = 0;
	int moy_g = 0;
	int moy_b = 0;
	int c, d;

	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			moy_r = 0;
			moy_g = 0;
			moy_b = 0;

			for (int a = 0; a < 3; a++)
				for (int b = 0; b < 3; b++)
				{
					c = i + a - 1;
					d = j + b - 1;
					// printf("%d\n", masque[a][b]);
					if (c >= 0 && d >= 0 && c <= model->entetebmp.hauteur && d <= model->entetebmp.largeur && moyenne(model->image[c][d].r, model->image[c][d].g, model->image[c][d].b) != 0)
					{
						moy_r += masque[a][b] * model->image[c][d].r;
						moy_g += masque[a][b] * model->image[c][d].g;
						moy_b += masque[a][b] * model->image[c][d].b;
					}
				}
			conv->image[i][j] = nouveauPixel(moy_r / div, moy_g / div, moy_b / div);
		}
}

void filtre_moyen(fichierimage *model, fichierimage *moyen, int masque[3][3], int div)
{
	convolution(model, masque, moyen, div);
}

int trierListePixels(pixels temp[9], pixels med[9])
{
	int compt = 0;
	for (int a = 0; a < 9; a++)
		if (moyenne(med[a].r, med[a].g, med[a].b) != 0)
		{
			temp[compt] = med[a];
			compt++;
		}

	for (int a = 0; a < compt; a++)
		for (int b = a + 1; b < compt; b++)
			if (moyenne(temp[a].r, temp[a].g, temp[a].b) > moyenne(temp[b].r, temp[b].g, temp[b].b))
			{
				pixels tmp = temp[a];
				temp[a] = temp[b];
				temp[b] = tmp;
			}

	return compt;
}

void listeVoisins(int c, int d, pixels med[9], fichierimage *model, int i, int j, int a, int b)
{
	c = i + a - 1;
	d = j + b - 1;

	if (c >= 0 && d >= 0 && c < model->entetebmp.hauteur && d < model->entetebmp.largeur)
		med[a * 3 + b] = model->image[c][d];
	else
		med[a * 3 + b] = nouveauPixel(0, 0, 0);
}

void filtre_median(fichierimage *model, fichierimage *median)
{
	pixels med[9], temp[9];
	int c, d;

	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			for (int a = 0; a < 3; a++)
				for (int b = 0; b < 3; b++)
				{
					listeVoisins(c, d, med, model, i, j, a, b);
				}

			int compt = trierListePixels(temp, med);

			median->image[i][j] = temp[compt / 2];
		}
}

void luminosite(fichierimage *model, fichierimage *luminosite, int intensite)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
			luminosite->image[i][j] = nouveauPixel((int)(model->image[i][j].r * intensite / 100), (int)(model->image[i][j].g * intensite / 100), (int)(model->image[i][j].b * intensite / 100));
}

void contraste(fichierimage *model, fichierimage *contraste, int intensite)
{
}

entete_fichier recupInformations(fichierimage *image)
{
	return image->entetefichier;
}

void selection(fichierimage *model, fichierimage *selection, int x1, int y1, int x2, int y2)
{
	int a = 0;
	int b = 0;
	int temp;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	else if (y1 > y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;

		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	else if (x1 < 0)
		x1 = 0;
	else if (y1 < 0)
		y1 = 0;
	else if (x2 > model->entetebmp.largeur)
		x2 = model->entetebmp.largeur;
	else if (y2 > model->entetebmp.hauteur)
		y2 = model->entetebmp.hauteur;

	for (int i = y1; i < y2; i++)
	{
		b = 0;
		for (int j = x1; j < x2; j++)
		{
			selection->image[a][b] = model->image[i][j];
			b++;
		}
		a++;
	}
}

void filtre_laplacien(fichierimage *model, fichierimage *laplacien, int masque[3][3], int div)
{
	convolution(model, masque, laplacien, div);
}

void filtre_sobel(fichierimage *model, fichierimage *sobel, int masquev[3][3], int masqueh[3][3], int div0, int div1)
{
	fichierimage *sobelv = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);
	fichierimage *sobelh = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);

	convolution(model, masquev, sobelv, div0);
	convolution(model, masqueh, sobelh, div1);

	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			int tmpr = sqrt((sobelv->image[i][j].r * sobelv->image[i][j].r + sobelh->image[i][j].r * sobelh->image[i][j].r));
			int tmpg = sqrt((sobelv->image[i][j].g * sobelv->image[i][j].g + sobelh->image[i][j].g * sobelh->image[i][j].g));
			int tmpb = sqrt((sobelv->image[i][j].b * sobelv->image[i][j].b + sobelh->image[i][j].b * sobelh->image[i][j].b));
			sobel->image[i][j] = nouveauPixel(tmpr, tmpg, tmpb);
		}
}

void superposition(fichierimage *model, fichierimage *model1, fichierimage *nouvelle)
{
	for (int i = 0; i < model->entetebmp.hauteur; i++)
		for (int j = 0; j < model->entetebmp.largeur; j++)
		{
			if (i < model1->entetebmp.hauteur && i > 0 && j < model1->entetebmp.largeur && j > 0)
			{
				int tmpr = (model->image[i][j].r + model1->image[i][j].r) / 2;
				int tmpg = (model->image[i][j].g + model1->image[i][j].g) / 2;
				int tmpb = (model->image[i][j].b + model1->image[i][j].b) / 2;
				nouvelle->image[i][j] = nouveauPixel(tmpr, tmpg, tmpb);
			}
			else
				nouvelle->image[i][j] = model->image[i][j];
		}
}

void menu()
{
	system("cls");

	printf("Veuillez choisir l'action %c effectuer:\n", 133);
	printf("\t a :\tNuances de gris \n");
	printf("\t b :\tImage miroir \n");
	printf("\t c :\tImage sym%ctrique \n", 130);
	printf("\t d :\tRotation gauche \n");
	printf("\t e :\tRotation droite \n");
	printf("\t f :\tPourcentage d'une couleur \n");
	printf("\t g :\tInversion des canneaux \n");
	printf("\t h :\tImage en n%cgatif \n", 130);
	printf("\t i :\tImage monochrome \n");
	printf("\t j :\tSeuillage de l'image \n");
	printf("\t k :\tAgrandissement de l'image \n");
	printf("\t l :\tR%cduction de l'image \n", 130);
	printf("\t m :\tRVB vers TSL \n");
	printf("\t n :\tTSL vers RVB \n");
	printf("\t o :\tEgalisation de l'histogramme \n");
	printf("\t p :\tFiltre moyen \n");
	printf("\t q :\tFiltre median \n");
	printf("\t r :\tContraste \n");
	printf("\t s :\tLuminosite \n");
	printf("\t t :\tSelection \n");
	printf("\t u :\tLaplacien \n");
	printf("\t v :\tSobel \n");
	printf("\t w :\tSuperpposition \n");

	printf("\t x :\tQuitter \n");

	char choix;
	printf("\n\nVotre choix: ");
	scanf("%c", &choix);
	getchar();

	printf("\nVeuillez entrer le nom de l'image %c modifier: ", 133);

	char nom[100];
	char nom_nouveau[100];
	char nom_fichier_result[200] = "resultats/";
	char commande_base[200] = "start ";
	char commande_result[200] = "start ";

	gets(nom);
	fichierimage *model = charger(nom);
	fichierimage *nouvelle;

	if (choix == 'a')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		nuancesDeGris(model, nouvelle);
	}

	else if (choix == 'b')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageMiroir(model, nouvelle);
	}

	else if (choix == 'c')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageSymetrique(model, nouvelle);
	}

	else if (choix == 'd')
	{
		nouvelle = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);
		imageRotationGauche(model, nouvelle);
	}

	else if (choix == 'e')
	{
		nouvelle = nouveau(model->entetebmp.hauteur, model->entetebmp.largeur);
		imageRotationDroite(model, nouvelle);
	}

	else if (choix == 'f')
	{
		char couleur;

		do
		{
			printf("Veuillez entrer une couleur (r, v ou b): ", 133);
			scanf("%c", &couleur);
			getchar();
		} while (couleur != 'r' || couleur != 'v' || couleur != 'b');
		printf("Il y a %lf pourcent de rouge dans cette image.\n", pourcentageCouleur(couleur, model));
	}

	else if (choix == 'g')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		inversiondescanneaux(model, nouvelle);
	}

	else if (choix == 'h')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		imageNegative(model, nouvelle);
	}

	else if (choix == 'i')
	{
		char couleur;

		do
		{
			printf("Veuillez entrer une couleur (r, g ou b): ", 133);
			scanf("%c", &couleur);
			getchar();
		} while (couleur != 'r' || couleur != 'g' || couleur != 'b');

		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		monochrome(model, couleur, nouvelle);
	}

	else if (choix == 'j')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		seuillage(model, 50, nouvelle);
	}

	else if (choix == 'k')
	{
		nouvelle = nouveau(model->entetebmp.largeur * 2, model->entetebmp.hauteur * 2);
		int taux;
		do
		{
			printf("Veuillez entrer le taux d'agransissement: ");
			scanf("%d", &taux);
		} while (taux <= 0);

		agrandissement(model, taux, nouvelle);
	}

	else if (choix == 'l')
	{
		nouvelle = nouveau(model->entetebmp.largeur / 2, model->entetebmp.hauteur / 2);
		int taux;
		do
		{
			printf("Veuillez entrer le taux de r%cduction: ", 130);
			scanf("%d", &taux);
		} while (taux <= 0);

		reduire(model, taux, nouvelle);
	}

	else if (choix == 'm')
	{
		fichierimage *t = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		fichierimage *s = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		fichierimage *l = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);

		imageRVBversTSL(model, t, s, l);

		free(t);
		free(s);
		free(l);
	}

	else if (choix == 'n')
	{
		fichierimage *rgb = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		fichierimage *t = charger("./resultats/NIDDAM_T.bmp");
		fichierimage *s = charger("./resultats/NIDDAM_S.bmp");
		fichierimage *l = charger("./resultats/NIDDAM_L.bmp");

		imageTSLversRGB(t, s, l, rgb);
		free(t);
		free(s);
		free(l);
		free(rgb);
	}

	else if (choix == 'o')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		egalisationhisto(model, nouvelle);
	}

	else if (choix == 'p')
	{
		int masque0[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
		int div0 = 9;

		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		filtre_moyen(model, nouvelle, masque0, div0);
	}

	else if (choix == 'q')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		filtre_median(model, nouvelle);
	}

	else if (choix == 'r')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		contraste(model, nouvelle, 5);
	}

	else if (choix == 's')
	{
		int pourcentage;

		do
		{
			printf("Veuillez entrer le pourcentage de luminance (>0): ");
			scanf("%d", &pourcentage);
		} while (pourcentage < 0);

		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		luminosite(model, nouvelle, pourcentage);
	}

	else if (choix == 't')
	{

		int x1, y1, x2, y2;

		do
		{
			printf("Veuillez entrer la coordonn%ce x1: ", 130);
			scanf("%d", &x1);
			getchar();
		} while (x1 < 0 || x1 >= model->entetebmp.largeur);

		do
		{
			printf("Veuillez entrer la coordonn%ce y1: ", 130);
			scanf("%d", &y1);
			getchar();
		} while (y1 < 0 || y1 >= model->entetebmp.hauteur);

		do
		{
			printf("Veuillez entrer la coordonn%ce x2: ", 130);
			scanf("%d", &x2);
			getchar();
		} while (x2 < 0 || x2 >= model->entetebmp.largeur || x2 <= x1);

		do
		{
			printf("Veuillez entrer la coordonn%ce y2: ", 130);
			scanf("%d", &y2);
			getchar();
		} while (y2 < 0 || y2 >= model->entetebmp.hauteur || y2 <= y1);

		int w = x2 - x1;
		int h = y2 - y1;

		nouvelle = nouveau(w, h);
		selection(model, nouvelle, x1, y1, x2, y2);
	}

	else if (choix == 'u')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);

		int masque0[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
		int masque1[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};

		filtre_laplacien(model, nouvelle, masque0, 8);
		filtre_laplacien(model, nouvelle, masque1, 18);
	}

	else if (choix == 'v')
	{
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);

		int sobelv[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
		int sobelh[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

		filtre_sobel(model, nouvelle, sobelv, sobelh, 8, 16);
	}

	else if (choix == 'w')
	{
		char nom[100];

		printf("Veuillez entrer le nom de la deuxieme image: ");
		gets(nom);

		fichierimage *model1 = charger(nom);
		nouvelle = nouveau(model->entetebmp.largeur, model->entetebmp.hauteur);
		superposition(model, model1, nouvelle);
	}

	strcat(commande_base, nom);

	gets(nom_nouveau);

	strcat(nom_fichier_result, nom_nouveau);

	strcat(commande_result, nom_fichier_result);

	enregistrer(nom_fichier_result, nouvelle);
	system(commande_base);
	system(commande_result);

	free(nouvelle);
	free(model);
}