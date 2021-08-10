/** @file mine_sweeper.h
 *  @brief マインスイーパ 公開ヘッダ
 *  @brief 関数の定義は"mine_sweeper.c"を参照してください
 *  @date 2021/8/10
 *  @author Shonosuke Gonda
 *   
 */

#ifndef _MINE_SWEEPER
#define _MINE_SWEEPER

#define NUM_OF_BOM 11
#define STAGE_ROW 9
#define STAGE_COLUMN 9

/**
 * @brief ステージ1ブロックの性質を表す列挙体です
 * 爆弾ブロック以外のブロックは自分の周囲8ブロックに
 * 存在する爆弾の数が性質となります
 */
enum BlockKind {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    BOM,
    Invalid
};

/**
 * @brief 1つのブロックが公開状態か非公開状態を示します
 * 値がTRUEの時は公開状態
 * 値がFALSE.PINの時は非公開状態
 * FALSEは・、PINは🚩を表します
 */
enum BlockState {
    FALSE,
    TRUE,
    PIN
};

/**
 * @brief 1つのブロックの性質と状態を管理します
 * 
 */
typedef struct {
    enum BlockKind kind_of_block;
    enum BlockState state_of_block;
}
OneBlock;

/*-----関数原型宣言-----*/
int GetRandam(int max_value);
void StageDisplay(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN]);
void OpenZeroRecursively(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
                         int row, int column);
void OpenAroundZero(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN]);
void MakeStage(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN],
               int start_row, int start_column);
void OpenBlocks(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
                int dig_row, int dig_column);
void RaiseFlag(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN], 
               int flag_row, int flag_column);
int JudgeGameClearOrOver(OneBlock stage_blocks[STAGE_ROW][STAGE_COLUMN],
                        int row, int column);
int TytleControl(void);
void Minesweeper(void);
#endif