/*
 * Metodología de la Programación: Kmer1
 * Curso 2023/2024
 */

/** 
 * @file ArrayKmerFreqFunctions.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * @author Javier Martínez Baena <jbaena@ugr.es>
 * 
 * Created on 25 Marzo 2024, 17:00
 */


#include "ArrayKmerFreqFunctions.h"
using namespace std;


void ReadArrayKmerFreq(KmerFreq array[], int dim, int nElements)
{
    if(nElements>dim)
    {
        nElements=dim;
    }
    if(nElements>0)
    {
        for(int i=0; i<nElements;i++)
        {
            //Ni puta idea
        }
    }
}
void PrintArrayKmerFreq(KmerFreq array[], int nElements)
{
    for(int i=0; i<nElements; i++)
    {
        cout << array[i].toString()<<endl;
    }
}

void SwapElementsArrayKmerFreq(KmerFreq array[], int nElements, int first, int second)
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

int FindKmerInArrayKmerFreq(KmerFreq array[], Kmer kmer, int initialPos, int finalPos)
{
    int posicion=-1;
    for(int i=initialPos; i<=finalPos; i++)
    {
        if( array[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

void SortArrayKmerFreq(KmerFreq array[], int nElements)
{
    for(int i=0; i<nElements-1; i++)
    {
        int posicion=i;
        KmerFreq aux;
        for(int j=i+1; j<nElements; j++)
        {
            if(array[posicion].getFrequency()<array[j].getFrequency())
                posicion=j;
        }
        aux=array[i];
        array[i]=array[posicion];
        array[posicion]=aux;
    }
}

void NormalizeArrayKmerFreq(KmerFreq array[], int nElements, string validNucleotides)
{
    for(int i=0; i<nElements; i++)
    {
        array[i].getKmer().normalize(validNucleotides);
    }
    for(int i=0; i<nElements; i++)
    {
        for(int j=i+1; j<nElements; j++)
        {
            if(array[i].getKmer().toString()==array[j].getKmer().toString())
            {
                array[i].setFrequency(array[i].getFrequency()+array[j].getFrequency());
                DeletePosArrayKmerFreq(array, nElements, j);
            }
        }
    }
}

void ZipArrayKmerFreq(KmerFreq array[], int nElements, bool deleteMissing, int lowerBound)
{
    deleteMissing=false;
    lowerBound=0;
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


