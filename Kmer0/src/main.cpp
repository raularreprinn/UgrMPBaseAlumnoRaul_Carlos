/*
 * Metodología de la Programación: Kmer0
 * Curso 2023/2024
 */

/* 
 * File:   main.cpp
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 *
 * Created on 24 October 2023, 13:58
 * Finished on 16 March 2024
 */

#include <iostream>
#include <string>

#include "Kmer.h"


using namespace std;

/**
 * This program first reads from the standard input an integer k (length of Kmer)
 * and a string with a genetic sequence. Then, it obtains from the genetic 
 * sequence, the list of kmers (of length k) and saves them in the array kmers. 
 * Then, the kmers are normalized. After that, the complementary kmers, 
 * converted to lowercase, are saved in the array complementaryKmers. Finally 
 * the kmers in the arrays kmers and complementaryKmers are shown in the 
 * standard output.
 * See the next example:
 * 
 * Running example:
 * > kmer0 < data/easyDNA5_missing.k0in
6
GCGCC<-->cgcgg
CGCCC<-->gcggg
GCCC_<-->cggg_
CCC_G<-->ggg_c
CC_G_<-->gg_c_
C_G_G<-->g_c_c
 */
int main(int argc, char* argv[]) {
    // This string contains the list of nucleotides that are considered as
    // valid within a genetic sequence. The rest of characters are considered as
    // unknown nucleotides 
    const string VALID_NUCLEOTIDES = "ACGT";
    
    // This string contains the list of complementary nucleotides for each
    // nucleotide in validNucleotides
    const string COMPLEMENTARY_NUCLEOTIDES = "TGCA";

    // This is a constant with the dimension of the array kmers
    const int DIM_ARRAY_KMERS = 100;
    
    // This is the array where the kmers of the input genetic sequence will be
    // saved
    Kmer kmers[DIM_ARRAY_KMERS];
    
    // This is the array where the complementary kmers will be
    // saved
    Kmer complementaryKmers[DIM_ARRAY_KMERS];
    
    // Read K (integer) and a string with the input nucleotides list
    int k, nKmers=0;
    string inputString;
    
    cin >> k;
    cin >> inputString;
    
    int tamaniostring=inputString.size();
    // Obtain the kmers: find the kmers in the input string and put them in an array of Kmers
    for(int i=0; i<tamaniostring && i<DIM_ARRAY_KMERS; i++)
    {
        if(tamaniostring-i>=k)
        {
            string kmerSequence = inputString.substr(i,k);
            kmers[i] = Kmer(kmerSequence);
            nKmers++;
        }
            // Si el número de kameros supera la dimensión el bucle se corta.
    }
    
  
    // Normalize each Kmer in the array
    for(int i=0; i<nKmers; i++)
    {
        kmers[i].normalize(VALID_NUCLEOTIDES);
    }
    // Obtain the complementary kmers and turn them into lowercase
    for(int i=0; i<nKmers; i++)
    {
        complementaryKmers[i]=kmers[i].complementary(VALID_NUCLEOTIDES,COMPLEMENTARY_NUCLEOTIDES);
        ToLower(complementaryKmers[i]);
    }
    // Show the list of kmers and complementary kmers as in the example
    cout << nKmers << endl;
    for(int i = 0; i< nKmers; i++)
    {
        cout << kmers[i].toString() << "<-->" << complementaryKmers[i].toString() << endl;
    }
    
    return 0;
}