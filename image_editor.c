// Rosu Mihai Cosmin 313CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define CMAX 200

// Folosesc o structura pentru imaginile color care necesita trei valori
// pentru fiecare pixel
typedef struct{
	short r, g, b;
} rgb;

// Functia nr_chr calculeaza cati octeti trebuie sariti cand deschid imaginea
// in mod binar
int nr_chr(int a, int b)
{
	int c = 2;
	while (a > 9) {
		c++;
		a /= 10;
	}
	while (b > 9) {
		c++;
		b /= 10;
	}
	return 9 + c;
}

void free_mat(rgb **a, short **b, int t, int h)
{
	if (t == 3) {
		for (int i = 0; i < h; i++)
			free(a[i]);
		free(a);
	} else {
		for (int i = 0; i < h; i++)
			free(b[i]);
		free(b);
	}
}

void load_file(char *name, int *l, int *t, int *w, int *h, rgb ***a, short ***b)
{
	// Verific daca am mai avut alta imagine incarcata inainte, caz in care
	// trebuie sa o eliberez mai intai
	if (*l == 1) {
		free_mat(*a, *b, *t, *h);
		*l = 0;
	}
	FILE * img = fopen(name, "r");
	if (!img) {
		printf("Failed to load %s\n", name);
		return;
	}
	printf("Loaded %s\n", name);
	*l = 1;
	char aux[3];
	// Retin tipul imaginii si dimensiunile
	fscanf(img, "%s", aux);
	*t = aux[1] - 48;
	fscanf(img, "%d%d", w, h);
	int x;
	if (*t != 1 && *t != 4)
		fscanf(img, "%d", &x);
	// Verific daca trebuie sa citesc imaginea binar sau text
	// in functie de tipul ei
	if (*t < 4) {
		if (*t == 3) {
			*a = (rgb **)calloc(*h, sizeof(rgb *));
			for (int i = 0; i < *h; i++) {
				(*a)[i] = (rgb *)calloc(*w, sizeof(rgb));
				// Fac citirea pe doua randuri deoarece altfel as depasi
				// limita de 80 de caractere
				for (int j = 0; j < *w; j++) {
					fscanf(img, "%hd%hd", &(*a)[i][j].r, &(*a)[i][j].g);
					fscanf(img, "%hd", &(*a)[i][j].b);
				}
			}
		} else {
			*b = (short **)calloc(*h, sizeof(short *));
			for (int i = 0; i < *h; i++) {
				(*b)[i] = (short *)calloc(*w, sizeof(short));
				for (int j = 0; j < *w; j++)
					fscanf(img, "%hd", &(*b)[i][j]);
			}
		}
	} else {
		fclose(img);
		img = fopen(name, "rb");
		// Sar informatia deja citita, pentru a ajunge la pixelii imaginii
		fseek(img, nr_chr(*h, *w), SEEK_SET);
		*t = *t - 3;
		if (*t == 3) {
			*a = (rgb **)calloc(*h, sizeof(rgb *));
			for (int i = 0; i < *h; i++) {
				(*a)[i] = (rgb *)calloc(*w, sizeof(rgb));
				for (int j = 0; j < *w; j++) {
					fread(&(*a)[i][j].r, sizeof(char), 1, img);
					fread(&(*a)[i][j].g, sizeof(char), 1, img);
					fread(&(*a)[i][j].b, sizeof(char), 1, img);
				}
			}
		} else {
			*b = (short **)calloc(*h, sizeof(short *));
			for (int i = 0; i < *h; i++) {
				(*b)[i] = (short *)calloc(*w, sizeof(short));
				for (int j = 0; j < *w; j++)
					fread(&(*b)[i][j], sizeof(char), 1, img);
			}
		}
	}
	fclose(img);
}

void select_all(int *x1, int *y1, int *x2, int *y2, int h, int w)
{
	*x1 = 0;
	*y1 = 0;
	*x2 = w;
	*y2 = h;
}

// Functia str_int face conversia unui numar dintr-un string in int
// pe care o intoarce
int str_int(char *wrd)
{
	int nr = 0;
	for (int i = 0; i < (int)strlen(wrd); i++)
		nr = nr * 10 + wrd[i] - 48;
	return nr;
}

