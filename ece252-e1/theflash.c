#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>


#define CITIES_LENGTH 7
#define NUM_CITIES (CITIES_LENGTH - 1)

int pid = 1;
int dist;
FILE * fp;


static const char* cities[] = { "Central City", "Starling City", "Gotham City", "Metropolis", "Coast City", "National City" };

const int distances[CITIES_LENGTH - 1][ CITIES_LENGTH - 1] = {
    {0, 793, 802, 254, 616, 918},
    {793, 0, 197, 313, 802, 500},
    {802, 197, 0, 496, 227, 198},
    {254, 313, 496, 0, 121, 110},
    {616, 802, 227, 121, 0, 127},
    {918, 500, 198, 110, 127, 0}
};

int initial_vector[CITIES_LENGTH] = { 0, 1, 2, 3, 4, 5, 0 };

typedef struct {
    int cities[CITIES_LENGTH];
    int total_dist; 
} route;

void print_route ( route* r ) {
    printf ("Route: ");
    for ( int i = 0; i < CITIES_LENGTH; i++ ) {
        if ( i == CITIES_LENGTH - 1 ) {
            printf( "%s\n", cities[r->cities[i]] );
        } else {
            printf( "%s - ", cities[r->cities[i]] );
        }
    }
}

void calculate_distance( route* r ) {

        if ( r->cities[0] != 0 ) {
            printf( "Route must start with %s (but was %s)!\n", cities[0], cities[r->cities[0]]);
            exit( -1 );
        } 
        if ( r->cities[6] != 0 ) {
            printf( "Route must end with %s (but was %s)!\n", cities[0], cities[r->cities[6]]);
            exit ( -2 );
        }
        int distance = 0;
        for ( int i = 1; i < CITIES_LENGTH; i++ ) {
            int to_add = distances[r->cities[i-1]][r->cities[i]];
            if ( to_add == 0 ) {
                printf( "Route cannot have a zero distance segment.\n");
                exit ( -3 );
            }
            distance += to_add;
        }

        fwrite(&distance, sizeof(int), 1, fp);
        fseek(fp, SEEK_SET, 0);
        //dist = distance;
}

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void permute(route* r, int left, int right, route* best) {

    int buf = 0;

    if (left == right) {

        if ((pid = fork()) == 0) {
            calculate_distance(r);
        }

        if (pid == 0) {
            return;
        }

        wait(NULL);

        fread(&buf, sizeof(int), 1, fp);
        fseek(fp, SEEK_SET, 0);

        r->total_dist = buf; //shared int variable

        if (r->total_dist < best->total_dist) {
            memcpy(best, r, sizeof(route));
        }

        return;
    }

    for (int i = left; i <= right; i++) {
        swap(&r->cities[left], &r->cities[i]);
        permute(r, left + 1, right, best);
        swap(&r->cities[left], &r->cities[i]);
    }
}   

void assign_best(route** best, route* candidate) {
    if (*best == NULL) {
        *best = candidate;
        return;
    }

    int a = candidate->total_dist;
    int b = (*best)->total_dist;

    if (a < b) {
        free(*best);
        *best = candidate;
    } else {
        free(candidate);
    }
}

route* find_best_route( ) {
    route* candidate = malloc( sizeof(route) );
    memcpy (candidate->cities, initial_vector, CITIES_LENGTH * sizeof( int ));
    candidate->total_dist = 0;

    route* best = malloc( sizeof(route) );
    memset( best, 0, sizeof(route) );
    best->total_dist = 999999;

    permute( candidate, 1, 5, best );

    if (pid == 0) {
        
        printf("Checkpoint ");

        return best;
    }

    free( candidate );
    return best;
}

int main( int argc, char** argv ) {

    //dist = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);


    fp = fopen("file.c", "r+");

    //fwrite(&i, sizeof(int), 1, fp);
    //fseek(fp, SEEK_SET, 0);

    //int test;

    //fread(&test, sizeof(int), 1, fp);
    //printf("%d\n", test);

    route * best = find_best_route( );

    if (pid == 0) {
        return 0;
    }

    print_route( best );
    printf( "Distance: %d\n", best->total_dist ); 
    free( best );

    fclose(fp);

    return 0;

}

