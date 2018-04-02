


	.def _getIFRASM


	.data

	.text



_getIFRASM
	.asmfunc

	MVC IFR,B4
	MV B4,A4

	B B3
	NOP 5

	.endasmfunc
