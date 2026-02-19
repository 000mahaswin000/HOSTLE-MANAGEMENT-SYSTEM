/* 
     HOSTEL MANAGEMENT SYSTEM
    ---------------------------------
    UNIQUE EXTRA FEATURES ADDED:
    1. SEARCH STUDENT BY ID
    2. SEARCH STUDENT BY NAME
    3. SORT STUDENTS BY NAME
    4. SORT STUDENTS BY ROOM NUMBER
    5. VIEW STUDENTS FILTERED BY CAMPUS 
    6. VIEW BASIC DASHBOARD 
    7. SOFT DELETE STUDENT (MARK AS INACTIVE)
    8. VIEW ONLY ACTIVE STUDENTS
    9. VIEW ONLY OPEN ISSUES
    10. EXPORT ALL STUDENTS TO A TEXT REPORT FILE (students_report.txt)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 200
#define MAX_TICKETS 500

#define STUDENT_FILE "students.dat"
#define TICKET_FILE  "tickets.dat"
#define REPORT_FILE  "students_report.txt"

#define ADMIN_PASSWORD "ADMIN123"

typedef struct {
    int id;                         // STUDENT / ROLL ID (UNIQUE)
    char name[50];
    char parent_name[50];
    char mother_name[50];
    char phone[20];
    char email[50];
    char room_no[10];
    char blood_group[5];
    char year[10];
    char department[30];
    char campus[5];                 // CAMPUS A / B
    char district[30];

    /* +10 EXTRA BASIC DETAILS */
    char state[30];
    char pincode[10];
    char address[100];
    char guardian_name[50];
    char guardian_phone[20];
    char dob[15];                   // DD/MM/YYYY
    char gender[10];
    char hostel_block[10];
    char admission_year[10];
    char fee_status[20];           // "PAID" / "NOT_PAID"

    int is_active;                  // 1 = ACTIVE, 0 = DELETED
} Student;

typedef struct {
    int ticket_id;
    int student_id;                 // LINKED TO STUDENT
    char student_name[50];          // COPY FOR EASY VIEW
    char issue[200];
    char status[20];                // "OPEN", "IN_PROGRESS", "RESOLVED"
} Ticket;

/* GLOBAL ARRAYS AND COUNTERS */
Student students[MAX_STUDENTS];
int student_count = 0;

Ticket tickets[MAX_TICKETS];
int ticket_count = 0;
int next_ticket_id = 1;

/* FUNCTION PROTOTYPES */
void load_data();
void save_data();

void main_menu();
void admin_login();
void admin_menu();
void student_menu();

void add_student();
void update_student();
void delete_student();
void view_all_students();
void view_active_students();
void search_student_by_id();
void search_student_by_name();
void sort_students_by_name();
void sort_students_by_room();
void view_students_by_campus();
void dashboard();
void export_students_report();

int find_student_by_id(int id);
void print_student_row(Student s);
void print_student_table_header();
void print_line();

void raise_issue_student_portal();
void check_issue_status_student_portal();

void raise_issue_admin();
void view_all_tickets();
void view_open_tickets();
void update_ticket_status();
int find_ticket_by_id(int tid);

/* HELPER INPUT FUNCTIONS */
void clear_input_buffer();
void read_string(char *prompt, char *buffer, int size);
int read_int(char *prompt);

/* ------------------------ MAIN ------------------------ */

int main() {
    load_data();
    main_menu();
    save_data();

    printf("\n\nTHANK YOU FOR USING HOSTEL MANAGEMENT SYSTEM.\n");
    return 0;
}

/* --------------------- DATA PERSISTENCE --------------------- */

