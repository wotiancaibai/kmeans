#include "kmeans.h"
#include <stdio.h>
#include <stdlib.h>

int main(int args, char** argv) {

    if (args != 2) {
        printf("Argument should contain a file\n");
        return 1;
    }

    ad_info* users[USER_NUMBER];
    // Read users' click information from text
    if (!read_file(argv[1], users)) {
        printf("failed to read users\n");
        return 1;
    }
        
    double centers[CLUSTER][DIM];
    unsigned char flag[USER_NUMBER];

    random_init(users, centers/*, flag*/);
    cluster(users, centers, flag, 1000/*iters*/, 0.000001/*valve*/);
    
    int i;
    FILE *fp;
    fp = fopen("cat_output.txt", "w");
    for (i = 0; i < USER_NUMBER; ++i) {
        fprintf(fp, "%d\n", flag[i]);
    }
        
	//system("pause");
	//printf("%ld times get_dis cost %f seconds\n", get_dis_count, get_dis_time_cost);
	return 0;
}
