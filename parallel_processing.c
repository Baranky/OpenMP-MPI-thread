#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>  // Matematik fonksiyonları için

#define DATA_SIZE 10000000
#define CHUNK_SIZE 1000
#define DATA_FILE "data/numbers.npy"

double process_chunk(double* data, int size) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < size; i++) {
        double x = data[i];
        for(int j = 0; j < 100; j++) {
            x = sin(x) * cos(x) + tan(x);
        }
        sum += x;
    }
    return sum;
}

double* read_npy_file(const char* filename, int* size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Hata: Dosya açılamadı: %s\n", filename);
        return NULL;
    }

    char header[128];
    fread(header, 1, 128, file);

    *size = DATA_SIZE;
    double* data = (double*)malloc(*size * sizeof(double));
    
    fread(data, sizeof(double), *size, file);
    fclose(file);
    
    return data;
}

int main(int argc, char** argv) {
    int world_size, world_rank;
    double* data = NULL;
    double* local_data = NULL;
    double local_sum = 0.0;
    double global_sum = 0.0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) {
        int data_size;
        data = read_npy_file(DATA_FILE, &data_size);
        if (!data) {
            printf("Hata: Veri okunamadı!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        printf("Veri başarıyla okundu. Toplam boyut: %d\n", data_size);
        start_time = MPI_Wtime();
    }

    int chunk_size = DATA_SIZE / world_size;
    local_data = (double*)malloc(chunk_size * sizeof(double));

    MPI_Scatter(data, chunk_size, MPI_DOUBLE,
                local_data, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int start_index = world_rank * chunk_size;
    int end_index = start_index + chunk_size - 1;

   local_sum = process_chunk(local_data, chunk_size);
    
    printf("Node %d: Veri aralığı [%d - %d], İşlenen veri sayısı: %d, Yerel toplam: %f\n",
           (world_rank+1), start_index, end_index, chunk_size, local_sum);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        end_time = MPI_Wtime();
        printf("\n=== Özet Bilgiler ===\n");
        printf("Toplam sonuç: %f\n", global_sum);
        printf("İşlem süresi: %f saniye\n", end_time - start_time);
        printf("Kullanılan düğüm sayısı: %d\n", world_size);
        printf("Her düğümdeki OpenMP thread sayısı: %d\n", omp_get_max_threads());
        printf("Her düğüme düşen veri miktarı: %d sayı\n", chunk_size);
    }

    if (world_rank == 0) {
        free(data);
    }
    free(local_data);

    MPI_Finalize();
    return 0;
} 