void load_data() {
    FILE *fp;

    /* LOAD STUDENTS */
    fp = fopen(STUDENT_FILE, "rb");
    if (fp != NULL) {
        fread(&student_count, sizeof(int), 1, fp);
        if (student_count > 0 && student_count <= MAX_STUDENTS) {
            fread(students, sizeof(Student), student_count, fp);
        } else {
            student_count = 0;
        }
        fclose(fp);
    }

    /* LOAD TICKETS */
    fp = fopen(TICKET_FILE, "rb");
    if (fp != NULL) {
        fread(&ticket_count, sizeof(int), 1, fp);
        if (ticket_count > 0 && ticket_count <= MAX_TICKETS) {
            fread(tickets, sizeof(Ticket), ticket_count, fp);
        } else {
            ticket_count = 0;
        }
        fclose(fp);
    }

    /* SET NEXT TICKET ID */
    next_ticket_id = 1;
    for (int i = 0; i < ticket_count; i++) {
        if (tickets[i].ticket_id >= next_ticket_id) {
            next_ticket_id = tickets[i].ticket_id + 1;
        }
    }
}

void save_data() {
    FILE *fp;

    fp = fopen(STUDENT_FILE, "wb");
    if (fp != NULL) {
        fwrite(&student_count, sizeof(int), 1, fp);
        fwrite(students, sizeof(Student), student_count, fp);
        fclose(fp);
    }

    fp = fopen(TICKET_FILE, "wb");
    if (fp != NULL) {
        fwrite(&ticket_count, sizeof(int), 1, fp);
        fwrite(tickets, sizeof(Ticket), ticket_count, fp);
        fclose(fp);
    }
}

/* ------------------------ MENUS ------------------------ */

void main_menu() {
    int choice;
    do {
        printf("\n===============================================\n");
        printf("          HOSTEL MANAGEMENT SYSTEM\n");
        printf("===============================================\n");
        printf("1. ADMIN LOGIN\n");
        printf("2. STUDENT ISSUE PORTAL\n");
        printf("0. EXIT\n");
        printf("===============================================\n");
        choice = read_int("ENTER YOUR CHOICE: ");

        switch (choice) {
            case 1:
                admin_login();
                break;
            case 2:
                student_menu();
                break;
            case 0:
                printf("\nEXITING SYSTEM AND SAVING DATA...\n");
                break;
            default:
                printf("\nINVALID CHOICE. PLEASE TRY AGAIN.\n");
        }
    } while (choice != 0);
}

void admin_login() {
    char password[50];
    int attempts = 0;

    while (attempts < 3) {
        printf("\nENTER ADMIN PASSWORD: ");
        scanf("%49s", password);
        clear_input_buffer();
        if (strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("\nLOGIN SUCCESSFUL. WELCOME ADMIN.\n");
            admin_menu();
            return;
        } else {
            printf("\nWRONG PASSWORD. TRY AGAIN.\n");
            attempts++;
        }
    }
    printf("\nMAXIMUM ATTEMPTS REACHED. RETURNING TO MAIN MENU.\n");
}

void admin_menu() {
    int choice;
    do {
        printf("\n===============================================\n");
        printf("              ADMIN MAIN MENU\n");
        printf("===============================================\n");
        printf("1. ADD NEW STUDENT\n");
        printf("2. UPDATE STUDENT DETAILS\n");
        printf("3. DELETE (SOFT DELETE) STUDENT\n");
        printf("4. VIEW ALL STUDENTS (INCLUDING INACTIVE)\n");
        printf("5. VIEW ONLY ACTIVE STUDENTS\n");
        printf("6. SEARCH STUDENT BY ID\n");
        printf("7. SEARCH STUDENT BY NAME\n");
        printf("8. SORT STUDENTS BY NAME\n");
        printf("9. SORT STUDENTS BY ROOM NUMBER\n");
        printf("10. VIEW STUDENTS BY CAMPUS\n");
        printf("11. DASHBOARD SUMMARY\n");
        printf("12. EXPORT STUDENT REPORT TO FILE\n");
        printf("13. RAISE ISSUE (ADMIN SIDE)\n");
        printf("14. VIEW ALL ISSUES\n");
        printf("15. VIEW ONLY OPEN ISSUES\n");
        printf("16. UPDATE ISSUE STATUS\n");
        printf("0. LOGOUT AND RETURN TO MAIN MENU\n");
        printf("===============================================\n");
        choice = read_int("ENTER YOUR CHOICE: ");

        switch (choice) {
            case 1:  add_student(); break;
            case 2:  update_student(); break;
            case 3:  delete_student(); break;
            case 4:  view_all_students(); break;
            case 5:  view_active_students(); break;
            case 6:  search_student_by_id(); break;
            case 7:  search_student_by_name(); break;
            case 8:  sort_students_by_name(); break;
            case 9:  sort_students_by_room(); break;
            case 10: view_students_by_campus(); break;
            case 11: dashboard(); break;
            case 12: export_students_report(); break;
            case 13: raise_issue_admin(); break;
            case 14: view_all_tickets(); break;
            case 15: view_open_tickets(); break;
            case 16: update_ticket_status(); break;
            case 0: 
                printf("\nLOGGING OUT OF ADMIN PANEL...\n");
                break;
            default:
                printf("\nINVALID CHOICE. PLEASE TRY AGAIN.\n");
        }

        save_data();  // SAVE AFTER EVERY CHANGE
    } while (choice != 0);
}

