#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define ADMIN_USERNAME "ADMIN"
#define ADMIN_PASSWORD "Frazzy"
#define INITIAL_PASSWORD "123"
#define MAX_NAME 50
#define MAX_DAYS 365
#define MAX_STUDENTS 50
#define MAX_SECTIONS 4

char teacherName1[MAX_NAME] = "Narcisan Galamiton";
char teacherName2[MAX_NAME] = "Carla Jane Resnera";
char teacherPassword1[50] = INITIAL_PASSWORD;
char teacherPassword2[50] = INITIAL_PASSWORD;

// Captures attendance for a single day
typedef struct {
    char status[10];       // Present, Late, Absent
    char timestamp[20];    // Date and time of attendance
} DailyAttendance;

// Represents a student
typedef struct {
    char studentName[MAX_NAME];     // Student name
    DailyAttendance attendance[MAX_DAYS]; // Attendance for up to MAX_DAYS
} StudentAttendance;

// Represents a section
typedef struct {
    char sectionName[MAX_NAME];
    StudentAttendance students[MAX_STUDENTS]; // Array of students
    int studentCount; // Number of students in the section
    char attendanceDates[MAX_DAYS][11]; // Dates of attendance
    int attendanceCount; // Number of attendance records
} Section;

Section sections[MAX_SECTIONS] = {
    {"Section 1-A1", {}, 0, {}, 0},
    {"Section 1-A2", {}, 0, {}, 0},
    {"Section 1-B1", {}, 0, {}, 0},
    {"Section 1-B2", {}, 0, {}, 0}
};
int sectionCount = 0;

//database functions
void loadStudentsFromFile(Section *section, const char *filePath);
void addStudentToSection(Section *section, const char *filename);
void removeStudent(Section *section, const char *filename);
void editStudent(Section *section, const char *filename);
void saveStudents(const char *filename, Section *section);

//login related functions
void loginMenu();
void adminLogin();
void teacherLogin();
void changePassword(char *currentPassword);
void hidePassword(char *password);
void resetPassword(char *password);

//admin related functions
void adminMenu();
void editTeacherNameMenu();
void resetPasswordMenu();
void addStudentsMenu();
void editStudentNameMenu();
void removeStudentsMenu();
void viewAllStudentsMenu();

//teacher1 related functions
void teacherMenu1();
    //take attendance functions
    void displayTakeAttendance1(const char *day1, int hour1, int minute1);//when to display attendance
    void processAttendance1();

//display attendance functions
void parentDisplayAttendanceMenu1();
    void displayAttendanceByDateMenu1();
    void displayIndividualAttendanceMenu1();
    void viewIndividualAttendanceLogMenu1();

void addStudentMenu1();
void editStudentNameMenu1();
void removeStudentMenu1();
void viewStudentsMenu1();

//teacher2 related functions
void teacherMenu2();
    //take attendance functions
    void displayTakeAttendance2(const char *day2, int hour2, int minute2);//when to display attendance
    void processAttendance2();

    //display attendance functions
void parentDisplayAttendanceMenu2();
    void displayAttendanceByDateMenu2();
    void displayIndividualAttendanceMenu2();
    void viewIndividualAttendanceLogMenu2();

void addStudentMenu2();
void editStudentNameMenu2();
void removeStudentMenu2();
void viewStudentsMenu2();

//shared take attendance functions
void getCurrentTime(int *hour, int *minute);
void takeAttendanceAM(Section *section);
void takeAttendancePM(Section *section);
void determineAttendanceStatusAM(int hour, int minute, DailyAttendance *attendance);
void determineAttendanceStatusPM(int hour, int minute, DailyAttendance *attendance);

//shared display attendance functions
void displayAttendance(Section *section);
void displayAttendanceByDate(Section *section);
void displayIndividualAttendance(Section *section);
void viewIndividualAttendanceLog(Section *section);

