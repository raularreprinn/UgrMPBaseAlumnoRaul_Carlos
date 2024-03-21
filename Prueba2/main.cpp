//Se pide a los estudiantes que implementen dos funciones: una que nos
//duplique nuestro array original en otro array nuevo pasado en la función y
//otra que modifique nuestro array original con los cuadrados de nuestro array
//original. Esta última función debe crearse de manera protegida y de manera
//no protegida.

#include <iostream>
using namespace std;

const int TAM=5;

void duplicarElementos(int a[], int b[])
{
    for(int i=0; i<TAM; i++)
    {
        b[i] = a[i] * 2;
    }
}

void cuadradoElementosSinproteger(int a[])
{
    for(int i=0; i<TAM; i++)
    {
        a[i] *= a[i];
    }
}

void cuadradoElementosProtegido(const int a[], int b[])
{
    for(int i=0; i<TAM; i++)
    {
        b[i] = a[i] * a[i];
    }
}

int main() 
{

int miArray[] = {1, 2, 3, 4, 5};
int duplicadoArray[TAM];

cout << "Contenido inicial del array: ";
for (int i = 0; i < 5; ++i) {
cout << miArray[i] << " ";
}
cout << endl;

duplicarElementos(miArray,duplicadoArray);

cout << "Después de duplicar por referencia: ";
for (int i = 0; i < 5; ++i) {
cout << duplicadoArray[i] << " ";
}
cout << endl;

cuadradoElementosSinproteger(duplicadoArray);
cout << "Después de elevar al cuadrado: ";
for (int i = 0; i < 5; ++i) {
cout << duplicadoArray[i] << " ";
}
cout << std::endl;

cuadradoElementosProtegido(miArray,duplicadoArray);
for (int i = 0; i < 5; ++i) {
cout << miArray[i] << "<--->" << duplicadoArray[i] << endl;
}
}