#!/bin/sh

fname=$1
rm -f ${fname}.zip
zip -j ${fname} Makefile NNet.cpp NNet.h preprocessor.pl preprocessor.sh README resultChecker.cpp resultChecker.h trainer.cpp trainer.sh test/best.nn