int main(){

    loadStudentsFromFile(&sections[0], "D:\\Downloads\\MAJORSYSTEM\\1-A1.txt");
    loadStudentsFromFile(&sections[1], "D:\\Downloads\\MAJORSYSTEM\\1-A2.txt");
    loadStudentsFromFile(&sections[2], "D:\\Downloads\\MAJORSYSTEM\\1-B1.txt");
    loadStudentsFromFile(&sections[3], "D:\\Downloads\\MAJORSYSTEM\\1-B2.txt");

    printf("\n\nWelcome To The BSIT(DAY) 1st Year Attendance Management System\n\t for the subject of PROGRAMMING!");
    printf("\n\n\n\tPresented by: \n\tAlthea Lila E. Ocariza\n\tYahzee Leyson\n\tMa. Paulene Valle\n\tFranzen Beltran\n");
    printf("\n\n\tEnter any key to proceed...");
    getchar();
    loginMenu();
    return 0;
}
//databasefunctions
// Utility to sort students alphabetically
void sortStudents(StudentAttendance students[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(students[i].studentName, students[j].studentName) > 0) {
                StudentAttendance temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}
void loadStudentsFromFile(Section *section, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    section->studentCount = 0;
    while (fgets(section->students[section->studentCount].studentName, MAX_NAME, file)) {
        section->students[section->studentCount].studentName[strcspn(section->students[section->studentCount].studentName, "\n")] = 0; // Remove newline
        section->studentCount++;
    }

    fclose(file);
    sortStudents(section->students, section->studentCount);
}
void addStudentToSection(Section *section, const char *filename) {
    char studentName[MAX_NAME];

    // Open the file in append mode
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    printf("Enter student name to add (or type 'exit' to cancel): ");
    scanf(" %[^\n]", studentName); // Read the student name

    // Check if the user wants to exit
    if (strcmp(studentName, "exit") == 0) {
        printf("No student added. Exiting...\n");
        fclose(file); // Close the file before returning
        return;
    }

    // Add the student to the section
    if (section->studentCount < MAX_STUDENTS) {
        strcpy(section->students[section->studentCount].studentName, studentName);
        section->studentCount++;
        fprintf(file, "%s\n", studentName); // Write to the file
        printf("Student '%s' added to section '%s'.\n", studentName, section->sectionName);
    } else {
        printf("Maximum number of students reached for section '%s'.\n", section->sectionName);
    }

    fclose(file); // Close the file after finishing
    saveStudents(filename, section);
    sortStudents(section->students, section->studentCount);
}
void removeStudent(Section *section, const char *filename) {
    loadStudentsFromFile(section, filename); // Load students from the file

    // Display the list of students with numbers
    printf("Students in section '%s':\n", section->sectionName);
    for (int i = 0; i < section->studentCount; i++) {
        printf("%d. %s\n", i + 1, section->students[i].studentName);
    }

    // Prompt for the number of the student to remove
    int studentNumber;
    printf("Enter the number of the student to remove(Enter 0 to cancel): ");
    scanf("%d", &studentNumber);

    // Validate input
    if (studentNumber < 1 || studentNumber > section->studentCount) {
        printf("Invalid number. Please try again.\n");
        return;
    }

    // Remove the student
    int indexToRemove = studentNumber - 1; // Convert to zero-based index
    for (int i = indexToRemove; i < section->studentCount - 1; i++) {
        section->students[i] = section->students[i + 1]; // Shift students down
    }
    section->studentCount--; // Decrease the student count

    // Rewrite the file without the removed student
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < section->studentCount; i++) {
        fprintf(file, "%s\n", section->students[i].studentName);
    }

    fclose(file); // Close the file after finishing
    printf("Student removed successfully.\n");
    saveStudents(filename, section);
}
void editStudent(Section *section, const char *filename) {
    loadStudentsFromFile(section, filename); // Load students from the file

    // Display the list of students with numbers
    printf("Students in section '%s':\n", section->sectionName);
    for (int i = 0; i < section->studentCount; i++) {
        printf("%d. %s\n", i + 1, section->students[i].studentName);
    }

    // Prompt for the number of the student to edit
    int studentNumber;
    printf("Enter the number of the student to edit(Enter 0 to cancel): ");
    scanf("%d", &studentNumber);

    // Validate input
    if (studentNumber < 1 || studentNumber > section->studentCount) {
        printf("Invalid number. Please try again.\n");
        return;
    }

    // Get the new name for the student
    char newName[MAX_NAME];
    printf("Enter the new name for student '%s': ", section->students[studentNumber - 1].studentName);
    scanf(" %[^\n]", newName); // Read the new name

    // Update the student's name
    strcpy(section->students[studentNumber - 1].studentName, newName);

    // Rewrite the file with updated names
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < section->studentCount; i++) {
        fprintf(file, "%s\n", section->students[i].studentName);
    }

    fclose(file); // Close the file after finishing
    saveStudents(filename, section);
    sortStudents(section->students, section->studentCount);
    printf("Student name updated successfully.\n");
}

void viewStudents(Section *section) {
    printf("Students in section '%s':\n", section->sectionName);
    if (section->studentCount == 0) {
        printf("No students in this section.\n");

        printf("Press any key to return to previous...\n");
        getchar();
        getchar();
    }

    for (int i = 0; i < section->studentCount; i++) {
        printf("%d. %s\n", i + 1, section->students[i].studentName);
    }
    printf("Press any key to return to previous...\n");
    getchar();
    getchar();
}

void saveStudents(const char *filename, Section *section) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < section->studentCount; i++) {
        fprintf(file, "%s\n", section->students[i].studentName);
    }

    fclose(file);
}

//login related functions
void loginMenu() {
    char choice;

    do {
        system("cls");
        printf("==== Login Menu ====\n");
        printf("1. Admin Login\n");
        printf("2. Teacher Login\n");
        printf("E. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                adminLogin();
                break;
            case '2':
                teacherLogin();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    } while (1);
}
void adminLogin() {
    char username[50];
    char password[50];

    system("cls");
    printf("==== Admin Login ====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    hidePassword(password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Admin logged in successfully!\n");
        adminMenu();
    } else {
        printf("Invalid admin credentials.\n");
    }
    system("pause");
}
void teacherLogin() {
 int wrongPass=0;
    char username[MAX_NAME];
    char password[50];

    while (getchar() != '\n'); // Clear input buffer

    system("cls");
    printf("==== Teacher Login ====\n");
    printf("Username: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove trailing newline

    printf("Password: ");
    hidePassword(password);

    if (strcmp(username, teacherName1) == 0 && strcmp(password, teacherPassword1) == 0) {
        printf("Welcome, %s!\n", teacherName1);

        if (strcmp(teacherPassword1, INITIAL_PASSWORD) == 0) {
            changePassword(teacherPassword1);
            printf("\nPassword Changed Successfully!\n");
        }
        teacherMenu1(); // Proceed to Teacher 1 menu

    } else if (strcmp(username, teacherName2) == 0 && strcmp(password, teacherPassword2) == 0) {
        printf("Welcome, %s!\n", teacherName2);

        if (strcmp(teacherPassword2, INITIAL_PASSWORD) == 0) {
            changePassword(teacherPassword2);
            printf("\nPassword Changed Successfully!\n");
        }
        teacherMenu2(); // Proceed to Teacher 2 menu
    }else {
        printf("Invalid teacher credentials.\n");
        wrongPass=0;
        system("pause");
    }

}
void changePassword(char *currentPassword) {
    char newPassword[50], confirmPassword[50];

    printf("\nYou must change your password.\n\n");

    while (1) {
        printf("Enter new password: ");
        hidePassword(newPassword);
        printf("Confirm new password: ");
        hidePassword(confirmPassword);

        if (strcmp(newPassword, confirmPassword) == 0) {
            strcpy(currentPassword, newPassword);
            printf("\nPassword changed successfully!\n");
            break;
        } else {
            printf("\nPasswords do not match. Please try again.\n");
        }
    }
}
void hidePassword(char *password) {
    char ch;
    int i = 0;

    while (1) {
        ch = getch(); // Get character without echoing
        if (ch == 13) { // Enter key
            password[i] = '\0'; // Null terminate the string
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the last asterisk
            }
        } else {
            password[i] = ch;
            i++;
            printf("*"); // Print asterisk for each character
        }
    }
    printf("\n");
}
void resetPassword(char *password) {
    strcpy(password, INITIAL_PASSWORD); // Reset to default password
    printf("Password has been reset to default.\n");
}

