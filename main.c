/** @file main.c
 *  @brief マインスイーパライブラリを読みこむテストファイル
 *  @date 2021/8/10
 *  @author Shonosuke Gonda
 *   
 */
#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include "mine_sweeper.h"

int main(void) {
    srand(time(0));
    Minesweeper();
    return 0;
}