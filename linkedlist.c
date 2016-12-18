//linkedlist.c

//Some practice with linked lists. Will be cleaning and finishing it up later this week.
//The program allows you to enter in student names and IDs to a list, and allows you to add them in a different order.

typedef struct students {
  char name[21], id[7];
  struct students* nextStudent; //pointer to the next student
} *student;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

student makeStudent (char* name, char* id);
void insertStudent (student first, student prev, student insert, int mode);
student findStudent(student first, char* name);

int main(int argc, char **argv) {
  char inputname[21], inputid[7], inputnext[21];
  char enter[4];
  student frontStudent = NULL, placeholder;
  struct students* inputnextp = NULL;
  int firstStudent = 1;
  
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
    //for loop to print all the students
    student slist;
    for (slist = frontStudent; slist != NULL; slist = (student)(slist->nextStudent)) {
      printf("Name: %s, ", slist->name);
      printf("ID: %s, ", slist->id);
      printf("Next student pointer: %p \n", slist->nextStudent);
    }

    printf("Enter another? (yes/no)");
    scanf("%s", enter);
    if (strcmp(enter, "no") == 0)
      exit(0);
  
  }

}

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
