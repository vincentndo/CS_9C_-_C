#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"

#define FALSE 0
#define TRUE !FALSE
#define MAXNUMQS 64
#define MAXSTR 80

char guess_question[MAXSTR + 1];
void PrintTreeHelper (TreeType tree, PositionType pos, char *spaces);

typedef struct treeStruct {
	char *nodes[MAXNUMQS];
} *TreeType;

typedef struct positionStruct {
	int nodeIndex;
} *PositionType;

char *getNode(TreeType tree, PositionType posPtr) {
	return tree->nodes[posPtr->nodeIndex];
}

void setNode_Memory(TreeType tree, PositionType posPtr, char *str) {
	tree->nodes[posPtr->nodeIndex] = str;
}

void setNode_String(TreeType tree, PositionType posPtr, char *str) {
	strcpy(tree->nodes[posPtr->nodeIndex], str);
}

PositionType makePosPtr(int index) {
	PositionType ptr = (PositionType) malloc(1 * sizeof(struct positionStruct));

	ptr->nodeIndex = index;

	return ptr;
}

int getIndex(PositionType ptr) {
	return ptr->nodeIndex;
}

void setIndex(PositionType ptr, int index) {
	ptr->nodeIndex = index;
}

TreeType InitTree (char *file) {
	FILE *stream;
	PositionType posPtr;

	TreeType tree = (TreeType) malloc(1 * sizeof(struct treeStruct));
	if (tree == NULL) {
		perror("Not enough space for tree");
		exit(EXIT_FAILURE);
	}

	char *storage = (char *) malloc(MAXNUMQS * (MAXSTR + 1) * sizeof(char));
	if (storage == NULL) {
		perror("Not enough space for data");
		free(tree);
		exit(EXIT_FAILURE);
	}

	if ((stream = fopen(file, "r")) == NULL) {
		perror("Cannot open file");
		free(tree);
		free(storage);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < MAXNUMQS; i++) {
		posPtr = makePosPtr(i);

		setNode_Memory(tree, posPtr, &storage[(MAXSTR + 1) * i]);
		char *nl = "\n";
		if (fgets(getNode(tree, posPtr), MAXSTR + 1, stream) != NULL) {
			;
		} 
		else {
			setNode_String(tree, posPtr, nl);
		}
		free(posPtr);
	}

	fclose(stream);
	return tree;
}

void WriteTree (TreeType tree, char *file) {
	FILE *stream;
	int check;
	PositionType ptr;

	stream = fopen(file, "w");

	for (int i = 0; i < MAXNUMQS; i++) {

		ptr = makePosPtr(i);
		check = fputs(getNode(tree, ptr), stream);
		free(ptr);

		if (check < 0) {
			perror("Write error");
			exit(EXIT_FAILURE);
		}
	}

	fclose(stream);
}

void PrintTree (TreeType tree) {
	PositionType posPointer = makePosPtr(0);

	printf("==== GAME TREE ====\n");
	PrintTreeHelper(tree, posPointer, "");
	printf("===================\n");
	free(posPointer);
}

void PrintTreeHelper (TreeType tree, PositionType pos, char *spaces) {
	printf("%s%s", spaces, getNode(tree, pos));

    if (IsLeaf(tree, pos)) {
        ;
    } else {
 
 		int index = getIndex(pos);
 		char newSpaces[MAXSTR + 1];
    	PositionType lptr = makePosPtr(index * 2 + 1);
    	PositionType rptr = makePosPtr(index * 2 + 2);

    	strcpy(newSpaces, spaces);
        strcat(newSpaces, "    ");
        PrintTreeHelper(tree, lptr, newSpaces);
        free(lptr);
        PrintTreeHelper(tree, rptr, newSpaces);
        free(rptr);
    }
}

PositionType Top (TreeType tree) {
	PositionType ptr = makePosPtr(0);

	return ptr;
}

boolean IsLeaf (TreeType tree, PositionType pos){
	int index = getIndex(pos);
	int ret;

    PositionType lptr = makePosPtr(index * 2 + 1);
    PositionType rptr = makePosPtr(index * 2 + 2);

    ret = strcmp(getNode(tree, lptr), "\n") == 0 && strcmp(getNode(tree, rptr), "\n") == 0;

    free(lptr);
    free(rptr);

	return ret;
}

boolean Answer (char *q) {
	char answer[MAXSTR + 1];

    printf("%s", q);
    fgets(answer, MAXSTR + 1, stdin);

    if (answer[0] == 'y') {
        return TRUE;
    } else {
        return FALSE;
    }
}

char *Question (TreeType tree, PositionType pos) {
	return getNode(tree, pos);
}

char *Guess (TreeType tree, PositionType pos) {
	char guess_animal[MAXSTR + 1];
	strncpy(guess_animal, getNode(tree, pos), strlen(getNode(tree, pos)) - 1);

	sprintf(guess_question, "Is it %s?\n", guess_animal);

    return guess_question;
}

PositionType YesNode (TreeType tree, PositionType pos) {
	int index = getIndex(pos);

	free(pos);
	PositionType yesPosPtr = makePosPtr(2 * index + 1);

	return yesPosPtr;
}

PositionType NoNode (TreeType tree, PositionType pos) {
	int index = getIndex(pos);

	free(pos);
	PositionType noPosPtr = makePosPtr(2 * index + 2);
	
	return noPosPtr;
}

void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
	int index = getIndex(pos);
	PositionType lPosPtr = makePosPtr(2 * index + 1);
	PositionType rPosPtr = makePosPtr(2 * index + 2);

	strcpy(getNode(tree, lPosPtr), getNode(tree, pos));
	strcpy(getNode(tree, pos), newQ);
	strcpy(getNode(tree, rPosPtr), newA);
	free(newA);
	free(newQ);
	free(lPosPtr);
	free(rPosPtr);
	free(pos);
}

void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
	char buf[MAXSTR + 1];
	char wrong_animal[MAXSTR + 1];

    printf("I give up. What is it? ");

    if ((*newA = (char *) malloc((MAXSTR + 1) * sizeof(char))) == NULL) {
        perror("Not enough space for new answer");
        exit(EXIT_FAILURE);
    }

    fgets(*newA, MAXSTR + 1, stdin);
    strncpy(buf, *newA, strlen(*newA) - 1);
    strncpy(wrong_animal, getNode(tree, pos), strlen(getNode(tree, pos)) - 1);
    printf("Provide a question whose answer is yes \nfor %s and no for %s.\n", wrong_animal, buf);

    if ((*newQ = (char *) malloc((MAXSTR + 1) * sizeof(char))) == NULL) {
        perror("Not enough space for new question");
        free(*newA);
        exit(EXIT_FAILURE);
    }

    fgets(*newQ, MAXSTR + 1, stdin);
}
