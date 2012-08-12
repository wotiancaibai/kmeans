#include "kmeans.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void random_init(ad_info* users[USER_NUMBER], double centers[CLUSTER][DIM]/*, unsigned char flag[USER_NUMBER]*/) {
    int i, j;
    int is_same; // a flag which determine this user's behavior is the same as one's of before
    long index;
    long indexes[CLUSTER]; // just for debug - show which indexes were chosen
    ad_info* p;
    double min_dis, temp_dis;
    long counts[CLUSTER]; // how many users in each cluster
    
    clock_t start, finish;
    double duration;
    start = clock();
    printf("-Start initialing:\n");
    // set to 0s
    for (i = 0; i < CLUSTER; ++i) {
        for (j = 0; j < DIM; ++j) {
            centers[i][j] = 0;
        }
    }
    
    // initialize random number generator
    srand(time(0));
    // initialize the cluster centers
    index = rand() % USER_NUMBER;
    indexes[0] = index;
    p = users[index];

    printf(" - Start generate random cluster centers:\n");
    while (p) {
        centers[0][p->ad_id] = p->ad_clicked;
        p = p->next;
    }
    for (i = 1; i < CLUSTER; ++i) {
        index = rand() % USER_NUMBER;
        p = users[index];
        is_same = 0;
        for (j = 0; j < i; ++j) {
            temp_dis = get_kl_distance(centers[j], p);
            if (temp_dis == 0) {
                is_same = 1;
                --i; // roll back and generator a new one
                break;
            }
        }
        if (!is_same) {
            indexes[i] = index;
            while (p) {
                centers[i][p->ad_id] = p->ad_clicked;
                p = p->next;
            }
        }
    }
    
    /*
    for (i = 0; i < USER_NUMBER; ++i) {
        min_dis = 1e15;
        for (j = 0; j < CLUSTER; ++j) {
            temp_dis = get_kl_distance(centers[j], users[i]);
            if (temp_dis < min_dis) {
                min_dis = temp_dis;
                flag[i] = j;
            }
        }
    }

    printf(" - Recalculate the centers:\n");
    // set centers to 0s
    for (i = 0; i < CLUSTER; ++i) {
        counts[i] = 0;
        for (j = 0; j < DIM; ++j) {
            centers[i][j] = 0;
        }
    }
    
    // calculate the center of the clusters
    for (i = 0; i < USER_NUMBER; ++i) {
        j = flag[i]; // the ith user is in cluster j
        ++counts[j];
        p = users[i];
        while (p) {
            centers[j][p->ad_id] += p->ad_clicked;
            p = p->next;
        }
    }
    for (i = 0; i < CLUSTER; ++i) {
        for (j = 0; j < DIM; ++j) {
            if (counts[i])
                centers[i][j] /= counts[i];
        }
    }
    */
    finish = clock();
    // how much time did it cost
    duration = (double)(finish - start)/CLOCKS_PER_SEC/60;
    printf(" - Duration = %.2f min\n", duration);
    printf("-Initialing done.\n\n");
        
}

double cluster(ad_info* users[USER_NUMBER], double centers[CLUSTER][DIM], unsigned char flag[USER_NUMBER], long iters, double valve) {
    int i, j;
    long iter = 0;
    long counts[CLUSTER]; // how many users in each cluster
    ad_info* p;
    double temp_dis, min_dis, total_dis, last_total_dis;
    
    last_total_dis = 1e15;
    
    clock_t start, finish;
    double duration;
    start = clock();
    
    printf("-Start clustering:\n");
    while (iter++ < iters) {
        printf(" - %ld clusters\n", iter);
        total_dis = 0;
        for (i = 0; i < USER_NUMBER; ++i) {
            min_dis = 1e15;
            for (j = 0; j < CLUSTER; ++j) {
                temp_dis = get_kl_distance(centers[j], users[i]);
                if (temp_dis < min_dis) {
                    min_dis = temp_dis;
                    flag[i] = j;
                }
            }
            // calculate the total distance
            total_dis += min_dis;
        }
        
        // set centers to 0s
        for (i = 0; i < CLUSTER; ++i) {
            counts[i] = 0;
            for (j = 0; j < DIM; ++j) {
                centers[i][j] = 0;
            }
        }
        // update the center of the clusters
        for (i = 0; i < USER_NUMBER; ++i) {
            j = flag[i]; // the ith user is in cluster j
            ++counts[j];
            p = users[i];
            while (p) {
                centers[j][p->ad_id] += p->ad_clicked;
                p = p->next;
            }
        }
        for (i = 0; i < CLUSTER; ++i) {
            if (counts[i]) {
                for (j = 0; j < DIM; ++j) {
                    centers[i][j] /= counts[i];
                }
            }
            else {
                printf("error\n");
            }
        }
        
        if (last_total_dis < total_dis) {
            printf("    - last_total_dis < total_dis, quit clustering\n");
	    // last_total_dis = total_dis;
            // continue;
            break;
        }
        // if the change of total_dis is small enough, quit
        if (last_total_dis - total_dis <= valve) {
            printf("    - last_total_dis - total_dis <= valve, quit clustering\n");
            break;
        }
        
        last_total_dis = total_dis;
    }

    finish = clock();
    // how much time did it cost
    duration = (double)(finish - start)/CLOCKS_PER_SEC/60;
    printf(" - Duration = %.2f min\n", duration);
    printf("-Clustering done.\n\n");
	
	// return the total error
	return total_dis;
}