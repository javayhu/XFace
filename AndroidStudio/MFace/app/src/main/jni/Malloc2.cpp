static char rcsid[]= 
"@(#) $Id: Malloc.cpp,v 2.0 2005/11/08 00:50:11 kuni Exp $";
// ========================================================================
// (C) Copyright 2004,2005 Sony Corporation, All Rights Reserved
//
// NAME
//  Malloc.cpp 
//
// DESCRIPTION
//
// HISTORY
// Takano Hiroaki
// Ver 0.1 2005.01.17 初版

//
// LOG
//   $Log: Malloc.cpp,v $
//   Revision 2.0  2005/11/08 00:50:11  kuni
//   no message
//
//   Revision 1.1.1.1  2005/11/08 00:48:22  kuni
//   Initial Revision
//
//   Revision 1.2  2005/04/01 08:29:03  takano
//   ファイルの先頭にヘッダをつけたことと、誤記訂正
//
// ========================================================================
//

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

void *pvMalloc1D(
	size_t Zigen1,
	size_t Size // sizeof(int),sizeof(double),sizeof(Complex)
)
{
	void *pvThis;
	pvThis=malloc(Zigen1*Size);
	if(pvThis==NULL){
			fprintf(stderr,"Malloc Error1\n");
			exit(1);
	}
	return(pvThis);
}
void *ppvMalloc2D(
	size_t Zigen1,
	size_t Zigen2,
	size_t SizeP,  // example: sizeof(int *)
	size_t Size    // sizeof(int )
)
{
	unsigned int uiIndex1;
	void **ppvThis;
	ppvThis=(void **)malloc(Zigen1*SizeP);
	if(ppvThis==NULL){
			fprintf(stderr,"Malloc Error2\n");
			exit(1);
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		ppvThis[uiIndex1]=(void *)malloc(Zigen2*Size);
		if(ppvThis[uiIndex1]==NULL){
			fprintf(stderr,"Malloc Error2\n");
			exit(1);
		}
	}
	
	return(ppvThis);
}
void *pppvMalloc3D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
)
{
	unsigned int uiIndex1,uiIndex2;
	void ***pppvThis;
	pppvThis=(void ***)malloc(Zigen1*SizePP);
	if(pppvThis==NULL){
			fprintf(stderr,"Malloc Error3\n");
			exit(1);
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		pppvThis[uiIndex1]=(void **)malloc(Zigen2*SizeP);
		if(pppvThis[uiIndex1]==NULL){
			fprintf(stderr,"Malloc Error3\n");
			exit(1);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			pppvThis[uiIndex1][uiIndex2]=(void *)malloc(Zigen3*Size);
			if(pppvThis[uiIndex1][uiIndex2]==NULL){
				fprintf(stderr,"Malloc Error3\n");
				exit(1);
			}
		}
	}
	return(pppvThis);
}
void *ppppvMalloc4D(
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t SizePPP, // sizeof(int ***)
	size_t SizePP, // sizeof(int **)
	size_t SizeP,  // sizeof(int *)
	size_t Size    // sizeof(int )
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3;
	void ****ppppvThis;
	ppppvThis=(void ****)malloc(Zigen1*SizePPP);
	if(ppppvThis==NULL){
			fprintf(stderr,"Malloc Error4\n");
			exit(1);
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		ppppvThis[uiIndex1]=(void ***)malloc(Zigen2*SizePP);
		if(ppppvThis[uiIndex1]==NULL){
			fprintf(stderr,"Malloc Error4\n");
			exit(1);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			ppppvThis[uiIndex1][uiIndex2]=(void **)malloc(Zigen3*SizeP);
			if(ppppvThis[uiIndex1][uiIndex2]==NULL){
				fprintf(stderr,"Malloc Error4\n");
				exit(1);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				ppppvThis[uiIndex1][uiIndex2][uiIndex3]=(void *)malloc(Zigen4*Size);
				if(ppppvThis[uiIndex1][uiIndex2][uiIndex3]==NULL){
					fprintf(stderr,"Malloc Error4\n");
					exit(1);
				}
			}
		}
	}
	return(ppppvThis);
}
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
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3,uiIndex4;
	void *****pppppvThis;
	pppppvThis=(void *****)malloc(Zigen1*SizePPPP);
	if(pppppvThis==NULL){
			fprintf(stderr,"Malloc Error5\n");
			exit(1);
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		pppppvThis[uiIndex1]=(void ****)malloc(Zigen2*SizePPP);
		if(pppppvThis[uiIndex1]==NULL){
			fprintf(stderr,"Malloc Error5\n");
			exit(1);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			pppppvThis[uiIndex1][uiIndex2]=(void ***)malloc(Zigen3*SizePP);
			if(pppppvThis[uiIndex1][uiIndex2]==NULL){
				fprintf(stderr,"Malloc Error5\n");
				exit(1);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				pppppvThis[uiIndex1][uiIndex2][uiIndex3]=(void **)malloc(Zigen4*SizeP);
				if(pppppvThis[uiIndex1][uiIndex2][uiIndex3]==NULL){
					fprintf(stderr,"Malloc Error5\n");
					exit(1);
				}
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					pppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]=(void *)malloc(Zigen5*Size);
					if(pppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]==NULL){
						fprintf(stderr,"Malloc Error5\n");
						exit(1);
					}
				}
			}
		}
	}
	return(pppppvThis);
}
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
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3,uiIndex4,uiIndex5;
	void ******ppppppvThis;
	ppppppvThis=(void ******)malloc(Zigen1*SizePPPPP);
	if(ppppppvThis==NULL){
			fprintf(stderr,"Malloc Error6\n");
			exit(1);
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		ppppppvThis[uiIndex1]=(void *****)malloc(Zigen2*SizePPPP);
		if(ppppppvThis[uiIndex1]==NULL){
			fprintf(stderr,"Malloc Error6\n");
			exit(1);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			ppppppvThis[uiIndex1][uiIndex2]=(void ****)malloc(Zigen3*SizePPP);
			if(ppppppvThis[uiIndex1][uiIndex2]==NULL){
				fprintf(stderr,"Malloc Error6\n");
				exit(1);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				ppppppvThis[uiIndex1][uiIndex2][uiIndex3]=(void ***)malloc(Zigen4*SizePP);
				if(ppppppvThis[uiIndex1][uiIndex2][uiIndex3]==NULL){
					fprintf(stderr,"Malloc Error6\n");
					exit(1);
				}
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]=(void **)malloc(Zigen5*SizeP);
					if(ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]==NULL){
						fprintf(stderr,"Malloc Error6\n");
						exit(1);
					}
				}
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					for(uiIndex5=0;uiIndex5<Zigen5;uiIndex5++){
						ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4][uiIndex5]=(void *)malloc(Zigen6*Size);
						if(ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4][uiIndex5]==NULL){
							fprintf(stderr,"Malloc Error6\n");
							exit(1);
						}
					}
				}
			}
		}
	}
	return(ppppppvThis);
}


