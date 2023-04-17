//  gcc pong.c -o pong -lncurses
// ./pong
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define DELAY 40000
#define PAD_LEN 3
#define MARGIN 3


typedef struct {
    int x;      //Poisicon x
    int y;      //Poisicon x
    int x_dir;  //Direccion x
    int y_dir;  //Direccion y
} Ball;

typedef struct {
    int x;      //Posicion x
    int y;      //Posicion y    
    int length; //Longitud de la pala
    int score;  //Puntuacion
} Paddle;

typedef struct {
    int width;  // Ancho de la pantalla
    int height; // Alto de la pantalla
} Screen;

void paddleCollision(Paddle *paddle, Ball *ball) {
    // Comprobar colisión con la pala del jugador 1
    if (ball->x == paddle->x + 1 && ball->y >= paddle->y && ball->y < paddle->y + paddle->length) {
        ball->x_dir = 1;
        ball->y_dir = (ball->y - paddle->y) / (paddle->length / 2) - 1;
        ball->y += (paddle->y - ball->y) / paddle->length;
    }

    // Comprobar colisión con la pala del jugador 2
    if (ball->x == paddle->x - 1 && ball->y >= paddle->y && ball->y < paddle->y + paddle->length) {
        ball->x_dir = -1;
        ball->y_dir = (ball->y - paddle->y) / (paddle->length / 2) - 1;
        ball->y += (paddle->y - ball->y) / paddle->length;
    }
}

void init_Ball(Ball *b, int x, int y, int xdir, int ydir){
    b->x = x;
    b->y = y;
    b->x_dir = xdir;
    b->y_dir = ydir;
}

void init_Paddle(Paddle *p, int x, int y, int len, int sc){
    p->x = x;
    p->y = y;
    p->length = len;
    p->score = sc;
}

