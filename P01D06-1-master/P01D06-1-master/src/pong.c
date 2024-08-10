#include <stdio.h>

int input(int* y_r1, int* y_r2);
void cleanbuf();
void phys(int* pad_player_left, int* pad_player_rigth, int* bx, int* by, int* bvx, int* bvy, int* score_1,
          int* score_2);
void draw_digit(int digit, int row);
void draw_score(int score1, int score2, int field_length);
void render(int r1, int r2, int b_x, int b_y, int field_length, int field_height, int score1, int score2);

int main(void) {
    int field_length = 80;
    int field_height = 25;
    int x = 40;
    int y = 13;
    int v_x = 1;
    int v_y = 1;
    int player_left_score = 0;
    int player_right_score = 0;
    int pad_player_left = 13;
    int pad_player_right = 13;
    int esc_flag = 0;  // 1 if esc pressed

    render(pad_player_left, pad_player_right, x, y, field_length, field_height, player_left_score,
           player_right_score);

    while (esc_flag != -1 && player_left_score < 21 && player_right_score < 21) {
        esc_flag = input(&pad_player_left, &pad_player_right);
        if (esc_flag == 1) {
            phys(&pad_player_left, &pad_player_right, &x, &y, &v_x, &v_y, &player_left_score,
                 &player_right_score);
            render(pad_player_left, pad_player_right, x, y, field_length, field_height, player_left_score,
                   player_right_score);

        } else {
            continue;
        }
    }

    printf("\033[2J");
    printf("GAME OVER\n");
    printf("Player 1: %d\n", player_left_score);
    printf("Player 2: %d\n", player_right_score);
    printf("THANKS FOR PLAYING PING-PONG\n");

    return 0;
}

void cleanbuf() {
    while ((getchar()) != '\n') {
        continue;
    }
    return;
}

int input(int* y_r1, int* y_r2) {
    char inputChar;  // Считываем символ
    if ((inputChar = getchar()) != '\n') {
        cleanbuf();
        if (inputChar == 'Z' || inputChar == 'z') {
            if (*y_r1 < 24) {  //проверка левой ракетки в верхнй точке
                *y_r1 += 1;
            }
            return 1;
        }
        if (inputChar == 'A' || inputChar == 'a') {
            if (*y_r1 > 2) {  //проверка левой ракетки в нижней точке
                *y_r1 -= 1;
            }
            return 1;
        }
        if (inputChar == 'M' || inputChar == 'm') {
            if (*y_r2 < 24) {  //проверка правой ракетки в верхнй точке
                *y_r2 += 1;
            }
            return 1;
        }
        if (inputChar == 'K' || inputChar == 'k') {
            if (*y_r2 > 2) {  //проверка правой ракетки в нижней точке
                *y_r2 -= 1;
            }
            return 1;
        }
        if (inputChar == ' ') {
            return 1;
        }
        if (inputChar == '\033') {
            return -1;
        }
        return 0;
    }

    return 0;
}

void phys(int* pad_player_left, int* pad_player_right, int* bx, int* by, int* bvx, int* bvy, int* score_1,
          int* score_2) {
    if (*bx <= 2) {
        if ((*pad_player_left - 1) <= *by && (*pad_player_left + 1) >= *by) {
            *bvx = -(*bvx);
            *bx = 3;
        } else {
            *score_2 += 1;
            *bx = 40;
            *by = 12;
            *pad_player_right = 13;
            *pad_player_left = 13;
        }
    }
    if (*bx >= 79) {
        if ((*pad_player_right - 1) <= *by && (*pad_player_right + 1) >= *by) {
            *bvx = -*bvx;
            *bx = 78;
        } else {
            *score_1 += 1;
            *bx = 40;
            *by = 12;
            *pad_player_right = 13;
            *pad_player_left = 13;
        }
    }
    if (*by <= 1) {
        *bvy = -*bvy;
        *by = 1;
    }
    if (*by >= 25) {
        *bvy = -*bvy;
        *by = 25;
    }
    *bx = *bx + *bvx;
    *by = *by + *bvy;
}

