#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void print_matrix(int n, int m, double matrix[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("[%.2f]\t", matrix[i][j]);
        }
        printf("\n");
    }
}


double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}


void distance_matrix(int n, double x[n], double y[n], double matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = distance(x[i], y[i], x[j], y[j]);
        }
    }
}


void saving_matrix(int n, double matrix[n][n], double saving[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            saving[i][j] = matrix[0][i] + matrix[0][j] - matrix[i][j];
        }
    }
}


void max_index(int n, double matrix[n][n], int *i, int *j) {
    double max = -1;
    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
            if (matrix[p][q] > max) {
                max = matrix[p][q];
                *i = p;
                *j = q;
            }
        }
    }
}

void combine(int n, double saving[n][n], double demand[n], double capacity, int route[n], int *k) {
    int i, j;
    max_index(n, saving, &i, &j); 
    if (demand[i] + demand[j] <= capacity) { 
        route[*k] = i; 
        route[*k + 1] = j; 
        *k = *k + 2; 
        for (int p = 0; p < n; p++) { 
            saving[i][p] = -1;
            saving[p][i] = -1;
            saving[j][p] = -1;
            saving[p][j] = -1;
        }
    } else { 
        saving[i][j] = -1; 
    }
}


double total_volume(int k, double demand[k], int route[k]) {
    double total = 0;
    for (int i = 0; i < k; i++) {
        total += demand[route[i]];
    }
    return total;
}


double total_distance(int k, double matrix[k][k], int route[k]) {
    double total = 0;
    total += matrix[0][route[0]]; 
    for (int i = 0; i < k - 1; i++) {
        total += matrix[route[i]][route[i + 1]]; 
    }
    total += matrix[route[k - 1]][0]; 
    return total;
}


void sort_route(int k, double matrix[k][k], int route[k]) {
    int temp[k]; 
    temp[0] = route[0]; 
    int n = 1; 
    for (int i = 1; i < k; i++) { 
        int min = -1; 
        double min_dist = -1; 
        for (int j = 0; j < n; j++) { 
            double dist = matrix[temp[j]][route[i]] + matrix[route[i]][temp[(j + 1) % n]] - matrix[temp[j]][temp[(j + 1) % n]]; 
            if (min == -1 || dist < min_dist) { 
                min = j; 
                min_dist = dist; 
            }
        }
        for (int j = n; j > min + 1; j--) { 
            temp[j] = temp[j - 1];
        }
        temp[min + 1] = route[i]; 
        n++; 
    }
    for (int i = 0; i < k; i++) { 
        route[i] = temp[i];
    }
}

#define n 16

int main() {
    
    double x[n] = {13, 10, 2, 3, 17, 4, 3, 2, 1, 5, 6, 4, 6, 4, 7.5, 6}; 
    double y[n] = {13, 10, 2, 3, 5, 4}; 
    double demand[n] = {37, 29, 29, 25, 15, 16, 8, 14, 22, 26, 21, 15, 8, 27, 13, 22}; 

    
    x[0] = 0;
    y[0] = 0;
    demand[0] = 0;

    double capacity = 20; 

    
    double matrix[n][n]; 
    distance_matrix(n, x, y, matrix); 
    print_matrix(n, n, matrix); 
    printf("\n");

    
    double saving[n][n]; 
    saving_matrix(n, matrix, saving); 
    printf("Si,j = do,i + do,j – di,j:\n");
    print_matrix(n, n, saving); 
    printf("\n");

    
    int route[n]; 
    int k = 0; 
    while (k < n - 1) { 
        combine(n, saving, demand, capacity, route, &k); 
    }
    for (int i = 0; i < k; i++) {
        printf("%d ", route[i]);
    }
    printf("\n\n");

    
    double volume = total_volume(k, demand, route); 
    double distance = total_distance(k, matrix, route); 
    printf("%.2f\n", volume);
    printf("%.2f\n\n", distance);

    
    sort_route(k, matrix, route); 
    printf("Nearest insert:\n");
    for (int i = 0; i < k; i++) {
        printf("%d ", route[i]);
    }
    printf("\n\n");

    
    volume = total_volume(k, demand, route); 
    distance = total_distance(k, matrix, route); 
    printf("%.2f\n", volume);
    printf("%.2f\n", distance);

    return 0;
}
