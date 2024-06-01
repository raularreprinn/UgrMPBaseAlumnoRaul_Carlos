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
    _allNucleotides=Kmer::MISSING_NUCLEOTIDE + _validNucleotides;
    _frequency= new int*[getNumRows()];
    for(int i=0; i<getNumRows(); i++)
    {
        _frequency[i]=new int[getNumCols()];
    }
    initFrequencies();
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

 void KmerCounter::increaseFrequency(const Kmer &kmer, int frequency)
 {
     int contador=0;
     for(int i=0; i<kmer.getK(); i++)
     {
         for(int j=0; j<getNumNucleotides(); j++)
         {
             if(kmer[i]==_allNucleotides[j])
             {
                 contador++;
             }
         }
     }
     if(contador!=kmer.getK()||kmer.getK()!=getK())
     {
         throw std::invalid_argument("Diferentes nucleótidos válidos o k");
     }
     
     int fila, columna;
     getRowColumn(kmer, fila, columna);
     _frequency[fila][columna]+=frequency;
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
 
 void KmerCounter::calculateFrequencies(const char* fileName)
 {
     string cadena;
     ifstream is;
     
     is.open(fileName,ifstream::in);
     if(is)
     {
         getline(is,cadena);
         for(int i=0; i+getK()-1<cadena.size();i++)
         {
             string aux;
             for(int j=i; j<i+getK(); j++)
             {
                 aux+=cadena[j];
             }
             Kmer kmer(aux);
             kmer.normalize(_allNucleotides);
             increaseFrequency(kmer);
         }
         is.close();
     }else
     {
         throw std::ios_base::failure("No se pudo abrir");
     }
 }
 
 Profile KmerCounter:: toProfile()
 {
     Profile aux;
     for(int i=0; i<getNumRows(); i++)
     {
         for(int j=0; j<getNumCols(); j++)
         {
             if(_frequency[i][j]>0)
             {
                 KmerFreq kmerfreq;
                 kmerfreq.setKmer(getKmer(i,j));
                 kmerfreq.setFrequency(_frequency[i][j]);
                 aux+=kmerfreq;
             }
         }
     }
     return aux;
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

void KmerCounter::getRowColumn(const Kmer &kmer, int &row, int &column)
{
    int contador=0;
     for(int i=0; i<kmer.getK(); i++)
     {
         for(int j=0; j<getNumNucleotides(); j++)
         {
             if(kmer[i]==_allNucleotides[j])
             {
                 contador++;
             }
         }
     }
     if(contador!=kmer.getK()||kmer.getK()!=getK())
     {
         row=-1;
         column=-1;
     }else
     {
        string cadena;
        int aux=(_k+1)/2;
        cadena=kmer.toString().substr(0,aux);
        row=getIndex(cadena);
        cadena=kmer.toString().substr(aux,_k-aux);
        column=getIndex(cadena);         
     }
}

Kmer KmerCounter::getKmer(const int row, const int column)const
{
    if(row<0 || row>getNumRows()||column<0||column>getNumCols())
    {
        throw std::invalid_argument("Número de filas o columnas no válido");
    }
    int lfilas=(_k+1)/2;
    int lcolumnas=_k-lfilas;
    string nucs;
    nucs=getInvertedIndex(row,lfilas)+getInvertedIndex(column,lcolumnas);
    Kmer kmer(nucs);
    return kmer;
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