void student_menu() {
    int choice;
    do {
        printf("\n===============================================\n");
        printf("           STUDENT ISSUE PORTAL\n");
        printf("===============================================\n");
        printf("1. RAISE NEW ISSUE\n");
        printf("2. CHECK ISSUE STATUS\n");
        printf("0. BACK TO MAIN MENU\n");
        printf("===============================================\n");
        choice = read_int("ENTER YOUR CHOICE: ");

        switch (choice) {
            case 1: raise_issue_student_portal(); break;
            case 2: check_issue_status_student_portal(); break;
            case 0: printf("\nRETURNING TO MAIN MENU...\n"); break;
            default: printf("\nINVALID CHOICE. TRY AGAIN.\n");
        }
        save_data();
    } while (choice != 0);
}

/* ---------------------- STUDENT FUNCTIONS ---------------------- */

int find_student_by_id(int id) {
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("\nSTUDENT LIMIT REACHED. CANNOT ADD MORE.\n");
        return;
    }

    Student s;
    int id;

    printf("\nADDING NEW STUDENT\n");
    printf("------------------\n");

    id = read_int("ENTER UNIQUE STUDENT ID / ROLL NO: ");
    if (find_student_by_id(id) != -1) {
        printf("\nSTUDENT WITH THIS ID ALREADY EXISTS. DUPLICATES NOT ALLOWED.\n");
        return;
    }
    s.id = id;

    read_string("ENTER STUDENT NAME: ", s.name, sizeof(s.name));
    read_string("ENTER FATHER / PARENT NAME: ", s.parent_name, sizeof(s.parent_name));
    read_string("ENTER MOTHER NAME: ", s.mother_name, sizeof(s.mother_name));
    read_string("ENTER PHONE NUMBER: ", s.phone, sizeof(s.phone));
    read_string("ENTER EMAIL ID: ", s.email, sizeof(s.email));
    read_string("ENTER ROOM NUMBER: ", s.room_no, sizeof(s.room_no));
    read_string("ENTER BLOOD GROUP: ", s.blood_group, sizeof(s.blood_group));
    read_string("ENTER YEAR (I / II / III / IV): ", s.year, sizeof(s.year));
    read_string("ENTER DEPARTMENT: ", s.department, sizeof(s.department));
    read_string("ENTER CAMPUS: ", s.campus, sizeof(s.campus));
    read_string("ENTER DISTRICT: ", s.district, sizeof(s.district));

    /* EXTRA 10 FIELDS */
    read_string("ENTER STATE: ", s.state, sizeof(s.state));
    read_string("ENTER PINCODE: ", s.pincode, sizeof(s.pincode));
    read_string("ENTER FULL ADDRESS: ", s.address, sizeof(s.address));
    read_string("ENTER GUARDIAN NAME: ", s.guardian_name, sizeof(s.guardian_name));
    read_string("ENTER GUARDIAN PHONE: ", s.guardian_phone, sizeof(s.guardian_phone));
    read_string("ENTER DATE OF BIRTH (DD/MM/YYYY): ", s.dob, sizeof(s.dob));
    read_string("ENTER GENDER: ", s.gender, sizeof(s.gender));
    read_string("ENTER HOSTEL BLOCK: ", s.hostel_block, sizeof(s.hostel_block));
    read_string("ENTER ADMISSION YEAR: ", s.admission_year, sizeof(s.admission_year));
    read_string("ENTER FEE STATUS (PAID / NOT_PAID): ", s.fee_status, sizeof(s.fee_status));

    s.is_active = 1;

    students[student_count++] = s;
    printf("\nSTUDENT ADDED SUCCESSFULLY.\n");
}

