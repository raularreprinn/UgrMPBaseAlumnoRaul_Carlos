/*
 * Metodología de la Programación: Kmer3
 * Curso 2023/2024
 */

/**
 * @file main.cpp
 * @author Raúl Antonio Arredondo Principal <raularreprin@correo.ugr.es>
 * @author Carlos Romero García <cromgar939@correo.ugr.es>
 * Created on 23 April 2024, 17:00
 */

#include <iostream>
#include <cstring>
#include <fstream>

#include "Profile.h"

using namespace std;


/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown (for example,
 * cout, cerr, etc) 
 */
void showEnglishHelp(ostream& outputStream) {
    outputStream << "ERROR in Kmer3 parameters" << endl;
    outputStream << "Run with the following parameters:" << endl;
    outputStream << "kmer3 [-t min|max] <file1.prf> <file2.prf> [ ... <filen.prf>]" << endl;
    outputStream << endl;
    outputStream << "Parameters:" << endl;
    outputStream << "-t min | -t max: search for minimun distances or maximum distances (-t min by default)" << endl;
    outputStream << "<file1.prf>: source profile file for computing distances" << endl;
    outputStream << "<file2.prf> [ ... <filen.prf>]: target profile files for computing distances" << endl;  
    outputStream << endl;
    outputStream << "This program computes the distance from profile <file1.prf> to the rest" << endl;
    outputStream << endl;
}

/**
 * This program reads an undefined number of Profile objects from the set of 
 * files passed as parameters to main(). All the Profiles object, except the 
 * first one, must be stored in a dynamic array of Profile objects. Then, 
 * for each Profile in the dynamic array, this program prints to the 
 * standard output the name of the file of that Profile and the distance from 
 * the first Profile to the current Profile. 
 * Finally, the program should print in the standard output, the name of 
 * the file with the Profile with the minimum|maximum  distance to the Profile 
 * of the first file and its profile identifier.
 * 
 * At least, two Profile files are required to run this program.
 * 
 * This program assumes that the profile files are already normalized and 
 * sorted by frequency. This is not checked in this program. Unexpected results
 * will be obtained if those conditions are not met.
 * 
 * Running sintax:
 * > kmer3 [-t min|max] <file1.prf> <file2.prf> [  ... <filen.prf>] 
 * 
 * Running example:
 * > kmer3 ../Genomes/human1.prf ../Genomes/worm1.prf ../Genomes/mouse1.prf 
Distance to ../Genomes/worm1.prf: 0.330618
Distance to ../Genomes/mouse1.prf: 0.224901
Nearest profile file: ../Genomes/mouse1.prf
Identifier of the nearest profile: mus musculus
 * 
 * Running example:
 * > kmer3 -t max ../Genomes/human1.prf ../Genomes/worm1.prf ../Genomes/mouse1.prf 
Distance to ../Genomes/worm1.prf: 0.330618
Distance to ../Genomes/mouse1.prf: 0.224901
Farthest profile file: ../Genomes/worm1.prf
Identifier of the farthest profile: worm
 */
int main(int argc, char* argv[]) {
    
    // Process the main() arguments
    bool min=true;
    int ini=1;
    
    if(argc<3)
    {
        showEnglishHelp(cerr);
        return 1;
    }
    
    if(strcmp(argv[1],"-t")==0)
    {
        ini++;
        if(strcmp(argv[2],"min")==0)
        {
            ini++;
        }else if(strcmp(argv[2],"max")==0)
        {
            ini++;
            min=false;
        }else
        {
            showEnglishHelp(cerr);
            return 1;
        }
    }else
    {
        string arg1=argv[1];
        if(strcmp(argv[1],"min")==0 || strcmp(argv[1],"max")==0)
        {
            ini++;
        }else if(arg1.substr(arg1.find_last_of(".")+1)!="prf")
        {
            showEnglishHelp(cerr);
            return 1;
        }
    }
    
    if(argc-ini<2)
    {
        showEnglishHelp(cerr);
        return 1;
    }
    
    // Allocate a dynamic array of Profiles
    Profile p1;
    Profile *perfiles= new Profile[argc-ini-1];
    
    // Load the input Profiles
    p1.load(argv[ini]);
    for(int i=0; i<argc-ini-1; i++)
    {
        perfiles[i].load(argv[ini+i+1]);
    }
    
    // Calculate and print the distance from the first Profile to the rest
    double *distancias= new double[argc-ini-1];
    
    for(int i=0; i<argc-ini-1; i++)
    {
        distancias[i]=p1.getDistance(perfiles[i]);
        cout << "Distance to " << argv[ini+i+1] <<  ": " << distancias[i] << endl;
    }
    
    // Print name of the file and identifier that takes min|max distance to the first one
    int posicion=0;
    if(min==true)
    {
        for(int i=1; i<argc-ini-1;i++)
        {
            if(distancias[posicion]>distancias[i])
            {
                posicion=i;
            }
        }
        cout <<  "Nearest profile file: " << argv[ini+posicion+1]<< endl<< "Identifier of the nearest profile: " << perfiles[posicion].getProfileId()<<endl;
    }else
    {
        for(int i=1; i<argc-ini-1;i++)
        {
            if(distancias[posicion]<distancias[i])
            {
                posicion=i;
            }
        }
        cout <<  "Farthest profile file: " << argv[ini+posicion+1]<< endl<< "Identifier of the farthest profile: " << perfiles[posicion].getProfileId()<<endl;
    }

    // Deallocate the dynamic array of Profile
    delete []perfiles;
    delete []distancias;
    perfiles=NULL;
    distancias=NULL;
    
    return 0;
}
