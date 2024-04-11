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
    _profileId = "unknown";
    _size=0;
}

Profile::Profile(int size)
{
    if(size<0 || size > DIM_VECTOR_KMER_FREQ)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        _profileId = "unknown";
         for(int i=0; i<size; i++)
    {
        _vectorKmerFreq[i].setKmer(Kmer::MISSING_NUCLEOTIDE);
        _vectorKmerFreq[i].setFrequency(0);
    } 
    }
}

string Profile::getProfileId()
{
    return _profileId;
}

void Profile::setProfileId(string id)
{
    _profileId = id;
}

const KmerFreq Profile::at(int index) const
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

KmerFreq Profile::at(int index)
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

int Profile::getSize()
{
    return _size;
}

int Profile::getCapacity()
{
    return DIM_VECTOR_KMER_FREQ;
}

int Profile::findKmer(Kmer kmer, int initialPos, int finalPos)
{
    int posicion=-1;
    for(int i=initialPos; i<=finalPos; i++)
    {
        if(_vectorKmerFreq[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

int Profile::findKmer(Kmer kmer)
{
    int posicion=-1;
    for(int i=0; i<_size; i++)
    {
        if(_vectorKmerFreq[i].getKmer().toString()==kmer.toString())
            posicion=i;
    }
    return posicion;
}

string Profile::toString()
{
    string aux;
    aux = _profileId+"\n"+to_string(_size)+"\n";
    for(int i=0; i<_size; i++)
    {
        aux += _vectorKmerFreq[i].toString()+"\n";
    }
    return aux;
}

void Profile::sort()
{
    for(int i=0; i<_size-1; i++)
    {
        int posicion=i;
        for(int j=i+1; j<_size; j++)
        {
            if(_vectorKmerFreq[posicion].getFrequency()<_vectorKmerFreq[j].getFrequency())
                posicion=j;
            else if(_vectorKmerFreq[posicion].getFrequency()==_vectorKmerFreq[j].getFrequency())
            {
                if(_vectorKmerFreq[posicion].getKmer().toString()<_vectorKmerFreq[j].getKmer().toString())
                {
                    posicion = j;
                }
            }
        }
        KmerFreq aux=_vectorKmerFreq[posicion];
        _vectorKmerFreq[posicion]=_vectorKmerFreq[i];
        _vectorKmerFreq[i]=aux;
    }
}

void Profile::save(char fileName[])
{
    FILE *pFile;
    pFile=fopen(fileName,"w");
    if(pFile==NULL)
    {
        throw std::ios_base::failure("No se pudo abrir");
    }else
    {
        fprintf(pFile,"%s\n",Profile::MAGIC_STRING_T);
        fprintf(pFile,"%d\n",_size);
        for(int i=0; i<_size; i++)
        {
            fprintf(pFile,"%s %d\n",_vectorKmerFreq[i].getKmer(),_vectorKmerFreq[i].getFrequency());
        }
        fclose(pFile);
    }
}

void Profile::load(char fileName[])
{
    
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
    if(_size == DIM_VECTOR_KMER_FREQ)
     {
        throw out_of_range ("Vector lleno");
     }
    else
     {
         _vectorKmerFreq[_size].setKmer(kmerFreq.getKmer());
         _vectorKmerFreq[_size].setFrequency(kmerFreq.getFrequency());
         _size++;
     }  
  }
}

void Profile::normalize(string validNucleotides)
{
    for(int i=0; i<_size; i++)
    {
        Kmer aux=_vectorKmerFreq[i].getKmer();
        aux.normalize(validNucleotides);
        _vectorKmerFreq[i].setKmer(aux);
    }
    for(int i=0; i<_size; i++)
    {
        int posicion=findKmer(_vectorKmerFreq[i].getKmer(), i+1, _size-1);
        if(posicion!=-1)
        {
            _vectorKmerFreq[i].setFrequency(_vectorKmerFreq[i].getFrequency()+_vectorKmerFreq[posicion].getFrequency());
            deletePos(posicion);
        }
    }
}

void Profile::deletePos(int pos)
{
    if(pos<0 || pos>_size)
    {
        throw out_of_range("Fuera de rango");
    }else
    {
        for(int i=pos; i<_size-1; i++)
        {
            _vectorKmerFreq[i]=_vectorKmerFreq[i+1];
        }
    }
    _size--;
}

void Profile::zip()
{
  bool deleteMissing=false;
    const int lowerBound=0;
    for(int i=0; i<_size; i++)
    {
        for(int j=0; j<_vectorKmerFreq[i].getKmer().size(); j++)
        {
            if(_vectorKmerFreq[i].getKmer().at(j)==Kmer::MISSING_NUCLEOTIDE)
            {
                deleteMissing=true;
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

void Profile::join(Profile profile)
{
    for(int i=0; i<profile.getSize(); i++)
    {
        append(profile.at(i));
    }
}
