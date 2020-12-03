//Подключение нужных библиотек(игра написана на OpenGL)
#include <time.h>
#include <GL/glut.h>


//Задаем характеристики клеточек
int N = 30, M = 20;
int Scale = 25;

int w = Scale * N;
int h = Scale * M;

//Параметры змейки
int dir, num = 4;

//Змейка
struct {
	int x;
	int y;
} s[100];

//Еда
class Food
{
public:
	int x, y;

	void New()
	{
		x = rand() % N;
		y = rand() % M;
	}

	//Отрисовка Еды
	void DrawFood()
	{
		glColor3f(0.0, 1.0, 1.0);
		glRectf(x * Scale, y * Scale, (x + 1) * Scale, (y + 1) * Scale);
	}
} m[10];

//Рисовка клеточек
void DrawField()
{
	glColor3f(1.5, 1.1, 1.0);
	glBegin(GL_LINES);

	for (int i = 0; i < w; i += Scale)
	{
		glVertex2f(i, 0);
		glVertex2f(i, h);
	}
	for (int j = 0; j < h; j += Scale)
	{
		glVertex2f(0, j);
		glVertex2f(w, j);
	}
	glEnd();
}

//Перемещение змейки
void Tick()
{
	//Остальные элементы
	for (int i = num; i > 0; --i) {
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	//Голова
	if (dir == 0) s[0].y += 1;
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y -= 1;

	for (int i = 0; i < 10; i++)
	{
		if ((s[0].x == m[i].x) && (s[0].y == m[i].y))
		{
			num++;
			m[i].New();
		}
	}

	//Проверка на выход за границу карты
	if (s[0].x > N) dir = 1; if (s[0].x < 0) dir = 2;
	if (s[0].y > M) dir = 3; if (s[0].y < 0) dir = 0;

	for (int i = 1; i < num; i++)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			num = i;
		}
	}
}

//Отрисовка змейки
void DrawSnake()
{
	glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < num; i++) {
		glRectf(s[i].x * Scale, s[i].y * Scale, (s[i].x + 0.9) * Scale, (s[i].y + 1) * Scale); 
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Вывод еды
	for (int i = 0; i < 10; i++)
	{
		m[i].DrawFood();
	}

	DrawField();
	DrawSnake();
	//Вывод на экран
	glFlush();
}

//Задает анимацию
void timer(int = 0) {
	display();

	Tick();

	glutTimerFunc(500, timer, 0);
}

//Управление
void MyKeyboard(int key, int a, int b)
{
	switch (key)
	{
		case 101 : dir = 0; break;
		case 102 : dir = 2; break;
		case 100 : dir = 1; break;
		case 103 : dir = 3; break;
	}
}

int main(int argc, char **argv)
{
	for (int i = 0; i < 10; i++)
	{
		m[i].New();
	}
	//Начальные координаты
	s[0].x = 10;
	s[0].y = 10;
	//Подключение OpenGl
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutCreateWindow("Test Window");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);

	glutDisplayFunc(display);
	glutTimerFunc(500, timer, 0);
	glutSpecialFunc(MyKeyboard);

	glutMainLoop();
}