#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char title[50];
    char authors[50];
    int year;
    char genres[30];
    struct Song* left;
    struct Song* right;
} Song;

typedef struct User {
    char username[50];
    char email[50];
    int register_year;
    char follows[50];
    struct User* left;
    struct User* right;
} User;

//Ditop_pointerlay functions

void ditop_pointerlay_songs(Song* matched_songs[], int match_count) {
    printf("\n%-5s | %-64s | %-32s | %-4s | %s\n", "ID", "Title", "Artist", "Year", "Genre");
    printf("===========================================================================================================================================\n");
    for (int i = 0; i < match_count; i++) {
        printf("%-5d | %-64s | %-32s | %-4d | %s\n",
            i + 1, matched_songs[i]->title, matched_songs[i]->authors,
            matched_songs[i]->year, matched_songs[i]->genres);
    }
}

void ditop_pointerlay_users(User* matched_users[], int match_count) {
    printf("\n%-5s | %-32s | %-20s | %-12s | %s\n", "ID", "Username", "Email", "Reg. Year", "Follows");
    printf("=========================================================================================================================\n");
    for (int i = 0; i < match_count; i++) {
        printf("%-5d | %-32s | %-20s | %-12d | %s\n",
            i + 1, matched_users[i]->username, matched_users[i]->email,
            matched_users[i]->register_year, matched_users[i]->follows);
    }
}

//Search functions


int search_songs(Song* root, int search_type, const char* search_term, int target_year, Song* matched_songs[]) {
    int match_count = 0;
    Song* search_buffer[1024];
    int top_pointer = -1;
    Song* curr = root;

    while (curr != NULL || top_pointer != -1) {
        while (curr != NULL) {
            search_buffer[++top_pointer] = curr;
            curr = curr->left;
        }
        curr = search_buffer[top_pointer--];

        int match = 0;
        switch (search_type) {
        case 0: match = 1; break;
        case 1: if (strstr(curr->title, search_term))   match = 1; break;
        case 2: if (strstr(curr->authors, search_term)) match = 1; break;
        case 3: if (curr->year == target_year)          match = 1; break;
        case 4: if (strstr(curr->genres, search_term))  match = 1; break;
        }
        if (match) matched_songs[match_count++] = curr;
        curr = curr->right;
    }
    return match_count;
}

int search_users(User* root, int search_type, const char* search_term, int target_year, User* matched_users[]) {
    int match_count = 0;
    User* search_buffer[1024];
    int top_pointer = -1;
    User* curr = root;

    while (curr != NULL || top_pointer != -1) {
        while (curr != NULL) {
            search_buffer[++top_pointer] = curr;
            curr = curr->left;
        }
        curr = search_buffer[top_pointer--];

        int match = 0;
        switch (search_type) {
        case 0: match = 1; break;
        case 1: if (strstr(curr->username, search_term))     
            match = 1; break;
        case 2: if (strstr(curr->email, search_term))        
            match = 1; break;
        case 3: if (curr->register_year == target_year)      
            match = 1; break;
        case 4: if (strstr(curr->follows, search_term))      
            match = 1; break;
        }
        if (match) matched_users[match_count++] = curr;
        curr = curr->right;
    }
    return match_count;
}

//Input Functions

void get_song_search_criteria(int* search_type, char* search_term, int* target_year) {
    printf("\nSearch by: 1. Title  2. Artist  3. Year  4. Genre  0. Show All\n> ");
    scanf_s("%d", search_type);

    if (*search_type == 3) {
        printf("Year: ");
        scanf_s("%d", target_year);
    }
    else if (*search_type != 0) {
        printf("Search Term: ");
        scanf_s(" %[^\n]", search_term, (unsigned int)50);
    }
    while (getchar() != '\n');
}

