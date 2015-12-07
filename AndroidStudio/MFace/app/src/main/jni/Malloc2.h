// ========================================================================
// (C) Copyright 2004,2005 Sony Corporation, All Rights Reserved
// @(#) $Id: Malloc.h,v 2.0 2005/11/08 00:50:11 kuni Exp $ 
//
// NAME
// Malloc.h
//
// DESCRIPTION
//
// HISTORY
//
// LOG
//   $Log: Malloc.h,v $
//   Revision 2.0  2005/11/08 00:50:11  kuni
//   no message
//
//   Revision 1.1.1.1  2005/11/08 00:48:22  kuni
//   Initial Revision
//
//   Revision 1.2  2005/05/27 08:48:08  takano
//   �w�b�_�[�Ƀw�b�_�[�������B
//
// ========================================================================

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


void *pvMalloc1D(
	size_t Zigen1,
	size_t Size // sizeof(int),sizeof(double),sizeof(Complex)
);
void *ppvMalloc2D(
	size_t Zigen1,
	size_t Zigen2,
	size_t SizeP,  // example: sizeof(int *)
	size_t Size    // sizeof(int )
);
void *pppvMalloc3D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
);
void *ppppvMalloc4D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t SizePPP, // sizeof(int ***)
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
);
void *pppppvMalloc5D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t Zigen5,
	size_t SizePPPP, // sizeof(int ****)
	size_t SizePPP, // sizeof(int ***)
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
);
void *ppppppvMalloc6D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t Zigen5,
	size_t Zigen6,
	size_t SizePPPPP, // sizeof(int *****)
	size_t SizePPPP, // sizeof(int ****)
	size_t SizePPP, // sizeof(int ***)
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
);

void Free1D(
	void *pvThis  // Use cast (void *) to recomment nomal free
//	size_t Zigen1
);

void Free2D(
	void **ppvThis, // Use cast (void **)
	size_t Zigen1
//	size_t Zigen2
);

void Free3D(
	void ***pppvThis, // Use cast (void ***)
	size_t Zigen1,
	size_t Zigen2
//	size_t Zigen3
);

void Free4D(
	void ****ppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3
//	size_t Zigen4
);

void Free5D(
	void *****pppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4
//	size_t Zigen5
);

void Free6D(
	void ******ppppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t Zigen5
//	size_t Zigen6
);

