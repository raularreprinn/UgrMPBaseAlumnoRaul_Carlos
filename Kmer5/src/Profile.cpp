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
const string Profile::MAGIC_STRING_B="MP-KMER-B-1.0";

Profile::Profile()
{
    this->_profileId = "unknown";
    this->_size=0;
    allocate();
    this->_capacity=INITIAL_CAPACITY;
}

Profile::Profile(const int size)
{
    if(size<0)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        this->_profileId = "unknown";
        this->_size=size;
        allocate();
        while(_size>_capacity)
        {
            reallocate();
        }
    }
}

Profile::Profile(const Profile &orig)
{
    copy(orig);
}

Profile::~Profile()
{
    deallocate();
}

void Profile:: operator=(const Profile &orig)
{
    if(this!=&orig)
    {
        deallocate();
        copy(orig);
    }
}

const string Profile::getProfileId() const
{
    return this->_profileId;
}

void Profile::setProfileId(const string id)
{
    this->_profileId = id;
}

const KmerFreq Profile::at(const int index) const
{
    if(index<0 || index>_size)
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
    if(index<0 || index>_size)
    {
        throw out_of_range ("Fuera de rango");
    }
    else
    {
        return this->_vectorKmerFreq[index];
    }
}

const int Profile::getSize() const
{
    return this->_size;
}

const int Profile::getCapacity() const
{
    return _capacity;
}

double Profile::getDistance(const Profile &otherProfile)const
{
    int posicion;
    double suma=0.0;
    
    for(int i=0; i<_size; i++)
    {
        posicion=otherProfile.findKmer(at(i).getKmer());
        if(posicion==-1)
        {
            posicion=otherProfile.getSize();
        }
        
        suma=suma+abs(i-posicion);
    }
    
    double distancia=suma/(getSize()*otherProfile.getSize());
    return distancia;
}

int Profile::findKmer(Kmer kmer, const int initialPos, const int finalPos)const
{
    int posicion=-1;
    for(int i=initialPos; i<finalPos; i++)
    {
        if(this->_vectorKmerFreq[i].getKmer()==kmer)
            posicion=i;
    }
    return posicion;
}

int Profile::findKmer(const Kmer &kmer)const
{
    int posicion=-1;
    for(int i=0; i<this->_size; i++)
    {
        if(this->_vectorKmerFreq[i].getKmer()==kmer)
            posicion=i;
    }
    return posicion;
}

string Profile::toString() const
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
            if(_vectorKmerFreq[posicion]<_vectorKmerFreq[j])
                posicion=j;
        }
        KmerFreq aux=this->_vectorKmerFreq[posicion];
        this->_vectorKmerFreq[posicion]=_vectorKmerFreq[i];
        this->_vectorKmerFreq[i]=aux;
    }
}

void Profile::save(const char fileName[], const char mode)
{
    if(mode=='t')
    {
        ofstream outstream(fileName,ios::out);
        if(outstream)
        {
            outstream<<*this;
            outstream.close();
        }else
        {
            throw std::ios_base::failure("No se pudo abrir");
        }
    }else
    {
        ofstream outstream(fileName, ios::out|ios::binary);
        if(outstream)
        {
            outstream.write(reinterpret_cast<const char*>(&this->_profileId),sizeof(string));
            outstream.write(reinterpret_cast<const char*>(&this->_size),sizeof(int));
            for(int i=0; i<_size; i++)
            {
                _vectorKmerFreq[i].write(outstream);
            }
            outstream.close();
        }else
        {
            throw std::ios_base::failure("No se pudo abrir");
        }
    }
    
}

