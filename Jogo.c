#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <pthread.h>

//BUGS
//01. Quando muitas bombas são disparadas juntas, eventualmente alguma atravessa uma nave e não explode (Já há mutexes para impedir que isso ocorra, mas continua ocorrendo)
//02. As naves estão se sobrepondo na tela
//03. A fabricação de bombas está funcionando conforme está no enunciado, mas foi implementada de uma forma mais simples do que o professor deseja. Não há um sistema de buffer conforme ele explicou em aula.


//FALTA IMPLEMENTAR
//01. Escolha de dificuldade do jogo. Aqui no código você mudar a variável speed_game para alterar a velocidade em que as naves se movimentam na tela.
//02. A questão de ter um tempo até abater todas as naves
//03. Finalizar o jogo ao acabar o tempo ou quando o jogador eliminar todas as naves

int x;
int bomba_y = -1;
int colisao_y = -10;
int colisao = 0;
int speed_game = 100000/2;

int qtd_bombas = 6;
int nro_bomba = 21;
int naves = 0;

pthread_mutex_t gotoxy_mutex;
pthread_mutex_t bomba_y_mutex;
pthread_mutex_t bomba_mutex;
pthread_mutex_t colisao_mutex;

// Funcao gotoxy
void gotoxy(int x, int y)
{
	pthread_mutex_lock(&gotoxy_mutex);
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
	pthread_mutex_unlock(&gotoxy_mutex);
}
void bomba_horizontal(int x, int y)
{
	gotoxy(x, y);
	printf("<");
	gotoxy(x + 1, y);
	printf("___");
}
void apaga_bomba_horizontal(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
	gotoxy(x + 1, y);
	printf("   ");
}
void bomba(int x, int y)
{
	gotoxy(x, y);
	printf("/\\");
	gotoxy(x, y + 1);
	printf("||");
	gotoxy(x - 1, y + 2);
	printf("/||\\");
}
void apaga_bomba(int x, int y)
{
	gotoxy(x, y);
	printf("  ");
	gotoxy(x, y + 1);
	printf("  ");
	gotoxy(x - 1, y + 2);
	printf("    ");
}
void explode_bomba(int x, int y)
{
	gotoxy(x, y);
	printf("*");
	usleep(speed_game);
	printf(" ");
	gotoxy(x, y - 1);
	printf("O");
	gotoxy(x - 1, y);
	printf("O O");
	gotoxy(x, y + 1);
	printf("O");
	usleep(speed_game);
	gotoxy(x, y - 1);
	printf(" ");
	gotoxy(x - 1, y);
	printf("   ");
	gotoxy(x, y + 1);
	printf(" ");

	gotoxy(x, y - 2);
	printf("o");
	gotoxy(x - 2, y);
	printf("o   o");
	gotoxy(x, y + 2);
	printf("o");
	usleep(speed_game);
	gotoxy(x, y - 2);
	printf(" ");
	gotoxy(x - 2, y);
	printf("     ");
	gotoxy(x, y + 2);
	printf(" ");
}
void nave(int x, int y)
{	
	gotoxy(x, y);
	printf("/");
	gotoxy(x + 1, y - 1);
	printf("___");
	gotoxy(x + 4, y);
	printf("\\");
	gotoxy(x, y + 1);
	printf("v-V-v");
	gotoxy(x, y + 1);
	printf("V-v-V");
}
void apaga_nave(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
	gotoxy(x + 1, y - 1);
	printf("   ");
	gotoxy(x + 4, y);
	printf(" ");
	gotoxy(x, y + 1);
	printf("     ");
	gotoxy(x, y + 1);
	printf("     ");
}
void canhao(int x, int y)
{
	gotoxy(x, y);
	printf("+-+");
	gotoxy(x, y + 1);
	printf("| |");
	gotoxy(x, y + 2);
	printf("| |");
	gotoxy(x, y + 3);
	printf("+-+");
}

int houve_colisao(int nave_x, int nave_y)
{
	pthread_mutex_lock(&colisao_mutex);
	pthread_mutex_lock(&bomba_y_mutex);
	if (bomba_y == nave_y && abs(nave_x - 40) <= 3)
	{
		pthread_mutex_unlock(&colisao_mutex);
		colisao = 1;
		colisao_y = bomba_y;
		pthread_mutex_unlock(&bomba_y_mutex);
		return 1;
	}
	pthread_mutex_unlock(&colisao_mutex);
	pthread_mutex_unlock(&bomba_y_mutex);
	return 0;
}

