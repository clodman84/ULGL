#include <stdio.h>
#include "matrix.h"

void app_main(void)
{
	matrix a = make_matrix(3, 3);

	a.data[idx(0, 1, &a)] = 5;
	a.data[idx(0, 2, &a)] = 5;
	a.data[idx(0, 3, &a)] = 5;
	a.data[idx(1, 1, &a)] = 6;

	show(&a);
	printf("\n");
	transpose(&a);
	scale(&a, 5);
	show(&a);

	destroy_matrix(&a);
}