// Functia check_coord verifica daca coordonatele introduse sunt valide
int check_coord(int x1, int y1, int x2, int y2, int w, int h)
{
	if (x1 < 0)
		return 1;
	if (y1 < 0)
		return 1;
	if (x2 <= 0)
		return 1;
	if (y2 <= 0)
		return 1;
	if (x1 >= w)
		return 1;
	if (y1 >= h)
		return 1;
	if (x2 > w)
		return 1;
	if (y2 > h)
		return 1;
	if (x2 == x1)
		return 1;
	if (y2 == y1)
		return 1;
	return 0;
}

// Functia check_digits verifica daca parametrii comenzii SELECT
// contin numai cifre
int check_digits(char *wrd)
{
	for (int i = 0; i < (int)strlen(wrd); i++)
		if (!isdigit(wrd[i]))
			return 0;
	return 1;
}

void slct(int *x1, int *y1, int *x2, int *y2, int h, int w, char *wrd, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	wrd = strtok(NULL, " ");
	int sgn = 1;
	// Preiau parametrii comenzii SELECT
	if (!strcmp(wrd, "ALL\n") || !strcmp(wrd, "ALL")) {
		select_all(x1, y1, x2, y2, h, w);
		printf("Selected ALL\n");
	} else {
		int k = 0, crd[4];
		do {
			if (wrd[0] == '-') {
				for (int i = 0; i < (int)strlen(wrd); i++)
					wrd[i] = wrd[i + 1];
				sgn = -1;
			}
			if (!check_digits(wrd)) {
				printf("Invalid command\n");
				return;
			}
			crd[k] = str_int(wrd);
			crd[k] *= sgn;
			sgn = 1;
			wrd = strtok(NULL, " ");
			k++;
		} while (k < 3  && wrd);
		// Verific daca am primit numarul necesar de parametrii
		if (k < 3) {
			printf("Invalid command\n");
			return;
		}
		wrd[strlen(wrd) - 1] = '\0';
		if (wrd[0] == '-') {
			for (int i = 0; i < (int)strlen(wrd); i++)
				wrd[i] = wrd[i + 1];
			sgn = -1;
		}
		if (!check_digits(wrd)) {
			printf("Invalid command\n");
			return;
		}
		crd[k] = str_int(wrd);
		crd[k] *= sgn;
		// Interschimb valorile daca este cazul
		if (crd[0] > crd[2]) {
			crd[0] += crd[2];
			crd[2] = crd[0] - crd[2];
			crd[0] = crd[0] - crd[2];
		}
		if (crd[1] > crd[3]) {
			crd[1] += crd[3];
			crd[3] = crd[1] - crd[3];
			crd[1] = crd[1] - crd[3];
		}
		if (check_coord(crd[0], crd[1], crd[2], crd[3], w, h)) {
			printf("Invalid set of coordinates\n");
		} else {
			*x1 = crd[0];
			*y1 = crd[1];
			*x2 = crd[2];
			*y2 = crd[3];
			printf("Selected %d %d %d %d\n", crd[0], crd[1], crd[2], crd[3]);
		}
	}
}

void crop(int x1, int y1, int x2, int y2, int *h, int *w, short **b, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	// Pentru comanda CROP modific dimensiunile si mut selectia in coltul
	// din stanga sus
	*w = x2 - x1;
	*h = y2 - y1;
	for (int i = 0; i < *h; i++)
		for (int j = 0; j < *w; j++)
			b[i][j] = b[y1 + i][x1 + j];
	printf("Image cropped\n");
}

// Functia crop_rgb este identica cu functia crop
// doar ca este aplicata imaginilor color
void crop_rgb(int x1, int y1, int x2, int y2, int *h, int *w, rgb **a, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	*w = x2 - x1;
	*h = y2 - y1;
	for (int i = 0; i < *h; i++)
		for (int j = 0; j < *w; j++)
			a[i][j] = a[y1 + i][x1 + j];
	printf("Image cropped\n");
}

