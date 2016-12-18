//saverec.c

//A version of linkedlist.c that is able to save these lists and read/write them into and out of a file called "studentList.txt".

typedef struct students {
  char name[21], id[7];
  struct students* nextStudent; //pointer to the next student
} *student;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

student makeStudent (char* name, char* id);
void insertStudent (student first, student prev, student insert, int mode);
student findStudent(student first, char* name);
void printList(student front);

int main(int argc, char **argv) {
  char inputname[21], inputid[7], inputnext[21];
  char enter[4];
  student frontStudent = NULL, placeholder;
  struct students* inputnextp = NULL;
  int firstStudent = 1;

  //start with an already-saved list
  printf("Create list from saved file? (yes/no)");
  scanf("%s", enter);
  if (strcmp(enter, "yes") == 0) {
    int handle = open("studentList.txt", O_RDONLY|O_CREAT, 0640);
    char insname[21], insid[7], entire[40];
    student current, prev;
    struct students readStudent;
    //while (read(handle, insname, 21)) {
    while (read(handle, &readStudent, sizeof(readStudent)) > 0) {
      current = (student) malloc(sizeof(struct students));
      *current = readStudent;

      current->nextStudent = NULL;

      if (firstStudent == 1) {
	frontStudent = current;
	firstStudent = 0;
      }
      else
	prev->nextStudent = current;

      printf("The updated list: \n");
      printList(frontStudent);
      
      prev = current;
    }
    close(handle);

    //printList(frontStudent);
  }
  
  //make sure you can handle too-long input
  while (1) {
    printf("Please enter a student name. (no greater than 20 chars)\n");
    scanf("%s", inputname);
    printf("Please enter a 6-digit ID number.\n");
    scanf("%s", inputid);
    if (frontStudent) {
      printf("Who will the new student be AFTER (below on list)? Type a NAME, OR type 'none'.\n");
      scanf("%s", inputnext);
    }

    //make the new student
    student latest = makeStudent(inputname, inputid);

    //insert the new student
    if (frontStudent == NULL) {
      frontStudent = latest;
      latest->nextStudent = NULL;
    }
    else {
      //determine mode of insert, and insert the student to the list
      if (strcmp(inputnext, "none") == 0) {
	insertStudent(frontStudent, placeholder, latest, 0); //insert at front
	frontStudent = latest;
      }
      
      //find place BEFORE where the new student will be inserted
      //then insert behind another student
      else {
	printf("     inside else statement/n");
	placeholder = findStudent(frontStudent, inputnext);
	if (placeholder == NULL) {
	  printf("Student could not be found.");
	  exit(0);
	}
	else {
	  insertStudent(frontStudent, placeholder, latest, 1); //insert behind another student
	}
      }
    }

    printf("Here is the updated list of students:\n");
    printList(frontStudent);

    printf("Enter another? (yes/no)");
    scanf("%s", enter);
    if (strcmp(enter, "no") == 0)
      break;
  }

  
  //option to store contents in a file
  printf("Store the current list in a file? (yes/no)");
  scanf("%s", enter);
  if (strcmp(enter, "no") == 0)
    exit(0);
  
  int handle = open("studentList.txt", O_WRONLY|O_CREAT, 0640);
  student current;
  for (current = frontStudent; current; current = current->nextStudent)
    write(handle, current, sizeof(struct students));
  close(handle);
    

}


//helper functions

//make the new student
student makeStudent (char* name, char* id) {
  student newStudent = (student) malloc(sizeof(struct students));
  strcpy(newStudent->name, name);
  strcpy(newStudent->id, id);
  return newStudent;
}

//front = has nothing pointing to it; back = points to null
//next will point to new student, and new student will point to "behind"
//0 = at front; 1 = before another student
//if only one student in list, then "prev" will be NULL.
//"prev" is the previous student.
void insertStudent (student first, student next, student insert, int mode) {  
  student prevStudent, behind;
  //int fp = (first != prev);
  int fp = (first->nextStudent != NULL); //check if the list has more than one student
 
  if (fp) { //if there is more than one student in the list
    prevStudent = next->nextStudent;
    behind = prevStudent;
  }
  
  if (mode == 0) { //place insert at front of the list
    insert->nextStudent = first;
    //if "prev" is the first student in the list
    //if (fp)
    //  ahead->nextStudent = insert;
  }
  else { //place insert behind "next"
    if (fp)
      insert->nextStudent = behind;
    next->nextStudent = insert;
  }
}

//front = has nothing pointing to it; back = points to null
//meant to work in conjunction with insertStudent.
//returns the student IN FRONT OF the student to be found using "name."
//Makes it easier to do splicing and insertion
//returns the first student in the list if there is only one student
student findStudent(student first, char* name) {
  student current = first;
  int notfound = 1;

  do {
    if (strcmp(current->name, name) == 0) //found correct student
      notfound = 0;
    else {
      current = current->nextStudent;
    }
  } while (notfound);
  
  if (notfound == 1)
    return NULL;
  else
    return current;
}

void printList(student front) {
  student slist;
  for (slist = front; slist != NULL; slist = (student)(slist->nextStudent)) {
    printf("Name: %s, ", slist->name);
    printf("ID: %s, ", slist->id);
    printf("Next student pointer: %p \n", slist->nextStudent);
  }
}
