#!/bin/bash

PROB=test_fpga_5
../../dag -s zcu102-mat64-128-hf --solve -max-threads --create-bounds -osol $PROB.sol -olp $PROB.lp -odot $PROB.dot -oyaml $PROB.yaml \
	-d 120000000,120000000 \
		-t 20000000 -t 40000000 -t 30000000 -t 40000000 -t 30000000 -t 20000000 -t 60000000 -t 10000000 \
		-p 1,0 -p 2,1 -p 3,0 -p 4,3 -p 5,2 -p 5,4 -p 6,0 -p 5,6 -p 7,6 -p 5,7 \

dot -Tpdf $PROB.dot -o $PROB.pdf