void get_user_search_criteria(int* search_type, char* search_term, int* target_year) {
    printf("\nSearch by: 1. Username  2. Email  3. Reg. Year  4. Follows  0. Show All\n> ");
    scanf_s("%d", search_type);

    if (*search_type == 3) {
        printf("Registration Year: ");
        scanf_s("%d", target_year);
    }
    else if (*search_type != 0) {
        printf("Search Term: ");
        scanf_s(" %[^\n]", search_term, (unsigned int)50);
    }
    while (getchar() != '\n');
}

//Songs tree

void insert_song(Song** root, Song* new_song) {
    new_song->left = new_song->right = NULL;

    if (*root == NULL) {
        *root = new_song;
        return;
    }

    Song* curr = *root;
    Song* parent = NULL;

    while (curr != NULL) {
        parent = curr;
        curr = (strcmp(new_song->title, curr->title) >= 0) ? curr->right : curr->left;
    }

    if (strcmp(new_song->title, parent->title) >= 0)
        parent->right = new_song;
    else
        parent->left = new_song;
}

Song* remove_song_from_tree(Song** root, const char* title) {
    Song* parent = NULL;
    Song* del = *root;

    while (del != NULL && strcmp(del->title, title) != 0) {
        parent = del;
        del = (strcmp(title, del->title) < 0) ? del->left : del->right;
    }

    if (del == NULL) return NULL;

    Song* rep = NULL;
    if (del->left == NULL)
        rep = del->right;
    else if (del->right == NULL)
        rep = del->left;
    else {
        Song* top_pointer = del;
        rep = del->right;
        while (rep->left != NULL) {
            top_pointer = rep;
            rep = rep->left;
        }
        if (top_pointer != del) {
            top_pointer->left = rep->right;
            rep->right = del->right;
        }
        rep->left = del->left;
    }

    if (parent == NULL)
        *root = rep;
    else if (parent->left == del)
        parent->left = rep;
    else
        parent->right = rep;

    return del;
}

//Users tree

void insert_user(User** root, User* new_user) {
    new_user->left = new_user->right = NULL;

    if (*root == NULL) {
        *root = new_user;
        return;
    }

    User* curr = *root;
    User* parent = NULL;

    while (curr != NULL) {
        parent = curr;
        curr = (strcmp(new_user->username, curr->username) >= 0) ? curr->right : curr->left;
    }

    if (strcmp(new_user->username, parent->username) >= 0)
        parent->right = new_user;
    else
        parent->left = new_user;
}

User* remove_user_from_tree(User** root, const char* username) {
    User* parent = NULL;
    User* del = *root;

    while (del != NULL && strcmp(del->username, username) != 0) {
        parent = del;
        del = (strcmp(username, del->username) < 0) ? del->left : del->right;
    }

    if (del == NULL) return NULL;

    User* rep = NULL;
    if (del->left == NULL)
        rep = del->right;
    else if (del->right == NULL)
        rep = del->left;
    else {
        User* top_pointer = del;
        rep = del->right;
        while (rep->left != NULL) {
            top_pointer = rep;
            rep = rep->left;
        }
        if (top_pointer != del) {
            top_pointer->left = rep->right;
            rep->right = del->right;
        }
        rep->left = del->left;
    }

    if (parent == NULL)
        *root = rep;
    else if (parent->left == del)
        parent->left = rep;
    else
        parent->right = rep;

    return del;
}

//Files


void save_songs_to_file(Song* root) {
    char filename[64];
    printf("Save songs as: ");
    scanf_s(" %[^\n]", filename, (unsigned int)sizeof(filename));
    while (getchar() != '\n');

    FILE* f = NULL;
    if (fopen_s(&f, filename, "w") != 0) {
        printf("Error saving file.\n");
        return;
    }

    Song* search_buffer[100];
    int top_pointer = -1;
    if (root) search_buffer[++top_pointer] = root;

    while (top_pointer != -1) {
        Song* n = search_buffer[top_pointer--];
        fprintf(f, "%s|%s|%d|%s\n", n->title, n->authors, n->year, n->genres);
        if (n->right) search_buffer[++top_pointer] = n->right;
        if (n->left)  search_buffer[++top_pointer] = n->left;
    }
    fclose(f);
    printf("Songs saved to %s\n", filename);
}

