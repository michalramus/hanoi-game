#include <iostream>
#include <ncurses.h>
#include "list.h"


void drawIndicator(int selectedStack, int selectedDisk, int diskCount);

void drawStack(int diskCount, List<int> *stack, int selectedStack);

void draw(int diskCount, List<int> stacks[3], int selectedStack, int selectedDisk);

// program <disk count>
int main(int argc, char *argv[]) {
    List<int> stacks[3];

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <disk count>" << std::endl;
        return 1;
    }

    int count = std::stoi(argv[1]);
    int selectedStack = 0;
    int selectedDisk = 0;


    for (int i = count; i > 0; i--) {
        stacks[0].pushFront(i);
    }

    initscr(); //start
    cbreak(); // Disable line buffering
    keypad(stdscr, TRUE); // Enable arrow keys
    noecho(); // Don't echo input to screen
    nodelay(stdscr, FALSE); // Wait for input


    draw(count, stacks, selectedStack, selectedDisk);

    int ch;
    while ((ch = getch()) != 'q') {
        // Press 'q' to quit the program
        switch (ch) {
            case KEY_LEFT:
                selectedStack--;
                if (selectedStack < 0) {
                    selectedStack = 0;
                }

                break;
            case KEY_RIGHT:
                selectedStack++;
                if (selectedStack > 2) {
                    selectedStack = 2;
                }

                break;
            case ' ':
                if (selectedDisk == 0) {
                    //select disk
                    auto node = stacks[selectedStack].getNode(0);
                    if (node != nullptr) {
                        selectedDisk = node->item;
                        stacks[selectedStack].popFront();
                    }
                } else {
                    //release disk
                    if ((stacks[selectedStack].getNode(0) == nullptr) || (
                            stacks[selectedStack].getNode(0)->item > selectedDisk)) {
                        stacks[selectedStack].pushFront(selectedDisk);
                        selectedDisk = 0;
                    }
                }

                break;
            default: ;
        }
        refresh();
        draw(count, stacks, selectedStack, selectedDisk);
    }

    endwin();

    return 0;
}

void draw(int diskCount, List<int> stacks[3], int selectedStack, int selectedDisk) {
    if (selectedStack > 2) {
        selectedStack = 2;
    }
    if (selectedDisk > diskCount) {
        selectedDisk = diskCount;
    }

    clear();

    drawIndicator(selectedStack, selectedDisk, diskCount);
    for (int i = 0; i < 3; i++) {
        drawStack(diskCount, &stacks[i], i);
    }

    move(0, 0);
    refresh();
}

void drawIndicator(int selectedStack, int selectedDisk, int diskCount) {
    const int beginPos = (diskCount * 2 + 4) * selectedStack;
    // const int center = beginPos + diskCount + 2;

    //draw indicator
    move(0, beginPos);
    printw("<");
    for (int i = beginPos; i < beginPos + diskCount * 2 + 1; i++) {
        printw("-");
    }
    printw(">");

    //draw selected disk

    move(1, beginPos + 1);
    for (int i = 0; i < diskCount - selectedDisk; i++) {
        printw(" ");
    }
    for (int i = 0; i < selectedDisk; i++) {
        printw("X");
    }
    printw(" ");
    for (int i = 0; i < selectedDisk; i++) {
        printw("X");
    }
    for (int i = 0; i < diskCount - selectedDisk; i++) {
        printw(" ");
    }
}

void drawStack(int diskCount, List<int> *stack, int selectedStack) {
    const int beginPos = (diskCount * 2 + 4) * selectedStack + 1;
    const int center = beginPos + diskCount;

    Node<int> *disk = stack->getNode(-1);

    for (int i = 0; i < diskCount; i++) {
        move(diskCount + 2 - i, beginPos);

        if (disk == nullptr) {
            mvprintw(diskCount + 2 - i, center, "|");
        } else {
            for (int j = 0; j < diskCount - disk->item; j++) {
                printw(" ");
            }
            for (int j = 0; j < disk->item; j++) {
                printw("X");
            }
            printw("|");
            for (int j = 0; j < disk->item; j++) {
                printw("X");
            }
            for (int j = 0; j < diskCount - disk->item; j++) {
                printw(" ");
            }
            disk = List<int>::getPreviousNode(disk);
        }
    }
}
