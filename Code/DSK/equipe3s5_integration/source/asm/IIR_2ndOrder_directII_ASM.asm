;****************************************************************************************
; APPEL :
;int IIR_2ndOrder_directII_ASM(int x, int w[], const short C[])
;
; A4 : Adresse du pointeur sur l'échantillon précédent
; B4 : Valeur de l'échantillon courant
; A6 : Adresse du début du tableau de coefficients du filtre, h[]
; B6 : Adresse de l'échantillon de sortie
;
; Return : la nouvelle adresse de cPtr (A4)
;
; NB:
; 1) La longueur du filtre passe-haut, N = 63 (ordre = 62), est fixe (hardcoded)
;    et n'est donc pas passée comme paramètre à la fonction
; 2) Comme le mode d'adressage circulaire est employé pour le tampon, il n'est pas nécessaire
;    de spécifier l'adresse de début du tampon mais simplement l'adresse de l'échantillon
;    courant. En effet, quand le pointeur du tampon est incrémenté ou décrémenté, le
;    mode d'adressage circulaire force le pointeur à rester dans le tampon ("wrapping"). Il suffit simplement
;    de spécifier la bonne longueur avec l'AMR et de bien aligner le tampon lors de sa
;    déclaration dans le programme C avec "#pragma DATA_ALIGN"
;
;****************************************************************************************


;	lmem : A9:A8 ou B1:B0
;	memA : A0
;   memB : B0
;   memC : B7


; Return : Amplitude de l'échantillon filtré (A4)

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
;	Décalage des variables intermédiaires w
;	*******************************************************************************/

	LDW				*B4,			A2
	LDW				*B4[1],			B2
	NOP 4

	STW				A2,				*B4[1]
	STW				B2,				*B4[2]
	NOP 4

; Retourner l'adresse à la fonction qui a fait l'appel (return)
	B B3
	NOP 5

		.end

