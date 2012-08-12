#include "helper.h"
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>

/*
#ifdef linux
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

double getTimeval()
{
	struct rusage stRusage;
	struct timeval stTimeval;
	getrusage(RUSAGE_SELF, &stRusage);
	stTimeval = stRusage.ru_utime;
	return stTimeval.tv_sec + (double)stTimeval.tv_usec*1E-6;
}
#endif
 */

double bi_kl_distance(double* p, double* q, int n) {
	int i;
	double dis = 0;
	for (i = 0; i < n; ++i) {
		dis += p[i]*log(q[i]/p[i]) + q[i]*log(p[i]/q[i]);
	}
	return (-dis) > 0 ? (-dis) : .0f;
}

void normalize(double* p, int n) {
	int i;
	double sum = 0;
	for (i = 0; i < n; ++i) {
		sum += p[i];
	}
	for (i = 0; i < n; ++i) {
		p[i] /= sum;
	}
}

/*
double get_distance(ad_info* user1, ad_info* user2) {
	int i;
	double u1[DIM] = {0}, u2[DIM] = {0};
	
	flatten_user(user1, u1);
	flatten_user(user2, u2);
	
	// laplace smoothing
	for (i = 0; i < DIM; ++i) {
		u1[i] += 0.1;
		u2[i] += 0.1;
	}

	// normalization
	normalize(u1, DIM);
	normalize(u2, DIM);

	return bi_kl_distance(u1, u2, DIM);
}
*/

double get_kl_distance(double* center, ad_info* user) {
	int i;
	double user1[DIM] = {0}, user2[DIM] = {0};
	double dis;
	
	/*
	clock_t start, finish;
	double duration;
	start = clock();
	 */

	flatten_user(user, user1);
	// Laplace smoothing
	for (i = 0; i < DIM; ++i) {
		user1[i] += LAPLACE_FACTOR;
        	user2[i] = center[i] + LAPLACE_FACTOR;
	}

	// normalization
	normalize(user1, DIM);
	normalize(user2, DIM);

	dis = bi_kl_distance(user1, user2, DIM);
	/*
	finish = clock();
	get_dis_time_cost += (double)(finish - start)/CLOCKS_PER_SEC;
	printf("%%finish:%ld - %%start:%ld\n", finish, start);
	get_dis_count++;
	 */
	
	return dis;
}

int read_file(char* filename, ad_info* users[]) {
	FILE *fp;
	long qq;
	int i, j;
	int temp;
	ad_info* info_p;
	//ad_info* current_p;
	
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Can't open file %s\n", filename);
		return 0;
	}
	
    clock_t start, finish;
    double duration;
    start = clock();
    
	// read file - without file format detecting
    printf("-Start reading file:\n");
	for (i = 0; i < USER_NUMBER; ++i) {
		// print the progress
		if (i % 100000 == 0) {
			printf(" - progress: %6.2f%%\n", 100*(float)i/USER_NUMBER);
		}
		
		info_p = NULL; // set to NULL at the very beginning
		fscanf(fp, "%ld", &qq);
		// printf("%ld", qq);
		for (j = 0; j < DIM; ++j) {
			fscanf(fp, ",%i", &temp);
			if (temp > 0) {
				if (info_p) {
					info_p->next = (ad_info*)malloc(sizeof(ad_info));
					info_p = info_p->next;
					info_p->ad_id = j;
					info_p->ad_clicked = temp;
					info_p->next = NULL;
				}
				else { // if it's the first time, then set users[i] to info_p
					info_p = (ad_info*)malloc(sizeof(ad_info));
					users[i] = info_p;
					info_p->ad_id = j;
					info_p->ad_clicked = temp;
					info_p->next = NULL;
				}
			}
		}
	}

	fclose(fp);
    printf(" - progress: 100.00%%\n");
    
    finish = clock();
    // how much time did it cost
    duration = (double)(finish - start)/CLOCKS_PER_SEC/60;
    printf(" - Duration = %.2f min\n", duration);
    
    printf("-Reading file done.\n\n");
	return 1; // success
}

void flatten_user(ad_info* user, double* user_flattened) {
	ad_info* info_p;
	int index;
	
	info_p = user;
	while(info_p) {
		index = info_p->ad_id;
		user_flattened[index] = (double)info_p->ad_clicked;
		// printf("%f\n", user_flattened[index]);
		info_p = info_p->next;
	}
}

