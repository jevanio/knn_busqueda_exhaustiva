#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

/*
* matrix struct
* n_rows: Number of rows on matrix mat.
* n_cols: Number of columns on matrix mat.
* mat: List of elements on matrix.
* idx: List of idx of 
*/
typedef struct _matrix
{
    unsigned int n_rows;
    unsigned int n_cols;
    uint_fast8_t **mat;
    int *n_labels;
    int **labels;
} matrix;


/*
* list of idx of knn
*/
typedef struct _knn_matrix
{
    unsigned int n_rows;
    unsigned int n_cols;
    int **knn_idx;
} knn_matrix;

/*
* Error message and stop programm
*/
void error(char *message)
{
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}

/*
* Check errors
*/
void check_errors(int argc, char *argv[])
{
    if (argc < 4)
        error("Faltan Parámetros\n");
}

/*
* Create matrix with 0's on each row/column.
* Set number of rows and col.
* set 0's on idx list.
*/
matrix *init_matrix(matrix *mtrx, int row, int col)
{
    int i;
    mtrx->mat = calloc(row+1, sizeof(uint_fast8_t*));
    for(i = 0;i<row;i++) mtrx->mat[i] = calloc(col+1, sizeof(uint_fast8_t));
    mtrx->n_rows = row;
    mtrx->n_cols = col;
    mtrx->n_labels = calloc(row+1, sizeof(int));
    mtrx->labels = calloc(row+1, sizeof(int*));
    return mtrx;
}

/*
* Create matrix with 0's on each row/column.
* Set number of rows and col.
* set 0's on idx list.
*/
knn_matrix *init_knn_matrix(knn_matrix *knn, int row, int col)
{
    int i;
    knn->knn_idx = calloc(row+1, sizeof(int*));
    for(i = 0;i<row;i++) knn->knn_idx[i] = calloc(col+1, sizeof(int));
    knn->n_rows = row;
    knn->n_cols = col;
    return knn;
}

/*
* show matrix values
*/
void show_matrix(matrix *mtrx)
{
    int i,j;
    for(i = 0;i<mtrx->n_rows;i++)
    {
        for(j = 0;j<mtrx->n_cols;j++)
            printf("% " PRIuFAST8, mtrx->mat[i][j] );
        printf("\n");
    }
}

/*
* get 2**n
*/
int pow_2(int n)
{
    int i,a=2;
    if(n == 0)
        return 1;
    for(i=1;i<n;i++)
        a*=2;
    return a;
}

/*
* fill matrix
* Initiate matrix with 0's, then fill the matrix with a decimal representation of each 8 binary bits
*/
matrix* fill_matrix(matrix *mtrx, char* data)
{
    int row,col;
    FILE* ffile;

    ffile=fopen(data,"r");
    if (ffile==NULL)
    {
        char message[23]="Error abriendo archivo ";
        error(strcat(message,data));
    }
    fscanf(ffile,"%d %d\n",&row,&col);
    mtrx = init_matrix(mtrx,row,col/8);

    /* Read each element from data.dat  */
    int i,j,l,byte_flag;
    for(i = 0;i<mtrx->n_rows;i++)
    {
        for(j = 0;j<mtrx->n_cols;j++)
        {
            for(l=0;l<8;l++)
            {
                fscanf(ffile,"%d ", &byte_flag);
                if (byte_flag)
                    mtrx->mat[i][j] += pow_2(7-l);
            }
        }
        fscanf(ffile,"%d ",&mtrx->n_labels[i]);
        mtrx->labels[i] = calloc(mtrx->n_labels[i]+1, sizeof(int));
        for(l=0;l<mtrx->n_labels[i];l++)
            fscanf(ffile,"%d ",&mtrx->labels[i][l]);

    }

    fclose(ffile);

    return mtrx;
}

/*
* get hamming distance with xor operator and list of same len
*/
int hamming_distance(uint_fast8_t *hash1, uint_fast8_t *hash2, int n)
{
    uint bit_in_char[] = {  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3,
                            3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,
                            3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2,
                            2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5,
                            3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,
                            5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3,
                            2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
                            4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
                            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4,
                            4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,
                            5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5,
                            5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,};
    int dis=0,j,pos;
    for(j=0;j<n;j++)
    {
        pos=hash1[j] ^ hash2[j];
        dis+=bit_in_char[pos];
    }
    return dis;
}

