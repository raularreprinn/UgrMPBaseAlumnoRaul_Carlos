/*
 * Metodología de la Programación: Kmer1
 * Curso 2023/2024
 */

/** 
 * @file KmerFreq.cpp
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * 
 * Created on 25 de March de 2024, 14:18
 */

#include "KmerFreq.h"
using namespace std;

KmerFreq::KmerFreq()
{
    _kmer=Kmer::MISSING_NUCLEOTIDE;
    _frequency=0;
}

const Kmer& KmerFreq::getKmer() const
{
    return this->_kmer;
}

const int KmerFreq::getFrequency() const
{
    return this->_frequency;
}

void KmerFreq::setKmer(const Kmer &kmer)
{
    this->_kmer=kmer;
}

void KmerFreq::setFrequency(const int frequency)
{
    this->_frequency=frequency;
}

string KmerFreq:: toString() const
{
    string aux;
    aux=this->_kmer.toString();
    aux+=" ";
    aux+=to_string(this->_frequency);
    return aux;
}

void KmerFreq::write(std::ostream &outputStream)
{
    _kmer.write(outputStream);
    outputStream.write(reinterpret_cast<const char*>(&this->_frequency),sizeof(int));
}

void KmerFreq::read(std::istream &inputStream)
{
    _kmer.read(inputStream);
    inputStream.read(reinterpret_cast<char*>(&this->_frequency),sizeof(int));
}

ostream & operator<<(std::ostream &os, const KmerFreq &kmerFreq)
{
    os << kmerFreq.toString();
    return os;
}

istream & operator>>(std::istream &is, KmerFreq &kmerFreq)
{
    Kmer kmer;
    int frecuencia;
    kmer.read(is);
    is >> frecuencia;
    kmerFreq.setFrequency(frecuencia);
    kmerFreq.setKmer(kmer);
    return is;
}

bool operator>(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    bool mayor=false;
    if(kmerFreq1.getFrequency()>kmerFreq2.getFrequency())
        mayor= true;
    else if(kmerFreq1.getFrequency()==kmerFreq2.getFrequency())
        if((kmerFreq1.getKmer()>kmerFreq2.getKmer())==false)
            mayor= true;
    return mayor;
}

bool operator<(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    if(kmerFreq1>kmerFreq2)
        return false;
    else
        return true;
}

bool operator==(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    if(kmerFreq1.getFrequency()==kmerFreq2.getFrequency() && kmerFreq1.getKmer()==kmerFreq2.getKmer())
        return true;
    else
        return false;
}

bool operator!=(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    if(kmerFreq1==kmerFreq2)
        return false;
    else
        return true;
}

bool operator<=(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    if(kmerFreq1<kmerFreq2 || kmerFreq1==kmerFreq2)
        return true;
    else
        return false;
}

bool operator>=(const KmerFreq & kmerFreq1, const KmerFreq & kmerFreq2)
{
    if(kmerFreq1>kmerFreq2 || kmerFreq1==kmerFreq2)
        return true;
    else
        return false;
}