#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
//#include "structs&functions.h"

typedef struct {
    char username[50];
    char password[50];
    char email[100];
    int total_points;
    int total_gold;
    int rank;
    int games_completed;
    int days_since_first_game;
} User;


int at_least_7(char *username) {

    int length = strlen(username);
    if (length > 7) {
        return 1;
    }
    else {
        return 0;
    }

}

int at_most_20(char *username) {

    int length = strlen(username);
    if (length < 20) {
        return 1;
    }
    else {
        return 0;
    }

}

int num_upc_lowc(char *password) {

    int length = strlen(password);
    int halle = 0;
    for (int i = 0; i < length; i++) {
        if (password[i] >= '0' && password[i] <= '9') {
            halle++;
        }
        else if (password[i] >= 'A' && password[i] <= 'Z') {
            halle++;
        }
        else if (password[i] >= 'a' && password[i] <= 'z') {
            halle++;
        }
    }
    if (halle >= 3) {
        return 1;
    }
    else {
        return 0;
    }

}

int correct_email(char *email) {

    int length = strlen(email);
    int halle = 0;
    int test;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    int ii;
    int iii;
    for (int i = length - 1; i >= 0; i--) {
        if (email[i] == '.') {
            ii = i;
            for (int j = i + 1; j < length; j++) {
                if (email[j] >= 'a' && email[j] <= 'z') {
                    test1++;
                }
            }
            test = 1;
        }
        if (test == 1) {
            if (test1 == length - i - 1) {
                halle++;
            }
            test = 0;
            break;
        }
    }
    for (int i = ii - 1; i >= 0; i--) {
        if (email[i] == '@') {
            iii = i;
            for (int j = i + 1; j < ii; j++) {
                if (email[j] >= 'a' && email[j] <= 'z') {
                    test2++;
                }
            }
            test = 1;
        }
        if (test == 1) {
            if (test2 == ii - i - 1) {
                halle++;
            }
            test = 0;
            break;
        }
    }
    for (int i = iii - 1; i >= 0; i--) {
        if (email[i] != '@') {
            test3++;
        }
    }
    if (test3 == iii) {
        halle++;
    }
    if (halle == 3) {
        return 1;
    }
    else {
        return 0;
    }

}

void save_users (User user) {

    FILE *file = fopen("users.txt", "a");
    fprintf(file, "%s %s %s\n", user.username, user.password, user.email);
    fclose(file);

}

User current_user;

int check_users(char *username, char *password) {

    FILE *file = fopen("users.txt", "r");
    User user;
    while (fscanf(file, "%s %s %s", user.username, user.password, user.email) != EOF) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            current_user = user; 
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;

}

void forgot_your_password() {

    clear();
    FILE *file = fopen("users.txt", "r");
    User user;
    keypad(stdscr, TRUE);
    cbreak();
    curs_set(FALSE);

    char username[50];
    char email[100];

    const char *forgotclicks[] = {
        "Enter your username:",
        "Enter your e-mail:",
        "Show password",
        "Back"
    };

    int choice = 0;

    while (1) {
        for (int i = 0; i < 4; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 2 - 4 + (2 * i), COLS / 2 - 27, "%s", forgotclicks[i]);
            if (i == choice) {
                attron(A_REVERSE);
            }
        }
        refresh();

        int ch = getch();
        if (ch == KEY_UP) {
            if (choice == 0) {
                choice = 3;
            }
            else {
                choice--;
            }
        }
        else if (ch == KEY_DOWN) {
            if (choice == 3) {
                choice = 0;
            }
            else {
                choice++;
            }
        }
        else if (ch == '\n') {
            switch (choice) {
                case 0:
                    echo();
                    move(LINES / 2 - 4, COLS / 2 - 8);
                    getnstr(username, 50);
                    noecho();
                    break;
                case 1:
                    echo();
                    move(LINES / 2 - 2, COLS / 2 - 9);
                    getnstr(email, 100);
                    noecho();
                    break;
                case 2:
                    while (fscanf(file, "%s %s %s", user.username, user.password, user.email) != EOF) {
                        if (strcmp(username, user.username) == 0 && strcmp(email, user.email) == 0) {
                            mvprintw(LINES / 2, COLS / 2 - 14, "Your password is %s", user.password); 
                            break;
                        }
                    }
                    fclose(file);
                    break;
                case 3:
                    sign_in_page();
                    break;
            }
        }
    }

}

void enter_as_guest() {
    clear();
    mvprintw(LINES / 2, COLS / 2, "enter as guest page");
}

char *random_password() { 

    char *password;
    char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int len = strlen(chars);
    for(int i = 0; i < 10; i++){
        char temp = chars[rand() % len];
        password[i] = temp;
        //strncpy(password[i], temp, 1);
    }
    return password;

}

