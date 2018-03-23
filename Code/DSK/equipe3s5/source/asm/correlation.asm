;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	correlation.asm
;
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	.def _autocorrelation

	.data


	.text


_autocorrelation
	.asmfunc

	; Load data address
	MVKL A4,A7
	MVKH A4,A7

	ZERO A2

autoCorrelate:
	LDW *A7,


	[A1] B autoCorrelate
	NOP 5


	B B3
	NOP 5

	.endasmfunc
