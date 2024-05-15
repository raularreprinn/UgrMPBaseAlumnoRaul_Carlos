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

const Kmer KmerFreq::getKmer() const
{
    return this->_kmer;
}

const int KmerFreq::getFrequency() const
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
    aux+=to_string(this->_frequency);
    return aux;
}