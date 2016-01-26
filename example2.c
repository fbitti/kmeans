#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kmeans.h"

typedef struct point
{
	double x;
	double y;
} point;
	
	
static double pt_distance(const Pointer a, const Pointer b) 
{
	point *pa = (point*)a;
	point *pb = (point*)b;
	
	double dx = (pa->x - pb->x);
	double dy = (pa->y - pb->y);
	
	return dx*dx + dy*dy;
}

static void pt_centroid(const Pointer * objs, size_t num_objs, Pointer centroid)
{
	int i;
	point sum;
	point **pts = (point**)objs;
	point *center = (point*)centroid;

	sum.x = sum.y = 0.0;
	
	if (num_objs <= 0) return;
	
	for (i = 0; i < num_objs; i++)
	{
		sum.x += pts[i]->x;
		sum.y += pts[i]->y;
	}
	sum.x /= num_objs;
	sum.y /= num_objs;
	*center = sum;
	return;
}

int
main(int nargs, char **args)
{
	kmeans_config config;
	kmeans_result result;
	int i, j;
	int nptsincluster = 10000;
	int spread = 3;
	point *pts;
	point *init;
	
	srandom(time(NULL));

	/* Constants */
	config.k = 10;
	config.num_objs = config.k * nptsincluster;
	config.max_iterations = 100;
	config.distance_method = pt_distance;
	config.centroid_method = pt_centroid;

	/* Inputs for K-means */
	config.objs = calloc(config.num_objs, sizeof(Pointer));
	config.centers = calloc(config.k, sizeof(Pointer));
	config.clusters = calloc(config.num_objs, sizeof(int));

	/* Storage for raw data */
	pts = calloc(config.num_objs, sizeof(point));
	init = calloc(config.k, sizeof(point));

	/* Populate with K gaussian clusters of data */
	for (j = 0; j < config.k; j++) {
		for (i = 0; i < nptsincluster; i++)
		{
			double u1 = 1.0 * random() / RAND_MAX;
			double u2 = 1.0 * random() / RAND_MAX;
			double z1 = spread * j + sqrt(-2*log2(u1))*cos(2*M_PI*u2);
			double z2 = spread * j + sqrt(-2*log2(u1))*sin(2*M_PI*u2);	
			int n = j*nptsincluster + i;
			
			/* Populate raw data */
			pts[n].x = z1;
			pts[n].y = z2;
			
			/* Pointer to raw data */
			config.objs[n] = &(pts[n]);
		}
	}

	/* Populate the initial means vector */
	for (i = 0; i < config.k; i++) 
	{
		int r = lround(config.num_objs * (1.0 * random() / RAND_MAX));
		/* Populate raw data */
		init[i] = pts[r];
		/* Pointers to raw data */
		config.centers[i] = &(init[i]);
		// printf("center[%d] %g %g\n", i, init[i].x, init[i].y);
	}
	
	/* run k-means! */
	result = kmeans(&config);
	
	/* print results */
	for (i = 0; i < config.num_objs; i++)
	{
		point *pt = (point*)(config.objs[i]);
		
		if (config.objs[i])
			printf("%g\t%g\t%d\n", pt->x, pt->y, config.clusters[i]);
		else
			printf("N\tN\t%d\n", config.clusters[i]);
	}
	
	free(config.objs);
	free(config.clusters);
	free(config.centers);
	
	free(init);
	free(pts);
	
}