//admin related functions
void adminMenu(){
char choice;
    do {
        system("cls");
        printf("==== Admin Menu ====\n");
        printf("1. Edit Teacher Name\n");
        printf("2. Reset Password Menu\n");
        printf("3. Add Students Menu\n");
        printf("4. Edit Student Name Menu\n");
        printf("5. Remove Students Menu\n");
        printf("6. View All Students Menu\n");
        printf("L - Log Out\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                editTeacherNameMenu();
                break;
            case '2':
                resetPasswordMenu();
                break;
            case '3':
                addStudentsMenu();
                break;
            case '4':
                editStudentNameMenu();//edit labels sections, present in addstudents
                break;
            case '5':
                removeStudentsMenu();
                break;
            case '6':
                viewAllStudentsMenu();
                break;
            case 'L':
            case 'l':
                loginMenu();//do not change
                break;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void editTeacherNameMenu(){
char choice;
    do {
        system("cls");
        printf("==== Edit Teacher Name ====\n");
        printf("Choose which teacher to edit:\n");
        printf("1. %s\n", teacherName1);
        printf("2. %s\n", teacherName2);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);
        char newTeacherName1[MAX_NAME];
        char newTeacherName2[MAX_NAME];
        switch (choice) {
            case '1':
                system("cls");

                // Prompt for new teacher name
                printf("Enter new teacher name (or type 'cancel' to cancel): ");
                getchar(); // Clear the newline character from the input buffer
                fgets(newTeacherName1, sizeof(newTeacherName1), stdin);
                newTeacherName1[strcspn(newTeacherName1, "\n")] = 0; // Remove newline

                // Check if the user wants to cancel
                if (strcmp(newTeacherName1, "cancel") == 0) {
                    printf("No changes made. Operation canceled.\n");
                } else {
                    // Update the teacher's name
                    strcpy(teacherName1, newTeacherName1);
                    printf("Teacher name updated successfully to '%s'!\n", teacherName1);
                }

                // Wait for user input before exiting
                printf("Press Enter to continue...");
                getchar(); // Use getchar() to pause the console
                break;
            case '2':

                // Prompt for new teacher name
                printf("Enter new teacher name (or type 'cancel' to cancel): ");
                getchar(); // Clear the newline character from the input buffer
                fgets(newTeacherName2, sizeof(newTeacherName2), stdin);
                newTeacherName2[strcspn(newTeacherName2, "\n")] = 0; // Remove newline

                // Check if the user wants to cancel
                if (strcmp(newTeacherName2, "cancel") == 0) {
                    printf("No changes made. Operation canceled.\n");
                } else {
                    // Update the teacher's name
                    strcpy(teacherName1, newTeacherName2);
                    printf("Teacher name updated successfully to '%s'!\n", teacherName1);
                }

                // Wait for user input before exiting
                printf("Press Enter to continue...");
                getchar(); // Use getchar() to pause the console
                break;
            case 'B':
            case 'b':
                adminMenu();//do not change
                break;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void resetPasswordMenu(){
    int choice;
    char confirm;
    system("cls");
    printf("\n==== Reset Password Menu ====\n");
    printf("1. Reset %s's Password\n", teacherName1);
    printf("2. Reset %s's Password\n", teacherName2);
    printf("B - Back\t");
    printf("E - Exit\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); // Clear input buffer

    switch (choice) {
        case 1:
            printf("Are you sure you want to reset %s's password? (y/n): ", teacherName1);
            confirm = getchar();
            while (getchar() != '\n'); // Clear input buffer

            if (confirm == 'y' || confirm == 'Y') {
                resetPassword(teacherPassword1);
            } else {
                printf("Reset cancelled.\n");
            }
            break;

        case 2:
            printf("Are you sure you want to reset %s's password? (y/n): ", teacherName2);
            confirm = getchar();
            while (getchar() != '\n'); // Clear input buffer

            if (confirm == 'y' || confirm == 'Y') {
                resetPassword(teacherPassword2);
            } else {
                printf("Reset cancelled.\n");
            }
            break;
        case 'B':
        case 'b':
            adminMenu();//do not change
            break;
        case 'E':
        case 'e':
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
            break;
    }
}
void addStudentsMenu(){
char choice;

    do {
        system("cls");
        printf("==== Add Students Menu ====\n");
        printf("Choose Section to Add Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[1].sectionName);
        printf("3. %s\n", sections[2].sectionName);
        printf("4. %s\n", sections[3].sectionName);
        printf("B - Back to Admin Menu\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Add Student: %s===\n", sections[0].sectionName);
                addStudentToSection(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Add Student: %s===\n", sections[1].sectionName);
                addStudentToSection(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '3':
                system("cls");
                printf("===Add Student: %s===\n", sections[2].sectionName);
                addStudentToSection(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case '4':
                system("cls");
                printf("===Add Student: %s===\n", sections[3].sectionName);
                addStudentToSection(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                adminMenu(); // Do not change
                return;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                getchar();
        }
    } while (1);
}
void editStudentNameMenu(){
char choice;

    do {
        system("cls");
        printf("==== Edit Student Name Menu ====\n");
        printf("Choose Section to Edit Student Name:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[1].sectionName);
        printf("3. %s\n", sections[2].sectionName);
        printf("4. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[0].sectionName);
                editStudent(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[1].sectionName);
                editStudent(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '3':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[2].sectionName);
                editStudent(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case '4':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[4].sectionName);
                editStudent(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                adminMenu();//do not change
                break;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void removeStudentsMenu(){
char choice;

    do {
        system("cls");
        printf("==== Remove Students Menu ====\n");
        printf("Choose Section to Remove Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[1].sectionName);
        printf("3. %s\n", sections[2].sectionName);
        printf("4. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Remove Student: %s===\n", sections[0].sectionName);
                removeStudent(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Remove Student: %s===\n", sections[1].sectionName);
                removeStudent(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '3':
                system("cls");
                printf("===Remove Student: %s===\n", sections[2].sectionName);
                removeStudent(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case '4':
                system("cls");
                printf("===Remove Student: %s===\n", sections[0].sectionName);
                removeStudent(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                adminMenu();//do not change
                break;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void viewAllStudentsMenu(){
char choice;

    do {
        system("cls");
        printf("==== View All Students Menu ====\n");
        printf("Choose Section to View All Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[1].sectionName);
        printf("3. %s\n", sections[2].sectionName);
        printf("4. %s\n", sections[3].sectionName);
        printf("B - Back to Admin Menu\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===View Student: %s===\n", sections[0].sectionName);
                viewStudents(&sections[0]);
                break;
            case '2':
                system("cls");
                printf("===View Student: %s===\n", sections[1].sectionName);
                viewStudents(&sections[1]);
                break;
            case '3':
                system("cls");
                printf("===View Student: %s===\n", sections[2].sectionName);
                viewStudents(&sections[2]);
                break;
            case '4':
                system("cls");
                printf("===View Student: %s===\n", sections[3].sectionName);
                viewStudents(&sections[3]);
                break;
            case 'B':
            case 'b':
                adminMenu();//do not change
                break;
            case 'E':
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

//Teacher Menu For Teacher 1(ONGOING)
void teacherMenu1() {
    const char *subjectsFile = "subjects.txt"; // File to store subjects
    char choice;

    do {
        system("cls");
        printf("Welcome, %s!\n", teacherName1);
        printf("==== Teacher Menu =====\n");
        printf("1. Take Attendance\n");
        printf("2. View Attendance\n");
        printf("3. Add Student\n");//new
        printf("4. Edit Student Name\n");//both admin and teacher
        printf("5. Remove Student\n");//new
        printf("6. View Student List\n");//admin and teacher
        printf("7. Change Password\n");
        printf("8. Add Subject\n");
        printf("9. List Subjects\n");
        printf("A. Edit Subject\n");
        printf("B. Delete Subject\n");
        printf("L - Log Out\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                processAttendance1();//tbc
                break;
            case '2':
                parentDisplayAttendanceMenu1();
                break;
            case '3':
                addStudentMenu1();//new not implemented
                break;
            case '4':
                editStudentNameMenu1();
                break;
            case '5':
                removeStudentMenu1();
                break;
            case '6':
                viewStudentsMenu1(); // new not implemented
                break;
            case '7':
                changePassword(teacherPassword1);
                break;
            case '8':
                addSubject(subjectsFile);
                break;
            case '9':
                listSubjects(subjectsFile);
                break;
                case 'A':
            editSubject(subjectsFile);
                break;
            case 'B':
                deleteSubject(subjectsFile);
                break;
            case 'L':
            case 'l':
                loginMenu();//do not change
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");//do not change
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

void addSubject(const char *filePath) {
    char subjectName[MAX_NAME];
    char startTime[10];
    int totalHours;
    char lateTime[10];
    char misCode[20];

    // Open file in append mode to save the subject data
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Gather input from the user
    printf("Enter Subject Name: ");
    getchar(); // Clear newline from input buffer
    fgets(subjectName, MAX_NAME, stdin);
    subjectName[strcspn(subjectName, "\n")] = 0; // Remove newline

    printf("Enter Start of Class (e.g., 08:00AM): ");
    scanf("%s", startTime);

    printf("Enter Total Hours: ");
    scanf("%d", &totalHours);

    printf("Enter Time Considered Late (e.g., 08:15AM): ");
    scanf("%s", lateTime);

    printf("Enter MIS Code: ");
    scanf("%s", misCode);

    // Save the subject data to the file
    fprintf(file, "%s,%s,%d,%s,%s\n", subjectName, startTime, totalHours, lateTime, misCode);
    fclose(file);

    printf("\nSubject '%s' added successfully and saved to '%s'.\n", subjectName, filePath);
    printf("Press any key to return to the menu...\n");
    getchar(); // Consume the newline
    getchar(); // Wait for user input
}

void listSubjects(const char *filePath) {
    char line[256];
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    printf("==== List of Subjects ====");
    printf("\n%-3s %-20s %-10s %-10s %-10s %-10s\n", "No.", "Subject Name", "Start Time", "Hours", "Late Time", "MIS Code");
    printf("--------------------------------------------------------------------\n");

    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        char subjectName[MAX_NAME], startTime[10], lateTime[10], misCode[20];
        int totalHours;

        sscanf(line, "%[^,],%[^,],%d,%[^,],%s", subjectName, startTime, &totalHours, lateTime, misCode);
        printf("%-3d %-20s %-10s %-10d %-10s %-10s\n", ++count, subjectName, startTime, totalHours, lateTime, misCode);
    }

    fclose(file);

    printf("\nPress any key to return to the menu...\n");
    getchar();
    getchar();
}

void editSubject(const char *filePath) {
    char line[256], tempFile[] = "temp.txt";
    FILE *file = fopen(filePath, "r");
    FILE *temp = fopen(tempFile, "w");

    if (file == NULL || temp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("==== Edit Subject ====");
    listSubjects(filePath);

    int choice;
    printf("\nEnter the number of the subject to edit: ");
    scanf("%d", &choice);

    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        count++;

        if (count == choice) {
            char subjectName[MAX_NAME], startTime[10], lateTime[10], misCode[20];
            int totalHours;

            printf("Editing Subject #%d:\n", count);

            printf("Enter New Subject Name: ");
            getchar(); // Clear newline from input buffer
            fgets(subjectName, MAX_NAME, stdin);
            subjectName[strcspn(subjectName, "\n")] = 0;

            printf("Enter New Start Time: ");
            scanf("%s", startTime);

            printf("Enter New Total Hours: ");
            scanf("%d", &totalHours);

            printf("Enter New Late Time: ");
            scanf("%s", lateTime);

            printf("Enter New MIS Code: ");
            scanf("%s", misCode);

            fprintf(temp, "%s,%s,%d,%s,%s\n", subjectName, startTime, totalHours, lateTime, misCode);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove(filePath);
    rename(tempFile, filePath);

    printf("\nSubject edited successfully.\nPress any key to return to the menu...\n");
    getchar();
    getchar();
}

void deleteSubject(const char *filePath) {
    char line[256], tempFile[] = "temp.txt";
    FILE *file = fopen(filePath, "r");
    FILE *temp = fopen(tempFile, "w");

    if (file == NULL || temp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("==== Delete Subject ====");
    listSubjects(filePath);

    int choice;
    printf("\nEnter the number of the subject to delete: ");
    scanf("%d", &choice);

    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        count++;
        if (count != choice) {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove(filePath);
    rename(tempFile, filePath);

    printf("\nSubject deleted successfully.\nPress any key to return to the menu...\n");
    getchar();
    getchar();
}

//take attendance functions
void displayTakeAttendance1(const char *day1, int hour1, int minute1) {
    system("cls"); // Clear the console

    printf("Current Day: %s, Hour: %d, Minute: %d\n", day1, hour1, minute1); // Debugging output

    if (strcmp(day1, "Monday") == 0) {
        if ((hour1 == 6 || hour1 == 7 || hour1 == 8) || (hour1 == 9 && minute1 == 0)) {
            takeAttendanceAM(&sections[0]);
        } else if ((hour1 == 12 || hour1 == 13 || hour1 == 14) || (hour1 == 15 && minute1 == 0)) {
            takeAttendancePM(&sections[2]);
        } else {
            printf("No attendance available at this time\n");
            printf("Press any key to return to the menu...\n");
            getchar(); // Wait for user input
            getchar();
        }
    } else if (strcmp(day1, "Tuesday") == 0 || strcmp(day1, "Wednesday") == 0 || strcmp(day1, "Thursday") == 0) {
        if ((hour1 >= 6 && hour1 <= 9) || (hour1 == 10 && minute1 == 0)) {
            takeAttendanceAM(&sections[0]);
        } else if ((hour1 >= 12 && hour1 <= 15) || (hour1 == 16 && minute1 == 0)) {
            takeAttendancePM(&sections[2]);
        } else {
            printf("No attendance available at this time\n");
            printf("Press any key to return to the menu...\n");
            getchar(); // Wait for user input
            getchar();
        }
    } else {
        printf("No attendance available at this time\n");
        printf("Press any key to return to the menu...\n");
        getchar(); // Wait for user input
        getchar();
    }
}

void processAttendance1() {
    int hour1, minute1;
    getCurrentTime(&hour1, &minute1); // Get current time

    time_t currentTime1;
    struct tm *localTime1;

    // Get current time
    time(&currentTime1);
    localTime1 = localtime(&currentTime1);

    // Determine the day of the week
    const char *day1;
    switch (localTime1->tm_wday) {
        case 1: day1 = "Monday"; break;
        case 2: day1 = "Tuesday"; break;
        case 3: day1 = "Wednesday"; break;
        case 4: day1 = "Thursday"; break;
        case 5: day1 = "Friday"; break;
        case 6: day1 = "Saturday"; break;
        case 7: day1 = "Sunday"; break;
    }

    // Display attendance message based on the time and day
    displayTakeAttendance1(day1, hour1, minute1);
}

//display attendance functions
void parentDisplayAttendanceMenu1(){
char choice;
        system("cls");
        printf("==== View Attendance ====\n");
        printf("Choose View Option:\n");
        printf("1. Display Attendance by Date\n" );
        printf("2. Display Attendance Summary\n" );
        printf("3. Display Individual Attendance Log\n" );
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                displayAttendanceByDateMenu1();
                break;
            case '2':
                displayIndividualAttendanceMenu1();
                break;
            case '3':
                viewIndividualAttendanceLogMenu1();
                break;
            case 'B':
            case 'b':
                teacherMenu1();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
    void displayAttendanceByDateMenu1(){
    char choice;
            system("cls");
            printf("==== Display Attendance By Date ====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[0].sectionName);
            printf("2. %s\n", sections[2].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    displayAttendance(&sections[0]);
                    break;
                case '2':
                    displayAttendance(&sections[2]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu1();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }
    void displayIndividualAttendanceMenu1(){
    char choice;
            system("cls");
            printf("==== Display Individual Attendance====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[0].sectionName);
            printf("2. %s\n", sections[2].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    displayIndividualAttendance(&sections[0]);
                    break;
                case '2':
                    displayIndividualAttendance(&sections[2]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu1();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }
    void viewIndividualAttendanceLogMenu1(){
    char choice;
            system("cls");
            printf("==== View Individual Attendance Log====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[0].sectionName);
            printf("2. %s\n", sections[2].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    viewIndividualAttendanceLog(&sections[0]);
                    break;
                case '2':
                    viewIndividualAttendanceLog(&sections[2]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu1();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }

void addStudentMenu1(){
char choice;
        system("cls");
        printf("==== Add Students Menu ====\n");
        printf("Choose Section to Add Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[2].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Add Student: %s===\n", sections[0].sectionName);
                addStudentToSection(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Add Student: %s===\n", sections[2].sectionName);
                addStudentToSection(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu1();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
void editStudentNameMenu1(){
char choice;
        system("cls");
        printf("==== Edit Student Name Menu ====\n");
        printf("Choose Section to Edit Student Name:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[2].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[0].sectionName);
                editStudent(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[2].sectionName);
                editStudent(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu1();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
void removeStudentMenu1(){
char choice;

    do {
        system("cls");
        printf("==== Remove Students Menu ====\n");
        printf("Choose Section to Remove Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[2].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Remove Student: %s===\n", sections[0].sectionName);
                removeStudent(&sections[0], "C:\\Users\\althea\\Desktop\\Version 2\\1-A1.txt");
                break;
            case '2':
                system("cls");
                printf("===Remove Student: %s===\n", sections[2].sectionName);
                removeStudent(&sections[2], "C:\\Users\\althea\\Desktop\\Version 2\\1-B1.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu1();//do not change
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void viewStudentsMenu1(){
char choice;
        system("cls");
        printf("==== View Students List ====\n");
        printf("Choose Section to View Students:\n");
        printf("1. %s\n", sections[0].sectionName);
        printf("2. %s\n", sections[2].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===View Student: %s===\n", sections[0].sectionName);
                viewStudents(&sections[0]);
                break;
            case '2':
                system("cls");
                printf("===View Student: %s===\n", sections[2].sectionName);
                viewStudents(&sections[2]);
                break;
            case 'B':
            case 'b':
                teacherMenu1();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}

//Teacher Menu For Teacher 2(ONGOING)
void teacherMenu2() {
    char choice;

    do {
        system("cls");
        printf("Welcome, %s!\n", teacherName2);
        printf("==== Teacher Menu =====\n");
        printf("1. Take Attendance\n");
        printf("2. View Attendance\n");
        printf("3. Add Student\n");//new
        printf("4. Edit Student Name\n");//both admin and teacher
        printf("5. Remove Student\n");//new
        printf("6. View Student List\n");//admin and teacher
        printf("7. Change Password\n");
        printf("L - Log Out\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                processAttendance2();//tbc
                break;
            case '2':
                parentDisplayAttendanceMenu2();
                break;
            case '3':
                addStudentMenu2();
                break;
            case '4':
                editStudentNameMenu2();
                break;
            case '5':
                removeStudentMenu2();
                break;
            case '6':
                viewStudentsMenu2();
                break;
            case '7':
                changePassword(teacherPassword2);
                break;
            case 'L':
            case 'l':
                loginMenu();//do not change
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");//do not change
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
//take attendance functions
void displayTakeAttendance2(const char *day2, int hour2, int minute2) {
    if (strcmp(day2, "Monday") == 0) {
        if ((hour2 == 6 || hour2 == 7 || hour2 == 8) || (hour2 == 9 && minute2 == 0)) {
            system("cls");
            takeAttendanceAM(&sections[1]);
        } else if ((hour2 == 12 || hour2 == 13 || hour2 == 14) || (hour2 == 15 && minute2 == 0)) {
            system("cls");
            takeAttendancePM(&sections[3]);
        } else {
            system("cls");
            printf("no attendance available at this time\n");
            printf("Press any key to return to the menu...\n");
            getchar();
            getchar();
        }
    } else if (strcmp(day2, "Tuesday") == 0 || strcmp(day2, "Wednesday") == 0 || strcmp(day2, "Thursday") == 0) {
        if ((hour2 >= 6 && hour2 <= 9) || (hour2 == 10 && minute2 == 0)) {
            system("cls");
            takeAttendanceAM(&sections[1]);
        } else if ((hour2 >= 12 && hour2 <= 15) || (hour2 == 16 && minute2 == 0)) {
            system("cls");
            takeAttendancePM(&sections[3]);
        } else {
            system("cls");
            printf("no attendance available at this time\n");
            printf("Press any key to return to the menu...\n");
            getchar();
            getchar();
        }
    } else {
        system("cls");
        printf("no attendance available at this time\n");
        printf("Press any key to return to the menu...\n");
        getchar();
        getchar();
    }
}
void processAttendance2() {
    int hour2, minute2;
    getCurrentTime(&hour2, &minute2); // Get current time

    time_t currentTime2;
    struct tm *localTime2;

    // Get current time
    time(&currentTime2);
    localTime2 = localtime(&currentTime2);

    // Determine the day of the week
    const char *day2;
    switch (localTime2->tm_wday) {
        case 1: day2 = "Monday"; break;
        case 2: day2 = "Tuesday"; break;
        case 3: day2 = "Wednesday"; break;
        case 4: day2 = "Thursday"; break;
        default: day2 = "Other"; break;
    }

    // Display attendance message based on the time and day
    displayTakeAttendance2(day2, hour2, minute2);
}

//display attendance functions
void parentDisplayAttendanceMenu2(){
char choice;
        system("cls");
        printf("==== View Attendance ====\n");
        printf("Choose View Option:\n");
        printf("1. Display Attendance by Date\n" );
        printf("2. Display Attendance Summary\n" );
        printf("3. Display Individual Attendance Log\n" );
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                displayAttendanceByDateMenu2();
                break;
            case '2':
                displayIndividualAttendanceMenu2();
                break;
            case '3':
                viewIndividualAttendanceLogMenu2();
                break;
            case 'B':
            case 'b':
                teacherMenu2();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
    void displayAttendanceByDateMenu2(){
    char choice;
            system("cls");
            printf("==== Display Attendance By Date ====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[1].sectionName);
            printf("2. %s\n", sections[3].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    displayAttendance(&sections[1]);
                    break;
                case '2':
                    displayAttendance(&sections[3]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu2();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }
    void displayIndividualAttendanceMenu2(){
    char choice;
            system("cls");
            printf("==== Display Individual Attendance====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[1].sectionName);
            printf("2. %s\n", sections[3].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    displayIndividualAttendance(&sections[1]);
                    break;
                case '2':
                    displayIndividualAttendance(&sections[3]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu2();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }
    void viewIndividualAttendanceLogMenu2(){
    char choice;
            system("cls");
            printf("==== View Individual Attendance Log====\n");
            printf("Choose Section to View:\n");
            printf("1. %s\n", sections[1].sectionName);
            printf("2. %s\n", sections[3].sectionName);
            printf("B - Back\t");
            printf("E - Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
                case '1':
                    viewIndividualAttendanceLog(&sections[1]);
                    break;
                case '2':
                    viewIndividualAttendanceLog(&sections[3]);
                    break;
                case 'B':
                case 'b':
                    parentDisplayAttendanceMenu2();
                    break;
                case 'E':
                case 'e':
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
    }

void addStudentMenu2(){
char choice;
        system("cls");
        printf("==== Add Students Menu ====\n");
        printf("Choose Section to Add Students:\n");
        printf("1. %s\n", sections[1].sectionName);
        printf("2. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Add Student: %s===\n", sections[1].sectionName);
                addStudentToSection(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '2':
                system("cls");
                printf("===Add Student: %s===\n", sections[3].sectionName);
                addStudentToSection(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu2();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
void editStudentNameMenu2(){
char choice;
        system("cls");
        printf("==== Edit Student Name Menu ====\n");
        printf("Choose Section to Edit Student Name:\n");
        printf("1. %s\n", sections[1].sectionName);
        printf("2. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[1].sectionName);
                editStudent(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '2':
                system("cls");
                printf("===Edit Student Name: %s===\n", sections[3].sectionName);
                editStudent(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu2();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}
void removeStudentMenu2(){
char choice;

    do {
        system("cls");
        printf("==== Remove Students Menu ====\n");
        printf("Choose Section to Remove Students:\n");
        printf("1. %s\n", sections[1].sectionName);
        printf("2. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===Remove Student: %s===\n", sections[1].sectionName);
                removeStudent(&sections[1], "C:\\Users\\althea\\Desktop\\Version 2\\1-A2.txt");
                break;
            case '2':
                system("cls");
                printf("===Remove Student: %s===\n", sections[3].sectionName);
                removeStudent(&sections[3], "C:\\Users\\althea\\Desktop\\Version 2\\1-B2.txt");
                break;
            case 'B':
            case 'b':
                teacherMenu2();//do not change
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}
void viewStudentsMenu2(){
char choice;
        system("cls");
        printf("==== View Students List ====\n");
        printf("Choose Section to View Students:\n");
        printf("1. %s\n", sections[1].sectionName);
        printf("2. %s\n", sections[3].sectionName);
        printf("B - Back\t");
        printf("E - Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                system("cls");
                printf("===View Student: %s===\n", sections[1].sectionName);
                viewStudents(&sections[1]);
                break;
            case '2':
                system("cls");
                printf("===View Student: %s===\n", sections[3].sectionName);
                viewStudents(&sections[3]);
                break;
            case 'B':
            case 'b':
                teacherMenu1();
                break;
            case 'E':
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
}

//shared take attendance functions
void getCurrentTime(int *hour, int *minute) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    *hour = current_time->tm_hour;
    *minute = current_time->tm_min;
}
void determineAttendanceStatusAM(int hour, int minute, DailyAttendance *attendance) {
    if ((hour == 5) || (hour == 6) || (hour == 7 && minute <= 15)) {
        strcpy(attendance->status, "Present");
    } else if (hour == 7 && minute > 15) {
        strcpy(attendance->status, "Late");
    } else if (hour >= 8 && hour < 9) {
        strcpy(attendance->status, "Late");
    } else if (hour >= 9) {
        strcpy(attendance->status, "Absent");
    } else {
        strcpy(attendance->status, "Absent");
    }
}
void takeAttendanceAM(Section *section) {
    int hour, minute;
    getCurrentTime(&hour, &minute);

    // Get current date
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    char currentDate[11];
    snprintf(currentDate, sizeof(currentDate), "%04d-%02d-%02d", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday);

    // Check if attendance for today has already been initialized
    int attendanceIndex = -1;
    for (int i = 0; i < section->attendanceCount; i++) {
        if (strcmp(section->attendanceDates[i], currentDate) == 0) {
            attendanceIndex = i;
            break;
        }
    }

    if (attendanceIndex == -1) {
        // Initialize attendance for the current date
        strcpy(section->attendanceDates[section->attendanceCount], currentDate);
        attendanceIndex = section->attendanceCount;
        section->attendanceCount++;

        // Reset attendance data for all students for the new date
        for (int i = 0; i < section->studentCount; i++) {
            strcpy(section->students[i].attendance[attendanceIndex].timestamp, "");
            strcpy(section->students[i].attendance[attendanceIndex].status, "Absent");
        }
    }

    // Loop to mark attendance
    while (1) {
        system("cls");
        printf("Taking attendance for %s on %s\n", section->sectionName, currentDate);
        printf("Student List:\n");

        // Display the list of students with attendance status
        for (int i = 0; i < section->studentCount; i++) {
            DailyAttendance *attendance = &section->students[i].attendance[attendanceIndex];
            if (strcmp(attendance->timestamp, "") == 0) {
                printf("%d. %s\n", i + 1, section->students[i].studentName);
            } else {

            }
        }
        printf("0. Back\n");

        // Get input
        printf("\nEnter the number of the student to mark attendance: ");
        int studentNumber;
        scanf("%d", &studentNumber);

        if (studentNumber == 0) {
            // Go back to the previous function
            return;
        }

        if (studentNumber < 1 || studentNumber > section->studentCount) {
            printf("Invalid student number. Please try again.\n");
            Sleep(1500); // Pause for a moment before continuing
            continue;
        }

        int index = studentNumber - 1;
        DailyAttendance *attendance = &section->students[index].attendance[attendanceIndex];

        // Check if the student has already been marked
        if (strcmp(attendance->timestamp, "") != 0) {
            printf("%s's attendance has already been recorded for today at %s.\n", section->students[index].studentName, attendance->timestamp);
            Sleep(1500); // Pause for a moment before continuing
            continue; // Skip to the next student
        }

        // Mark attendance
        determineAttendanceStatusAM(hour, minute, attendance);

        // Set the timestamp for the attendance
        snprintf(attendance->timestamp, sizeof(attendance->timestamp), "%02d:%02d", hour, minute);
        printf("%s - %s at %s\n", section->students[index].studentName, attendance->status, attendance->timestamp);
        Sleep(1000); // Pause for a moment before continuing
    }

    printf("Attendance automated successfully!\n");
    Sleep(1000); // Pause before returning to the menu
}

void determineAttendanceStatusPM(int hour, int minute, DailyAttendance *attendance) {
    if ((hour == 11) || (hour == 12) || (hour == 13 && minute <= 15)) {
        strcpy(attendance->status, "Present");
    } else if (hour == 13 && minute > 15) {
        strcpy(attendance->status, "Late");
    } else if (hour >= 14 && hour < 15) {
        strcpy(attendance->status, "Late");
    } else if (hour >= 15) {
        strcpy(attendance->status, "Absent");
    } else {
        strcpy(attendance->status, "Absent");
    }
}
void takeAttendancePM(Section *section) {
    int hour, minute;
    getCurrentTime(&hour, &minute);

    // Get current date
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    char currentDate[11];
    snprintf(currentDate, sizeof(currentDate), "%04d-%02d-%02d", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday);

    // Check if attendance for today has already been initialized
    int attendanceIndex = -1;
    for (int i = 0; i < section->attendanceCount; i++) {
        if (strcmp(section->attendanceDates[i], currentDate) == 0) {
            attendanceIndex = i;
            break;
        }
    }

    if (attendanceIndex == -1) {
        // Initialize attendance for the current date
        strcpy(section->attendanceDates[section->attendanceCount], currentDate);
        attendanceIndex = section->attendanceCount;
        section->attendanceCount++;

        // Reset attendance data for all students for the new date
        for (int i = 0; i < section->studentCount; i++) {
            strcpy(section->students[i].attendance[attendanceIndex].timestamp, "");
            strcpy(section->students[i].attendance[attendanceIndex].status, "Absent");
        }
    }

    // Loop to mark attendance
    while (1) {
        system("cls");
        printf("Taking attendance for %s on %s\n", section->sectionName, currentDate);
        printf("Student List:\n");

        // Display the list of students with attendance status
        for (int i = 0; i < section->studentCount; i++) {
            DailyAttendance *attendance = &section->students[i].attendance[attendanceIndex];
            if (strcmp(attendance->timestamp, "") == 0) {
                printf("%d. %s\n", i + 1, section->students[i].studentName);
            } else {
                printf("%d. %s \t| already marked attendance at %s\n", i + 1, section->students[i].studentName, attendance->timestamp);
            }
        }
        printf("0. Back\n");

        // Get input
        printf("\nEnter the number of the student to mark attendance: ");
        int studentNumber;
        scanf("%d", &studentNumber);

        if (studentNumber == 0) {
            // Go back to the previous function
            return;
        }

        if (studentNumber < 1 || studentNumber > section->studentCount) {
            printf("Invalid student number. Please try again.\n");
            Sleep(1500); // Pause for a moment before continuing
            continue;
        }

        int index = studentNumber - 1;
        DailyAttendance *attendance = &section->students[index].attendance[attendanceIndex];

        // Check if the student has already been marked
        if (strcmp(attendance->timestamp, "") != 0) {
            printf("%s's attendance has already been recorded for today at %s.\n", section->students[index].studentName, attendance->timestamp);
            Sleep(1500); // Pause for a moment before continuing
            continue; // Skip to the next student
        }

        // Mark attendance
        determineAttendanceStatusPM(hour, minute, attendance);

        // Set the timestamp for the attendance
        snprintf(attendance->timestamp, sizeof(attendance->timestamp), "%02d:%02d", hour, minute);
        printf("%s - %s at %s\n", section->students[index].studentName, attendance->status, attendance->timestamp);
        Sleep(1000); // Pause for a moment before continuing
    }

    printf("Attendance automated successfully!\n");
    Sleep(1000); // Pause before returning to the menu
}

//shared display attendance functions
void displayAttendance(Section *section) {
    system("cls");
    printf("Attendance records for %s:\n", section->sectionName);
    for (int i = 0; i < section->attendanceCount; i++) {
        printf("%d. %s\n", i + 1, section->attendanceDates[i]);
    }
    displayAttendanceByDate(section);
}
void displayAttendanceByDate(Section *section) {
    int choice;
    system("cls");
    printf("=== Attendance for %s ===\n", section->sectionName);

    // Display available dates for attendance
    printf("Available Dates:\n");
    for (int i = 0; i < section->attendanceCount; i++) {
        printf("%d. %s\n", i + 1, section->attendanceDates[i]);
    }

    printf("Select a date to view attendance (1 to %d): ", section->attendanceCount);
    scanf("%d", &choice);

    if (choice < 1 || choice > section->attendanceCount) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    int dateIndex = choice - 1;

    // Get current date for display
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char currentDate[20];
    strftime(currentDate, sizeof(currentDate), "%m-%d-%Y", local);

    // Clear the console and display the selected date
    system("cls");
    printf("Attendance for %s:\n", section->sectionName);
    printf("Date: %s\n", currentDate);
    printf("\n%-3s %-40s %-10s %-10s\n", "No.", "Name", "STATUS", "TIME");
    printf("----------------------------------------------------------\n");

    // Display attendance records for the selected date
    for (int i = 0; i < section->studentCount; i++) {
        DailyAttendance *attendance = &section->students[i].attendance[dateIndex];
        printf("%-3d %-40s %-10s %-10s\n",
               i + 1,
               section->students[i].studentName,
               attendance->status,
               attendance->timestamp);
    }

    printf("\nPress any key to return to the previous menu...\n");
    getchar(); // To consume the newline character left by previous input
    getchar(); // Wait for user input
}
void displayIndividualAttendance(Section *section) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int currentMonth = current_time->tm_mon; // Months are 0-11 in struct tm

    // Array of month names
    const char *monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    system("cls");
    printf("Viewing Total Attendance: %s\n", section->sectionName);
    printf("Month: %s\n\n", monthNames[currentMonth]); // Use month name

    // Print the table header
    printf("%-3s %-40s %-10s %-10s %-10s\n", "No.", "Student Name", "Present", "Late", "Absent");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < section->studentCount; i++) {
        int presentCount = 0, lateCount = 0, absentCount = 0;

        // Count attendance for the current month
        for (int j = 0; j < section->attendanceCount; j++) {
            DailyAttendance *attendance = &section->students[i].attendance[j];
            if (strcmp(attendance->status, "Present") == 0) {
                presentCount++;
            } else if (strcmp(attendance->status, "Late") == 0) {
                lateCount++;
            } else if (strcmp(attendance->status, "Absent") == 0) {
                absentCount++;
            }
        }

        // Display attendance for the current student in a tabular format
        printf("%-3d %-40s %-10d %-10d %-10d\n", i + 1, section->students[i].studentName, presentCount, lateCount, absentCount);
    }

    printf("Press any key to go back...\n");
    getchar(); // To consume the newline character left by previous input
    getchar(); // Wait for user input
}
void viewIndividualAttendanceLog(Section *section) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int currentMonth = current_time->tm_mon + 1; // Months are 0-11 in struct tm
    system("cls");
    printf("Select a student to view attendance log:\n");
    for (int i = 0; i < section->studentCount; i++) {
        printf("%d. %s\n", i + 1, section->students[i].studentName);
    }

    int studentNumber;
    printf("Enter student number (1 to %d): ", section->studentCount);
    scanf("%d", &studentNumber);

    if (studentNumber < 1 || studentNumber > section->studentCount) {
        printf("Invalid student number. Please try again.\n");
        return;
    }
    system("cls");
    printf("Name: %s\n", section->students[studentNumber - 1].studentName);
    printf("--------------------------------------------------------\n");
    printf("Date\t\tStatus\t\tTime\n");

    for (int j = 0; j < section->attendanceCount; j++) {
        DailyAttendance *attendance = &section->students[studentNumber - 1].attendance[j];
        if (strcmp(section->attendanceDates[j], "") != 0) {
            printf("%s\t%s\t\t%s\n", section->attendanceDates[j], attendance->status, attendance->timestamp);
        }
    }

    printf("Press any key to go back...\n");
    getchar(); // To consume the newline character left by previous input
    getchar(); // Wait for user input
}
