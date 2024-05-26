/*
 * Metodología de la Programación: Kmer5
 * Curso 2023/2024
 */

/** 
 * @file KmerCounter.cpp
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 *
 * Created on 25 May 2024, 12:00
 */

#include "KmerCounter.h"

using namespace std;

/**
 * DEFAULT_VALID_NUCLEOTIDES is a c-string that contains the set of characters
 * that will be considered as valid nucleotides. 

 * The constructor of the class KmerCounter uses this c-string as a 
 * default parameter. It is possible to use a different c-string if that
 * constructor is used with a different c-string
 */
const char* const KmerCounter::DEFAULT_VALID_NUCLEOTIDES="ACGT";

KmerCounter::KmerCounter(int k, std::string validNucleotides)
{
    _k=k;
    _validNucleotides=validNucleotides;
    _allNucleotides= DEFAULT_VALID_NUCLEOTIDES + _validNucleotides + Kmer::MISSING_NUCLEOTIDE;
    _frequency= new int*[getNumRows()];
    for(int i=0; i<getNumRows(); i++)
    {
        _frequency[i]=new int[getNumCols()];
    }
}

KmerCounter::KmerCounter(const KmerCounter &orig)
{
    _k=orig._k;
    _validNucleotides=orig._validNucleotides;
    _allNucleotides=orig._allNucleotides;
    _frequency= new int *[orig.getNumRows()];
    for(int i=0; i<orig.getNumRows();i++)
    {
        _frequency[i]=new int[getNumCols()];
    }
    for(int i=0; i<getNumRows();i++)
    {
        for(int j=0; j<getNumCols(); j++)
        {
            _frequency[i][j]=orig._frequency[i][j];
        }
    }
}

KmerCounter::~KmerCounter()
{
    for(int i=0; i<getNumRows(); i++)
    {
        delete []_frequency[i];
    }
    delete []_frequency;
    _frequency=NULL;
}

int KmerCounter::getNumNucleotides()const
{
    return _allNucleotides.size();
}

int KmerCounter::getK() const
{
    return _k;
}

int KmerCounter::getNumKmers()const
{
    return pow(getNumNucleotides(),getK());
}

int KmerCounter::getNumRows() const
{
    int m=getNumNucleotides();
    int lfilas=(_k+1)/2;
    int nfilas= pow(m,lfilas);
    return nfilas;
}

int KmerCounter::getNumCols() const
{
    int m=getNumNucleotides();
    int lfilas=(_k+1)/2;
    int lcolumnas=_k-lfilas;
    int ncolumnas=pow(m,lcolumnas);
    return ncolumnas;
}

int KmerCounter::getNumberActiveKmers() const
{
    int contador=0;
    for(int i=0; i<getNumRows(); i++)
    {
        for(int j=0; j<getNumCols(); j++)
        {
            if(_frequency[i][j]>0)
            {
                contador++;
            }
        }
    }
    return contador;
}

std::string KmerCounter::toString()const{
    string outputString = _allNucleotides + " " + to_string(_k) + "\n";
    
    for(int row=0; row<this->getNumRows(); row++){
        for(int col=0; col<this->getNumCols(); col++){
            outputString += to_string((*this)(row,col)) + " ";
        }
        outputString += "\n";
    }
    
    return outputString;
}

 void KmerCounter::increaseFrequency(Kmer kmer, int frequency)
 {
     
 }
 
 KmerCounter & KmerCounter:: operator=(const KmerCounter &orig)
 {
    _k=orig._k;
    _validNucleotides=orig._validNucleotides;
    _allNucleotides=orig._allNucleotides;
    _frequency= new int *[orig.getNumRows()];
    for(int i=0; i<orig.getNumRows();i++)
    {
        _frequency[i]=new int[getNumCols()];
    }
    for(int i=0; i<getNumRows();i++)
    {
        for(int j=0; j<getNumCols(); j++)
        {
            _frequency[i][j]=orig._frequency[i][j];
        }
    }
    return *this;
 }
 
 KmerCounter & KmerCounter::operator+=(const KmerCounter & kc)
 {
     if(_allNucleotides!=kc._allNucleotides||getK()!=kc.getK())
     {
         throw std::invalid_argument("Diferente k o nucleótidos válidos");
     }else
     {
         for(int i=0; i<getNumRows(); i++)
            {
                for(int j=0; j<getNumCols(); j++)
                    {
                    _frequency[i][j]+=kc._frequency[i][j];
                    }  
            }
     }
     return *this;
 }

int KmerCounter::getIndex(const std::string& kmer) const{
    int index = 0;
    int base = 1;

    for (size_t i = 0; i < kmer.size(); i++) {
        size_t pos = _allNucleotides.find(kmer[kmer.size()-i-1]);
        if (pos == string::npos)
            return -1;
        index += pos * base;
        base *= _allNucleotides.size();
    }
    return index;
}

string KmerCounter::getInvertedIndex(int index, int nCharacters) const {
    string result(nCharacters, Kmer::MISSING_NUCLEOTIDE);

    for (int i = result.size(); i > 0; i--) {
        result[i - 1] = _allNucleotides[index % _allNucleotides.size()];
        index = index / _allNucleotides.size();
    }
    return result;
}

void KmerCounter::initFrequencies()
{
    for(int i=0; i<getNumRows(); i++)
    {
        for(int j=0; j<getNumCols(); j++)
        {
            _frequency[i][j]=0;
        }
    }
}

const int & KmerCounter:: operator()(int row, int column) const
 {
     return _frequency[row][column];
 }

 int & KmerCounter:: operator()(int row, int column)
 {
     return _frequency[row][column];
 }