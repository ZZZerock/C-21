#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void input_manual(int matrix[HEIGHT][WIDTH]);
void output(int matrix[HEIGHT][WIDTH]);
void change_frame(int matrix[HEIGHT][WIDTH], int result_matrix[HEIGHT][WIDTH]);
int count_neighbour(int matrix[HEIGHT][WIDTH], int i, int j);
void replace(int matrix[HEIGHT][WIDTH], int matrix2[HEIGHT][WIDTH]);
void game_over(int score);
int counter(int matrix[HEIGHT][WIDTH]);
int change_speed(char l, int speed);

int main() {
    int matrix[HEIGHT][WIDTH], flag, score;
    score = 0;
    flag = 0;
    initscr();
    nodelay(stdscr, TRUE);  // Для работы getch
    noecho();               // Отключает отображение ввода
    curs_set(FALSE);        // Скрывает курсор
    input_manual(matrix);
    if (freopen("/dev/tty", "r", stdin) != NULL) {  // Перенаправляет ввод к терминалу(/dev/tty)
        int result_matrix[HEIGHT][WIDTH], speed;
        speed = 100000;
        while (flag != 1) {
            char l = getch();
            if (l == 'q' || counter(matrix) == 0) {
                flag = 1;
                break;
            } else if (l == '1' || l == '2')
                speed = change_speed(l, speed);
            usleep(speed);
            clear();
            output(matrix);
            mvprintw(27, 32, "Generation Number %d", score);
            mvprintw(28, 32, "1 - decrease speed");
            mvprintw(29, 32, "2 - increase speed");
            mvprintw(30, 32, "q - quit");
            refresh();
            change_frame(matrix, result_matrix);
            replace(result_matrix, matrix);
            score++;
        }
    }
    endwin();
    game_over(score);
    return flag;
}

void input_manual(int matrix[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            while (1) {
                int res = scanf("%d", &matrix[i][j]);
                if (res == 1 && (matrix[i][j] == 0 || matrix[i][j] == 1)) break;
            }
        }
    }
}

void output(int matrix[HEIGHT][WIDTH]) {
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(2));
    for (int i = 0; i < HEIGHT + 1; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, 81, "|");
    }
    for (int j = 0; j < WIDTH + 2; j++) {
        mvprintw(0, j, "-");
        mvprintw(26, j, "-");
    }
    attron(COLOR_PAIR(1));
    for (int i = 1; i < HEIGHT + 1; i++) {
        for (int j = 1; j < WIDTH + 1; j++) {
            if (matrix[i - 1][j - 1] == 1)
                mvprintw(i, j, "#");  //Отрисовка живой клетки
            else if (matrix[i - 1][j - 1] == 0)
                mvprintw(i, j, " ");
        }
    }
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    refresh();
}

void change_frame(int matrix[HEIGHT][WIDTH], int result_matrix[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int sum = count_neighbour(matrix, i, j);
            if (matrix[i][j] == 0 && sum == 3)
                result_matrix[i][j] = 1;
            else if (matrix[i][j] == 1 && (sum == 3 || sum == 2))
                result_matrix[i][j] = 1;
            else
                result_matrix[i][j] = 0;
        }
    }
}

int count_neighbour(int matrix[HEIGHT][WIDTH], int i, int j) {
    int sum = 0;
    int row, col;

    // Перебираем соседние клетки
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            // Пропускаем саму клетку
            if (di == 0 && dj == 0) continue;
            // Вычисляем индексы соседей
            row = (i + di + HEIGHT) % HEIGHT;
            col = (j + dj + WIDTH) % WIDTH;
            sum += matrix[row][col];
        }
    }
    return sum;
}

void replace(int matrix[HEIGHT][WIDTH], int matrix2[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matrix2[i][j] = matrix[i][j];
        }
    }
}

void game_over(int score) {
    printf("\n");
    printf(
        "\033[31m*     ######      ###    ##     ## ########     #######  ##     ## ######## ########  \n");
    printf("*    ##    ##    ## ##   ###   ### ##          ##     ## ##     ## ##       ##     ## \n");
    printf("*    ##         ##   ##  #### #### ##          ##     ## ##     ## ##       ##     ## \n");
    printf("*    ##   #### ##     ## ## ### ## ######      ##     ## ##     ## ######   ########  \n");
    printf("*    ##    ##  ######### ##     ## ##          ##     ##  ##   ##  ##       ##   ##   \n");
    printf("*    ##    ##  ##     ## ##     ## ##          ##     ##   ## ##   ##       ##    ##  \n");
    printf("*     ######   ##     ## ##     ## ########     #######     ###    ######## ##     ## \n");
    printf("\n");
    printf("                                  Generation Number %d\033[0m\n", score);
}

int counter(int matrix[HEIGHT][WIDTH]) {
    int sum = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) sum += matrix[i][j];
    }
    return sum;
}

int change_speed(char l, int speed) {
    if (l == '1') {
        if (speed < 1000000) speed += 10000;
    } else if (l == '2') {
        if (speed > 10000) {
            speed -= 10000;
        }
    }
    return speed;
}