/* NESTED SWITCH IMPLEMENTATION FOR FIELD UPDATE */
void update_student() {
    int id, index, choice;
    printf("\nUPDATE STUDENT DETAILS\n");
    printf("----------------------\n");

    id = read_int("ENTER STUDENT ID TO UPDATE: ");
    index = find_student_by_id(id);

    if (index == -1) {
        printf("\nSTUDENT NOT FOUND.\n");
        return;
    }

    printf("\nSELECT WHICH DETAIL TO UPDATE (NESTED MENU):\n");
    printf("1. BASIC INFO (NAME, PARENT NAMES)\n");
    printf("2. CONTACT INFO (PHONE, EMAIL, ADDRESS)\n");
    printf("3. ACADEMIC INFO (YEAR, DEPARTMENT, CAMPUS)\n");
    printf("4. HOSTEL INFO (ROOM, BLOCK)\n");
    printf("5. PERSONAL INFO (BLOOD GROUP, DOB, GENDER)\n");
    printf("6. FEE DETAILS\n");
    printf("0. CANCEL\n");

    choice = read_int("ENTER YOUR OPTION: ");

    switch (choice) {
        case 1: {
            int sub;
            printf("\n1. UPDATE STUDENT NAME\n");
            printf("2. UPDATE FATHER / PARENT NAME\n");
            printf("3. UPDATE MOTHER NAME\n");
            printf("0. BACK\n");
            sub = read_int("ENTER CHOICE: ");
            switch (sub) {
                case 1: read_string("ENTER NEW STUDENT NAME: ", students[index].name, sizeof(students[index].name)); break;
                case 2: read_string("ENTER NEW PARENT NAME: ", students[index].parent_name, sizeof(students[index].parent_name)); break;
                case 3: read_string("ENTER NEW MOTHER NAME: ", students[index].mother_name, sizeof(students[index].mother_name)); break;
                case 0: printf("\nNO CHANGE MADE.\n"); break;
                default: printf("\nINVALID OPTION.\n"); break;
            }
            break;
        }
        case 2: {
            int sub;
            printf("\n1. UPDATE PHONE NUMBER\n");
            printf("2. UPDATE EMAIL ID\n");
            printf("3. UPDATE ADDRESS\n");
            printf("4. UPDATE GUARDIAN NAME\n");
            printf("5. UPDATE GUARDIAN PHONE\n");
            printf("0. BACK\n");
            sub = read_int("ENTER CHOICE: ");
            switch (sub) {
                case 1: read_string("ENTER NEW PHONE: ", students[index].phone, sizeof(students[index].phone)); break;
                case 2: read_string("ENTER NEW EMAIL: ", students[index].email, sizeof(students[index].email)); break;
                case 3: read_string("ENTER NEW ADDRESS: ", students[index].address, sizeof(students[index].address)); break;
                case 4: read_string("ENTER NEW GUARDIAN NAME: ", students[index].guardian_name, sizeof(students[index].guardian_name)); break;
                case 5: read_string("ENTER NEW GUARDIAN PHONE: ", students[index].guardian_phone, sizeof(students[index].guardian_phone)); break;
                case 0: printf("\nNO CHANGE MADE.\n"); break;
                default: printf("\nINVALID OPTION.\n"); break;
            }
            break;
        }
        case 3: {
            int sub;
            printf("\n1. UPDATE YEAR\n");
            printf("2. UPDATE DEPARTMENT\n");
            printf("3. UPDATE CAMPUS\n");
            printf("4. UPDATE ADMISSION YEAR\n");
            printf("0. BACK\n");
            sub = read_int("ENTER CHOICE: ");
            switch (sub) {
                case 1: read_string("ENTER NEW YEAR: ", students[index].year, sizeof(students[index].year)); break;
                case 2: read_string("ENTER NEW DEPARTMENT: ", students[index].department, sizeof(students[index].department)); break;
                case 3: read_string("ENTER NEW CAMPUS : ", students[index].campus, sizeof(students[index].campus)); break;
                case 4: read_string("ENTER NEW ADMISSION YEAR: ", students[index].admission_year, sizeof(students[index].admission_year)); break;
                case 0: printf("\nNO CHANGE MADE.\n"); break;
                default: printf("\nINVALID OPTION.\n"); break;
            }
            break;
        }
        case 4: {
            int sub;
            printf("\n1. UPDATE ROOM NUMBER\n");
            printf("2. UPDATE HOSTEL BLOCK\n");
            printf("0. BACK\n");
            sub = read_int("ENTER CHOICE: ");
            switch (sub) {
                case 1: read_string("ENTER NEW ROOM NUMBER: ", students[index].room_no, sizeof(students[index].room_no)); break;
                case 2: read_string("ENTER NEW HOSTEL BLOCK: ", students[index].hostel_block, sizeof(students[index].hostel_block)); break;
                case 0: printf("\nNO CHANGE MADE.\n"); break;
                default: printf("\nINVALID OPTION.\n"); break;
            }
            break;
        }
        case 5: {
            int sub;
            printf("\n1. UPDATE BLOOD GROUP\n");
            printf("2. UPDATE DATE OF BIRTH\n");
            printf("3. UPDATE GENDER\n");
            printf("0. BACK\n");
            sub = read_int("ENTER CHOICE: ");
            switch (sub) {
                case 1: read_string("ENTER NEW BLOOD GROUP: ", students[index].blood_group, sizeof(students[index].blood_group)); break;
                case 2: read_string("ENTER NEW DOB (DD/MM/YYYY): ", students[index].dob, sizeof(students[index].dob)); break;
                case 3: read_string("ENTER NEW GENDER: ", students[index].gender, sizeof(students[index].gender)); break;
                case 0: printf("\nNO CHANGE MADE.\n"); break;
                default: printf("\nINVALID OPTION.\n"); break;
            }
            break;
        }
        case 6: {
            read_string("ENTER NEW FEE STATUS (PAID / NOT_PAID): ", students[index].fee_status, sizeof(students[index].fee_status));
            break;
        }
        case 0:
            printf("\nUPDATE CANCELLED.\n");
            return;
        default:
            printf("\nINVALID MAIN OPTION.\n");
            return;
    }

    printf("\nSTUDENT DETAILS UPDATED SUCCESSFULLY.\n");
}

