/*
 * Metodología de la Programación: Kmer0
 * Curso 2023/2024
 */

/** 
 * @file Kmer.cpp
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * Created on 1 March 2024, 19:00
 */

//@throw = excepción
#include "Kmer.h"
using namespace std;

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

void Kmer::toLower()
{
    for(int i=0; i< this->_text.size(); i++)
    {
        _text[i]=tolower(_text[i]);
    }
}

void Kmer::toUpper()
{
    for(int i=0; i< this->_text.size(); i++)
    {
        _text[i]=toupper(_text[i]);
    }
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
        nuc=toupper(nuc);
    
        if (IsValidNucleotide(nuc, validNucleotides))
        {
            this->_text[i] = nuc;
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
        size_t pos = nucleotides.find(this->_text[i]);
        if (pos==string::npos ){
            if(i!=0)
            {
                aux._text.push_back(MISSING_NUCLEOTIDE);
            }else
            {
                aux._text[i]=MISSING_NUCLEOTIDE;
            }
        }
        else{
            if(i!=0)
            {
                aux._text.push_back(complementaryNucleotides[pos]);
            }else
            {
                 aux._text[i]=complementaryNucleotides[pos];
            }
        }
    }
    
    return aux;
}

void Kmer::write(std::ostream outputStream)
{
    
}

void Kmer::read(std::istream inputStream)
{
    
}

const char & Kmer::operator[] (int index)const
{
    if (index > this->_text.size() - 1)
    {
        throw out_of_range ("Fuera de rango");
    }
    return _text[index];    
}

char & Kmer::operator[] (int index)
{
    if (index > this->_text.size() - 1)
    {
        throw out_of_range ("Fuera de rango");
    }
    return _text[index];  
}

bool IsValidNucleotide(char nucleotide, const string& validNucleotides){
    bool validnuc = false;

    for (int i = 0; i < validNucleotides.size(); i++)
    {
        if (nucleotide == validNucleotides[i])
        {
            validnuc = true;
        }
    }

    return validnuc;
}

void ToLower(Kmer& kmer){
    for (int i = 0; i < kmer.size(); i++)
    {
        kmer.at(i)=tolower(kmer.at(i));
    }
}

void ToUpper(Kmer& kmer){
    for (int i = 0; i < kmer.size(); i++)
    {
        kmer.at(i)=toupper(kmer.at(i));
    }
}

ostream & operator<<(ostream &os, const Kmer &kmer)
{
    os<<kmer.toString();
    return os;
}

istream & operator>>(istream &is, Kmer &kmer)
{
    //Suponemos que usamos el constructor con parámetros
    for(int i=0; i<kmer.getK(); i++)
    {
        is >> kmer[i];
    }
    return is;
}

bool operator >(const Kmer & kmer1,const Kmer & kmer2)
{
    if(kmer1.toString()>kmer2.toString())
        return true;
    else
        return false;
}

bool operator ==(const Kmer & kmer1, const Kmer & kmer2)
{
    if(kmer1.toString()==kmer2.toString())
        return true;
    else
        return false;
}