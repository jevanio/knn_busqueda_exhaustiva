# knn_busqueda_exhaustiva

### Input
El input consiste en los documentos, representados como una cadena binaria, y sus etiquetas, representados con números enteros positivos. 
```
Cantidad_elementos Largo_Cadena_Binaria
Cadena_Binaria_1 Cantidad_Etiquetas_1 Etiquetas_1
...
Cadena_Binaria_n Cantidad_Etiquetas_n Etiquetas_n
```
Por ejemplo:

```
3 5
1 0 0 1 1 2 1 0
1 1 1 0 0 1 2
0 0 0 1 0 4 1 0 2 3
```

### Output
El output consiste en las etiquetas de los documentos recuperados, indicando k como la cantidad de documentos recuperados por consulta.
```
k
etiqueta_1_recuperado_1_consulta_1 etiqueta_2_recuperado_2_consulta_1 ...
...
etiqueta_1_recuperado_k_consulta_1 etiqueta_2_recuperado_k_consulta_1 ...
...
etiqueta_1_recuperado_1_consulta_n etiqueta_2_recuperado_2_consulta_n ...
...
etiqueta_1_recuperado_k_consulta_n etiqueta_2_recuperado_k_consulta_n ...
```

Por ejemplo, sea k=2 y se realizan 2 consultas.

```
2
1 0
1 0 2 3
2
1 0
```


### Compilación

```
gcc busqueda_exhaustiva.c -o out -Wall
./out query_file recover_file k
```

En donde, query_file posee las cadenas binarias a utilizar como consultas, recover_file posee las cadenas binarias de los documentos sobre los que se recuperará y k es la cantidad de documentos a recuperar. Luego, se generará un archivo k-nn.dat, que posee las etiquetas de los documentos recuperados.
