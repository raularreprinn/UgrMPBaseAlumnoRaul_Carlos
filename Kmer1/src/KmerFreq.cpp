/*
 * Metodología de la Programación: Kmer1
 * Curso 2023/2024
 */

/** 
 * @file KmerFreq.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * @author Javier Martínez Baena <jbaena@ugr.es>
 * 
 * Created on 25 de marzo de 2024, 14:18
 */

#include "KmerFreq.h"
using namespace std;

KmerFreq::KmerFreq()
{
    _kmer=Kmer::MISSING_NUCLEOTIDE;
    _frequency=0;
}

Kmer KmerFreq::getKmer()
{
    return this->_kmer;
}

int KmerFreq::getFrequency()
{
    return this->_frequency;
}

void KmerFreq::setKmer(Kmer kmer)
{
    this->_kmer=kmer;
}

void KmerFreq::setFrequency(int frequency)
{
    this->_frequency=frequency;
}

string KmerFreq:: toString()
{
    string aux;
    aux=this->_kmer.toString();
    aux+=" ";
    aux+=this->_frequency;
    return aux;
}