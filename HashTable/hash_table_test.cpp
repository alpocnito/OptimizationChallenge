#include "hash_table.cpp"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

const int NUMBER_HASHES    = 9;
const int MAX_FILE_NAME    = 10;
const int MAX_NUMBER_FILES = 20;


//-----------------------------------------------------------
//
// Function merge several vertical csv files into on csv file
//  with several vertical lines
//
// Merge as long as the longest file does not reach
//
//-----------------------------------------------------------
int merge(char* first_name,...) {
  // init valist
  va_list valist;
  va_start(valist, MAX_NUMBER_FILES);
  
  // open results file
  FILE* result = NULL;
  OpenFile("results.csv", "w", &result);
  
  // array with files
  FILE** all_files = (FILE**)calloc(MAX_NUMBER_FILES, sizeof(FILE*));

  // init files
  int number_files = 0;
  for (number_files = 0; number_files < MAX_NUMBER_FILES; ++number_files) {
    char* file_name = va_arg(valist, char*);
    FILE* file = NULL;

    if (file_name == NULL) break;

    OpenFile(file_name, "r", &file);
    all_files[number_files] = file;
  } 

  // MERGING!!!
  int files_end = 0;  //!< to understand when stop reading files
  while (files_end != number_files) {
    
    files_end = 0;
    for (int i = 0; i < number_files; ++i) {
      
      char put_char = '-';
      while (put_char != '\n' && put_char != EOF) {
        
        put_char = fgetc(all_files[i]);
        if (put_char != '\n' && put_char != EOF) fputc(put_char, result); //File is good
        if (put_char == EOF) files_end++;                                 //Oh now, file is end
      }
      if (i != number_files-1) fputc(',', result);
      
    }
    fputc('\n', result);

  }
  
  // disnit files
  for (int i = 0; i < number_files; ++i)
    fclose(all_files[i]);

  va_end(valist);
  free(all_files);
  return 1;
}




//-----------------------------------------------------------
//
//  Function run an array list_of_lines with number lines = 
//    number_lines through Hash = (*Hash) and write a vertical
//    line into file = file_name
//
//-----------------------------------------------------------
int RunTest(Hash_t (*Hash)(void* buffer, size_t buffer_sizei), String* \
            list_of_lines, int number_lines, const char* file_name) {
  
  HashTable htable = {}; if (!HashTableInit(&htable)) return 0;
  
  // computing hash
  for (int i = 0; i < number_lines; ++i)
    HashTablePush(&htable, list_of_lines[i].str, Hash);
  
  // writing hash
  FILE* file = NULL;
  OpenFile(file_name, "w", &file);
  for (int i = 0; i < TABLE_SIZE; ++i)
    fprintf(file, "%lu\n", htable.hash_array[i]->number_nodes);

  fclose(file);
  HashTableDisinit(&htable);
  return 1;
}
 
int GetLines(const char* file_name, String** list_of_lines, char** buffer) {

  // reading dict
  int buffer_size = ReadFile(file_name, buffer);
  int number_lines = SeparateLines(*buffer, buffer_size, list_of_lines);
  NormalizeStrings(list_of_lines, number_lines);

  return number_lines;
}

int main() {
  
  char*   buffer        = NULL;
  String* list_of_lines = NULL;
  int number_lines = GetLines("dict", &list_of_lines, &buffer);

  // name of files
  char* name_of_result_files[NUMBER_HASHES] = {};
  for (int i = 0; i < NUMBER_HASHES; ++i) {
    name_of_result_files[i] = (char*) calloc(MAX_FILE_NAME, sizeof(char));
    sprintf(name_of_result_files[i], "%d", i);
  }
    
  RunTest(Hash_First,  list_of_lines, number_lines, name_of_result_files[0]);
  RunTest(Hash_Sum,    list_of_lines, number_lines, name_of_result_files[1]);
  RunTest(Hash_Len,    list_of_lines, number_lines, name_of_result_files[2]);
  RunTest(Hash_Rotate, list_of_lines, number_lines, name_of_result_files[3]);
  RunTest(Hash_GNU,    list_of_lines, number_lines, name_of_result_files[4]);
  RunTest(Hash_Rot13,  list_of_lines, number_lines, name_of_result_files[5]);
  RunTest(Hash_FAQ6,   list_of_lines, number_lines, name_of_result_files[6]);
  RunTest(Hash_Ly,     list_of_lines, number_lines, name_of_result_files[7]);
  RunTest(Hash_Rs,     list_of_lines, number_lines, name_of_result_files[8]);
  
  merge(name_of_result_files[0] ,\
        name_of_result_files[1] ,\
        name_of_result_files[2] ,\
        name_of_result_files[3] ,\
        name_of_result_files[4] ,\
        name_of_result_files[5] ,\
        name_of_result_files[6] ,\
        name_of_result_files[7] ,\
        name_of_result_files[8] ,\
        NULL                    );
  
  free(list_of_lines);
  free(buffer);
  return 0;
}
