#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

typedef struct {
    int *friends;
    int size;
    int capacity;
} FriendList;

typedef struct {
    FriendList *records;
    int nAccounts;
    int *path;
    int pathSize;
} BFS;

void initFriendList(FriendList *list) {
    list->size = 0;
    list->capacity = 2;
    list->friends = (int *)malloc(list->capacity * sizeof(int));
}

void addFriend(FriendList *list, int id) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->friends = (int *)realloc(list->friends, list->capacity * sizeof(int));
    }
    list->friends[list->size++] = id;
}

void freeFriendList(FriendList *list) {
    free(list->friends);
}

void initBFS(BFS *bfs, FriendList *records, int nAccounts) {
    bfs->records = records;
    bfs->nAccounts = nAccounts;
    bfs->path = (int *)malloc(nAccounts * sizeof(int));
    bfs->pathSize = 0;
}

void clearPath(BFS *bfs) {
    bfs->pathSize = 0;
}

int searchFriends(BFS *bfs, int id) {
    printf("Person %d have %d friends!\n", id, bfs->records[id].size);
    printf("List of friends: ");

    if (bfs->records[id].size > 0) {
        int *visited = (int *)calloc(bfs->nAccounts, sizeof(int));
        int start = id;

        visited[start] = 1;

        for (int i = 0; i < bfs->records[start].size; i++) {
            int element = bfs->records[start].friends[i];
            if (!visited[element]) {
                visited[element] = 1;
                printf("%d ", element);
            }
        }
        printf("\n");
        free(visited);
        return 1;
    }

    printf("\n");
    return 0;
}

int searchConnection(BFS *bfs, int id1, int id2) {
    int *visited = (int *)calloc(bfs->nAccounts, sizeof(int));
    int *parent = (int *)malloc(bfs->nAccounts * sizeof(int));
    int *queue = (int *)malloc(bfs->nAccounts * sizeof(int));

    int front = 0, rear = 0;
    int start = id1;

    clearPath(bfs);

    for (int i = 0; i < bfs->nAccounts; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int current = queue[front++];

        for (int i = 0; i < bfs->records[current].size; i++) {
            int element = bfs->records[current].friends[i];

            if (!visited[element]) {
                parent[element] = current;
                visited[element] = 1;
                queue[rear++] = element;

                if (element == id2) {
                    int nodePrevious = element;
                    while (nodePrevious != -1) {
                        bfs->path[bfs->pathSize++] = nodePrevious;
                        nodePrevious = parent[nodePrevious];
                    }

                    free(visited);
                    free(parent);
                    free(queue);
                    return 1;
                }
            }
        }
    }

    free(visited);
    free(parent);
    free(queue);
    return 0;
}

void freeBFS(BFS *bfs) {
    free(bfs->path);
}