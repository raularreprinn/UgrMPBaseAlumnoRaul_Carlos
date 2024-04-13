/*
 * Metodología de la Programación: Kmer2
 * Curso 2023/2024
 */

/** 
 * @file Profile.cpp
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * Created on 7 April 2024, 10:00
 */


#include "Profile.h"    

using namespace std;

const string Profile::MAGIC_STRING_T="MP-KMER-T-1.0";

Profile::Profile()
{
    this->_profileId = "unknown";
    this->_size=0;
}

Profile::Profile(const int size)
{
    if(size<0 || size > DIM_VECTOR_KMER_FREQ)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        this->_profileId = "unknown";
         for(int i=0; i<size; i++)
    {
        this->_vectorKmerFreq[i].setKmer(Kmer::MISSING_NUCLEOTIDE);
        this->_vectorKmerFreq[i].setFrequency(0);
    } 
    }
}

string Profile::getProfileId()
{
    return this->_profileId;
}

void Profile::setProfileId(const string id)
{
    this->_profileId = id;
}

const KmerFreq Profile::at(const int index) const
{
    if(index<0 || index>DIM_VECTOR_KMER_FREQ)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        return _vectorKmerFreq[index];
    }
}

KmerFreq Profile::at(const int index)
{
    if(index<0 || index>DIM_VECTOR_KMER_FREQ)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        return this->_vectorKmerFreq[index];
    }
}

int Profile::getSize()
{
    return this->_size;
}

int Profile::getCapacity()
{
    return DIM_VECTOR_KMER_FREQ;
}

int Profile::findKmer(Kmer kmer, const int initialPos, const int finalPos)
{
    int posicion=-1;
    for(int i=initialPos; i<finalPos; i++)
    {
        if(this->_vectorKmerFreq[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

int Profile::findKmer(Kmer kmer)
{
    int posicion=-1;
    for(int i=0; i<this->_size; i++)
    {
        if(this->_vectorKmerFreq[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

string Profile::toString()
{
    string aux;
    aux = this->_profileId+"\n"+to_string(this->_size)+"\n";
    for(int i=0; i<this->_size; i++)
    {
        aux += this->_vectorKmerFreq[i].toString()+"\n";
    }
    return aux;
}

void Profile::sort()
{
    for(int i=0; i<this->_size-1; i++)
    {
        int posicion=i;
        for(int j=i+1; j<_size; j++)
        {
            if(this->_vectorKmerFreq[posicion].getFrequency()<this->_vectorKmerFreq[j].getFrequency())
                posicion=j;
            else if(this->_vectorKmerFreq[posicion].getFrequency()==this->_vectorKmerFreq[j].getFrequency())
            {
                if(this->_vectorKmerFreq[posicion].getKmer().toString()>this->_vectorKmerFreq[j].getKmer().toString())
                {
                    posicion = j;
                }
            }
        }
        KmerFreq aux=this->_vectorKmerFreq[posicion];
        this->_vectorKmerFreq[posicion]=_vectorKmerFreq[i];
        this->_vectorKmerFreq[i]=aux;
    }
}

void Profile::save(const char fileName[])
{
    ofstream outstream(fileName,ios::out);
    if(outstream)
    {
        outstream<<MAGIC_STRING_T<<endl;
        outstream<<this->_profileId<<endl;
        outstream<<to_string(this->_size)<<endl;
        for(int i=0; i<this->_size; i++)
        {
            outstream<<this->_vectorKmerFreq[i].getKmer().toString()<<" ";
            outstream<<to_string(this->_vectorKmerFreq[i].getFrequency())<<endl;
        }
        outstream.close();
    }else
    {
        throw std::ios_base::failure("No se pudo abrir");
    }
}

void Profile::load(const char fileName[])
{
    const char *archivo=fileName;
    string num_mag;
    string nkmers;
    string kmer;
    string freq;
    ifstream InputStream;
    
    InputStream.open(archivo,ifstream::in);
    if(InputStream)
    {
        InputStream>>num_mag;
        InputStream.get();
        if(num_mag==MAGIC_STRING_T)
        {
            getline(InputStream,this->_profileId);
            getline(InputStream,nkmers);
            if(stoi(nkmers)>0 || stoi(nkmers)<DIM_VECTOR_KMER_FREQ)
            {
                for(int i=0; i<stoi(nkmers);i++)
                {
                    InputStream>>kmer;
                    InputStream>>freq;
                    InputStream.get();
                    this->_vectorKmerFreq[i].setKmer(kmer);
                    this->_vectorKmerFreq[i].setFrequency(stoi(freq));
                    this->_size++;
                }
            }else
            {
                throw std::out_of_range("Número de kmeros inválido");
            }
        }else
        {
            throw std::invalid_argument("Números mágicos diferentes");
        }
        InputStream.close();
    }else
    {
        throw std::ios_base::failure("No se pudo abrir");
    }
}

void Profile::append(KmerFreq kmerFreq)
{
  int aux = findKmer(kmerFreq.getKmer());
    if(aux != -1)
     {
         _vectorKmerFreq[aux].setFrequency(_vectorKmerFreq[aux].getFrequency()+kmerFreq.getFrequency());
     }
    else
     {
    if(this->_size == DIM_VECTOR_KMER_FREQ)
     {
        throw out_of_range ("Vector lleno");
     }
    else
     {
         this->_vectorKmerFreq[_size].setKmer(kmerFreq.getKmer());
         this->_vectorKmerFreq[_size].setFrequency(kmerFreq.getFrequency());
         this->_size++;
     }  
  }
}

void Profile::normalize(const string validNucleotides)
{
    for(int i=0; i<this->_size; i++)
    {
        Kmer aux=this->_vectorKmerFreq[i].getKmer();
        aux.normalize(validNucleotides);
        this->_vectorKmerFreq[i].setKmer(aux);
    }
    for(int i=0; i<this->_size; i++)
    {
        int posicion=findKmer(this->_vectorKmerFreq[i].getKmer(), i+1, this->_size);
        if(posicion!=-1)
        {
            this->_vectorKmerFreq[i].setFrequency(this->_vectorKmerFreq[i].getFrequency()+this->_vectorKmerFreq[posicion].getFrequency());
            deletePos(posicion);
        }
    }
}

void Profile::deletePos(const int pos)
{
    if(pos<0 || pos>this->_size)
    {
        throw out_of_range("Fuera de rango");
    }else
    {
        for(int i=pos; i<this->_size-1; i++)
        {
            this->_vectorKmerFreq[i]=this->_vectorKmerFreq[i+1];
        }
    }
    this->_size--;
}

void Profile::zip(bool deleteMissing, const int lowerBound)
{
    for(int i=0; i<this->_size; i++)
    {
        for(int j=0; j<this->_vectorKmerFreq[i].getKmer().size(); j++)
        {
            deleteMissing=true;
            if(_vectorKmerFreq[i].getKmer().at(j)!=Kmer::MISSING_NUCLEOTIDE)
            {
                deleteMissing=false;
            }   
        }
        if(_vectorKmerFreq[i].getFrequency()<=lowerBound)
        {
            deleteMissing=true;
        }
        if(deleteMissing==true)
        {
            deletePos(i);
            i--;
        }
        deleteMissing=false;
    }  
}

void Profile::join(Profile &profile)
{
    for(int i=0; i<profile.getSize(); i++)
    {
        append(profile.at(i));
    }
}
