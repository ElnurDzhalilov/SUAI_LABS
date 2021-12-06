#include <stdio.h>
#include <stdlib.h>

#define defWordLen 10000

struct wordList;

typedef struct wordNode {
    char *word;
    int wordLen;
    int wordCount;
    struct wordNode *prev;
    struct wordNode *next;
} word_t;

typedef struct list {
    word_t *head;
    int size;
} list_t;

void init(list_t *list) {
    list->head = NULL;
    list->size = 0;
}

void pushBack ( list_t *list, char *word, int len, int count) {
    word_t *new, *bf;
    new = (word_t*) malloc(sizeof(word_t));
    new->word = word;
    new->wordLen = len;
    new->wordCount = count;
    new->next = NULL;
    if (list->head == NULL) {
        new->prev = NULL;
        list->head = new;
    }
    else {
        bf = list->head;
        while (bf->next != NULL)    bf = bf->next;
        bf->next = new;
        new->prev = bf;
    }
    list->size++;
}

void printList(list_t *list)    {
    word_t *bf = list->head;
    while (bf != NULL) {
        //printf("%s|%d||%d|%d|%d\n", bf->word, bf->wordCount, bf->prev, bf, bf->next);
        printf("%s ", bf->word);
        bf = bf->next;
    }
}

void checkWord(list_t *list, char *word, int len) {
    int lim, check, close = 0;
    char *value;
    word_t *wordNode = list->head;
    while (wordNode != NULL) {
        check = 0;
        value = wordNode->word;
        lim = wordNode->wordLen;
        for (int i = 0; i < lim; i++) {
            if (value[i] == word[i]) check++;
            else break;
        }
        if (check == len) {
            wordNode->wordCount++;
            close = 1;
            break;
        }
        wordNode = wordNode->next;
    }
    if (close == 0) pushBack(list, word, len, 1);
}

int createList (char *wordStr, list_t *list)  {
    int i = 0, inWord = 0, startPos = 0, endPos = 0, len = 0, first = 0;
    char *word;
    while (wordStr[i] != 0) {
        if (wordStr[i] != ' ' && wordStr[i] != '\t' && wordStr[i] != '\n' && inWord == 0) {
            startPos = i;
            endPos = i;
        }
        if (wordStr[i] != ' ' && wordStr[i] != '\t' && wordStr[i] != '\n')  {
            inWord = 1;
            len++;
            endPos++;
        }
        if ((wordStr[i] == ' ' || wordStr[i] == '\t' || wordStr[i] == '\n') && inWord == 1) {
            int num = 0;
            word = (char*) malloc((len + 1) * sizeof (char));
            for (int bf = startPos; bf < endPos; bf++) word[num++] = wordStr[bf];
            word[num] = 0;
            checkWord(list, word, len);
            len = 0;
            startPos = 0;
            endPos = 0;
            inWord = 0;
        }
        i++;
    }
}

void swap(word_t *word1, word_t *word2) {
    word_t *tempPrev, *tempNext;
    if (word2->prev->next != word2) {
        word1->prev->next = word2;
        word2->prev->next = word1;
        word1->next->prev = word2;
        if (word2->next != NULL) word2->next->prev = word1;
        tempPrev = word1->prev;
        tempNext = word1->next;
        word1->prev = word2->prev;
        word1->next = word2->next;
        word2->prev=tempPrev;
        word2->next=tempNext;
    }
    else {                                                      //word1 above word2
        word1->prev->next = word2;
        word1->next = word2->next;
        if (word2->next != NULL) word2->next->prev = word1;
        word2->prev = word1->prev;
        word2->next = word1;
        word1->prev = word2;
    }
}

void sortList (list_t *list) {
    int minLen = defWordLen;
    word_t *mem, *temp, *startNode, *startNodeOld;
    word_t *wordNode = list->head;
    while (wordNode != NULL) {
        if (wordNode->wordLen < minLen) {
            minLen = wordNode->wordLen;
            mem = wordNode;
        }
        wordNode = wordNode->next;
    }
    if (list->head != mem) {
        list->head->prev = mem->prev;
        mem->prev->next = list->head;
        if (mem->next != NULL) mem->next->prev = list->head;
        temp = list->head->next;
        list->head->next = mem->next;
        list->head = mem;
        mem->prev = NULL;
        mem->next = temp;
        mem->next->prev = mem;
    }
    for (int cycle = 1; cycle < list->size; cycle++) {
        startNode = list->head;
        minLen = defWordLen;
        for (int cycle2 = 0; cycle2 < cycle; cycle2++) startNode = startNode->next;
        startNodeOld = startNode;
        while(startNode != NULL) {
            if (startNode->wordLen < minLen) {
                minLen = startNode->wordLen;
                mem = startNode;
            }
            startNode = startNode->next;
        }
        if (mem != startNodeOld) swap(startNodeOld, mem);
    }
}

void sortWords (list_t *mainList, list_t *uniqueList, list_t *notUniqueList) {
    word_t *wordNode = mainList->head;
    while (wordNode != NULL) {
        if (wordNode->wordCount == 1) pushBack(uniqueList, wordNode->word, wordNode->wordLen, wordNode->wordCount);
        else pushBack(notUniqueList, wordNode->word, wordNode->wordLen, wordNode->wordCount);
        wordNode = wordNode->next;
    }
}

void freeRam (list_t *list) {
    word_t *wordNode = list->head;
    word_t *temp;
    while (wordNode != NULL) {
        free(wordNode->word);
        temp = wordNode;
        wordNode = wordNode->next;
        free(temp);
    }
}

int main() {
    char wordString[defWordLen] = "";
    fgets(wordString, defWordLen, stdin);
    list_t wordList;
    init(&wordList);
    createList(wordString, &wordList);
    //part_2
    list_t uniqueWords, notUniqueWords;
    init(&uniqueWords); init(&notUniqueWords);
    printList(&wordList);
    sortList(&wordList);
    printf("\n");
    printList(&wordList);
    sortWords(&wordList, &uniqueWords, &notUniqueWords);
    printf("\n");
    printList(&uniqueWords);
    printf("\n");
    printList(&notUniqueWords);
    freeRam(&wordList);
    return 0;
}
