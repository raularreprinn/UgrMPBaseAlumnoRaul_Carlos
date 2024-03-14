/*
 * Metodología de la Programación: Kmer0
 * Curso 2023/2024
 */

/** 
 * @file Kmer.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * @author Javier Martínez Baena <jbaena@ugr.es>
 * 
 * Created on 24 October 2023, 14:00
 */

//@throw = excepción
/*
* En las matrices, las filas no se pasan porque el compilador crea
* un puntero para apuntar a las columnas de las matrices y reserva 
* memoria ESTÁTICA para la matriz.
* 
* UNA VARIABLE NO VECTOR NO CONSTANTE, NO PUEDE SER PASADA A UNA FUNCIÓN QUE REQUIERA UNA VARIBLE CONSTANTE
* Dos structs se pueden igualar porque apuntan a la misma dirección de memoria
* Las funciones toLower y toUpper las declaramos porque usamos objetos 
* de tipo Kmer. El compilador tiene esas funciones para char. 
* "This" es un puntero que apunta al vector de kmers[0], formado por 
* elementos de la clase kmer
*/

using namespace std;

#include "Kmer.h"

Kmer::Kmer(int k)
{
    if(k<=0)
    {
        throw invalid_argument("Fuera de rango");
    }
    else
    {
        for(int i=0; i<k; i++)
        {
            _text += MISSING_NUCLEOTIDE;
        }
    }
}

Kmer::Kmer(const string& text)
{
   if(text.empty())
   {
    throw invalid_argument("String vacio");
   }
   else
   {
    this->_text=text;
   }
}

int Kmer::getK() const
{
 return this->_text.size();
}

int Kmer::size() const
{
  return getK();
}

string Kmer::toString () const{
    return this->_text;
}

const char& Kmer::at(int index) const{
    if (index > this->_text.size() - 1)
    {
        throw out_of_range ("Fuera de rango");
    }
    return _text[index];    
}

char& Kmer::at(int index){
    if (index > this->_text.size() - 1)
    {
        throw out_of_range ("Fuera de rango");
    }
    return _text[index];    
}

void Kmer::normalize(const string& validNucleotides){
    for (int i = 0; i < this->_text.size(); i++)
    {
        char& nuc = _text[i];
    
        if (IsValidNucleotide(nuc, validNucleotides))
        {
            this->_text[i] = toupper(nuc);
        }
        else
        {
            this->_text[i] = MISSING_NUCLEOTIDE;
        }
    }
}

Kmer Kmer::complementary(const string& nucleotides, const string& complementaryNucleotides) const{
    if (nucleotides.size() != complementaryNucleotides.size())
    {
        throw invalid_argument("Longitudes diferentes");
    }

    Kmer aux;
    
    for (int i = 0; i < this->_text.size(); i++){
        for (int j = 0; j < nucleotides.size(); j++)
        {
            if (this->_text[i] == nucleotides[j])
            {
                aux._text += complementaryNucleotides[j];
            }
        }
    }
    
    return aux;
}

bool IsValidNucleotide(char nucleotide, const string& validNucleotides){
    bool valid = false;

    for (int i = 0; i < validNucleotides.size(); i++)
    {
        if (nucleotide == validNucleotides[i])
        {
            valid = true;
        }
    }

    return valid;
}

void ToLower(Kmer& Kmer){
    for (int i = 0; i < Kmer.size(); i++)
    {
        tolower(Kmer.at(i));
    }
}

void ToUpper(Kmer& Kmer){
    for (int i = 0; i < Kmer.size(); i++)
    {
        toupper(Kmer.at(i));
    }
}