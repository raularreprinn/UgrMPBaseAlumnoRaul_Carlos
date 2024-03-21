//Se proporciona una función simple que realiza una operación matemática de
//multiplicación sobre dos números y se pide que se implementen dos
//versiones de la función, una utilizando paso por referencia y otra utilizando
//paso por valor.

#include <iostream>
using namespace std;

void MultiplicarDosValoresPorReferencia(int &a,int &b)
{
  int c = a * b;
  cout << c;
}

int MultiplicarDosValoresPorValor(int a,int b)
{
  int c = a * b;
  return c;
}



int main() {
    
int numero1 = 10;
int numero2 = 5;

MultiplicarDosValoresPorReferencia(numero1, numero2);

cout << endl;

int resultado = MultiplicarDosValoresPorValor(numero1, numero2);
cout << resultado << endl;
}