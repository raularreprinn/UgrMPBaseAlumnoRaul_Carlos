/*
 * Metodología de la Programación: Kmer1
 * Curso 2023/2024
 */

/** 
 * @file ArrayKmerFreqFunctions.cpp
 *@author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * 
 * Created on 25 Marzo 2024, 17:00
 */


#include "ArrayKmerFreqFunctions.h"
using namespace std;


void ReadArrayKmerFreq(KmerFreq array[], const int dim, int &nElements)
{
    if(nElements>dim)
    {
        nElements=dim;
    }
    if(nElements>0)
    {
        for(int i=0; i<nElements;i++)
        {
            string secuencia;
            cin >> secuencia;
            Kmer aux(secuencia);
            array[i].setKmer(aux);
            int frecuencia;
            cin>>frecuencia;
            array[i].setFrequency(frecuencia);
        }
    }
}

void PrintArrayKmerFreq(KmerFreq array[], const int nElements)
{
    for(int i=0; i<nElements; i++)
    {
        cout << array[i].toString()<<endl;
    }
}

void SwapElementsArrayKmerFreq(KmerFreq array[], const int nElements, const int first, const int second)
{
    if(first<0 || first>nElements || second<0 || second>nElements)
    {
        throw out_of_range("Fuera de rango");
    }else
    {
        KmerFreq aux= array[first];
        array[first]=array[second];
        array[second]=aux;
    }
}

int FindKmerInArrayKmerFreq(KmerFreq array[], const Kmer kmer, const int initialPos, const int finalPos)
{
    int posicion=-1;
    for(int i=initialPos; i<=finalPos; i++)
    {
        if( array[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

void SortArrayKmerFreq(KmerFreq array[], const int nElements)
{
    for(int i=0; i<nElements-1; i++)
    {
        int posicion=i;
        for(int j=i+1; j<nElements; j++)
        {
            if(array[posicion].getFrequency()<array[j].getFrequency())
                posicion=j;
        }
        SwapElementsArrayKmerFreq(array, nElements, i, posicion);
    }
}

void NormalizeArrayKmerFreq(KmerFreq array[], int nElements, const string validNucleotides)
{
    for(int i=0; i<nElements; i++)
    {
        array[i].getKmer().normalize(validNucleotides);
    }
    for(int i=0; i<nElements; i++)
    {
        int posicion=FindKmerInArrayKmerFreq(array, array[i].getKmer(), i, nElements-1);
        if(posicion!=-1)
        {
            array[i].setFrequency(array[i].getFrequency()+array[posicion].getFrequency());
            DeletePosArrayKmerFreq(array, nElements, posicion);
        }
    }
}

void ZipArrayKmerFreq(KmerFreq array[], int nElements)
{
    bool deleteMissing=false;
    const int lowerBound=0;
    for(int i=0; i<nElements; i++)
    {
        for(int j=0; j<array[i].getKmer().size(); j++)
        {
            if(array[i].getKmer().at(j)==Kmer::MISSING_NUCLEOTIDE)
            {
                deleteMissing=true;
            }   
        }
        if(array[i].getFrequency()<=lowerBound)
        {
            deleteMissing=true;
        }
        if(deleteMissing==true)
        {
            DeletePosArrayKmerFreq(array, nElements, i);
            i--;
        }
        deleteMissing=false;
    }
}

void DeletePosArrayKmerFreq(KmerFreq array[], int &nElements, int pos)
{
    if(pos<0 || pos>nElements)
    {
        throw out_of_range("Fuera de rango");
    }else
    {
        for(int i=pos; i<nElements-1; i++)
        {
            array[i]=array[i+1];
        }
    }
    nElements--;
}