void Free1D(
	void *pvThis  // Use cast (void *) to recomment nomal free
//	size_t Zigen1
)
{
	free(pvThis);
}



void Free2D(
	void **ppvThis, // Use cast (void **)
	size_t Zigen1
//	size_t Zigen2
)
{
	unsigned int uiIndex1;
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		free(ppvThis[uiIndex1]);
	}
	free(ppvThis);
}
void Free3D(
	void ***pppvThis, // Use cast (void ***)
	size_t Zigen1,
	size_t Zigen2
//	size_t Zigen3
)
{
	unsigned int uiIndex1,uiIndex2;
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			free(pppvThis[uiIndex1][uiIndex2]);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		free(pppvThis[uiIndex1]);
	}
	free(pppvThis);
}

void Free4D(
	void ****ppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3
//	size_t Zigen4
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3;
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				free(ppppvThis[uiIndex1][uiIndex2][uiIndex3]);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			free(ppppvThis[uiIndex1][uiIndex2]);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		free(ppppvThis[uiIndex1]);
	}
	free(ppppvThis);
}

void Free5D(
	void *****pppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4
//	size_t Zigen5
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3,uiIndex4;
	
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					free(pppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]);
				}
			}
		}
	}


	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				free(pppppvThis[uiIndex1][uiIndex2][uiIndex3]);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			free(pppppvThis[uiIndex1][uiIndex2]);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		free(pppppvThis[uiIndex1]);
	}
	free(pppppvThis);
}

void Free6D(
	void ******ppppppvThis, // Use cast (void ****)
	size_t Zigen1,
	size_t Zigen2,
	size_t Zigen3,
	size_t Zigen4,
	size_t Zigen5
//	size_t Zigen6
)
{
	unsigned int uiIndex1,uiIndex2,uiIndex3,uiIndex4,uiIndex5;
	
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					for(uiIndex5=0;uiIndex5<Zigen5;uiIndex5++){
						free(ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4][uiIndex5]);
					}
				}
			}
		}
	}

	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				for(uiIndex4=0;uiIndex4<Zigen4;uiIndex4++){
					free(ppppppvThis[uiIndex1][uiIndex2][uiIndex3][uiIndex4]);
				}
			}
		}
	}


	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			for(uiIndex3=0;uiIndex3<Zigen3;uiIndex3++){
				free(ppppppvThis[uiIndex1][uiIndex2][uiIndex3]);
			}
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		for(uiIndex2=0;uiIndex2<Zigen2;uiIndex2++){
			free(ppppppvThis[uiIndex1][uiIndex2]);
		}
	}
	for(uiIndex1=0;uiIndex1<Zigen1;uiIndex1++){
		free(ppppppvThis[uiIndex1]);
	}
	free(ppppppvThis);
}


