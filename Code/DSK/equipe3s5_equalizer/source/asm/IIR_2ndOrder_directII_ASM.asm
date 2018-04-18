;****************************************************************************************
; APPEL :
;int IIR_2ndOrder_directII_ASM(int x, int w[], const short C[])
;
; A4 : Adresse du pointeur sur l'�chantillon pr�c�dent
; B4 : Valeur de l'�chantillon courant
; A6 : Adresse du d�but du tableau de coefficients du filtre, h[]
; B6 : Adresse de l'�chantillon de sortie
;
; Return : la nouvelle adresse de cPtr (A4)
;
; NB:
; 1) La longueur du filtre passe-haut, N = 63 (ordre = 62), est fixe (hardcoded)
;    et n'est donc pas pass�e comme param�tre � la fonction
; 2) Comme le mode d'adressage circulaire est employ� pour le tampon, il n'est pas n�cessaire
;    de sp�cifier l'adresse de d�but du tampon mais simplement l'adresse de l'�chantillon
;    courant. En effet, quand le pointeur du tampon est incr�ment� ou d�cr�ment�, le
;    mode d'adressage circulaire force le pointeur � rester dans le tampon ("wrapping"). Il suffit simplement
;    de sp�cifier la bonne longueur avec l'AMR et de bien aligner le tampon lors de sa
;    d�claration dans le programme C avec "#pragma DATA_ALIGN"
;
;****************************************************************************************


;	lmem : A9:A8 ou B1:B0
;	memA : A0
;   memB : B0
;   memC : B7


; Return : Amplitude de l'�chantillon filtr� (A4)

		.def	_IIR_2ndOrder_directII_ASM

_IIR_2ndOrder_directII_ASM:


;	/*******************************************************************************
;	w(n) = a0*x(n)-a1*w(n-1)-a2*w(n-2)
;	*******************************************************************************/

;saturation
	MVKL			0x00FFFFFF,		A1
	MVKH			0x00FFFFFF,		A1
	CMPGT			A4,				A1,				A2
	[A2]MV			A1,				A4

	MVKL			0xFF000000,		A1
	MVKH			0xFF000000,		A1
	CMPLT			A4,				A1,				A2
	[A2]MV			A1,				A4


;C3 C4
	LDH				*A6[3],			A2
	NOP 4
	MPYID			A2,				A4,				A9:A8
	NOP 9

	SHR				A9:A8,			13,				A1:A0;memA

	LDH				*A6[4],			A2
	LDW				*B4[1],			B2
	NOP 4
	MPYID			A2,				B2,				B9:B8
	NOP 9

	SHR				B9:B8,			13,				B1:B0;memB
	SUB				A0,				B0,				A7;memC

;C5
	LDH				*A6[5],			A2
	LDW				*B4[2],			B2
	NOP 4
	MPYID			A2,				B2,				B9:B8
	NOP 9

	SHR				B9:B8,			13,				B1:B0;memB
	SUB				A7,				B0,				A0;memA

;saturation
	MVKL			0x00FFFFFF,		B1
	MVKH			0x00FFFFFF,		B1
	CMPGT			A0,				B1,				A2
	[A2]MV			B1,				A0

	MVKL			0xFF000000,		B1
	MVKH			0xFF000000,		B1
	CMPLT			A0,				B1,				A2
	[A2]MV			B1,				A0

	STW				A0,				*B4
	NOP 4

;	/*******************************************************************************
;	y(n) = B8*w(n)+B9*w(n-1)+b2*w(n-2)
;	*******************************************************************************/


	LDH				*A6,			A2
	LDW				*B4,			B2
	NOP 4
	MPYID			A2,				B2,				A9:A8
	NOP 9

	SHR				A9:A8,			13,				A1:A0;memA

	LDH				*A6[1],			A2
	LDW				*B4[1],			B2
	NOP 4
	MPYID			A2,				B2,				B9:B8
	NOP 9

	SHR				B9:B8,			13,				B1:B0;memB
	ADD				A0,				B0,				A7;memC

;C5
	LDH				*A6[2],			A2
	LDW				*B4[2],			B2
	NOP 4
	MPYID			A2,				B2,				A9:A8
	NOP 9

	SHR				A9:A8,			13,				A1:A0;memA
	ADD				A7,				A0,				A4;y

;	/*******************************************************************************
;	D�calage des variables interm�diaires w
;	*******************************************************************************/

	LDW				*B4,			A2
	LDW				*B4[1],			B2
	NOP 4

	STW				A2,				*B4[1]
	STW				B2,				*B4[2]
	NOP 4

; Retourner l'adresse � la fonction qui a fait l'appel (return)
	B B3
	NOP 5

		.end

