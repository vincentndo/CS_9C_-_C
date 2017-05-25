#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_vowel(char ch);

int main() {
	char query[] = "Enter a word: ";
	char word[32], new_word[32];
	char consonant[8];

	printf("%s", query);
	scanf("%s", word);

	if (is_vowel(word[0])) {
		strcpy(new_word, word);
		strcat(new_word,"yay");
		printf("%s\n", new_word);
	} else if (word[0] == 'q') {
		strcpy(new_word, &word[2]);
		strcat(new_word, "quay");
		printf("%s\n", new_word);
	} else {
		int count = 0;
		int word_length = strlen(word);
		while (!is_vowel(word[count]) && count < word_length) {
			if (count && word[count] == 'y') {
				break;
			} else if (count + 1 == word_length) {
				printf("Not a word since no vowel!\n");
				exit(1);
			}
			consonant[count] = word[count];
			count++;
		}
		strcat(consonant, "ay");
		strcpy(new_word, &word[count]);
		strcat(new_word, consonant);
		printf("%s\n", new_word);
	}
	return 0;
}

int is_vowel(char ch) {
	return ch == 'a' || ch == 'e' 
		|| ch == 'i' || ch == 'o' 
		|| ch == 'u';
}