void render(int r1, int r2, int b_x, int b_y, int field_length, int field_height, int score1, int score2) {
    int r1_d = r1;
    int r2_d = r2;
    int b_x_d = b_x;
    int b_y_d = b_y;
    printf("\033[2J");
    draw_score(score1, score2, field_length);
    for (int i = 0; i <= field_length; i++) {
        printf("-");
    }
    printf("\n");

    // поле
    for (int j = 1; j <= field_height; j++) {
        for (int i = 0; i <= field_length + 1; i++) {
            if (i == 0) {
                printf(">");
            } else if (i == field_length + 1) {
                printf("<");
            } else {
                if (b_y_d == j && b_x_d == i) {  // позиция мяча
                    printf("\033[1;33m*\033[0m");
                } else if (i == 1 && (j <= r1_d + 1 && j >= r1_d - 1)) {  // левая ракетка
                    printf("\033[1;31m|\033[0m");
                } else if (i == field_length && (j <= r2_d + 1 && j >= r2_d - 1)) {  //правая ракетка
                    printf("\033[1;34m|\033[0m");
                } else if (i == 39 || i == 40) {
                    printf(".");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    for (int i = 0; i <= field_length; i++) {
        printf("-");
    }
    printf("\n");
}

//  цифры
void draw_digit(int digit, int row) {
    if (digit == 0) {
        if (row == 0) {
            printf(" ### ");

        }

        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf("#   #");
        else if (row == 3)
            printf("#   #");
        else if (row == 4)
            printf(" ### ");
    } else if (digit == 1) {
        if (row == 0)
            printf("  #  ");
        else if (row == 1)
            printf(" ##  ");
        else if (row == 2)
            printf("  #  ");
        else if (row == 3)
            printf("  #  ");
        else if (row == 4)
            printf("#####");
    } else if (digit == 2) {
        if (row == 0)
            printf(" ### ");
        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf("   # ");
        else if (row == 3)
            printf("  #  ");
        else if (row == 4)
            printf("#####");
    } else if (digit == 3) {
        if (row == 0)
            printf(" ### ");
        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf("  ## ");
        else if (row == 3)
            printf("#   #");
        else if (row == 4)
            printf(" ### ");
    } else if (digit == 4) {
        if (row == 0)
            printf("#   #");
        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf("#####");
        else if (row == 3)
            printf("    #");
        else if (row == 4)
            printf("    #");
    } else if (digit == 5) {
        if (row == 0)
            printf("#####");
        else if (row == 1)
            printf("#    ");
        else if (row == 2)
            printf("#### ");
        else if (row == 3)
            printf("    #");
        else if (row == 4)
            printf("#### ");
    } else if (digit == 6) {
        if (row == 0)
            printf(" ### ");
        else if (row == 1)
            printf("#    ");
        else if (row == 2)
            printf("#### ");
        else if (row == 3)
            printf("#   #");
        else if (row == 4)
            printf(" ### ");
    } else if (digit == 7) {
        if (row == 0)
            printf("#####");
        else if (row == 1)
            printf("    #");
        else if (row == 2)
            printf("   # ");
        else if (row == 3)
            printf("  #  ");
        else if (row == 4)
            printf(" #   ");
    } else if (digit == 8) {
        if (row == 0)
            printf(" ### ");
        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf(" ### ");
        else if (row == 3)
            printf("#   #");
        else if (row == 4)
            printf(" ### ");
    } else if (digit == 9) {
        if (row == 0)
            printf(" ### ");
        else if (row == 1)
            printf("#   #");
        else if (row == 2)
            printf(" ####");
        else if (row == 3)
            printf("    #");
        else if (row == 4)
            printf(" ### ");
    }
}

// Отрисовка счёта
void draw_score(int score1, int score2, int field_length) {
    int center_offset = (field_length - 29) / 2;  // Смещение для центрирования счёта
    for (int row = 0; row < 5; row++) {
        for (int i = 1; i < center_offset; i++) {
            printf(" ");
        }
        draw_digit(score1 / 10, row);
        printf("   ");
        draw_digit(score1 % 10, row);
        printf("      ");
        draw_digit(score2 / 10, row);
        printf("   ");
        draw_digit(score2 % 10, row);
        for (int i = 0; i < center_offset - 1; i++) {
            printf(" ");
        }
        printf("\n");
    }
}

