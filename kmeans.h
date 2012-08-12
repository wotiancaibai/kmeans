
#ifndef KMEANS_H
#define	KMEANS_H

#include "helper.h"

// cluster number
#define CLUSTER 3

// randomly initialize the cluster center
void random_init(ad_info* users[USER_NUMBER], double centers[CLUSTER][DIM]/*, unsigned char flag[USER_NUMBER]*/);

double cluster(ad_info* users[USER_NUMBER], double centers[CLUSTER][DIM], unsigned char flag[USER_NUMBER], long iters, double valve);

#endif // KMEANS_H