void *fabrica_bomba_func(void *arg)
{
	while (true)
	{
		if (qtd_bombas < 6)
		{	
			usleep(speed_game*15);
			bomba_horizontal(43, nro_bomba-1);
			nro_bomba -= 1;
			qtd_bombas += 1;
		}
	}
}

void *bomba_func(void *arg)
{
	qtd_bombas -= 1;
	apaga_bomba_horizontal(43, nro_bomba);
	nro_bomba += 1;
	for (int y = 20; y >= 0; y--)
	{
		bomba(40, y);
		usleep(speed_game);
		apaga_bomba(40, y);
		usleep(speed_game);
		pthread_mutex_lock(&bomba_y_mutex);
		bomba_y = y;
		pthread_mutex_unlock(&bomba_y_mutex);
		if (colisao == 1 && colisao_y >= y - 1 && colisao_y <= y + 1)
		{
			pthread_mutex_lock(&colisao_mutex);
			colisao = 0;
			pthread_mutex_unlock(&colisao_mutex);
			pthread_exit(NULL);
		}
	}
	apaga_bomba(40, 1);
	usleep(speed_game);
}

void *nave_func(void *arg)
{
	int linha;
	linha = 2 + (rand() % 5);
	while (1)
	{
		for (int x = 79; x >= 0; x--)
		{
			nave(x, linha);
			usleep(speed_game);
			apaga_nave(x, linha);
			if (houve_colisao(x, linha))
			{
				explode_bomba(x, linha);
				naves -= 1;
				pthread_exit(NULL);
			}
		}
		usleep(rand() % 1000000 + 100000);
	}
}

void *criar_naves_func(void *arg)
{
	int i;
	for (i = 0; i < 20; i++)
	{
		pthread_t nave;
		pthread_create(&nave, NULL, nave_func, NULL);
		naves += 1;
		usleep(rand() % 2000000 + 200000);
	}
}

void *game_over_func(void *arg){
	while(true){
		int naveslast = -1;
		if (naves != naveslast){
			gotoxy(2, 26);
			printf("Naves: %02d", naves);
			naveslast = naves;
			usleep(1000000);
		}

	}
}

int main()
{
	char montanhas[] = {10, 17, 14, 18, 16, 19, 12, 18, 13, 24, 10, 17, 14, 18, 16, 19, 12, 18, 13, 24, 10, 17, 14, 18, 16, 19, 12, 18, 13, 24, 10, 17, 14, 18, 16, 19, 12, 18, 13, 24};
	int coluna = 5;
	int linha = 3;
	int k = 0;
	system("clear");

	initscr();
	keypad(stdscr, TRUE);
	noecho();

	for (int i = 0; i < 18;)
	{
		for (int j = montanhas[i]; j <= montanhas[i + 1]; j++)
		{
			gotoxy(k, j); // printf("%i %i\n", k, j);//
			printf("\\");
			k++;
		}

		for (int j = montanhas[i]; j >= montanhas[i + 1]; j--)
		{
			gotoxy(k, j); // printf("%i %i\n", k, j);//
			printf("/");
			k++;
		}
		i++;
	}
	// Providing a seed value
	srand(time(NULL));
	k = 21;

	canhao(40, 23);
	bomba_horizontal(43, 26);
	bomba_horizontal(43, 25);
	bomba_horizontal(43, 24);
	bomba_horizontal(43, 23);
	bomba_horizontal(43, 22);
	bomba_horizontal(43, 21);

	pthread_t criar_naves;
	pthread_create(&criar_naves, NULL, criar_naves_func, NULL);

	pthread_t fabrica_bomba;
	pthread_create(&fabrica_bomba, NULL, fabrica_bomba_func, NULL);

	pthread_t game_over;
	pthread_create(&game_over, NULL, game_over_func, NULL);

	while (1)
	{

		int ch = getchar();
		if (ch == ' ')
		{
			if (qtd_bombas > 0)
			{
				pthread_t bomba_t;
				pthread_create(&bomba_t, NULL, bomba_func, NULL);
			}
		}
	}
}