void rotate(int x1, int y1, int x2, int y2, char *wrd, short **b, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	// Preiau unghiul pentru rotire
	wrd = strtok(NULL, " ");
	int sgn = 1;
	if (wrd[0] == '-') {
		sgn = -1;
		for (int i = 0; i < (int)strlen(wrd); i++)
			wrd[i] = wrd[i + 1];
	}
	wrd[strlen(wrd) - 1] = '\0';
	int angle = str_int(wrd);
	if (angle % 90) {
		printf("Unsupported rotation angle\n");
	} else {
		if ((x2 - x1) != (y2 - y1) && angle % 180) {
			printf("The selection must be square\n");
		} else {
			angle *= sgn;
			if (angle == 360 || angle == 0 || angle == -360) {
				printf("Rotated %d\n", angle);
				return;
			}
			int h = y2 - y1;
			int w = x2 - x1;
			// Fac o copie a selectiei pentru a o roti mai usor
			short **cop = (short **)calloc(h, sizeof(short *));
			for (int i = 0; i < h; i++) {
				cop[i] = (short *)calloc(w, sizeof(short));
				for (int j = 0; j < w; j++)
					cop[i][j] = b[y1 + i][x1 + j];
			}
			// Rotesc matricea in-place
			if (angle == 90 || angle == -270) {
				for (int i = 0; i < h / 2; i++)
					for (int j = i; j < h - 1 - i; j++) {
						int aux = cop[h - 1 - j][i];
						cop[h - 1 - j][i] = cop[h - 1 - i][h - 1 - j];
						cop[h - 1 - i][h - 1 - j] = cop[j][h - 1 - i];
						cop[j][h - 1 - i] = cop[i][j];
						cop[i][j] = aux;
					}
			}
			if (angle == 270 || angle == -90) {
				for (int i = 0; i < h / 2; i++)
					for (int j = i; j < h - 1 - i; j++) {
						int aux = cop[i][j];
						cop[i][j] = cop[j][h - 1 - i];
						cop[j][h - 1 - i] = cop[h - 1 - i][h - 1 - j];
						cop[h - 1 - i][h - 1 - j] = cop[h - 1 - j][i];
						cop[h - 1 - j][i] = aux;
					}
			}
			if (angle == 180 || angle == -180) {
				for (int i = 0; i < h / 2; i++)
					for (int j = 0; j < w; j++) {
						int aux = cop[i][j];
						cop[i][j] = cop[h - i - 1][w - j - 1];
						cop[h - i - 1][w - j - 1] = aux;
					}
				if (h % 2)
					for (int j = 0; j < w / 2; j++) {
						int aux = cop[h / 2][j];
						cop[h / 2][j] = cop[h / 2][w - j - 1];
						cop[h / 2][w - j - 1] = aux;
					}
			}
			// Trec valorile copiei inapoi in matricea originala
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					b[y1 + i][x1 + j] = cop[i][j];
			for (int i = 0; i < h; i++)
				free(cop[i]);
			free(cop);
			printf("Rotated %d\n", angle);
		}
	}
}

// Functia rotate_rgb este identica cu functia rotate
// doar ca este aplicata imaginilor color
void rotate_rgb(int x1, int y1, int x2, int y2, char *wrd, rgb **a, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	wrd = strtok(NULL, " ");
	int sgn = 1;
	if (wrd[0] == '-') {
		sgn = -1;
		for (int i = 0; i < (int)strlen(wrd); i++)
			wrd[i] = wrd[i + 1];
	}
	wrd[strlen(wrd) - 1] = '\0';
	int angle = str_int(wrd);
	if (angle % 90) {
		printf("Unsupported rotation angle\n");
	} else {
		if ((x2 - x1) != (y2 - y1) && angle % 180) {
			printf("The selection must be square\n");
		} else {
			angle *= sgn;
			if (angle == 360 || angle == 0 || angle == -360) {
				printf("Rotated %d\n", angle);
				return;
			}
			int h = y2 - y1;
			int w = x2 - x1;
			rgb **cop = (rgb **)calloc(h, sizeof(rgb *));
			for (int i = 0; i < h; i++) {
				cop[i] = (rgb *)calloc(w, sizeof(rgb));
				for (int j = 0; j < w; j++)
					cop[i][j] = a[y1 + i][x1 + j];
			}
			if (angle == 90 || angle == -270) {
				for (int i = 0; i < h / 2; i++)
					for (int j = i; j < h - 1 - i; j++) {
						rgb aux = cop[h - 1 - j][i];
						cop[h - 1 - j][i] = cop[h - 1 - i][h - 1 - j];
						cop[h - 1 - i][h - 1 - j] = cop[j][h - 1 - i];
						cop[j][h - 1 - i] = cop[i][j];
						cop[i][j] = aux;
					}
			}
			if (angle == 270 || angle == -90) {
				for (int i = 0; i < h / 2; i++)
					for (int j = i; j < h - 1 - i; j++) {
						rgb aux = cop[i][j];
						cop[i][j] = cop[j][h - 1 - i];
						cop[j][h - 1 - i] = cop[h - 1 - i][h - 1 - j];
						cop[h - 1 - i][h - 1 - j] = cop[h - 1 - j][i];
						cop[h - 1 - j][i] = aux;
					}
			}
			if (angle == 180 || angle == -180) {
				for (int i = 0; i < h / 2; i++)
					for (int j = 0; j < w; j++) {
						rgb aux = cop[i][j];
						cop[i][j] = cop[h - i - 1][w - j - 1];
						cop[h - i - 1][w - j - 1] = aux;
					}
				if (h % 2)
					for (int j = 0; j < w / 2; j++) {
						rgb aux = cop[h / 2][j];
						cop[h / 2][j] = cop[h / 2][w - j - 1];
						cop[h / 2][w - j - 1] = aux;
					}
			}
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					a[y1 + i][x1 + j] = cop[i][j];
			for (int i = 0; i < h; i++)
				free(cop[i]);
			free(cop);
			printf("Rotated %d\n", angle);
		}
	}
}

