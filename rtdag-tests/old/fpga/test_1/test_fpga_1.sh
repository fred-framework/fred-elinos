#!/bin/bash

PROB=test_fpga_1
../../dag -s zcu102-mat64-128-hf --solve -max-threads --create-bounds -osol $PROB.sol -olp $PROB.lp -odot $PROB.dot -oyaml $PROB.yaml \
	-d 120000000,120000000 \
		-t 20000000 -t 40000000 -a 1,4381910 -t 30000000 -t 40000000 -a 1,897615 -t 30000000 -t 20000000 -t 70000000 \
		-p 1,0 -p 2,1 -p 3,0 -p 4,3 -p 5,2 -p 5,4 -p 6,0 -p 5,6 \

dot -Tpdf $PROB.dot -o $PROB.pdf
