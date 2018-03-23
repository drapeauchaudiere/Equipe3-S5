;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; interrupt.asm
;
; Author : Simon Milhomme
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	.def _enableInterrupts
	.def _enableInterrupt11
	.def _disableInterrupt11

	.data
Mask_NMIE .set 0x2				; Mask to enable non maskable interrupts in the IER
Mask_INT4 .set 0x10				; Mask to set interrupt 4 in the IER
Mask_INT11 .set 0x800			; Mask to set interrupt 11 in the IER
tildeMask_INT11 .set 0xFFFFF7FF	; ~0x800 used to clear interrupt 11 in the IER
Mask_INT14 .set 0x4000			; Mask to set interrupt 14 in the IER
Mask_INT15 .set 0x8000			; Mask to set interrupt 15 in the IER
Mask_GIE .set 0x1				; Mask to globally enable interrupts in the CSR
Mask_AllInt .set 0xFFFFFFFF		; Mask for all interrupts

	.text


_enableInterrupts
	.asmfunc

	; Clear flags
	MVKL Mask_AllInt,B4
	MVKH Mask_AllInt,B4
	MVC B4,ICR

	; Set Interrupts
	MVC IER, B5
	MVK Mask_NMIE, B4
	OR B5,B4,B5
	MVK Mask_INT14,B4
	OR B5,B4,B5
	MVKL Mask_INT15,B4
	MVKH Mask_INT15,B4
	OR B5,B4,B5
	MVK Mask_INT4,B4
	OR B5,B4,B5
	MVC B5,IER

	; Enable interrupts globally
	MVC CSR,B5
	MVK Mask_GIE,B4
	OR B5,B4,B5
	MVC B5,CSR

	B B3
	NOP 5

	.endasmfunc

_enableInterrupt11
	.asmfunc

	MVC IER, B5
	MVK Mask_INT11,B4
	OR B5,B4,B5
	MVC B5,IER

	B B3
	NOP 5

	.endasmfunc

_disableInterrupt11
	.asmfunc

	MVC IER, B5
	MVKL tildeMask_INT11,B4
	MVKH tildeMask_INT11,B4
	AND B5,B4,B5
	MVC B5,IER

	B B3
	NOP 5

	.endasmfunc