void delete_student() {
    int id, index;
    printf("\nDELETE (SOFT) STUDENT\n");
    printf("---------------------\n");
    id = read_int("ENTER STUDENT ID TO DELETE: ");
    index = find_student_by_id(id);

    if (index == -1) {
        printf("\nSTUDENT NOT FOUND.\n");
        return;
    }

    students[index].is_active = 0;
    printf("\nSTUDENT MARKED AS INACTIVE (SOFT DELETED).\n");
}

void print_line() {
    printf("---------------------------------------------------------------------------------------------\n");
}

void print_student_table_header() {
    print_line();
    printf("| ID  | NAME                 | ROOM | CAMPUS | YEAR | DEPT           | ACTIVE |\n");
    print_line();
}

void print_student_row(Student s) {
    printf("| %-3d | %-20s | %-4s | %-6s | %-4s | %-14s | %-6s |\n",
           s.id,
           s.name,
           s.room_no,
           s.campus,
           s.year,
           s.department,
           s.is_active ? "YES" : "NO");
}

void view_all_students() {
    if (student_count == 0) {
        printf("\nNO STUDENTS IN DATABASE.\n");
        return;
    }
    printf("\nALL STUDENTS (ACTIVE + INACTIVE)\n");
    print_student_table_header();
    for (int i = 0; i < student_count; i++) {
        print_student_row(students[i]);
    }
    print_line();
}

