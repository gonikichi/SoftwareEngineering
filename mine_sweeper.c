/** @file mine_sweeper.c
 *  @brief マインスイーパ 実現ファイル
 *  @brief マクロや構造体の定義は"mine_sweeper.h"を参照してください
 *  @date 2021/8/10
 *  @author Shonosuke Gonda
 *   
 */

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include "mine_sweeper.h"

/**
 * @brief 0から引数までの値の内でランダムな値を返却します
 * 
 * @param max_value 
 * @return int 
 */
int GetRandom(int max_value) {
    return rand() % (max_value + 1);
}

/**
 * @brief OneBlock配列を引数に取り、標準出力としてステージを描画します
 *
 * 引数:
 * -OneBlock[9][9]の様に、構造体の集合を与える様にして下さい
 * @param stage_blocks 
 */
void StageDisplay(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN]) {
    printf("  １２３４５６７８９\n");
    printf("  ーーーーーーーーー\n");
    for (int i = 0; i < STAGE_ROW; i++) {
        printf("%d|", i + 1);
        for (int j = 0; j < STAGE_COLUMN; j++) {
            //1:ブロックが判明していない時
            if (stage_blocks[i][j].state_of_block == FALSE) {
                printf("・");
            }
            //2:ブロックが判明していない時
            else if (stage_blocks[i][j].state_of_block == PIN) {
                printf("🚩");
            }
            //3:ブロックが判明している時
            else {
                switch (stage_blocks[i][j].kind_of_block) {
                case ZERO:
                    printf("０");
                    break;
                case ONE:
                    printf("１");
                    break;
                case TWO:
                    printf("２");
                    break;
                case THREE:
                    printf("３");
                    break;
                case FOUR:
                    printf("４");
                    break;
                case FIVE:
                    printf("５");
                    break;
                case SIX:
                    printf("６");
                    break;
                case SEVEN:
                    printf("７");
                    break;
                case EIGHT:
                    printf("８");
                    break;
                case BOM:
                    printf("💣");
                    break;
                }
            }
        }
        puts("");
    }
}

/**
 * @brief 再帰的に0のマスを開いてゆく関数です。
 * 引数:
 * -OneBlock配列を受取ります
 * -開きたいブロックの座標を(row,column)で指定します
 * -指定したブロックがZEROの性質を示す場合、
 *  そのブロックをTRUE(公開状態)にします
 * -指定したブロックがZEROの性質を示す場合、
 *  その周囲8ブロックに対しても同様の処理を行います
 * @param stage_blocks 
 * @param row 
 * @param column 
 */
void OpenZeroRecursively(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
                         int row, int column) {
    if (stage_blocks[row][column].kind_of_block == ZERO &&
        stage_blocks[row][column].state_of_block == FALSE) {
        stage_blocks[row][column].state_of_block = TRUE;
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = column - 1; j <= column + 1; j++) {
                if (i > -1 && i < STAGE_ROW && j > -1 && j < STAGE_COLUMN) {
                    if (stage_blocks[i][j].kind_of_block == ZERO) {
                        OpenZeroRecursively(stage_blocks, i, j);
                    }
                }
            }
        }
    }
}

/**
 * @brief ZEROの性質を表すブロックの周囲8マスを開く関数です
 * 引数:
 * -OneBlock配列を受取ります
 * -OneBlock配列の中で(TRUE)公開状態かつZEROを表すブロックに処理を行います
 * -その周囲8マスを(TRUE)公開状態に変更します
 * @param stage_blocks 
 */