void save_img(int l, char *wrd, int t, short **b, rgb **a, int h, int w)
{
	if (!l) {
		printf("No image loaded\n");
	} else {
		wrd = strtok(NULL, " ");
		// Preiau numele fisierului
		char *name = (char *)calloc(strlen(wrd) + 1, sizeof(char));
		strcpy(name, wrd);
		if (name[strlen(name) - 1] == '\n')
			name[strlen(name) - 1] = '\0';
		wrd = strtok(NULL, " ");
		// Verific daca exista parametrul optional si daca este "ascii"
		if (wrd && (!strcmp(wrd, "ascii") || !strcmp(wrd, "ascii\n"))) {
			FILE *img = fopen(name, "w");
			fprintf(img, "P%d\n%d %d\n", t, w, h);
			if (t != 1)
				fprintf(img, "255\n");
			if (t == 3)
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						fprintf(img, "%d %d ", a[i][j].r, a[i][j].g);
						fprintf(img, "%d ", a[i][j].b);
					}
					fprintf(img, "\n");
				}
			else
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++)
						fprintf(img, "%d ", b[i][j]);
					fprintf(img, "\n");
				}
			fclose(img);
		} else {
			FILE *img = fopen(name, "w");
			fprintf(img, "P%d\n%d %d\n", t + 3, w, h);
			if (t != 1)
				fprintf(img, "255\n");
			fclose(img);
			img = fopen(name, "ab");
			if (t == 3)
				for (int i = 0; i < h; i++)
					for (int j = 0; j < w; j++) {
						fwrite(&a[i][j].r, sizeof(char), 1, img);
						fwrite(&a[i][j].g, sizeof(char), 1, img);
						fwrite(&a[i][j].b, sizeof(char), 1, img);
					}
			else
				for (int i = 0; i < h; i++)
					for (int j = 0; j < w; j++)
						fwrite(&b[i][j], sizeof(char), 1, img);
			fclose(img);
		}
		printf("Saved %s\n", name);
		free(name);
	}
}

// Functia aprox intoarce cel mai apropiat intreg de n
int aprox(double n)
{
	n *= 10;
	int aux = (int)n;
	return (aux % 10 >= 5) ? aux / 10 + 1 : aux / 10;
}

void graysc(rgb **a, int x1, int y1, int x2, int y2, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++) {
			double aux = (double)(a[i][j].r + a[i][j].g + a[i][j].b) / 3;
			a[i][j].r = aprox(aux);
			a[i][j].g = aprox(aux);
			a[i][j].b = aprox(aux);
		}
	printf("Grayscale filter applied\n");
}

// Functia range verifica daca valoarea lui n iese din raza de [0, 255]
// caz in care intoarce 255, altfel intoarce valoarea lui n
double range(double n)
{
	return (n > 255.0) ? 255.0 : n;
}

void sepia(rgb **a, int x1, int y1, int x2, int y2, int l)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++) {
			double red, green, blue;
			red = 0.393 * a[i][j].r + 0.769 * a[i][j].g + 0.189 * a[i][j].b;
			green = 0.349 * a[i][j].r + 0.686 * a[i][j].g + 0.168 * a[i][j].b;
			blue = 0.272 * a[i][j].r + 0.534 * a[i][j].g + 0.131 * a[i][j].b;
			red = range(red);
			green = range(green);
			blue = range(blue);
			a[i][j].r = aprox(red);
			a[i][j].g = aprox(green);
			a[i][j].b = aprox(blue);
		}
	printf("Sepia filter applied\n");
}

