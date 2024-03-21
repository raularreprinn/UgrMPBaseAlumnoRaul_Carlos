//Se pide a los estudiantes que implementen dos funciones: una que realice el
//cuadrado de que cada elemento sobre la matriz y otra que imprima la matriz
//resultante. Tener en cuenta si se necesita proteger o no proteger la matriz.

#include <iostream>
using namespace std;

const int FILAS = 3;
const int COLUMNAS = 3;

void cuadradoMatriz(const int a[][COLUMNAS], int b[][COLUMNAS])
{
    for(int i = 0; i < FILAS; i++)
    {
        for(int j = 0; j < COLUMNAS; j++)
        {
            b[i][j] = a[i][j] * a[i][j];
        }
    }
}

void imprimirMatriz(const int a[][COLUMNAS])
{
    for(int i = 0; i < FILAS; i++)
    {
        for(int j = 0; j < COLUMNAS; j++)
        {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int miMatriz[FILAS][COLUMNAS] = {{1, 2, 3},{4, 5, 6},{7, 8, 9}};
    int MatrizCuadrada[FILAS][COLUMNAS];

    cout << "Contenido inicial de la matriz:" << endl;
    imprimirMatriz(miMatriz);

    cuadradoMatriz(miMatriz, MatrizCuadrada);

    cout << "Matriz original:" << endl;
    imprimirMatriz(miMatriz);

    cout << "Matriz de cuadrados:" << endl;
    imprimirMatriz(MatrizCuadrada);
}