void load_songs_from_file(Song** root) {
    char filename[64] = { 0 };
    printf("Load songs from: ");
    scanf_s(" %[^\n]", filename, (unsigned int)sizeof(filename));
    while (getchar() != '\n');

    FILE* f = NULL;
    if (fopen_s(&f, filename, "r") != 0) {
        printf("Error loading file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strctop_pointern(line, "\n")] = 0;
        char* ctx = NULL;
        char* t = strtok_s(line, "|", &ctx);
        char* a = strtok_s(NULL, "|", &ctx);
        char* y = strtok_s(NULL, "|", &ctx);
        char* g = strtok_s(NULL, "\n", &ctx);

        if (t && a && y && g) {
            Song* n = (Song*)malloc(sizeof(Song));
            if (n) {
                strcpy_s(n->title, sizeof(n->title), t);
                strcpy_s(n->authors, sizeof(n->authors), a);
                n->year = atoi(y);
                strcpy_s(n->genres, sizeof(n->genres), g);
                insert_song(root, n);
            }
        }
    }
    fclose(f);
    printf("Songs loaded from %s\n", filename);
}

void save_users_to_file(User* root) {
    char filename[64];
    printf("Save users as: ");
    scanf_s(" %[^\n]", filename, (unsigned int)sizeof(filename));
    while (getchar() != '\n');

    FILE* f = NULL;
    if (fopen_s(&f, filename, "w") != 0) {
        printf("Error while saving file.\n");
        return;
    }

    User* search_buffer[100];
    int top_pointer = -1;
    if (root) search_buffer[++top_pointer] = root;

    while (top_pointer != -1) {
        User* n = search_buffer[top_pointer--];
        fprintf(f, "%s|%s|%d|%s\n", n->username, n->email, n->register_year, n->follows);
        if (n->right) search_buffer[++top_pointer] = n->right;
        if (n->left)  search_buffer[++top_pointer] = n->left;
    }
    fclose(f);
    printf("Users saved to %s\n", filename);
}

void load_users_from_file(User** root) {
    char filename[64] = { 0 };
    printf("Load users from: ");
    scanf_s(" %[^\n]", filename, (unsigned int)sizeof(filename));
    while (getchar() != '\n');

    FILE* f = NULL;
    if (fopen_s(&f, filename, "r") != 0) {
        printf("Error while loading file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strctop_pointern(line, "\n")] = 0;
        char* ctx = NULL;
        char* u = strtok_s(line, "|", &ctx);
        char* e = strtok_s(NULL, "|", &ctx);
        char* y = strtok_s(NULL, "|", &ctx);
        char* fw = strtok_s(NULL, "\n", &ctx);

        if (u && e && y && fw) {
            User* n = (User*)malloc(sizeof(User));
            if (n) {
                strcpy_s(n->username, sizeof(n->username), u);
                strcpy_s(n->email, sizeof(n->email), e);
                n->register_year = atoi(y);
                strcpy_s(n->follows, sizeof(n->follows), fw);
                insert_user(root, n);
            }
        }
    }
    fclose(f);
    printf("Users loaded from %s\n", filename);
}

// songs

void proc_add_song(Song** root) {
    printf("Songs > Add Song\n");
    Song* n = (Song*)malloc(sizeof(Song));
    if (!n) return;

    printf("Title: ");
    scanf_s(" %[^\n]", n->title, (unsigned int)sizeof(n->title));
    printf("Artist: ");
    scanf_s(" %[^\n]", n->authors, (unsigned int)sizeof(n->authors));
    printf("Year: ");
    scanf_s("%d", &n->year);
    printf("Genre: ");
    scanf_s(" %[^\n]", n->genres, (unsigned int)sizeof(n->genres));

    insert_song(root, n);
    printf("Song added successfully!\n");
}

void proc_edit_song(Song** root, Song* matched_songs[]) {
    printf("Songs > Edit Song\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_song_search_criteria(&st, sterm, &yr);
    int cnt = search_songs(*root, st, sterm, yr, matched_songs);

    if (cnt == 0) { printf("No songs found to edit.\n"); return; }

    ditop_pointerlay_songs(matched_songs, cnt);
    printf("\nEnter ID to Edit (0 to cancel): ");
    int id; scanf_s("%d", &id); while (getchar() != '\n');

    if (id > 0 && id <= cnt) {
        Song* edited = remove_song_from_tree(root, matched_songs[id - 1]->title);
        if (edited) {
            printf("\n--- Editing Song ---\n");
            printf("New Title (current: %s): ", edited->title);
            scanf_s(" %[^\n]", edited->title, (unsigned int)sizeof(edited->title));
            printf("New Artist (current: %s): ", edited->authors);
            scanf_s(" %[^\n]", edited->authors, (unsigned int)sizeof(edited->authors));
            printf("New Year (current: %d): ", edited->year);
            scanf_s("%d", &edited->year);
            printf("New Genre (current: %s): ", edited->genres);
            scanf_s(" %[^\n]", edited->genres, (unsigned int)sizeof(edited->genres));
            while (getchar() != '\n');

            insert_song(root, edited);
            printf("Song edited successfully!\n");
        }
    }
}

void proc_remove_song(Song** root, Song* matched_songs[]) {
    printf("Songs > Remove Song\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_song_search_criteria(&st, sterm, &yr);
    int cnt = search_songs(*root, st, sterm, yr, matched_songs);

    if (cnt == 0) { printf("No songs found to remove.\n"); return; }

    ditop_pointerlay_songs(matched_songs, cnt);
    printf("\nEnter ID to Remove (0 to cancel): ");
    int id; scanf_s("%d", &id); while (getchar() != '\n');

    if (id > 0 && id <= cnt) {
        Song* removed = remove_song_from_tree(root, matched_songs[id - 1]->title);
        if (removed) { free(removed); printf("Song removed successfully!\n"); }
    }
}

void proc_search_songs(Song* root, Song* matched_songs[]) {
    printf("Songs > Search Songs\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_song_search_criteria(&st, sterm, &yr);
    int cnt = search_songs(root, st, sterm, yr, matched_songs);

    if (cnt == 0) printf("No songs found.\n");
    else ditop_pointerlay_songs(matched_songs, cnt);
}

// users 

void proc_add_user(User** root) {
    printf("Users > Add User\n");
    User* n = (User*)malloc(sizeof(User));
    if (!n) return;

    printf("Username: ");
    scanf_s(" %[^\n]", n->username, (unsigned int)sizeof(n->username));
    printf("Email: ");
    scanf_s(" %[^\n]", n->email, (unsigned int)sizeof(n->email));
    printf("Registration Year: ");
    scanf_s("%d", &n->register_year);
    printf("Follows (authors, comma-separated): ");
    scanf_s(" %[^\n]", n->follows, (unsigned int)sizeof(n->follows));

    insert_user(root, n);
    printf("User added successfully!\n");
}

void proc_edit_user(User** root, User* matched_users[]) {
    printf("Users > Edit User\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_user_search_criteria(&st, sterm, &yr);
    int cnt = search_users(*root, st, sterm, yr, matched_users);

    if (cnt == 0) { printf("No users found to edit.\n"); return; }

    ditop_pointerlay_users(matched_users, cnt);
    printf("\nEnter ID to Edit (0 to cancel): ");
    int id; scanf_s("%d", &id); while (getchar() != '\n');

    if (id > 0 && id <= cnt) {
        User* edited = remove_user_from_tree(root, matched_users[id - 1]->username);
        if (edited) {
            printf("\n--- Editing User ---\n");
            printf("New Username (current: %s): ", edited->username);
            scanf_s(" %[^\n]", edited->username, (unsigned int)sizeof(edited->username));
            printf("New Email (current: %s): ", edited->email);
            scanf_s(" %[^\n]", edited->email, (unsigned int)sizeof(edited->email));
            printf("New Registration Year (current: %d): ", edited->register_year);
            scanf_s("%d", &edited->register_year);
            printf("New Follows (current: %s): ", edited->follows);
            scanf_s(" %[^\n]", edited->follows, (unsigned int)sizeof(edited->follows));
            while (getchar() != '\n');

            insert_user(root, edited);
            printf("User edited successfully!\n");
        }
    }
}

void proc_remove_user(User** root, User* matched_users[]) {
    printf("Users > Remove User\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_user_search_criteria(&st, sterm, &yr);
    int cnt = search_users(*root, st, sterm, yr, matched_users);

    if (cnt == 0) { printf("No users found to remove.\n"); return; }

    ditop_pointerlay_users(matched_users, cnt);
    printf("\nEnter ID to Remove (0 to cancel): ");
    int id; scanf_s("%d", &id); while (getchar() != '\n');

    if (id > 0 && id <= cnt) {
        User* removed = remove_user_from_tree(root, matched_users[id - 1]->username);
        if (removed) { free(removed); printf("User removed successfully!\n"); }
    }
}

void proc_search_users(User* root, User* matched_users[]) {
    printf("Users > Search Users\n");

    int st; char sterm[50] = ""; int yr = 0;
    get_user_search_criteria(&st, sterm, &yr);
    int cnt = search_users(root, st, sterm, yr, matched_users);

    if (cnt == 0) printf("No users found.\n");
    else ditop_pointerlay_users(matched_users, cnt);
}

// main ========================================================================================================================================

int main() {
    Song* song_root = NULL;
    Song* matched_songs[1024];

    User* user_root = NULL;
    User* matched_users[1024];

    int choice;

    printf("Song Catalogue\n\n");

    while (1) {
        printf("[Main Section]=========================================\n");
        printf("1. Users  2. Songs  3. Save  4. Load  0. Quit\n");
        printf("=======================================================\n\n\n");
        printf("> Go to section   ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 0:
            printf("\nExiting...\n");
            return 0;

        case 1:
            printf("[Users]================================================\n");
            printf("1. Add User  2. Edit User  3. Remove User\n4. Search Users  0. Go Back\n");
            printf("=======================================================\n\n\n");
            printf("> Select action   ");
            scanf_s("%d", &choice);

            switch (choice) {
                case 0: break;
                case 1: proc_add_user(&user_root); break;
                case 2: proc_edit_user(&user_root, matched_users); break;
                case 3: proc_remove_user(&user_root, matched_users); break;
                case 4: proc_search_users(user_root, matched_users); break;
                default: printf("\nError. Expected 1 2 3 4 or 0\n\n"); break;
            }
            break;

        case 2:
            printf("[Songs]================================================\n");
            printf("1. Add Song  2. Edit Song  3. Remove Song\n4. Search Songs  0. Go Back\n");
            printf("=======================================================\n\n\n");
            printf("> Select action   ");
            scanf_s("%d", &choice);

            switch (choice) {
                case 0: break;
                case 1: proc_add_song(&song_root); break;
                case 2: proc_edit_song(&song_root, matched_songs); break;
                case 3: proc_remove_song(&song_root, matched_songs); break;
                case 4: proc_search_songs(song_root, matched_songs); break;
                default: printf("\nError. Expected 1 2 3 4 or 0\n\n"); break;
            }
            break;

        case 3:
            printf("[Save Section]=========================================\n");
            save_songs_to_file(song_root);
            save_users_to_file(user_root);
            break;

        case 4:
            printf("[Load Section]=========================================\n");
            load_songs_from_file(&song_root);
            load_users_from_file(&user_root);
            break;

        default:
            printf("\nError. Expected 1 2 3 4 or 0\n\n");
            break;
        }
    }
    return 0;
}