// Functia rot_full_rgb este folosita pentru rotirea intregii imagini
void rot_full_rgb(int *w, int *h, rgb ***a, char *wrd, int l, int *org_h)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	// Folosesc o matrice auxiliara pentru a o roti mai usor
	rgb **cop = (rgb **)calloc(*h, sizeof(rgb *));
	for (int i = 0; i < *h; i++) {
		cop[i] = (rgb *)calloc(*w, sizeof(rgb));
		for (int j = 0; j < *w; j++)
			cop[i][j] = (*a)[i][j];
	}
	int og_h = *h;
	wrd = strtok(NULL, " ");
	int sgn = 1;
	if (wrd[0] == '-') {
		sgn = -1;
		for (int i = 0; i < (int)strlen(wrd); i++)
			wrd[i] = wrd[i + 1];
	}
	wrd[strlen(wrd) - 1] = '\0';
	int angle = str_int(wrd);
	angle *= sgn;
	if (angle % 90) {
		printf("Unsupported rotation angle\n");
	} else {
		if (angle == 360 || angle == 0 || angle == -360) {
			printf("Rotated %d\n", angle);
			for (int i = 0; i < og_h; i++)
				free(cop[i]);
			free(cop);
			return;
		}
		if (angle == 180 || angle == -180) {
			for (int i = 0; i < *h / 2; i++)
				for (int j = 0; j < *w; j++) {
					rgb aux = cop[i][j];
					cop[i][j] = cop[*h - i - 1][*w - j - 1];
					cop[*h - i - 1][*w - j - 1] = aux;
				}
			if (*h % 2)
				for (int j = 0; j < *w / 2; j++) {
					rgb aux = cop[*h / 2][j];
					cop[*h / 2][j] = cop[*h / 2][*w - j - 1];
					cop[*h / 2][*w - j - 1] = aux;
				}
			for (int i = 0; i < *h; i++)
				for (int j = 0; j < *w; j++)
					(*a)[i][j] = cop[i][j];
		} else {
			// Realoc matricea daca este cazul
			for (int i = 0; i < *org_h; i++)
				free((*a)[i]);
			free(*a);
			*a = (rgb **)calloc(*w, sizeof(rgb *));
			if (angle == 90 || angle == -270) {
				for (int i = 0; i < *w; i++) {
					(*a)[i] = (rgb *)calloc(*h, sizeof(rgb));
					for (int j = 0; j < *h; j++)
						(*a)[i][j] = cop[*h - j - 1][i];
				}
			}
			if (angle == -90 || angle == 270) {
				for (int i = 0; i < *w; i++) {
					(*a)[i] = (rgb *)calloc(*h, sizeof(rgb));
					for (int j = 0; j < *h; j++)
						(*a)[i][j] = cop[j][*w - i - 1];
				}
			}
			// Modific noile dimensiuni ale imaginii
			int aux = *w;
			*w = *h;
			*h = aux;
			*org_h = *h;
		}
		printf("Rotated %d\n", angle);
	}
	for (int i = 0; i < og_h; i++)
		free(cop[i]);
	free(cop);
}

// Functia rot_full este identica cu functia rot_full_rgb
// doar ca este folosita pentru imagini grayscale
void rot_full(int *w, int *h, short ***b, char *wrd, int l, int *org_h)
{
	if (!l) {
		printf("No image loaded\n");
		return;
	}
	short **cop = (short **)calloc(*h, sizeof(short *));
	for (int i = 0; i < *h; i++) {
		cop[i] = (short *)calloc(*w, sizeof(short));
		for (int j = 0; j < *w; j++)
			cop[i][j] = (*b)[i][j];
	}
	int og_h = *h;
	wrd = strtok(NULL, " ");
	int sgn = 1;
	if (wrd[0] == '-') {
		sgn = -1;
		for (int i = 0; i < (int)strlen(wrd); i++)
			wrd[i] = wrd[i + 1];
	}
	wrd[strlen(wrd) - 1] = '\0';
	int angle = str_int(wrd);
	angle *= sgn;
	if (angle % 90) {
		printf("Unsupported rotation angle\n");
	} else {
		if (angle == 360 || angle == 0 || angle == -360) {
			printf("Rotated %d\n", angle);
			for (int i = 0; i < og_h; i++)
				free(cop[i]);
			free(cop);
			return;
		}
		if (angle == 180 || angle == -180) {
			for (int i = 0; i < *h / 2; i++)
				for (int j = 0; j < *w; j++) {
					int aux = cop[i][j];
					cop[i][j] = cop[*h - i - 1][*w - j - 1];
					cop[*h - i - 1][*w - j - 1] = aux;
				}
			if (*h % 2)
				for (int j = 0; j < *w / 2; j++) {
					int aux = cop[*h / 2][j];
					cop[*h / 2][j] = cop[*h / 2][*w - j - 1];
					cop[*h / 2][*w - j - 1] = aux;
				}
			for (int i = 0; i < *h; i++)
				for (int j = 0; j < *w; j++)
					(*b)[i][j] = cop[i][j];
		} else {
			for (int i = 0; i < *org_h; i++)
				free((*b)[i]);
			free(*b);
			*b = (short **)calloc(*w, sizeof(short *));
			if (angle == 90 || angle == -270) {
				for (int i = 0; i < *w; i++) {
					(*b)[i] = (short *)calloc(*h, sizeof(short));
					for (int j = 0; j < *h; j++)
						(*b)[i][j] = cop[*h - j - 1][i];
				}
			}
			if (angle == -90 || angle == 270) {
				for (int i = 0; i < *w; i++) {
					(*b)[i] = (short *)calloc(*h, sizeof(short));
					for (int j = 0; j < *h; j++)
						(*b)[i][j] = cop[j][*w - i - 1];
				}
			}
			int aux = *w;
			*w = *h;
			*h = aux;
			*org_h = *h;
		}
		printf("Rotated %d\n", angle);
	}
	for (int i = 0; i < og_h; i++)
		free(cop[i]);
	free(cop);
}