void view_active_students() {
    int found = 0;
    printf("\nACTIVE STUDENTS ONLY\n");
    print_student_table_header();
    for (int i = 0; i < student_count; i++) {
        if (students[i].is_active) {
            print_student_row(students[i]);
            found = 1;
        }
    }
    print_line();
    if (!found) {
        printf("\nNO ACTIVE STUDENTS FOUND.\n");
    }
}

void search_student_by_id() {
    int id, index;
    printf("\nSEARCH STUDENT BY ID\n");
    id = read_int("ENTER STUDENT ID: ");
    index = find_student_by_id(id);
    if (index == -1) {
        printf("\nSTUDENT NOT FOUND.\n");
        return;
    }
    print_student_table_header();
    print_student_row(students[index]);
    print_line();
}

void search_student_by_name() {
    char name[50];
    int found = 0;
    printf("\nSEARCH STUDENT BY NAME\n");
    read_string("ENTER NAME (FULL OR PART): ", name, sizeof(name));

    print_student_table_header();
    for (int i = 0; i < student_count; i++) {
        if (strstr(students[i].name, name) != NULL) {
            print_student_row(students[i]);
            found = 1;
        }
    }
    print_line();
    if (!found) {
        printf("\nNO STUDENT MATCHING THIS NAME.\n");
    }
}

void sort_students_by_name() {
    if (student_count <= 1) {
        printf("\nNOT ENOUGH STUDENTS TO SORT.\n");
        return;
    }
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = 0; j < student_count - i - 1; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("\nSTUDENTS SORTED BY NAME.\n");
    view_all_students();
}

void sort_students_by_room() {
    if (student_count <= 1) {
        printf("\nNOT ENOUGH STUDENTS TO SORT.\n");
        return;
    }
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = 0; j < student_count - i - 1; j++) {
            if (strcmp(students[j].room_no, students[j + 1].room_no) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("\nSTUDENTS SORTED BY ROOM NUMBER.\n");
    view_all_students();
}

void view_students_by_campus() {
    char campus[5];
    int found = 0;
    read_string("ENTER CAMPUS: ", campus, sizeof(campus));

    printf("\nSTUDENTS IN CAMPUS: %s\n", campus);
    print_student_table_header();
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].campus, campus) == 0) {
            print_student_row(students[i]);
            found = 1;
        }
    }
    print_line();
    if (!found) {
        printf("\nNO STUDENTS FOUND FOR THIS CAMPUS.\n");
    }
}

void dashboard() {
    int total = student_count;
    int active = 0;
    int a = 0, b = 0;
    int open_issues = 0;

    for (int i = 0; i < student_count; i++) {
        if (students[i].is_active) active++;
        if (strcmp(students[i].campus, "A") == 0) a++;
        if (strcmp(students[i].campus, "B") == 0) b++;
    }

    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].status, "OPEN") == 0) open_issues++;
    }

    printf("\n=================== DASHBOARD SUMMARY ===================\n");
    printf("TOTAL STUDENTS          : %d\n", total);
    printf("ACTIVE STUDENTS         : %d\n", active);
    printf("CAMPUS A STUDENTS       : %d\n", a);
    printf("CAMPUS B STUDENTS       : %d\n", b);
    printf("TOTAL ISSUES RAISED     : %d\n", ticket_count);
    printf("OPEN ISSUES             : %d\n", open_issues);
    printf("=========================================================\n");
}