void reset_game(Screen *screen, Ball *ball, Paddle *p1,Paddle *p2){
    init_Ball(ball, screen->width / 2, screen->height / 2, 1, 1);
    init_Paddle(p1, MARGIN, screen->height / 2, PAD_LEN, 0);
    init_Paddle(p2, screen->width - MARGIN, screen->height / 2, PAD_LEN, 0);
}
bool menuFinal(WINDOW *w,Screen *screen, Paddle *p1,Paddle *p2){
    char cadPuntos[50];
    box(w,'$','$');
    int shift = screen->width/2 - 10;
    sprintf(cadPuntos, "%d\t -  %d", p1->score,p2->score);
    mvwprintw(w,screen->height/2-4, shift+10, cadPuntos);
    if(p1->score > p2->score)
	    mvwprintw(w,screen->height/2-2,shift,  " El ganador a sido el jugador1 ");
    else if(p1->score < p2->score)
        mvwprintw(w,screen->height/2-2,shift,  " El ganador a sido el jugador2 ");
    else
        mvwprintw(w,screen->height/2-2,shift,  " \t      Empate!");
	mvwprintw(w,screen->height/2,shift,  " Pulsa ENTER para jugar de nuevo ");  
	mvwprintw(w,screen->height/2+1,shift,  " Pulsa Q para salir del juego "); 

    switch (wgetch(w)){
        case KEY_ENTER:
            return true;
        case 'q':
        case 'Q':
            return false;
    }
}
int main(){
   
    bool terminar = false;

    Screen screen;
    
    initscr();
    noecho();
    curs_set(FALSE);

    //Colores para los diferentes objetos
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);     // Color de la puntuación
    init_pair(2, COLOR_CYAN, COLOR_BLACK);       // Color barra central
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);   // Color de la pelota
    init_pair(4, COLOR_RED, COLOR_RED);         // Color de las barras

    //Dimensiones de la pantalla
    getmaxyx(stdscr,  screen.height, screen.width);

    //Definicion de pelota
    Ball ball;
    init_Ball(&ball, screen.width / 2, screen.height / 2, 1, 1);

    //Definicion de palas y jugadores
    Paddle paddleP1, paddleP2;
    init_Paddle(&paddleP1, MARGIN, screen.height / 2, PAD_LEN, 0);
    init_Paddle(&paddleP2, screen.width - MARGIN, screen.height / 2, PAD_LEN, 0);
    
    //pantalla de inicio de juego
    WINDOW *init_screen = newwin(screen.height, screen.width, 0, 0);
    box(init_screen, '|', '-');
    int mid = screen.width/2 - 81/2;
    mvprintw(2,mid,  " .----------------.  .----------------.  .-----------------. .----------------. "); 
    mvprintw(3,mid,  "| .--------------. || .--------------. || .--------------. || .--------------. |");
    mvprintw(4,mid,  "| |   ______     | || |     ____     | || | ____  _____  | || |    ______    | |");
    mvprintw(5,mid,  "| |  |_   __ \\   | || |   .'    `.   | || ||_   \\|_   _| | || |  .' ___  |   | |");
    mvprintw(6,mid,  "| |    | |__) |  | || |  /  .--.  \\  | || |  |   \\ | |   | || | / .'   \\_|   | |");
    mvprintw(7,mid,  "| |    |  ___/   | || |  | |    | |  | || |  | |\\ \\| |   | || | | |    ____  | |");
    mvprintw(8,mid,  "| |   _| |_      | || |  \\  `--'  /  | || | _| |_\\   |_  | || | \\ `.___]  _| | |");
    mvprintw(9,mid,  "| |  |_____|     | || |   `.____.'   | || ||_____|\\____| | || |  `._____.'   | |");
    mvprintw(10,mid, "| |              | || |              | || |              | || |              | |");
    mvprintw(11,mid, "| '--------------' || '--------------' || '--------------' || '--------------' |");
    mvprintw(12,mid, " '----------------'  '----------------'  '----------------'  '----------------' ");

    //Controles de juego
            
    mvprintw(15, mid , " .----------------------------------CONTROLES---------------------------------. ");
    mvprintw(16, 55, "Player 1");
    mvprintw(17, 53.5, "UP   -> W");
    mvprintw(18, 53.5, "DOWN -> S");

    mvprintw(16, 105, "Player 2");
    mvprintw(17, 103.5, "UP   -> ^ (flecha arriba)");
    mvprintw(18, 103.5, "DOWN -> v (flecha abajo)");

    mvprintw(screen.height-6, 90, "Realizado por: ");
    mvprintw(screen.height-5, 90, "Jose Luis Molina Aguilar");
    mvprintw(screen.height-4 , 90, "Sergio España Maldonado");
    
    //Añadir lo que quieras
    getch();
   

    //Pulsamos cualquier tecla para continuar.
    

    //Definimos la pantalla donde se jugara
    WINDOW *game_screen = newwin(screen.height, screen.width, 0, 0); 

    int next_x = 0;
    int directionx = 1;
    int next_y = 0;
    int directiony = 1;
    
    keypad(game_screen, true);  //Captar teclas del teclado numerico

    while (!terminar){
        char cadPuntos[50];         // Cadena donde escribimos los puntos

       
        while(paddleP1.score < 5 && paddleP2.score < 5 && !terminar) {
            
            //Pintamos los bordes
            wattron(game_screen, COLOR_PAIR(2));
            box(game_screen, '|', '-');
            wattroff(game_screen, COLOR_PAIR(2));
            
            //Pintamos la pelota
            wattron(game_screen, COLOR_PAIR(3));
            mvwprintw(game_screen,ball.y, ball.x, "o");
            wattroff(game_screen, COLOR_PAIR(3));

            //Puntuacion
            sprintf(cadPuntos, "%d\t%d", paddleP1.score,paddleP2.score);
            mvwprintw(game_screen,1, screen.width / 2 - 3, cadPuntos);

            //Linea central divisoria
            mvwvline(game_screen, 1, screen.width / 2, ACS_VLINE, screen.height-2);
            
            //Linea horizontal
            //mvwhline(game_screen, screen.height / 2, 2, ACS_HLINE, screen.width-2);
            
            //Mover el conjunto de la pala
            wattron(game_screen, COLOR_PAIR(4));
            for (int i = 0; i < PAD_LEN ; i++){
                mvwprintw(game_screen, paddleP1.y + i, paddleP1.x, "|");
                mvwprintw(game_screen, paddleP2.y + i, paddleP2.x, "|");
            }
            wattroff(game_screen, COLOR_PAIR(4));
            
            //Recoger pulsacion de teclas
            nodelay(game_screen, true); 
            switch (wgetch(game_screen)){
                case 'q':
                case 'Q':
                    
                    terminar = true;
                    //werase(game_screen);
                    
                    break;

                case 'w':
                case 'W':
                    if(paddleP1.y > 1){
                        
                        paddleP1.y--;
                    }
                    break;
                case 's':
                case 'S':
                    if(paddleP1.y < screen.height - 2)
                        paddleP1.y++;
                    break;
                case KEY_UP:
                    if(paddleP2.y > 1)
                        paddleP2.y--;
                    break;

                case KEY_DOWN:
                    if(paddleP2.y < screen.height - 2)
                        paddleP2.y++;
                    break;

            }
            
            usleep(DELAY);

            next_x = ball.x + ball.x_dir;
            next_y = ball.y + ball.y_dir;

            paddleCollision(&paddleP1, &ball);
            paddleCollision(&paddleP2, &ball);

            if ( next_x >= screen.width){
                paddleP1.score++;
                ball.x = screen.width / 2;
                ball.y = screen.height / 2; 
            }

            if ( next_x < 0){
                paddleP2.score++;
                ball.x = screen.width / 2;
                ball.y = screen.height / 2;
            }

            if (next_x >= screen.width || next_x < 0) {
                ball.x_dir*= -1;
            } else {
                ball.x+= ball.x_dir;
            }

            if (next_y >= screen.height -1 || next_y < 1) {
                ball.y_dir*= -1;
            } else {
                ball.y+= ball.y_dir;
            }
            
            werase(game_screen);
        }
    
        WINDOW *end_screen = newwin(screen.height, screen.width, 0, 0); 
        terminar = menuFinal(end_screen,&screen ,&paddleP1,&paddleP2);
        if(terminar) reset_game(&screen, &ball, &paddleP1, &paddleP2);
        else break;
        wrefresh(end_screen);
        
    }
    endwin();
    return 0;

}