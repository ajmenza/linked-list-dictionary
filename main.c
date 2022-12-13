#include <stdio.h>
#include <stdlib.h>

// Arbitray max number of characters that user can input
#define MAX 51
// Arbitary max length of linked list (number of words)
#define MAX_WORDS 20

struct dict
{
  char word[5];
  int word_len;
  struct dict *next;
};

struct dict dict[MAX_WORDS];

void printList(struct dict *head)
{
  struct dict *curr = head;
  printf("\nDictionary:\n\n");
  while (curr)
  {
    printf("%s(%d) ", curr->word, curr->word_len);
    curr = curr->next;
  }
  printf("\n\n");
}

// converts a character to lowercase if it is uppercase
int to_lowercase(int chr)
{
  if (chr >= 65 && chr <= 90)
    return chr + 32;
  return chr;
}

// iterates through a word until we reach the length of the word with the least characters or 4 (the max chars for a word in our list). The loop checks for character equality and breaks when we find two different characters
void letters_to_compare(int *c, struct dict *i, struct dict *j)
{
  int word_len = 0;

  if (i->word_len < j->word_len)
    word_len = i->word_len;
  else
    word_len = j->word_len;

  if (word_len > 4)
    word_len = 4;

  for (int count = 0; count < word_len; count++)
  {
    *c = count;
    if (to_lowercase(i->word[*c]) == to_lowercase(j->word[*c]))
      continue;
    break;
  }
}

// Sorts a dictionary in alphabetical order (case insensitive)
struct dict *sort_dict(struct dict *head)
{

  struct dict *i, *j, *temp_ptr, *prev_ptr, *last_i;
  int i_count = 0;

  for (i = head; i->next != NULL; i = i->next)
  {
    prev_ptr = i;
    for (j = i->next; j != NULL; j = j->next)
    {
      // Index of the character we want to compare (needed for words that start with same letter)
      int c = 0;

      // Sets c to the correct index
      letters_to_compare(&c, i, j);

      // Get lowercase ASCII numbers just for comparison purposes
      int lower_i = to_lowercase(i->word[c]);
      int lower_j = to_lowercase(j->word[c]);

      if ((lower_i > lower_j) || ((lower_i == lower_j) && i->word_len > j->word_len))
      {
        // If i and j are directly next to each other, just swap what they are pointing to
        if (i->next == j)
        {
          i->next = j->next;
          j->next = i;
          // Set to j since the positions have changed in order to keep it behind the new j
          prev_ptr = j;
        }

        else
        {
          temp_ptr = j->next;
          j->next = i->next;
          i->next = temp_ptr;
          prev_ptr->next = i;
        }

        // Reset pointers so i and j are pointing to the same positions (not addresses) in the list
        temp_ptr = j;
        j = i;
        i = temp_ptr;

        // last_i isn't initialized until after the first i loop
        if (i_count > 0)
          // previous i position link must be changed every time there is a swap to keep to linked list together
          last_i->next = i;
      }
      // iterate every loop so we always have access to element directly behind j
      prev_ptr = prev_ptr->next;
    }
    i_count++;

    // Sets the new head of the linked list
    if (i_count == 1)
      head = i;
    // Set to current i position so we have access to the previous i position
    last_i = i;
  }

  return head;
}

struct dict *linked_list_dict(char str[], int str_len)
{

  // the current number of words that have been added to the list
  int word_element = 0;
  // the current character being parsed in the entire string
  int chr = 0;
  do
  {
    // the current character position being modified in the current node struct
    int word_chr = 0;

    // ignore special characters until we find a non-special character but keep counting characters
    if (str[chr] > 122 || str[chr] < 65)
    {
      chr++;
      continue;
    }

    // Add char to word in current node until we hit a space, special character, or the end of the string
    while (str[chr] != ' ' && chr < str_len && (str[chr] <= 122 && str[chr] >= 65))
    {

      if (word_chr < 4)
        dict[word_element].word[word_chr] = str[chr];

      chr++;
      word_chr++;
    }

    dict[word_element].word_len = word_chr;
    dict[word_element].next = NULL;

    if (word_element > 0)
      dict[word_element - 1].next = &dict[word_element];

    word_element++;
    chr++;

  } while (chr < str_len);