void OpenAroundZero(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN]) {
    for (int i = 0; i < STAGE_ROW; i++) {
        for (int j = 0; j < STAGE_COLUMN; j++) {
            if (stage_blocks[i][j].state_of_block == TRUE &&
                stage_blocks[i][j].kind_of_block == ZERO) {
                for (int k = i - 1; k < i + 2; k++) {
                    for (int l = j - 1; l < j + 2; l++) {
                        if (-1 < k && k < STAGE_ROW &&
                            -1 < l && l < STAGE_COLUMN) {
                            if (stage_blocks[k][l].state_of_block == FALSE) {
                                stage_blocks[k][l].state_of_block = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief ステージをランダム生成する関数です
 * 引数:
 * -初期化を行った直後のOneBlock配列を与えます
 * -最初に開くマスの座標(start_row,staet-column)を与えます
 * -GetRandom()を用いて爆弾の位置をランダムに決定します
 * -初期スタート位置の周囲8ブロックには爆弾が来ない様に設定してあります
 * -爆弾を配置し終えたら、全てのマスの性質を定めます
 * -最後にスタートマスのオープン処理を行います
 * @param stage_blocks 
 * @param start_row 
 * @param start_column 
 */
void MakeStage(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
               int start_row, int start_column) {
    int num_of_bom = 0;
    int memo_where_is_bom[NUM_OF_BOM][2] = {};
    //1ボムの配置
    while (num_of_bom < NUM_OF_BOM) {
        while (1) {
            //ボム再検索の条件
            //・以前に指定したボムと位置が被る時
            //・スタート位置の周囲8ブロックと位置が被る時
            int flag_undesireble__bom_location = 0;
            int candidate_new_bom_row = GetRandom(8);
            int candidate_new_bom_column = GetRandom(8);
            if ((start_row - 2 < candidate_new_bom_row &&
                 candidate_new_bom_row < start_row + 2) &&
                (start_column - 2 < candidate_new_bom_column &&
                 candidate_new_bom_column < start_column + 2)) {
                flag_undesireble__bom_location = 1;
            }
            for (int i = 0; i < NUM_OF_BOM; i++) {
                if (candidate_new_bom_row == memo_where_is_bom[i][0] &&
                    candidate_new_bom_column == memo_where_is_bom[i][1]) {
                    flag_undesireble__bom_location = 1;
                }
            }
            if (flag_undesireble__bom_location == 0) {
                memo_where_is_bom[num_of_bom][0] = candidate_new_bom_row;
                memo_where_is_bom[num_of_bom][1] = candidate_new_bom_column;
                num_of_bom += 1;
                stage_blocks[candidate_new_bom_row][candidate_new_bom_column].
                state_of_block = FALSE;
                stage_blocks[candidate_new_bom_row][candidate_new_bom_column].
                kind_of_block = BOM;
                break;
            }
        }
    }
    //2各ブロックに番号を割り振る
    for (int k = 0; k < STAGE_ROW; k++) {
        for (int l = 0; l < STAGE_COLUMN; l++) {
            if (stage_blocks[k][l].kind_of_block != BOM) {
                int block_number = 0;
                for (int m = k - 1; m <= k + 1; m++) {
                    for (int n = l - 1; n <= l + 1; n++) {
                        for (int o = 0; o < NUM_OF_BOM; o++) {
                            if ((memo_where_is_bom[o][0] == m) && 
                                (memo_where_is_bom[o][1] == n)) {
                                block_number += 1;
                            }
                        }
                    }
                }
                stage_blocks[k][l].state_of_block = FALSE;
                stage_blocks[k][l].kind_of_block = block_number;
            }
        }
    }
    //3OPEN処理をする(OpenZeroRecursively関数を呼び出す&OpenAroundZeroでオープン処理)
    OpenZeroRecursively(stage_blocks, start_row, start_column);
    OpenAroundZero(stage_blocks);
}

/**
 * @brief ブロックを開く為の関数です
 * 引数:
 * -OneBlock配列と開きたいブロックの座標(dig_row,dig_column)が引数
 * @param stage_blocks 
 * @param dig_row 
 * @param dig_column 
 */
void OpenBlocks(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
                int dig_row, int dig_column) {
    if (stage_blocks[dig_row][dig_column].state_of_block == TRUE) {
        printf("そのマスはもう空いているよ\n");
    } else {
        OpenZeroRecursively(stage_blocks, dig_row, dig_column);
        OpenAroundZero(stage_blocks);
        stage_blocks[dig_row][dig_column].state_of_block = TRUE;
        printf("%d行%d列をオープンしました\n", dig_row + 1, dig_column + 1);
    }
}

/**
 * @brief フラッグ🚩を立てる関数
 * 引数:
 * -OneBlock配列と旗を立てるブロックの座標(flag_row,flag_column)が引数
 * @param stage_blocks 
 * @param flag_row 
 * @param flag_column 
 */
void RaiseFlag(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
               int flag_row, int flag_column) {
    if (stage_blocks[flag_row][flag_column].state_of_block == TRUE) {
        printf("そこに旗をたてることは出来ません\n");
    } else if (stage_blocks[flag_row][flag_column].state_of_block == PIN) {
        printf("旗を回収しました\n");
        stage_blocks[flag_row][flag_column].state_of_block = FALSE;
    } else {
        stage_blocks[flag_row][flag_column].state_of_block = PIN;
        printf("%d行%d列に旗を立てました\n", flag_row + 1, flag_column + 1);
    }
}

/**
 * @brief ゲーム終了条件を満たしているかどうかを判別する関数です
 * 引数:
 * -OneBlock配列と開いたブロックの座標(row,column)が引数
 * 返り値:
 * 1  ゲームクリアを表す
 * 0　ゲームオーバーを表す
 * -1　どの判定にも引っかからない(ゲーム継続)
 * @param stage_blocks 
 * @param row 
 * @param column 
 * @return int 
 */
int JudgeGameClearOrOver(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN],
                        int row, int column) {
    int num_of_unknown_block = 0;
    if (stage_blocks[row][column].kind_of_block == BOM && 
        stage_blocks[row][column].state_of_block == TRUE) {
        printf("爆弾を掘り当ててしまいました!\nGAME OVER\n");
        return -1;
    } else {
        for (int k = 0; k < STAGE_ROW; k++) {
            for (int l = 0; l < STAGE_COLUMN; l++) {
                if (stage_blocks[k][l].state_of_block == TRUE) {
                    num_of_unknown_block += 1;
                }
            }
        }
        if (num_of_unknown_block == (STAGE_ROW * STAGE_COLUMN - NUM_OF_BOM)) {
            printf("ゲームクリアです!おめでとう!\n");
            return 1;
        } else {
            return 0;
        }
    }
}

/**
 * @brief タイトル画面を表示し続ける関数
 * 
 * 入力:
 * -ユーザにゲームを終了するか継続するかを入力させる
 * 返り値:
 * 1   ゲーム開始
 * -1  ゲーム終了
 * @return int 
 */
int TytleControl(void) {
    int user_input = 0;
    printf("🚩💣マインスイーパー💣🚩\n");
    while (user_input < 1 || user_input > 2) {
        printf("1ゲーム開始,2ゲーム終了:");
        scanf("%d", & user_input);
    }
    switch (user_input) {
    case 1:
        return 1;
        break;
    case 2:
        return -1;
        break;
    }
}

/**
 * @brief マインスイーパのゲームを遊ぶことが出来る関数
 * 
 */
void Minesweeper(void) {
    int key_input = TytleControl();
    while (key_input != -1) {
        int row = -1;
        int column = -1;
        int what_to_do_next = -1;
        int flag_while_roop = -1;
        OneBlock blocks[STAGE_ROW][STAGE_COLUMN] = {};
        StageDisplay(blocks);
        while (row < 1 || row > STAGE_ROW) {
            printf("開始位置を指定しましょう 行:");
            scanf("%d", & row);
        }
        while (column < 1 || column > STAGE_COLUMN) {
            printf("開始位置を指定しましょう 列:");
            scanf("%d", & column);
        }
        MakeStage(blocks, row - 1, column - 1);
        StageDisplay(blocks);
        while (1) {
            while (flag_while_roop != 0) {
                what_to_do_next = -1;
                while (what_to_do_next < 1 || what_to_do_next > 2) {
                    printf("穴を掘る:1,旗を立てる2:");
                    scanf("%d", & what_to_do_next);
                }
                row = -1;
                while (row < 1 || row > STAGE_ROW) {
                    printf("1:位置を指定しましょう 行:");
                    scanf("%d", & row);
                }
                column = -1;
                while (column < 1 || column > STAGE_COLUMN) {
                    printf("2:位置を指定しましょう 列:");
                    scanf("%d", & column);
                }
                switch (what_to_do_next) {
                case 1:
                    printf("%d行%d列に穴を掘りますyes...0 no...1:", row, column);
                    scanf("%d", & flag_while_roop);
                    break;
                case 2:
                    printf("%d行%d列に旗をたてますyes...0 no...1:", row, column);
                    scanf("%d", & flag_while_roop);
                    break;
                }
            }
            flag_while_roop = -1;
            switch (what_to_do_next) {
            case 1:
                OpenBlocks(blocks, row - 1, column - 1);
                break;
            case 2:
                RaiseFlag(blocks, row - 1, column - 1);
                break;
            }
            StageDisplay(blocks);
            int flag_for_switch = JudgeGameClearOrOver(blocks, row - 1,
                                                      column - 1);
            switch (flag_for_switch) {
            case 0:
                break;
            case -1:
            case 1:
                printf("再挑戦0,終了-1:");
                scanf("%d", & key_input);
            }
            if (flag_for_switch == -1 || flag_for_switch == 1) break;
        }
    }
}