int main(void)
{
	char *command = (char *)calloc(CMAX, sizeof(char));
	rgb **matp3;
	short **mat;
	int type = -1, height = 0, width = 0, loaded = 0, grscl = 0;
	int x1 = -1, x2 = 0, y1 = -1, y2 = 0;
	int og_h = 0, valid_cmd;
	while (1) {
		fgets(command, CMAX, stdin);
		valid_cmd = 0;
		// Citesc comenzile si verific daca sunt valide / de ce tip sunt
		char *wrd = strtok(command, " ");
		if (!strcmp(wrd, "LOAD")) {
			valid_cmd = 1;
			wrd = strtok(NULL, " ");
			wrd[strlen(wrd) - 1] = '\0';
			height = og_h;
			load_file(wrd, &loaded, &type, &width, &height, &matp3, &mat);
			if (!loaded)
				continue;
			og_h = height;
			select_all(&x1, &y1, &x2, &y2, height, width);
			if (type != 3)
				grscl = 1;
			else
				grscl = 0;
		} else if (!strcmp(wrd, "SELECT")) {
			valid_cmd = 1;
			slct(&x1, &y1, &x2, &y2, height, width, wrd, loaded);
		} else if (!strcmp(wrd, "CROP\n")) {
			valid_cmd = 1;
			if (type == 3)
				crop_rgb(x1, y1, x2, y2, &height, &width, matp3, loaded);
			else
				crop(x1, y1, x2, y2, &height, &width, mat, loaded);

		} else if (!strcmp(wrd, "ROTATE")) {
			valid_cmd = 1;
			if (x2 - x1 == width && y2 - y1 == height) {
				if (type == 3)
					rot_full_rgb(&width, &height, &matp3, wrd, loaded, &og_h);
				else
					rot_full(&width, &height, &mat, wrd, loaded, &og_h);
				select_all(&x1, &y1, &x2, &y2, height, width);
			} else {
				if (type != 3)
					rotate(x1, y1, x2, y2, wrd, mat, loaded);
				else
					rotate_rgb(x1, y1, x2, y2, wrd, matp3, loaded);
			}
		} else if (!strcmp(wrd, "SAVE")) {
			valid_cmd = 1;
			save_img(loaded, wrd, type, mat, matp3, height, width);
		} else if (!strcmp(wrd, "GRAYSCALE\n")) {
			valid_cmd = 1;
			if (!grscl)
				graysc(matp3, x1, y1, x2, y2, loaded);
			else
				printf("Grayscale filter not available\n");
		} else if (!strcmp(wrd, "SEPIA\n")) {
			valid_cmd = 1;
			if (!grscl)
				sepia(matp3, x1, y1, x2, y2, loaded);
			else
				printf("Sepia filter not available\n");
		} else if (!strcmp(wrd, "EXIT\n") || !strcmp(wrd, "EXIT")) {
			valid_cmd = 1;
			if (loaded == 0)
				printf("No image loaded\n");
			break;
		}
		if (valid_cmd == 0)
			printf("Invalid command\n");
	}
	free(command);
	if (loaded)
		free_mat(matp3, mat, type, og_h);
	return 0;
}