void export_students_report() {
    FILE *fp = fopen(REPORT_FILE, "w");
    if (fp == NULL) {
        printf("\nUNABLE TO CREATE REPORT FILE.\n");
        return;
    }

    fprintf(fp, "   HOSTEL MANAGEMENT SYSTEM - STUDENT REPORT\n");
    fprintf(fp, "=============================================\n");
    for (int i = 0; i < student_count; i++) {
        Student s = students[i];
        fprintf(fp, "ID: %d\n", s.id);
        fprintf(fp, "NAME: %s\n", s.name);
        fprintf(fp, "PARENT: %s\n", s.parent_name);
        fprintf(fp, "MOTHER: %s\n", s.mother_name);
        fprintf(fp, "PHONE: %s\n", s.phone);
        fprintf(fp, "EMAIL: %s\n", s.email);
        fprintf(fp, "ROOM: %s\n", s.room_no);
        fprintf(fp, "BLOOD GROUP: %s\n", s.blood_group);
        fprintf(fp, "YEAR: %s\n", s.year);
        fprintf(fp, "DEPARTMENT: %s\n", s.department);
        fprintf(fp, "CAMPUS: %s\n", s.campus);
        fprintf(fp, "DISTRICT: %s\n", s.district);
        fprintf(fp, "STATE: %s\n", s.state);
        fprintf(fp, "PINCODE: %s\n", s.pincode);
        fprintf(fp, "ADDRESS: %s\n", s.address);
        fprintf(fp, "GUARDIAN: %s (%s)\n", s.guardian_name, s.guardian_phone);
        fprintf(fp, "DOB: %s  GENDER: %s\n", s.dob, s.gender);
        fprintf(fp, "HOSTEL BLOCK: %s\n", s.hostel_block);
        fprintf(fp, "ADMISSION YEAR: %s\n", s.admission_year);
        fprintf(fp, "FEE STATUS: %s\n", s.fee_status);
        fprintf(fp, "ACTIVE: %s\n", s.is_active ? "YES" : "NO");
        fprintf(fp, "---------------------------------------------\n");
    }
    fclose(fp);
    printf("\nSTUDENT REPORT EXPORTED TO FILE: %s\n", REPORT_FILE);
}

/* ---------------------- TICKET / ISSUE FUNCTIONS ---------------------- */

int find_ticket_by_id(int tid) {
    for (int i = 0; i < ticket_count; i++) {
        if (tickets[i].ticket_id == tid) {
            return i;
        }
    }
    return -1;
}

void raise_issue_student_portal() {
    if (ticket_count >= MAX_TICKETS) {
        printf("\nTICKET LIMIT REACHED. CANNOT RAISE MORE ISSUES.\n");
        return;
    }

    int id, index;
    Ticket t;

    printf("\nRAISE ISSUE - STUDENT PORTAL\n");
    id = read_int("ENTER YOUR STUDENT ID: ");
    index = find_student_by_id(id);

    if (index == -1 || !students[index].is_active) {
        printf("\nSTUDENT NOT REGISTERED OR INACTIVE. ISSUE CANNOT BE RAISED.\n");
        return;
    }

    t.ticket_id = next_ticket_id++;
    t.student_id = id;
    strcpy(t.student_name, students[index].name);
    read_string("ENTER YOUR ISSUE (ROOM / HOSTEL PROBLEM): ", t.issue, sizeof(t.issue));
    strcpy(t.status, "OPEN");

    tickets[ticket_count++] = t;

    printf("\nISSUE RAISED SUCCESSFULLY.\n");
    printf("YOUR TICKET ID IS: %d\n", t.ticket_id);
}

void check_issue_status_student_portal() {
    int tid, index;
    printf("\nCHECK ISSUE STATUS\n");
    tid = read_int("ENTER YOUR TICKET ID: ");
    index = find_ticket_by_id(tid);

    if (index == -1) {
        printf("\nNO SUCH TICKET FOUND.\n");
        return;
    }

    printf("\nTICKET DETAILS:\n");
    printf("TICKET ID  : %d\n", tickets[index].ticket_id);
    printf("STUDENT ID : %d\n", tickets[index].student_id);
    printf("STUDENT    : %s\n", tickets[index].student_name);
    printf("ISSUE      : %s\n", tickets[index].issue);
    printf("STATUS     : %s\n", tickets[index].status);
}

