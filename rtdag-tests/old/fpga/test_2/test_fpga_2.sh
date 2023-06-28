#!/bin/bash

PROB=test_fpga_2
../../dag -s zcu102-mat64-128-hf --solve -max-threads --create-bounds -osol $PROB.sol -olp $PROB.lp -odot $PROB.dot -oyaml $PROB.yaml \
	-d 100000000,100000000 \
		-t 20000000 -t 40000000 -t 30000000 -a 1,1276500 -t 40000000 -a 1,897615 -t 20000000 \
		-p 1,0 -p 2,1 -p 3,0 -p 4,3 -p 4,2 \

dot -Tpdf $PROB.dot -o $PROB.pdf
