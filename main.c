#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bfs.c"

int main() {
    int nConnections, nAccounts;
    char filename[100];
    char fullPath[200];
    FILE *file;

    printf("Enter the name of the text file with extension (.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    sprintf(fullPath, "data/%s", filename);

    file = fopen(fullPath, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    if (fscanf(file, "%d %d", &nAccounts, &nConnections) != 2) {
        printf("Wrong read\n");
        fclose(file);
        return 1;
    }

    FriendList *records = (FriendList *)malloc(nAccounts * sizeof(FriendList));
    for (int i = 0; i < nAccounts; i++) {
        initFriendList(&records[i]);
    }

    for (int i = 0; i < nConnections; i++) {
        int account, friend;
        fscanf(file, "%d %d", &account, &friend);

        addFriend(&records[account], friend);
        addFriend(&records[friend], account);
    }

    fclose(file);

    printf("Graph Loaded!\n");

    BFS bfs;
    initBFS(&bfs, records, nAccounts);

    int choice;
    int valid = 0;

    do {
        do {
            printf("MAIN MENU!\n");
            printf("[1] Get friend list\n");
            printf("[2] Get connection\n");
            printf("[3] Exit\n\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
        } while (choice < 1 || choice > 3);

        if (choice == 1) {
            int id;
            valid = 0;

            do {
                printf("Enter ID of person: ");
                scanf("%d", &id);

                if (id >= 0 && id < nAccounts) {
                    valid = 1;
                } else {
                    printf("The ID inputted does not exist.\n");
                }
            } while (!valid);

            printf("\n");
            int hasFriends = searchFriends(&bfs, id);
            if (!hasFriends) {
                printf("No friends\n");
            }
            printf("\n");
        }
        else if (choice == 2) {
            int id1, id2;
            valid = 0;

            do {
                printf("Enter ID of first person: ");
                scanf("%d", &id1);

                if (id1 >= 0 && id1 < nAccounts) {
                    valid = 1;
                } else {
                    printf("The ID inputted does not exist.\n");
                }
            } while (!valid);

            valid = 0;
            do {
                printf("Enter ID of second person: ");
                scanf("%d", &id2);

                if (id2 >= 0 && id2 < nAccounts) {
                    valid = 1;
                } else {
                    printf("The ID inputted does not exist.\n");
                }
            } while (!valid);

            int isConnected = searchConnection(&bfs, id1, id2);

            if (isConnected) {
                printf("There is a connection between from %d to %d!\n", id1, id2);
                for (int i = bfs.pathSize - 1; i >= 1; i--) {
                    printf("%d is connected to %d\n", bfs.path[i], bfs.path[i - 1]);
                }
            } else {
                printf("Cannot find a connection between %d to %d!\n", id1, id2);
            }

            printf("\n");
        }

    } while (choice != 3);

    for (int i = 0; i < nAccounts; i++) {
        freeFriendList(&records[i]);
    }

    free(records);
    freeBFS(&bfs);

    return 0;
}