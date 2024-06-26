/*
 * Metodología de la Programación: Kmer5
 * Curso 2023/2024
 */

/** 
 * @file CLASSIFY.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * @author Javier Martínez Baena <jbaena@ugr.es>
 * 
 * Created on 22 December 2023, 10:00
 */

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc) 
 */

#include <iostream>
#include <cstring>
#include <fstream>

#include "KmerCounter.h"

using namespace std;

void showEnglishHelp(ostream& outputStream) {
    outputStream << "ERROR in CLASSIFY parameters" << endl;
    outputStream << "Run with the following parameters:" << endl;
    outputStream << "CLASSIFY [-k kValue] [-n nucleotidesSet] <file.dna> <profile1.prf> [<profile2.prf> <profile3.prf> ....]" << endl;
    outputStream << endl;
    outputStream << "Parameters:" << endl;
    outputStream << "-k kValue: number of nucleotides in a kmer (5 by default)" << endl;
    outputStream << "-n nucletiodesSet: set of possible nucleotides in a kmer (ACGT by default). "
                 << "It is used when learning a model for <file.dna>. " 
                 << "Note that the characters should be provided in uppercase" << endl;
    outputStream << "<profile1.prf> [<profile2.prf> <profile3.prf> ....] ....: "
                 << "names of the Profile models (at least one is mandatory)" << endl;
    outputStream << endl;
    outputStream << "This program obtains the identifier of the closest profile to the input DNA file" << endl;
    outputStream << endl;
}

/**
 * This program prints the profile identifier of the closest profile model
 * for an input DNA file (<file.dna>) among the set of provided models:
 * <profile1.prf>, <profile2.prf>, ...
 * The program uses the KmerCounter class to obtain a Profile for the input
 * file <file.dna>. That Profile should be zipped, to eliminate kmers with
 * any missing nucleotide, and sorted in decreasing order of frequency of
 * kmers. After that, the program compares the learned Profile with the ones
 * provided by the arguments <profile1.prf> [<profile2.prf> <profile3.prf> ....]
 * It classifies the input DNA file with the identifier of the Profile with
 * a minor distance.
 * 
 * This program assumes that the profile files are already normalized and 
 * sorted by frequency. This is not checked in this program. Unexpected results
 * will be obtained if those conditions are not met.
 * 
 * Running sintax:
 * > CLASSIFY [-k kValue] [-n nucleotidesSet] <file.dna> <profile1.prf> [<profile2.prf> <profile3.prf> ....]
 * 
 * Running example: 
 * > CLASSIFY ../Genomes/human_chr6_s60000_l500000.dna ../Genomes/brewers_yeast_chrVII.s1_l500000.prf ../Genomes/chimpanzee_chr9_s1_l500000.prf ../Genomes/covidFullGenomeDNA.prf ../Genomes/drosophila_chr2L_s1_l500000.prf ../Genomes/ebolaFullGenomeDNA.prf ../Genomes/human_chr9_s10000_l500000.prf ../Genomes/monkeypoxFullGenomeDNA.prf ../Genomes/mouse_chr6_s3050050_l500000.prf ../Genomes/nematode_chrI_s1l500000.prf ../Genomes/rat_chr6_s1l500000.prf ../Genomes/zebrafish_chr6_s1l500000.prf
Distance to ../Genomes/brewers_yeast_chrVII.s1_l500000.prf (saccharomyces cerevisiae): 0.20294
Distance to ../Genomes/chimpanzee_chr9_s1_l500000.prf (pan troglodytes): 0.0643864
Distance to ../Genomes/covidFullGenomeDNA.prf (severe acute respiratory syndrome coronavirus 2): 0.194633
Distance to ../Genomes/drosophila_chr2L_s1_l500000.prf (drosophila melanogaster): 0.189238
Distance to ../Genomes/ebolaFullGenomeDNA.prf (ebolavirus zaire): 0.179686
Distance to ../Genomes/human_chr9_s10000_l500000.prf (homo sapiens): 0.0557804
Distance to ../Genomes/monkeypoxFullGenomeDNA.prf (monkey pox virus): 0.262987
Distance to ../Genomes/mouse_chr6_s3050050_l500000.prf (mus musculus): 0.088129
Distance to ../Genomes/nematode_chrI_s1l500000.prf (caenorhabditis elegans): 0.221075
Distance to ../Genomes/rat_chr6_s1l500000.prf (rattus norvegicus): 0.111126
Distance to ../Genomes/zebrafish_chr6_s1l500000.prf (danio rerio): 0.145231

Final decision: homo sapiens with a distance of 0.0557804
 * 
 * @param argc The number of command line parameters
 * @param argv The vector of command line parameters (cstrings)
 * @return 0 If there is no error; a value > 0 if error
 */
int main(int argc, char *argv[]) {
    // Process the main() arguments
    if(argc<3)
    {
        showEnglishHelp(cerr);
        return 1;
    }
    
    int ini=0;
    int k=5;
    string nucleotides=KmerCounter::DEFAULT_VALID_NUCLEOTIDES;
    
    for(int i=1; i<argc||ini==i; i++)
    {
        string aux= argv[i];
        if(aux=="-k")
        {
            k=stoi(argv[i+1]);
            i++;
        }else if(aux=="-n")
        {
            nucleotides=argv[i+1];
            i++;
        }else if(aux.size()>3)
        {
            if(aux.substr(aux.size()-3,3)=="dna"||aux.substr(aux.size()-3,3)=="rna")
            {
                ini=i;
            }           
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
    
    if(ini<argc)
    {
        string prf=argv[ini+1];
        if(prf.size()<3)
        {
            if(prf.substr(prf.size()-3,3)!="prf")
            {
                showEnglishHelp(cerr);
                return 1;
            }
        }       
    }else
    {
        showEnglishHelp(cerr);
        return 1;
    }
    
    // Calculate the kmer frecuencies of the input genome file using 
    //    a KmerCounter object
    KmerCounter principal(k, nucleotides);
    principal.calculateFrequencies(argv[ini]);
    // Obtain a Profile object from the KmerCounter object
    Profile perfil;
    perfil=principal.toProfile();
    // Zip the Profile object
    perfil.zip();
    // Sort the Profile object
    perfil.sort();
    // Use a loop to print the distance from the input genome to 
    //   each one of the provided profile models
    Profile *perfiles= new Profile[argc-ini-1];
    for(int i=0; i<argc-ini-1; i++)
    {
        perfiles[i].load(argv[ini+i+1]);
    }
    double *distancias= new double[argc-ini-1];
    
    for(int i=0; i<argc-ini-1; i++)
    {
        distancias[i]=perfil.getDistance(perfiles[i]);
        cout << "Distance to " << argv[ini+i+1] << " (" << perfiles[i].getProfileId() << "): " << distancias[i] << endl;
    }
    // Print the identifier and distance to the closest profile
    int posicion=0;
    for(int i=1; i<argc-ini-1;i++)
        {
            if(distancias[posicion]>distancias[i])
            {
                posicion=i;
            }
        }
    cout << endl;
    cout <<"Final decision: " << perfiles[posicion].getProfileId() << " with a distance of " << distancias[posicion] << endl;
    
    delete []perfiles;
    delete []distancias;
    perfiles=NULL;
    distancias=NULL;
    
    return 0;
}

