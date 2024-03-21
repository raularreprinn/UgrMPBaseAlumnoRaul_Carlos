//Sumar dos vectores. Para llevarlo a cabo debe crear una función que haga la suma
//de los vectores y una función que imprima el resultado. Usa el cuantificador const
//cuando sea necesario.

#include <iostream>
using namespace std;

const int TAM =5;

void SumaElementos(const int a[], const int b[], int res[])
{
    for(int i=0; i<TAM; i++)
    {
        res[i] = a[i] + b[i];
    }
}

void ImprimirVector(const int a[])
{
    for(int i=0; i<TAM; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main()
{
const int vectorA[] = {1, 2, 3, 4, 5};
const int vectorB[] = {5, 4, 3, 2, 1};
int vectorResultado[TAM];

SumaElementos(vectorA, vectorB, vectorResultado);
ImprimirVector(vectorResultado);
}