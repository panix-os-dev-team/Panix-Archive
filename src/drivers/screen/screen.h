/**
 * File: screen.h
 * Author: Keeton Feavel
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "../../cpu/ports.h"
#include "../../libc/memory.h"
#include "../driver.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLUMNS 80
// TODO: Move to separate 'colors.h' file like in edix_v1
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4
#define YELLOW_ON_BLACK 0x0e

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

class Screen : public Driver {
    public:
        /**
         * @brief Clears the screen
         * 
         */
        static void clearScreen();

        /**
         * @brief Prints a string at current cursor position
         * 
         * @param message 
         */
        static void kprint(char* message);

        /**
         * @brief Prints a string at specified position
         * 
         * @param message String to print
         * @param column Column to start printing at, a value of -1 will print at cursor position
         * @param row Row to start printing at, a value of -1 will print at cursor position
         */
        static void kprintAtPosition(char* message, int column = -1, int row = -1);

        /**
         * @brief Moves the cursor back one column and removes last printed character
         * 
         */
        static void kprintBackspace();
    
    private:
        /**
         * @brief Get the cursor offset in video memory
         * 
         * @return int 
         */
        static int getCursorOffset();

        /**
         * @brief Get the offset of a specified position in video memory
         * 
         * @param column 
         * @param row 
         * @return int 
         */
        static int getOffset(int column, int row);

        /**
         * @brief Get the offset of a specified row in video memory
         * 
         * @param offset 
         * @return int 
         */
        static int getOffsetRow(int offset);

        /**
         * @brief Get the offset of a specified column in video memory
         * 
         * @param offset 
         * @return int 
         */
        static int getOffsetColumn(int offset);

        /**
         * @brief Print a single character at the specified position
         * 
         * @param c Character to print
         * @param column Column to print at, a value of -1 will print at cursor position
         * @param row Row to print at, a value of -1 will print at cursor position
         * @param color Color to print the character
         * @return int 
         */
        static int printCharacter(char c, int column = -1, int row = -1, char color = WHITE_ON_BLACK);

        /**
         * @brief Set the Cursor Offset object
         * 
         * @param offset u
         */
        static void setCursorOffset(int offset);
};

#endif /* SCREEN_H */