  return dict;
}

// Takes a string from a user of n length, stores it in str, and then returns it's length
int input(char str[], int str_len)
{
  int ch, i = 0;

  while ((ch = getchar()) != '\n')
    if (i < str_len)
      str[i++] = ch;
  str[i] = '\0';

  // string length
  return i;
}

// Looks up a word in a dictionary. Set search_case to 0 for case insensitive search and 1 for case sensitive
void word_lookup(char word[], int word_len, struct dict *head, char search_case)
{
  if (word_len > 4)
    word_len = 4;

  // search algorithm
  for (struct dict *i = head; i != NULL; i = i->next)
  {
    int dict_word_len;

    if (i->word_len > 4)
      dict_word_len = 4;
    else
      dict_word_len = i->word_len;

    if (word_len != dict_word_len)
      continue;

    for (int j = 0; j < word_len; j++)
    {
      int dict_char, lookup_char;
      if (search_case == '0')
      {
        dict_char = to_lowercase(i->word[j]);
        lookup_char = to_lowercase(word[j]);
      }
      else
      {
        dict_char = i->word[j];
        lookup_char = word[j];
      }

      if (lookup_char == dict_char)
      {
        if (j + 1 == word_len)
        {
          printf("\nFOUND WORD: %s\n\n", i->word);
          return;
        }
        continue;
      }
      break;
    }
  }

  printf("\nWORD NOT FOUND\n\n");
}

struct dict *add_dict_entry(char word[], int word_len, struct dict *head, struct dict *new_entry)
{
  // Check for special characters or spaces and reject if there are any
  for (int chr = 0; chr < word_len; chr++)
  {
    if (word[chr] > 122 || word[chr] < 65)
    {
      printf("\nPlease enter a word with no spaces or special characters\n\n");
      return head;
    }
  }

  // Parse word and add to new entry
  for (int chr = 0; chr <= 4; chr++)
  {

    if (chr >= word_len)
    {
      new_entry->word[chr] = '\0';
      continue;
    }

    new_entry->word[chr] = word[chr];
  }

  // Set word length of new entry
  new_entry->word_len = word_len;

  // Put new entry at beginning of dictionary and set it to the new head of the list
  new_entry->next = head;
  head = new_entry;

  printf("\nENTRY ADDED\n\n");
  return head;
}

int main()
{
  // keeping everything local to the loop allows for the user to overwrite their previous dictionary with a new one
  while (1)
  {
    struct dict *root;

    char sentence[MAX];

    printf("\nPlease enter a sentence:\n");
    int sentence_len = input(sentence, MAX);

    root = linked_list_dict(sentence, sentence_len);
    root = sort_dict(root);
    printList(root);

    // Maximum amount of possible new entries that can be added
    struct dict new_entries[20];

    // How many times we pass through the menu options
    int count = -1;

    // main menu loop
    while (1)
    {
      count++;

      printf("Pick a menu option to proceed:\n");
      printf("1. Look up a word\n");
      printf("2. Add word to dictionary\n");
      printf("3. Print the current dictionary\n");
      printf("4. Overwrite current dictionary with new one\n");
      printf("Enter q to quit\n\n");

      char option = getchar();
      while ((getchar()) != '\n')
        ;

      // Look up a word
      if (option == '1')
      {
        printf("\nEnter a word to look up:\n");
        // arbitrary word length
        char lookup_word[20];
        int lookup_word_len = input(lookup_word, 20);

        printf("\nPress 0 for case-insensitive search or 1 case-sensitive search:\n");
        char option = getchar();
        while ((getchar()) != '\n')
          ;

        word_lookup(lookup_word, lookup_word_len, root, option);
      }
      else if (option == '2')
      {
        printf("\nEnter a word to add:\n");
        char word_to_add[20];
        int word_to_add_len = input(word_to_add, 20);
        root = add_dict_entry(word_to_add, word_to_add_len, root, &new_entries[count]);
        root = sort_dict(root);
      }
      else if (option == '3')
        printList(root);

      else if (option == '4')
        break;

      else if (option == 'q')
        exit(1);

      else
        continue;
    }
  }
  return 0;
}