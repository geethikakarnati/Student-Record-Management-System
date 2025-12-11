#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[30];


void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int strCaseCompare(const char *a, const char *b) {
    char A[100], B[100];
    strcpy(A, a);
    strcpy(B, b);
    toLowerCase(A);
    toLowerCase(B);
    return strcmp(A, B);
}

int rollExists(int roll) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;

    struct Student st;

    while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            fclose(fp);
            return 1;   
        }
    }
    fclose(fp);
    return 0;  
}


void start();
int loginSystem();
void mainMenu();

void adminMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

void start() {
    printf("========= STUDENT MANAGEMENT SYSTEM =========\n");
}


int loginSystem() {
    char username[30], password[30];
    char fileUser[30], filePass[30], fileRole[20];

    int attempts = 3;

    while (attempts--) {
        printf("\n========== LOGIN SCREEN ==========\n");
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("\nError: %s file not found!\n", CREDENTIAL_FILE);
            return 0;
        }

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcasecmp(username, fileUser) == 0 &&
                strcmp(password, filePass) == 0) {

                strcpy(currentUser, fileUser);
                strcpy(currentRole, fileRole);
                fclose(fp);
                return 1;
            }
        }

        fclose(fp);

        printf("\nIncorrect username or password! Attempts left: %d\n", attempts);
    }

    printf("\nWrong password entered 3 times. Exiting...\n");
    return 0;
}


void mainMenu() {
    if (strcmp(currentRole, "admin") == 0)
        adminMenu();
    else if (strcmp(currentRole, "staff") == 0)
        staffMenu();
    else if (strcmp(currentRole, "guest") == 0)
        guestMenu();
    else
        printf("\nInvalid role detected.\n");
}


void adminMenu() {
    int choice;

    while (1) {
        printf("\n========== ADMIN MENU ==========\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("\nLogging out...\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}


void staffMenu() {
    int choice;

    while (1) {
        printf("\n========== STAFF MENU ==========\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4:
                printf("\nLogging out...\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}


void guestMenu() {
    int choice;

    while (1) {
        printf("\n========== GUEST MENU ==========\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");

        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3:
                printf("\nLogging out...\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}


void addStudent() {
    struct Student st;

    printf("\nEnter Roll Number: ");
    while (scanf("%d", &st.roll) != 1 || st.roll <= 0) {
        printf("Invalid roll number! Enter a valid positive number: ");
        clearBuffer();
    }
    clearBuffer();

    if (rollExists(st.roll)) {
        printf("\nError: Roll number already exists! Cannot add duplicate.\n");
        return;
    }

    printf("Enter Name: ");
    fgets(st.name, sizeof(st.name), stdin);
    st.name[strcspn(st.name, "\n")] = '\0';

    printf("Enter Marks: ");
    while (scanf("%f", &st.marks) != 1 || st.marks < 0) {
        printf("Invalid marks! Enter again: ");
        clearBuffer();
    }

    FILE *fp = fopen(STUDENT_FILE, "a");
    fprintf(fp, "%d,%s,%.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("\nStudent added successfully!\n");
}


void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("\nNo student records found.\n");
        return;
    }

    struct Student st;

    printf("\n========== STUDENT LIST ==========\n");
    while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}


void searchStudent() {
    int choice;
    printf("\nSearch by:\n1. Roll Number\n2. Name\nEnter choice: ");
    scanf("%d", &choice);
    clearBuffer();

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("\nNo records found.\n");
        return;
    }

    struct Student st;
    int found = 0;

    if (choice == 1) {
        int roll;
        printf("Enter Roll Number: ");
        while (scanf("%d", &roll) != 1) {
            printf("Invalid input! Enter a valid roll number: ");
            clearBuffer();
        }
        clearBuffer();

        while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
            if (st.roll == roll) {
                printf("\nRecord Found!\nRoll: %d | Name: %s | Marks: %.2f\n",
                       st.roll, st.name, st.marks);
                found = 1;
                break;
            }
        }
    }
    else if (choice == 2) {
        char name[50];
        printf("Enter Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
            if (strCaseCompare(st.name, name) == 0) {
                printf("\nRecord Found!\nRoll: %d | Name: %s | Marks: %.2f\n",
                       st.roll, st.name, st.marks);
                found = 1;
                break;
            }
        }
    }

    if (!found)
        printf("\nNo matching record found.\n");

    fclose(fp);
}


void updateStudent() {
    int roll;
    printf("\nEnter Roll Number to Update: ");
    while (scanf("%d", &roll) != 1) {
        printf("Invalid input! Enter a valid roll number: ");
        clearBuffer();
    }

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student st;
    int found = 0;

    while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;

            printf("Enter New Name: ");
            clearBuffer();
            fgets(st.name, sizeof(st.name), stdin);
            st.name[strcspn(st.name, "\n")] = '\0';

            printf("Enter New Marks: ");
            while (scanf("%f", &st.marks) != 1) {
                printf("Invalid input! Enter valid marks: ");
                clearBuffer();
            }
        }

        fprintf(temp, "%d,%s,%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nStudent updated successfully!\n");
    else
        printf("\nStudent not found.\n");
}


void deleteStudent() {
    int roll;
    printf("\nEnter Roll Number to Delete: ");
    while (scanf("%d", &roll) != 1) {
        printf("Invalid input! Enter a valid roll number: ");
        clearBuffer();
    }

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student st;
    int found = 0;

    while (fscanf(fp, "%d,%[^,],%f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue; 
        }
        fprintf(temp, "%d,%s,%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nRecord deleted successfully.\n");
    else
        printf("\nRoll number not found.\n");
}


int main() {
    start();



    if (!loginSystem())
        return 0;

    printf("\nLogin Successful! Welcome %s (%s)\n", currentUser, currentRole);

    mainMenu();

    return 0;
}