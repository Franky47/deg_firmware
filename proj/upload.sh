#!/bin/bash
# Script de compilation des projets Qt Cross-Platform


PLATFORM=`uname`

case $PLATFORM in
        d* | D*)
                echo "#### Mac OS X Build ####";
				#make test
				#clear
				#echo Cleaning up...
				#make cleanall
				echo #
				echo #
				echo #
				echo Building...
				make
				echo #
				echo #
				echo #
				echo Creating Hex...
				make hex
				echo #
				echo #
				echo #
				echo Stats:
				make stats
				echo #
				echo #
				echo #
				echo Uploading:
				make upload
				#echo Cleaning up...
				#make -f ./mac/Makefile clean
				echo #
                ;;
        l* | L*)
                echo "#### Linux Build ####";
				cd linux
				clear
				#echo Cleaning up...
				#make cleanall
				echo #
				echo #
				echo #
				echo Building...
				make
				echo #
				echo #
				echo #
				echo Creating Hex...
				make hex
				echo #
				echo #
				echo #
				echo Stats:
				make stats
				echo #
				echo #
				echo #
				echo Uploading:
				make upload
				#echo Cleaning up...
				#make -f ./linux/Makefile clean
				echo #
                ;;
        *)
                echo "Unknown system, cannot launch make";
                ;;
esac
