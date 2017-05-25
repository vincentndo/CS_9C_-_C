typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSTRLEN 80
#define MAXNUMQS 32

typedef char **TreeType;
typedef int PositionType;

/*
 * Function prototypes.
 */
TreeType InitTree ();
void PrintTree (TreeType tree, int pos, char *spaces);
PositionType Top (TreeType tree);
boolean IsLeaf (TreeType tree, PositionType pos);
boolean Answer (char *question);
char *Question (TreeType tree, PositionType pos);
char *Guess (TreeType tree, PositionType pos);
PositionType YesNode (TreeType tree, PositionType pos);
PositionType NoNode (TreeType tree, PositionType pos);
void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ);
void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ);
char guess_question[MAXSTRLEN + 1];

/*
 *  Play the "animal" game, in which the program attempts to guess an animal
 *  that the user is thinking of by asking yes or no questions. Eventually,
 *  the program either will guess the user's animal or run out of questions
 *  to ask. In the latter case, the program will ask the user to provide a
 *  yes-or-no question that would distinguish between the user's animal and
 *  the program's best guess.
 *  The data structure of questions and guesses is essentially a binary tree,
 *  with each internal node having a "yes" branch and a "no" branch. Leaves
 *  of the tree represent animals to be guessed by the program. If the program
 *  fails to guess the user's animal, it replaces one of the leaves of the tree
 *  by a node containing the new question, whose children are the program's
 *  best guess and the animal provided by the user.
 *  The structure of the program is simple. It initializes the question/guess
 *  data structure, then plays games as long as the user is interested. In each
 *  game, the program starts at the top of the tree (the root) and progresses
 *  toward the bottom (the leaves) depending on the user's responses. Once it
 *  reaches a leaf, it either has won or lost, and handles the situation as
 *  described above.
 */
void main ( ) {
    TreeType tree;
    PositionType pos;
    char *newQuestion, *newAnswer;
    tree = InitTree ();

    printf("%s", "Think of an animal. I will try to guess what it is.\n"
		 "Please answer my questions with yes or no.\n");

    while (TRUE) {
        // char spaces[MAXSTRLEN + 1] = "";
	pos = Top (tree);
	while (!IsLeaf (tree, pos)) {
	    pos = Answer (Question (tree, pos))? 
		YesNode (tree, pos): NoNode (tree, pos);
	}
	if (Answer (Guess (tree, pos))) {
	    printf ("I got it right!\n");
	} else {
	    GetNewInfo (tree, pos, &newAnswer, &newQuestion);
	    ReplaceNode (tree, pos, newAnswer, newQuestion);
	}
    printf("==== GAME TREE ====\n");
    PrintTree(tree, 0, "");
    printf("===================\n");
	if (!Answer ("Want to play again? ")) {
	    exit (0);
	}
    }
}

/*
 *	Return an animal tree.
 */
TreeType InitTree () {
    /* Your code goes here -- delete this line */
    char ** ret = (char **) malloc(MAXNUMQS * sizeof(char *));

    if (ret == NULL) {
        perror("Not enough space for pointer array");
        exit(EXIT_FAILURE);
    }

    char * storage = (char *) malloc(MAXNUMQS * (MAXSTRLEN + 1) * sizeof(char));

    if (storage == NULL) {
        perror("Not enough space for data");
        free(ret);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAXNUMQS; i++) {
        ret[i] = &storage[(MAXSTRLEN + 1) * i];
        strcpy(ret[i], "");
    }

    strcpy(ret[0],"a cat");

    return ret;
}

/*
 *	Print an animal tree (useful for debugging).
 */
void PrintTree (TreeType tree, int pos, char *spaces) {
    /* Your code goes here -- delete this line */
    printf("%s%s\n", spaces, tree[pos]);

    if (IsLeaf(tree, pos)) {
        ;
    } else {
        char newSpaces[MAXSTRLEN + 1];

        strcpy(newSpaces, spaces);
        strcat(newSpaces, "    ");
        PrintTree(tree, 2 * pos + 1, newSpaces);
        PrintTree(tree, 2 * pos + 2, newSpaces);
    }
}

/*
 *	Return the position of the "top" node in the animal tree.
 */
PositionType Top (TreeType tree)
{
    /* Your code goes here -- delete this line */
    return 0;
}

/*
 *	Return true exactly when pos is a "leaf" of the animal tree,
 *	that is, when the string stored at pos is a guess rather than
 *	a question.
 */
boolean IsLeaf (TreeType tree, PositionType pos) {
    /* Your code goes here -- delete this line */
    return strcmp(tree[2*pos + 1], "") == 0 && strcmp(tree[2*pos + 2], "") == 0;
}

/*
 *	Return true if the answer supplied by the user to the given question
 *	starts with 'y', false otherwise.
 */
boolean Answer (char *question) {
    /* Your code goes here -- delete this line */
    char answer[MAXSTRLEN + 1];

    printf("%s\n", question);
    fgets(answer, MAXSTRLEN + 1, stdin);

    if (answer[0] == 'y') {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 *	Form a question out of the string stored at position pos in the given
 *	animal tree.
 */
char *Question (TreeType tree, PositionType pos) {
    /* Your code goes here -- delete this line */
    return tree[pos];
}

/*
 *	Form a guess out of the string stored at position pos in the given
 *	animal tree.  (IsLeaf(tree, pos) must be true.)
 */
char *Guess (TreeType tree, PositionType pos) {
    /* Your code goes here -- delete this line */
    sprintf(guess_question, "Is it %s?", tree[pos]);

    return guess_question;
}

/*
 *	Return the position of the node that corresponds to a "yes" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType YesNode (TreeType tree, PositionType pos) {
    /* Your code goes here -- delete this line */
    return 2 * pos + 1;
}

/*
 *	Return the position of the node that corresponds to a "no" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType NoNode (TreeType tree, PositionType pos) {
    /* Your code goes here -- delete this line */
    return 2 * pos + 2;
}

/*
 *	Replace the node at position pos in the given animal tree by a node
 *	containing the given new question whose left child (down the tree in 
 *	the "yes" direction) contains the string stored at position pos, and 
 *	whose right child contains the new answer newA.
 */
void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
    /* Your code goes here -- delete this line */
    strcpy(tree[2*pos + 1], tree[pos]);
    strcpy(tree[pos], newQ);
    strcpy(tree[2*pos + 2], newA);
    free(newA);
    free(newQ);
}

/*
 *	Admit that you guessed wrong, ask the player what animal he/she was 
 *	thinking of, and return this in *newA.  Also ask for a question that 
 *	would be answered "yes" for what you guessed and "no" for what the 
 *	player was thinking of, and return this in *newQ.  (The node with
 *	your guess is at position pos in the tree.)
 */
void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
    /* Your code goes here -- delete this line */
    char buf[MAXSTRLEN + 1];

    printf("I give up. What is it? ");

    if ((*newA = (char *) malloc((MAXSTRLEN + 1) * sizeof(char))) == NULL) {
        perror("Not enough space for new answer");
        exit(EXIT_FAILURE);
    }

    fgets(buf, MAXSTRLEN + 1, stdin);
    strncpy(*newA, buf, strlen(buf) - 1);
    printf("Provide a question whose answer is yes \nfor %s and no for %s.\n", tree[pos], *newA);

    if ((*newQ = (char *) malloc((MAXSTRLEN + 1) * sizeof(char))) == NULL) {
        perror("Not enough space for new question");
        free(*newA);
        exit(EXIT_FAILURE);
    }

    fgets(buf, MAXSTRLEN + 1, stdin);
    strncpy(*newQ, buf, strlen(buf) - 1);
}
