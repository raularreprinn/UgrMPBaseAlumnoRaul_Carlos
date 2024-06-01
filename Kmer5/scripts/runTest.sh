#!/bin/bash
# Author: Luis Castillo Vidal L.Castillo@decsai.ugr.es
# Pass test file $1
SECS_WAIT=${1:-100000}
echo "Tiempo de espera inicial: $SECS_WAIT"

function toUTF8 {
    iconv -t UTF-8 -f ISO-8859-15 < $1 > $1.utf8
}

function toISO {
    iconv -f UTF-8 -t ISO-8859-15 < $1 > $1.iso
}

function doTest  {
    printf "\n${WHITE}Testing $1 \t\t"
    if [ ! -f $1 ]
    then
        return 1
    fi
    # Parse the codes for the call
    if grep --quiet $CALL_MARK $1; # Cond1
    then 
        MAKEOUT=$TESTS_FOLDER/.outmake
        if  grep --quiet $RELEASE_MARK $1; 
        then
            LINE=$(grep $RELEASE_MARK $1 | sed "s/$RELEASE_MARK //")
            RELEASE=$LINE
        fi

        if [ "$DO_INTERACTIVE" == "YES" ]
        then
            printf "${WHITE}Generating fresh binaries \t${GRAY}"
            make -f Makefile CONF=$RELEASE build &> $MAKEOUT
        else
            printf "${WHITE}Generating fresh binaries \t${GRAY}"
            make -f Makefile CONF=$RELEASE build &> $MAKEOUT
        fi

        if  grep -i Error $MAKEOUT;
        then
           printf "${RED} Failed build${WHITE}\n"
           exit
        else
            printf "${GREEN}"$BINARY"${WHITE}\n"
        fi

        if  grep --quiet $RELEASE_MARK $1; 
        then
            BINARY=./dist/$RELEASE/GNU-Linux/$RELEASE
        else
            if grep "está actualizado" $MAKEOUT;
            then
                BINARY=$(grep "está actualizado" $MAKEOUT | sed "s/^.*'dist/dist/;s/' está actualizado.*$//")
            elif grep "is up to date" $MAKEOUT;
            then
                BINARY=$(grep "is up to date" $MAKEOUT | sed "s/^.*'dist/dist/;s/' is up to date.*$//")
            else
                BINARY=$(grep "g++ .* -o dist/" $MAKEOUT | sed "s/^.*-o //;s/ build.*$//")
            fi
        fi

        MYCALL=$(grep $CALL_MARK $1 | sed s/$CALL_MARK//)
        DUE_OUTPUT=$(sed -n /$OUTPUT_MARK/,\$p $1)
        if  grep --quiet $VALGRIND_MARK $1; 
        then
            FORCE_VALGRIND=YES
        else
            FORCE_VALGRIND=NO
        fi
        if [ $USE_VALGRIND == "YES" ] || [ $FORCE_VALGRIND == "YES" ]
        then 
            VALGRIND="valgrind --leak-check=full "
        else
            VALGRIND=" "
        fi
        FULL_BINARY=$VALGRIND$PROJECT_FOLDER/$BINARY
        echo "Test #$k [$1] $VALGRIND $BINARY $MYCALL" > $TESTS_FOLDER/.call$k

        # Auto-compose a shell for the purpose of trying the binary
        echo "touch $TESTS_FOLDER/.timeout">$TESTS_FOLDER/.run.sh
        echo "CMD=\"$FULL_BINARY $MYCALL 1> $TESTS_FOLDER/.out$k 2>&1\"">> $TESTS_FOLDER/.run.sh
        echo "eval \$CMD">>$TESTS_FOLDER/.run.sh
        echo "rm $TESTS_FOLDER/.timeout">>$TESTS_FOLDER/.run.sh
        chmod u+x $TESTS_FOLDER/.run.sh

        # Runs the shell without a timeout
        sh $TESTS_FOLDER/.run.sh

        if [ $USE_VALGRIND == "YES" ] || [ $FORCE_VALGRIND == "YES" ]
        then 
            VALGRIND_OUTPUT="\n        "$(grep -a "ERROR SUMMARY" $TESTS_FOLDER/.out$k | sed s/==[0-9]*==//)
            if grep -a "ERROR SUMMARY" $TESTS_FOLDER/.out$k | grep --quiet " 0 errors";
            then 
                VALGRIND_LEAKS=FALSE
            else
                VALGRIND_LEAKS=TRUE
            fi
        else
            VALGRIND_OUTPUT=""
            VALGRIND_LEAKS=FALSE
        fi

        echo "$DUE_OUTPUT" > $TESTS_FOLDER/.due$k
        if file $TESTS_FOLDER/.due$k | grep -a --quiet ISO;
        then
            toUTF8 $TESTS_FOLDER/.due$k
            cp $TESTS_FOLDER/.due$k.utf8 $TESTS_FOLDER/.due$k
            toUTF8 $TESTS_FOLDER/.out$k
            cp $TESTS_FOLDER/.out$k.utf8 $TESTS_FOLDER/.out$k
            DUE_OUTPUT=$(cat $TESTS_FOLDER/.due$k)
        fi 
        if grep -a --quiet $OUTPUT_MARK $TESTS_FOLDER/.out$k; 
        then
            REAL_OUTPUT=$(grep -a -v == $TESTS_FOLDER/.out$k | sed -n /$OUTPUT_MARK/,\$p )
        else
            if grep --quiet $OUTPUTFILE_MARK $1; 
            then
                FILE_OUTPUT=$(grep $OUTPUTFILE_MARK $1 | sed s/$OUTPUTFILE_MARK//)
                REAL_OUTPUT=$(echo $OUTPUT_MARK;cat $FILE_OUTPUT )
            else
                REAL_OUTPUT=$(echo  $OUTPUT_MARK; grep -v == $TESTS_FOLDER/.out$k)
            fi
        fi

        echo "$REAL_OUTPUT" > $TESTS_FOLDER/.real$k
        if file $TESTS_FOLDER/.real$k | grep -a --quiet ISO;
        then
            toUTF8 $TESTS_FOLDER/.real$k
            cp $TESTS_FOLDER/.real$k.utf8 $TESTS_FOLDER/.real$k
            REAL_OUTPUT=$(cat $TESTS_FOLDER/.real$k)
        fi 
        if [ -f $TESTS_FOLDER/.timeout ]
        then
            rm $TESTS_FOLDER/.timeout
            printf  "\r${RED}[ HANG ] Test #$k [$1] ($VALGRIND $BINARY $MYCALL) $VALGRIND_OUTPUT"
            cp $TESTS_FOLDER/.out$k $TESTS_FOLDER/.fail$k
            FAILED_TESTS=YES
        elif [ ! "$DUE_OUTPUT" == "$REAL_OUTPUT" ] || [ "$VALGRIND_LEAKS" == "TRUE" ]
        then
            printf  "\r${RED}[FAILED] Test #$k [$1] ($VALGRIND $BINARY $MYCALL) $VALGRIND_OUTPUT"
            cp $TESTS_FOLDER/.out$k $TESTS_FOLDER/.fail$k
            FAILED_TESTS=YES
        else
            printf  "\r${GREEN}[  OK  ] Test #$k [$1] ($VALGRIND $BINARY $MYCALL) $VALGRIND_OUTPUT"
        fi
    else # Cond1
        printf "${RED}[ SKIP ] Test #$k [$1] CALL MARK not found in $1 . ${GRAY}\n"
    fi # Cond 1
}
#
# Main Body
#
# Load configuration & moves to the project root folder
if [ -d ../Scripts ]
then
    source ../Scripts/doConfig.sh
    source ../Scripts/ansiterminal.sh
else
   if [ -d ../../Scripts ] # Si estamos en carpeta scripts del proyecto
   then
       source ../../Scripts/doConfig.sh
       source ../Scripts/ansiterminal.sh
   else
    printf "\n${RED}Unable to find Scripts library${WHITE}\n\n"
    exit
   fi
fi
VERSION=2.0
printf "\n${WHITE}VALIDATION TOOL v"$VERSION"\n"
if [ $1 ]
then
   if [ "$1" == "ALL" ]
   then
      TESTS=$TESTS_FOLDER/*.test
   else
       TESTS=$1
   fi
else
          TESTS=$TESTS_FOLDER/*.test
fi
printf "${WHITE}Checking tests folder \t\t"

if [ -z "$(ls -A $TESTS_FOLDER/*.test)" ]
then
    printf "${RED} No test found. Abort${WHITE}\n"
    exit
else
    printf "${GREEN}OK ${WHITE}\n"
fi
printf "Setting everything up \t\t"
CALL_MARK="%%%CALL"
DESCRIPTION_MARK="%%%DESCRIPTION"
OUTPUT_MARK="%%%OUTPUT"
OUTPUTFILE_MARK="%%%FROMFILE"
VALGRIND_MARK="%%%VALGRIND"
RELEASE_MARK="%%%RELEASE"
rm -f $TESTS_FOLDER/.out* $TESTS_FOLDER/.due* $TESTS_FOLDER/.real* $TESTS_FOLDER/.fail*
printf "${GREEN}OK${WHITE}\n"
printf "Performing unit tests \t\t${WHITE}\n"
FAILED_TESTS=NO
k=0
for f in $TESTS
do
    let k=$k+1
    doTest $f
done
printf "${WHITE} Summary of unit tests\n"
if [ $FAILED_TESTS == "YES" ]
then
    printf "${RED}Some tests failed. Check ${TESTS_FOLDER}/.out* and ${TESTS_FOLDER}/.fail* for more info\n\n${WHITE}"
    exit 1
else
    printf "${GREEN}All tests passed successfully\n\n${WHITE}"
fi
exit 0