void raise_issue_admin() {
    if (ticket_count >= MAX_TICKETS) {
        printf("\nTICKET LIMIT REACHED. CANNOT RAISE MORE ISSUES.\n");
        return;
    }

    int id, index;
    Ticket t;

    printf("\nRAISE ISSUE - ADMIN SIDE\n");
    id = read_int("ENTER STUDENT ID: ");
    index = find_student_by_id(id);

    if (index == -1 || !students[index].is_active) {
        printf("\nSTUDENT NOT REGISTERED OR INACTIVE. ISSUE CANNOT BE RAISED.\n");
        return;
    }

    t.ticket_id = next_ticket_id++;
    t.student_id = id;
    strcpy(t.student_name, students[index].name);
    read_string("ENTER ISSUE DETAILS: ", t.issue, sizeof(t.issue));
    strcpy(t.status, "OPEN");

    tickets[ticket_count++] = t;

    printf("\nISSUE RAISED SUCCESSFULLY FOR STUDENT.\n");
    printf("TICKET ID: %d\n", t.ticket_id);
}

void view_all_tickets() {
    if (ticket_count == 0) {
        printf("\nNO ISSUES RAISED YET.\n");
        return;
    }
    printf("\nALL ISSUES / TICKETS\n");
    print_line();
    printf("| TID | STU_ID | STUDENT NAME        | STATUS       |\n");
    print_line();
    for (int i = 0; i < ticket_count; i++) {
        printf("| %-3d | %-6d | %-18s | %-11s |\n",
               tickets[i].ticket_id,
               tickets[i].student_id,
               tickets[i].student_name,
               tickets[i].status);
    }
    print_line();
}

void view_open_tickets() {
    int found = 0;
    printf("\nOPEN ISSUES ONLY\n");
    print_line();
    printf("| TID | STU_ID | STUDENT NAME        | STATUS |\n");
    print_line();
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].status, "OPEN") == 0) {
            printf("| %-3d | %-6d | %-18s | %-6s |\n",
                   tickets[i].ticket_id,
                   tickets[i].student_id,
                   tickets[i].student_name,
                   tickets[i].status);
            found = 1;
        }
    }
    print_line();
    if (!found) {
        printf("\nNO OPEN ISSUES.\n");
    }
}

void update_ticket_status() {
    int tid, index, choice;
    printf("\nUPDATE ISSUE STATUS (ADMIN ONLY)\n");
    tid = read_int("ENTER TICKET ID: ");
    index = find_ticket_by_id(tid);

    if (index == -1) {
        printf("\nTICKET NOT FOUND.\n");
        return;
    }

    printf("\nCURRENT STATUS: %s\n", tickets[index].status);
    printf("1. SET STATUS TO OPEN\n");
    printf("2. SET STATUS TO IN_PROGRESS\n");
    printf("3. SET STATUS TO RESOLVED\n");
    printf("0. CANCEL\n");

    choice = read_int("ENTER CHOICE: ");
    switch (choice) {
        case 1: strcpy(tickets[index].status, "OPEN"); break;
        case 2: strcpy(tickets[index].status, "IN_PROGRESS"); break;
        case 3: strcpy(tickets[index].status, "RESOLVED"); break;
        case 0: printf("\nNO CHANGE MADE.\n"); return;
        default: printf("\nINVALID OPTION.\n"); return;
    }
    printf("\nTICKET STATUS UPDATED SUCCESSFULLY.\n");
}

/* ---------------------- INPUT HELPERS ---------------------- */

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_string(char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    /* REMOVE NEWLINE */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int read_int(char *prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        clear_input_buffer();
        printf("INVALID INPUT. ENTER INTEGER VALUE: ");
    }
    clear_input_buffer();
    return value;
}
