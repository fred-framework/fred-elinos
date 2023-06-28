#!/bin/bash

PROB=test_fpga_4
../../dag -s zcu102-mat64-128-hf --solve -max-threads --create-bounds -osol $PROB.sol -olp $PROB.lp -odot $PROB.dot -oyaml $PROB.yaml \
	-d 90000000,90000000 \
		-t 20000000 -t 40000000 -a 1,897615 -t 30000000 -t 1000000 -a 1,4381910 -t 20000000 -t 30000000  \
		-p 1,0 -p 2,0 -p 3,0 -p 4,0 -p 5,1 -p 5,2 -p 5,3 -p 5,4 \

dot -Tpdf $PROB.dot -o $PROB.pdf