void Profile::load(const char fileName[])
{
    const char *archivo=fileName;
    string num_mag;
    ifstream InputStream;
    
    InputStream.open(archivo,ifstream::in);
    if(InputStream)
    {
        InputStream>>num_mag;
        InputStream.get();
        if(num_mag==MAGIC_STRING_T)
        {
            getline(InputStream,this->_profileId);
            InputStream >> *this;
        }else if(num_mag==MAGIC_STRING_B)
        {
            getline(InputStream,this->_profileId);
            string nkmers;
            getline(InputStream, nkmers);
            if(stoi(nkmers)>0)
            {
                InputStream.close();
                InputStream.open(archivo, ios::in|ios::binary);
                for(int i=0; i<4; i++)
                {
                    InputStream.get();
                }
                for(int i=0; i<stoi(nkmers);i++)
                {
                    if(_size==_capacity)
                    {
                        cout << "Llamar reallocate" << endl;
                        reallocate();
                    }
                    _vectorKmerFreq[i].read(InputStream);
                    cout << _vectorKmerFreq[i] << endl;
                    _size++;
                }
                cout << "Después del for" << endl;
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

void Profile::append(const KmerFreq &kmerFreq)
{
    int aux = findKmer(kmerFreq.getKmer());
    if(aux != -1)
    {
        _vectorKmerFreq[aux].setFrequency(_vectorKmerFreq[aux].getFrequency()+kmerFreq.getFrequency());
    }
    else
    {
        if(_size == _capacity)
        {
            reallocate();
        }
        this->_vectorKmerFreq[_size].setKmer(kmerFreq.getKmer());
        this->_vectorKmerFreq[_size].setFrequency(kmerFreq.getFrequency());
        _size++;
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
        int contador=0;
        for(int j=0; j<this->_vectorKmerFreq[i].getKmer().size(); j++)
        {
            if(_vectorKmerFreq[i].getKmer().at(j)==Kmer::MISSING_NUCLEOTIDE)
            {
                contador++;
            }
        }
        if(contador==this->_vectorKmerFreq[i].getKmer().size())
        {
            deleteMissing=true;
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

void Profile::join(const Profile &profile)
{
    if(profile.getProfileId()==this->_profileId)
    {
        for(int i=0; i<profile.getSize(); i++)
        {
            append(profile.at(i));
        }
    }
}

const KmerFreq & Profile::operator[](int index)const
{
    return _vectorKmerFreq[index];
}

KmerFreq & Profile::operator[](int index)
{
    return _vectorKmerFreq[index];
}

Profile &Profile::operator+=(const KmerFreq &kmerFreq)
{
    append(kmerFreq);
    return *this;
}

Profile &Profile::operator+=(const Profile &profile)
{
    join(profile);
    return *this;
}

void Profile::allocate()
{
    _vectorKmerFreq = new KmerFreq[INITIAL_CAPACITY];
}

void Profile::deallocate()
{
    delete []_vectorKmerFreq;
    _vectorKmerFreq = NULL;
    _capacity=0;
    _size=0;
}

void Profile::reallocate()
{
    KmerFreq * aux = new KmerFreq[_capacity+BLOCK_SIZE];
    int tam = _size;
    int cap = _capacity+BLOCK_SIZE;
    for(int i=0; i<_size; i++)
    {
        aux[i]=_vectorKmerFreq[i];
    }
    deallocate();
    _size=tam;
    _capacity=cap;
    _vectorKmerFreq=aux;
    aux = NULL;
}

void Profile::copy(const Profile &p)
{
    _capacity = p._capacity;
    _size = p._size;
    _profileId = p._profileId;
    _vectorKmerFreq = new KmerFreq[_capacity];
    
    for(int i=0; i<_size; i++)
        {
            _vectorKmerFreq[i].setKmer(p.at(i).getKmer());
            _vectorKmerFreq[i].setFrequency(p.at(i).getFrequency());
        }  
}

ostream & operator<<(std::ostream &os, const Profile &profile)
{
    os<< profile.toString();
    return os;
}

istream & operator>>(std::istream &is, Profile &profile)
{
    string nkmers;
    getline(is, nkmers);
            if(stoi(nkmers)>0)
            {
                for(int i=0; i<stoi(nkmers);i++)
                {
                    string freq;
                    string kmer;
                    is >> kmer;
                    is >> freq;
                    is.get();
                    //Kmer kmeraux(kmer);
                    KmerFreq aux;
                    aux.setKmer(kmer);
                    aux.setFrequency(stoi(freq));
                    profile.append(aux);
                }
            }else
            {
                throw std::out_of_range("Número de kmeros inválido");
            }
    return is;
}