/*
* get a list of distance of all element in mtrx with hash1
*/
int* all_distances(uint_fast8_t *hash, matrix *mtrx, int n)
{
    int i;
    int *d = calloc(mtrx->n_rows+1, sizeof(int));
    for(i=0;i<mtrx->n_rows;i++)
        d[i]=hamming_distance(hash, mtrx->mat[i],n);
    return d;
}

/*
* get pos of min value on list d.
*/
int* get_idxs(int *d, int n_dist, int k, int *knn_idx)
{
    int i,j1,j2,j;
    int *knn_dist = calloc(k+1,sizeof(int));
    for(j=0;j<k;j++)
        knn_dist[j] = 128;

    for(i=0;i<n_dist;i++)    
        for(j1=0;j1<k;j1++)
            if(d[i]<=knn_dist[j1])
            {
                for(j2=k;j2>j1;j2--)
                {
                    knn_dist[j2-1] = knn_dist[j2-2];
                    knn_idx[j2-1] = knn_idx[j2-2];
                }
                knn_dist[j1] = d[i];
                knn_idx[j1] = i;
                break;
            }

    free(knn_dist);
    return knn_idx;
}

/*
* find_knn
* Get knn from querys.
*/
void find_knn(matrix* querys, matrix* recover, knn_matrix* knn)
{
    int *d = NULL;
    int i;

    for(i=0;i<querys->n_rows;i++)
    {
        printf("%d/%d\r",i+1,querys->n_rows);
        fflush(stdout);

        d = all_distances(querys->mat[i], recover,querys->n_cols);
        knn->knn_idx[i]=get_idxs(d,recover->n_rows, knn->n_cols,knn->knn_idx[i]);

        free(d);
    }
    printf("\n");
}

/*
* Save knn on k-nn.dat file
*/
void save_knn(matrix* mtrx, knn_matrix *knn, int k)
{
    char *dirname;
    size_t sz;
    sz = snprintf(NULL, 0, "%d-nn.dat",k);
    dirname = (char *)malloc(sz + 1);
    snprintf(dirname, sz+1, "%d-nn.dat",k);
    FILE* ffile;
    ffile =fopen(dirname,"w+");
//    ffile =fopen("knn.dat","w+");
    

    fprintf(ffile, "%d\n",k);
    int i,j,l;
    for(i=0;i<knn->n_rows;i++)
        for(j=0;j<knn->n_cols;j++)
        {
            printf("%d\n", knn->knn_idx[i][j]);
            for(l=0;l<mtrx->n_labels[knn->knn_idx[i][j]];l++)
                fprintf(ffile,"%d ",mtrx->labels[knn->knn_idx[i][j]][l]);
            
            fprintf(ffile, "\n");
        }
    fclose(ffile);
    free(dirname);
}

/*
* free memory allocated to matrix
*/
void free_knn_matrix(knn_matrix *knn)
{
    int i;
    for(i=knn->n_rows;i>=0;i--) free(knn->knn_idx[i]);
    free(knn->knn_idx);
    free(knn);
}

/*
* free memory allocated to matrix
*/
void free_matrix(matrix *mtrx)
{
    int i;
    for(i=mtrx->n_rows;i>=0;i--) free(mtrx->mat[i]);
    free(mtrx->mat);
    free(mtrx->n_labels);
    free(mtrx);
}


int main(int argc, char *argv[])
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    check_errors(argc,argv);
    int k = atoi(argv[3]);
    char* query_file = argv[1];
    char* recover_file = argv[2]; 

    matrix* querys = malloc(sizeof(matrix));
    matrix* recover = malloc(sizeof(matrix));
    printf("Iniciar matrices\n");
    querys = fill_matrix(querys,query_file);
    recover = fill_matrix(recover,recover_file);

    knn_matrix* knn = malloc(sizeof(knn_matrix));
    knn = init_knn_matrix(knn, querys->n_rows, k);

    printf("Realiza KNN\n");
    find_knn(querys, recover,knn);
    printf("Guarda KNN en archivo .dat\n");
    save_knn(recover, knn, k);

    printf("Liberar espacio\n");
    free_knn_matrix(knn);
    free_matrix(querys);
    free_matrix(recover);

    gettimeofday(&end, NULL);
    printf("Time in seconds: %ld \n",
            (((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec)/1000000);
    return 0;
}
