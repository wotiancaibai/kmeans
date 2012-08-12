
#ifndef HELPER_H
#define HELPER_H

#define DIM 76
#define USER_NUMBER /*29697981*/10 /*1000*/
#define LAPLACE_FACTOR 0.1

/*
// Calculate the time cost for get_distance function
static double get_dis_time_cost = 0;
static long get_dis_count = 0;
 */

typedef struct ad_info {
	int ad_id;
	int ad_clicked;
	struct ad_info * next;
} ad_info;

double bi_kl_distance(double*, double*, int); // KL divergence
void normalize(double*, int); // normalize the array elements to [0, 1]
double get_kl_distance(double*, ad_info*); // flatten the data and get KL divergence between two users
// double get_distance(ad_info*, ad_info*); // flatten the data and get KL divergence between two users
int read_file(char*, ad_info**);
void flatten_user(ad_info*, double*);
#endif // HELPER_H