int is_username_taken(char *username) {

    FILE *file = fopen("users.txt", "r");
    User user;
    while (fscanf(file, "%s %s %s", user.username, user.password, user.email) != EOF) {
        if (strcmp(username, user.username) == 0) {
            fclose(file);
            return 1;  
        }
    }

    fclose(file);
    return 0;  

}

void sign_in_page() {

    clear();
    keypad(stdscr, TRUE);
    cbreak();
    //noecho();
    curs_set(FALSE);

    char username[50];
    char password[50];

    // if (check_users(username, password)) {
    //     mvprintw(4, 0, "Welcome back!");
    // } else {
    //     mvprintw(4, 0, "Invalid username or password!");
    // }

    const char *signinclicks[] = {
        "Username:",
        "Password:",
        "Back",
        "Enter", 
        "Forgot your password?", 
        "Enter as guest"
        };

    int choice = 0;

    while (1) {
        for (int i = 0; i < 6; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 2 - 8 + (2 * i), COLS / 2 - 27, "%s", signinclicks[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }
        refresh();

        int ch = getch();
        if (ch == KEY_UP) {
            if (choice == 0) {
                choice = 5;
            }
            else {
                choice--;
            }
        } 
        else if (ch == KEY_DOWN) {
            if (choice == 5) {
                choice = 0;
            }
            else {
                choice++;
            }
        } 
        else if (ch == '\n') {
            switch (choice) {
                case 0:
                    echo();
                    move(LINES / 2 - 8, COLS / 2 - 17);
                    getnstr(username, 50);
                    noecho();
                    break;
                case 1:
                    noecho();
                    move(LINES / 2 - 6, COLS / 2 - 17);
                    getnstr(password, 50);
                    noecho();
                    break;
                case 2:
                    sign_in_and_sign_up();
                    break;
                case 3:
                    if (check_users(username, password)) {
                        pre_game_menu();
                    }
                    else {
                        mvprintw(LINES - 1, COLS / 2 - 14, "Invalid username or password!");
                        refresh();
                        getch();
                        sign_in_page();
                    }
                    break;
                case 4:
                    forgot_your_password();
                    break;
                case 5:
                    enter_as_guest();
                    break;
            }
        }
    }

}

void sign_up_page() {

    clear();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    curs_set(FALSE);

    char username[50];
    char password[50];
    char email[100];
    User new_user;

    const char *signupclicks[] = {
        "Please enter your username:",
        "Your e-mail:",
        "Your password:",
        "Random password",
        "Done",
        "Back"
    };

    int choice = 0;

    while (1) {
        for (int i = 0; i < 6; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 2 - 8 + (2 * i), COLS / 2 - 27, "%s", signupclicks[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }
        refresh();

        int ch = getch();
        if (ch == KEY_UP) {
            if (choice == 0) {
                choice = 5;
            }
            else {
                choice--;
            }
        } 
        else if (ch == KEY_DOWN) {
            if (choice == 5) {
                choice = 0;
            }
            else {
                choice++;
            }
        } 
        else if (ch == '\n') {
            switch (choice) {
                case 0:
                    echo();
                    move(LINES / 2 - 8, COLS / 2 + 1);
                    getnstr(username, 50);
                    noecho();
                    break;
                case 1:
                    echo();
                    move(LINES / 2 - 6, COLS / 2 - 14);
                    getnstr(email, 100);
                    noecho();
                    break;
                case 2:
                    noecho();
                    move(LINES / 2 - 4, COLS / 2 - 12);
                    getnstr(password, 50);
                    noecho();
                    break;
                case 3:
                    //mvprintw(LINES / 2 - 4, COLS / 2 - 12, "                   ");
                    strcpy(new_user.password, random_password());
                    mvprintw(LINES / 2 - 4, COLS / 2 - 12, new_user.password);
                    strcpy(password, new_user.password);
                    break;
                case 4:
                    if (!is_username_taken(username) && at_least_7(password) && at_most_20(password)
                    && num_upc_lowc(password) && correct_email(email)) {
                        strcpy(new_user.username, username);
                        strcpy(new_user.password, password);
                        strcpy(new_user.email, email);
                        save_users(new_user);
                        sign_in_and_sign_up();
                    }
                    else if (is_username_taken(username)) {
                        mvprintw(LINES - 1, COLS / 2 - 15, "The username is already taken!");
                        refresh();
                        getch();
                        sign_up_page();
                    }
                    else if (!correct_email(email)) {
                        mvprintw(LINES - 1, COLS / 2 - 9, "Not a correct form!");
                        refresh();
                        getch();
                        sign_up_page();
                    }
                    else if (!at_least_7(password)) {
                        mvprintw(LINES - 1, COLS / 2 - 22, "The password must has at least 7 characters.");
                        refresh();
                        getch();
                        sign_up_page();
                    }
                    else if (!at_most_20(password)) {
                        mvprintw(LINES - 1, COLS / 2 - 21, "The password must has at most 20 characters.");
                        refresh();
                        getch();
                        sign_up_page();
                    }
                    else if (!num_upc_lowc(password)) {
                        mvprintw(LINES - 1, COLS / 2 - 39, "The password must has at least a number a upper case character a lower case characters.");
                        refresh();
                        getch();
                        sign_up_page();
                    }
                    break;
                case 5:
                    sign_in_and_sign_up();
                    break;
            }
        }
    }

    // if (!is_username_taken(username)) {
    //     strcpy(new_user.username, username);
    // }
    // else {
    //     mvprintw(LINES / 2, COLS / 2 - 15, "The username is already taken!");
    // }

    // if (at_least_7(password) && at_most_20(password) && num_upc_lowc(password)) {
    //     strcpy(new_user.password, password);
    // }
    // else if (!at_least_7(password)) {
    //     mvprintw(LINES / 2 - 2, COLS / 2 - 22, "The password must has at least 7 characters.");
    // }
    // else if (!at_most_20(password)) {
    //     mvprintw(LINES / 2 - 2, COLS / 2 - 21, "The password must has at most 20 characters.");
    // }
    // else if (!num_upc_lowc(password)) {
    //     mvprintw(LINES / 2 - 2, COLS / 2 - 39, "The password must has at least a number a upper case character a lower case characters.");
    // }
    
    // if (correct_email(email)) {
    //     strcpy(new_user.email, email);
    // }
    // else {
    //     mvprintw(LINES / 2 - 4, COLS / 2 - 9, "Not a correct form!");
    // }
    
    // if (!is_username_taken(new_user.username) && at_least_7(new_user.password) && 
    // at_most_20(new_user.password) && num_upc_lowc(new_user.password) && correct_email(new_user.email)) {
    //     save_users (new_user);
    //     mvprintw(LINES / 2 - 6, COLS / 2 - 4, "Welcome!");
    // }

}

void sign_in_and_sign_up() {

    clear();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    mvprintw(LINES / 8, COLS / 2 - 12, "Welcome to rogue game :)");
    mvprintw(LINES / 4, COLS / 2 - 24, "If you have account sign in if not please sign up");
    refresh();
    const char *signinandsignup[] = {"Sign in", "Sign up"};
    int choice = 0;
    while (1) {
        for (int i = 0; i < 2; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 2, COLS / 2 - 18 + (30 * i), "%s", signinandsignup[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        int ch = getch();
        if (ch == KEY_RIGHT) {
            choice = 1;
        } 
        else if (ch == KEY_LEFT) {
            choice = 0;
        } else if (ch == 10) {
            break;
        }
    }

    switch (choice) {
    case 0:
        sign_in_page();
        break;
    case 1:
        sign_up_page();
        break;
    }
    clear();

}

void new_game_page() {

    clear();
    mvprintw(LINES / 2, COLS / 2 - 4, "new game");
    getch();

}

void resume_game() {

    clear();
    mvprintw(LINES / 2, COLS / 2 - 5, "resume game");
    getch();

}

void score_board() {

    clear();
    mvprintw(LINES / 2, COLS / 2 - 5, "score board");
    getch();

}

// void select_dificulty() {
    
//     clear();
//     mvprintw(LINES / 8, COLS / 2 - 10, "Select dificulty");
//     const char *setting[] = {"Hard", "Medium", "Ez"};
//     int choice = 0;
//     while (1) {current_user = userw(LINES / 8 + (LINES / 12 * i), COLS / 2, "%s", setting[i]);
//             if (i == choice) {
//                 attroff(A_REVERSE);
//             }
//         }

//         int ch = getch();
//         if (ch == KEY_DOWN) {
//             if (choice == 2) {
//                 choice = 0;
//             }
//             else {
//                 choice++;
//             }
//         }
//         else if (ch == KEY_UP) {
//             if (choice == 0) {
//                 choice = 2;
//             }
//             else {
//                 choice--;
//             }
//         } 
//         else if (ch == 10) {
//             break;
//         }
//         getch();
//         clear();

//     }

//     switch (choice) {
//     case 0:
//         //getch();
//         hard_game_mode();
//         break;
//     case 1:
//         //getch();
//         medium_game_mode();
//         break;
//     case 2:
//         //getch();
//         ez_game_mode();
//         break;
//     }
//     getch();

// }

// void change_color() {

//     clear();
//     mvprintw(LINES / 8, COLS / 2 - 10, "Change color");
//     const char *setting[] = {"White", "Silver", "Violet"};
//     int choice = 0;
//     while (1) {
//         for (int i = 0; i < 3; i++) {
//             if (i == choice) {
//                 attron(A_REVERSE);
//             }
//             mvprintw(LINES / 8 + (LINES / 12 * i), COLS / 2, "%s", setting[i]);
//             if (i == choice) {
//                 attroff(A_REVERSE);
//             }
//         }

//         int ch = getch();
//         if (ch == KEY_DOWN) {
//             if (choice == 2) {
//                 choice = 0;
//             }
//             else {
//                 choice++;
//             }
//         }
//         else if (ch == KEY_UP) {
//             if (choice == 0) {
//                 choice = 2;
//             }
//             else {
//                 choice--;
//             }
//         } 
//         else if (ch == 10) {
//             break;
//         }
//         getch();
//         clear();

//     }

//     switch (choice) {
//     case 0:
//         //getch();
//         white_npc_color();
//         break;
//     case 1:
//         //getch();
//         silver_npc_color();
//         break;
//     case 2:
//         //getch();
//         violet_npc_color();
//         break;
//     }
//     getch();

// }

// void music() {

//     clear();
//     mvprintw(LINES / 8, COLS / 2 - 10, "Music");
//     const char *setting[] = {"Unforgiven |", "Unforgiven ||", "Unforgiven |||"};
//     int choice = 0;
//     while (1) {
//         for (int i = 0; i < 3; i++) {
//             if (i == choice) {
//                 attron(A_REVERSE);
//             }
//             mvprintw(LINES / 8 + (LINES / 12 * i), COLS / 2, "%s", setting[i]);
//             if (i == choice) {
//                 attroff(A_REVERSE);
//             }
//         }

//         int ch = getch();
//         if (ch == KEY_DOWN) {
//             if (choice == 2) {
//                 choice = 0;
//             }
//             else {
//                 choice++;
//             }
//         }
//         else if (ch == KEY_UP) {
//             if (choice == 0) {
//                 choice = 2;
//             }
//             else {
//                 choice--;
//             }
//         } 
//         else if (ch == 10) {
//             break;
//         }
//         getch();
//         clear();

//     }

//     switch (choice) {
//     case 0:
//         //getch();
//         //unforgiven_music();
//         break;
//     case 1:
//         //getch();
//         //unforgiven_two_music();
//         break;
//     case 2:
//         //getch();
//         //unforgiven_three_music();
//         break;
//     }
//     getch();

// }

void profile() {

    clear();
    mvprintw(LINES / 8, COLS / 2 - 10, "Profile");
    mvprintw(LINES / 4, COLS / 2 - 10, "Username: %s", current_user.username);
    mvprintw(LINES / 4 + 1, COLS / 2 - 10, "Email: %s", current_user.email);
    mvprintw(LINES - 1, COLS / 2 - 10, "Press ESC to return.");
    int ch = getch();
    if (ch == 27) {
        pre_game_menu();
    }
    //getch();

}

void setting() {

    clear();
    mvprintw(LINES / 8, COLS / 2 - 10, "setting");
    const char *setting[] = {"Select dificulty", "Change color", "Music"};
    int choice = 0;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 8 + (LINES / 12 * i), COLS / 2, "%s", setting[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        int ch = getch();
        if (ch == KEY_DOWN) {
            if (choice == 2) {
                choice = 0;
            }
            else {
                choice++;
            }
        }
        else if (ch == KEY_UP) {
            if (choice == 0) {
                choice = 2;
            }
            else {
                choice--;
            }
        } 
        else if (ch == 10) {
            break;
        }
        getch();
        clear();

    }

    switch (choice) {
    case 0:
        //getch();
        //select_dificulty();
        break;
    case 1:
        //getch();
        //change_color();
        break;
    case 2:
        //getch();
        //music();
        break;
    }
    getch();

}

void pre_game_menu() {

    clear();
    const char *signinandsignup[] = {"New game", "Resume", "Score board", "Setting", "Profile"};
    int choice = 0;
    while (1) {
        for (int i = 0; i < 5; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 8 + (LINES / 12 * i), COLS / 2, "%s", signinandsignup[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        int ch = getch();
        if (ch == KEY_DOWN) {
            if (choice == 4) {
                choice = 0;
            }
            else {
                choice++;
            }
        } 
        else if (ch == KEY_UP) {
            if (choice == 0) {
                choice = 4;
            }
            else {
                choice--;
            }
        } 
        else if (ch == 10) {
            break;
        }
        //getch();
        clear();

    }

    switch (choice) {
    case 0:
        //getch();
        new_game_page();
        break;
    case 1:
        //getch();
        resume_game();
        break;
    case 2:
        //getch();
        score_board();
        break;
    case 3:
        //getch();
        setting();
        break;
    case 4:
        //getch();
        profile();
        break;
    }

}

int main() {

    initscr();
    sign_in_and_sign_up();
    getch();
    endwin();
    return 0;

}
