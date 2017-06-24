#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"

#define FALSE 0
#define TRUE !FALSE
#define MAXNUMQS 64
#define MAXSTR 80

char guess_question[MAXSTR + 1];
TreeType ReadTreeHelper(TreeType tree, FILE *stream);
void WriteTreeHelper(TreeType tree, FILE *stream);
void PrintTreeHelper (TreeType tree, char *spaces);

typedef struct treeStruct {
	char string[MAXSTR + 1];
	struct treeStruct *left, *right;
} *TreeType;

typedef struct positionStruct {
	struct treeStruct *node;
} *PositionType;

TreeType getChild(TreeType tree, char *str) {
	if (strcmp(str, "left") == 0) {
		return tree->left;
	} else if (strcmp(str, "right") == 0) {
		return tree->right;
	} else {
		return NULL;
	}
}

void setChild(TreeType tree, TreeType child, char *str) {
	if (strcmp(str, "left") == 0) {
		tree->left = child;
	} else if (strcmp(str, "right") == 0) {
		tree->right = child;
	}
}

char *getString(TreeType tree) {
	return tree->string;
}

void setString(TreeType tree, char *str) {
	strcpy(tree->string, str);
}

PositionType makePosPtr(TreeType tree) {
	PositionType ptr = (PositionType) malloc(1 * sizeof(struct positionStruct));

	ptr->node = tree;

	return ptr;
}

TreeType getNode(PositionType ptr) {
	return ptr->node;
}

void setNode(PositionType ptr, TreeType tree) {
	ptr->node = tree;
}

TreeType InitTree (char *file) {
	FILE *stream;
	// PositionType posPtr;

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

	tree = ReadTreeHelper(tree, stream);

	// for (int i = 0; i < MAXNUMQS; i++) {
	// 	posPtr = makePosPtr(i);

	// 	setNode(tree, posPtr, &storage[(MAXSTR + 1) * i]);
	// 	if (fgets(getNode(tree, posPtr), MAXSTR + 1, stream) != NULL) {
	// 		;
	// 	}
	// 	free(posPtr);
	// }
	// printf("%p, %s\n", tree, tree->string);
	fclose(stream);
	return tree;
}

TreeType ReadTreeHelper(TreeType tree, FILE *stream) {
	char buf[MAXSTR + 1];

	if (!fgets(buf, MAXSTR + 1, stream) || strcmp(buf, "") == 0) {
		free(tree);
		return NULL;
	} else {
		setString(tree, buf);
		TreeType child = (TreeType) malloc(1 * sizeof(struct treeStruct));
		child = ReadTreeHelper(child, stream);
		setChild(tree, child, "left");
		if (child != NULL) {
			TreeType child2 = (TreeType) malloc(1 * sizeof(struct treeStruct));
			child2 = ReadTreeHelper(child2, stream);
			setChild(tree, child2, "right");
		}
		// printf("%p, %s\n", tree, tree->string);
		return tree;
	}
}

void WriteTree (TreeType tree, char *file) {
	FILE *stream;
	// int check;
	// PositionType ptr;

	stream = fopen(file, "w");

	// for (int i = 0; i < MAXNUMQS; i++) {

	// 	ptr = makePosPtr(i);
	// 	check = fputs(getNode(tree, ptr), stream);
	// 	free(ptr);

	// 	if (check < 0) {
	// 		perror("Write error");
	// 		exit(EXIT_FAILURE);
	// 	}
	// }
	WriteTreeHelper(tree, stream);

	fclose(stream);
}

void WriteTreeHelper(TreeType tree, FILE *stream) {
	if (tree != NULL) {
		fputs(getString(tree), stream);
		WriteTreeHelper(getChild(tree, "left"), stream);
		WriteTreeHelper(getChild(tree, "right"), stream);
	} else {
		fputs("\n", stream);
	}
}

void PrintTree (TreeType tree) {
	printf("==== GAME TREE ====\n");
	PrintTreeHelper(tree, "");
	printf("===================\n");
}

void PrintTreeHelper (TreeType tree, char *spaces) {
	printf("%s%s", spaces, getString(tree));

	PositionType posPointer = makePosPtr(tree);
    if (IsLeaf(tree, posPointer)) {
        ;
    } else {
 
 		char newSpaces[MAXSTR + 1];

    	strcpy(newSpaces, spaces);
        strcat(newSpaces, "    ");
        PrintTreeHelper(getChild(tree,"left"), newSpaces);
        PrintTreeHelper(getChild(tree,"right"), newSpaces);
    }
    free(posPointer);
}

PositionType Top (TreeType tree) {
	PositionType ptr = makePosPtr(tree);

	return ptr;
}

boolean IsLeaf (TreeType tree, PositionType pos){
	if (getChild(getNode(pos), "left") == NULL && getChild(getNode(pos), "right") == NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
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
	return getString(getNode(pos));
}

char *Guess (TreeType tree, PositionType pos) {
	char guess_animal[MAXSTR + 1];
	strncpy(guess_animal, getString(getNode(pos)), strlen(getString(getNode(pos))) - 1);

	sprintf(guess_question, "Is it %s?\n", guess_animal);

    return guess_question;
}

PositionType YesNode (TreeType tree, PositionType pos) {
	TreeType yesNodePtr = getChild(getNode(pos), "left");
	PositionType yesPtr = makePosPtr(yesNodePtr);
	free(pos);

	return yesPtr;
}

PositionType NoNode (TreeType tree, PositionType pos) {
	TreeType noNodePtr = getChild(getNode(pos), "right");
	PositionType noPtr = makePosPtr(noNodePtr);
	free(pos);
	
	return noPtr;
}

void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
	TreeType subTree = getNode(pos);
	TreeType leftChild = (TreeType) malloc(sizeof(struct treeStruct));
	setChild(subTree, leftChild, "left");
	TreeType rightChild = (TreeType) malloc(sizeof(struct treeStruct));
	setChild(subTree, rightChild, "right");

	setString(leftChild, getString(subTree));
	setString(subTree, newQ);
	setString(rightChild, newA);
	free(newA);
	free(newQ);
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
    strncpy(wrong_animal, getString(getNode(pos)), strlen(getString(getNode(pos))) - 1);
    printf("Provide a question whose answer is yes \nfor %s and no for %s.\n", wrong_animal, buf);

    if ((*newQ = (char *) malloc((MAXSTR + 1) * sizeof(char))) == NULL) {
        perror("Not enough space for new question");
        free(*newA);
        exit(EXIT_FAILURE);
    }

    fgets(*newQ, MAXSTR + 1, stdin);
}
