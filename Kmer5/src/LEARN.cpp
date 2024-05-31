/*
 * Metodología de la Programación: Kmer5
 * Curso 2023/2024
 */

/** 
 * @file LEARN.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * @author Javier Martínez Baena <jbaena@ugr.es>
 * 
 * Created on 22 December 2023, 10:00
 */
#include <iostream>
#include <cstring>
#include <fstream>

#include "KmerCounter.h"

using namespace std;

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc) 
 */
void showEnglishHelp(ostream& outputStream) {
    outputStream << "ERROR in LEARN parameters" << endl;
    outputStream << "Run with the following parameters:" << endl;
    outputStream << "LEARN [-t|-b] [-k kValue] [-n nucleotidesSet] [-p profileId] [-o outputFilename] <file1.dna> [<file2.dna> <file3.dna> .... ]" << endl;
    outputStream << endl;
    outputStream << "Parameters:" << endl;
    outputStream << "-t|-b: text mode or binary mode for the output file (-t by default)" << endl;
    outputStream << "-k kValue: number of nucleotides in a kmer (5 by default)" << endl;
    outputStream << "-n nucleotidesSet: set of possible nucleotides in a kmer (ACGT by default). " 
                 << "Note that the characters should be provided in uppercase" << endl;
    outputStream << "-p profileId: profile identifier (unknown by default)" << endl;
    outputStream << "-o outputFilename: name of the output file (output.prf by default)" << endl;
    outputStream << "<file1.dna> <file2.dna> <file3.dna> ....: names of the input files (at least one is mandatory)" << endl;
    outputStream << endl;
    outputStream << "This program learns a profile model from a set of "<< 
            "input DNA files <file1.dna> <file2.dna> <file3.dna> ...." << endl;
    outputStream << endl;
}

/**
 * This program learns a Profile model from a set of input DNA files (file1.dna,
 * file2.dna, ...). The learned Profile object is then zipped (kmers with any 
 * missing nucleotide or with frequency equals to zero will be removed) 
 * and ordered by frequency and saved in 
 * the file outputFilename (or output.prf if the output file is not provided).
 * 
 * Running sintax:
 * > LEARN [-t|-b] [-k kValue] [-n nucleotidesSet] [-p profileId] [-o outputFilename] <file1.dna> [<file2.dna> <file3.dna> ....]
 * 
 * Running example:
 * > LEARN -k 2 -p bug -o /tmp/unknownACGT.prf ../Genomes/unknownACGT.dna
 * 
 * > cat /tmp/unknownACGT.prf
MP-KMER-T-1.0
bug
7
GG 2
AC 1
AG 1
AT 1
CC 1
GA 1
TA 1
 * 
 * @param argc The number of command line parameters
 * @param argv The vector of command line parameters (cstrings)
 * @return 0 If there is no error; a value > 0 if error
 */
int main(int argc, char *argv[]) {   
    // Process the main() arguments
    
    if(argc<2)
    {
        showEnglishHelp(cerr);
        return 1;
    }
    
    int ini=0;
    char modo='t';
    int k=5;
    string nucleotides=KmerCounter::DEFAULT_VALID_NUCLEOTIDES, id="unknown";
    char *ArchivoSalida="output.prf";
    
    for(int i=1; i<argc||ini==i; i++)
    {
        string aux= argv[i];
        if(aux=='-t' || aux=='-b')
        {
            modo=*argv[i];
        }else if(aux=='-p')
        {
            id=argv[i+1];
            i++;
        }else if(aux=='-k')
        {
            k=*argv[i+1];
            i++;
        }else if(aux=='-n')
        {
            nucleotides=argv[i+1];
            i++;
        }else if(aux=='-o')
        {
            ArchivoSalida=argv[i+1];
            i++;
        }else if(aux.substr(aux.find_last_of(".")+1)=="dna")
        {
            ini=i;
        }else
        {
            showEnglishHelp(cerr);
            return 1;
        }
    }
    
     if(ini==0)
    {
        showEnglishHelp(cerr);
        return 1;
    }
    // Loop to calculate the kmer frecuencies of the input genome files using 
    // a KmerCounter object
    KmerCounter principal(k, nucleotides);
    principal.calculateFrequencies(argv[ini]);
    if(ini<argc)
    {
        for(int i=ini; i<argc; i++)
        {
            KmerCounter aux(k, nucleotides);
            aux.calculateFrequencies(argv[i]);
            principal+=aux;
        }
        
    }
    // Obtain a Profile object from the KmerCounter object
    Profile perfil;
    perfil=principal.toProfile();
    perfil.setProfileId(id);
    // Zip the Profile object
    perfil.zip();
    // Sort the Profile object
    perfil.sort();
    // Save the Profile object in the output file
    perfil.save(ArchivoSalida, modo);
}

