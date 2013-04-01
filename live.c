//complie command: gcc -o test test.c -lGL -lglut
/* light.c
此程序利用GLUT绘制一个OpenGL窗口，并显示一个加以光照的球。
*/
/* 由于头文件glut.h中已经包含了头文件gl.h和glu.h，所以只需要include 此文件*/
#include <GL/glut.h>
#include <stdlib.h>
#include <pthread.h>

/* 初始化材料属性、光源属性、光照模型，打开深度缓冲区 */
void init ( void )
{
	GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess [ ] = { 50.0 };
	GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 };

	glClearColor ( 0.0, 0.0, 0.0, 0.0 );
	glShadeModel ( GL_SMOOTH );

	glMaterialfv ( GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv ( GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv ( GL_LIGHT0, GL_POSITION, light_position);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_DEPTH_TEST);
}
/*调用GLUT函数，绘制一个球*/
void display ( void )
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSolidSphere (1.0, 40, 50);
	glFlush ();
}
/* 定义GLUT的reshape函数，w、h分别是当前窗口的宽和高*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
	if (w <= h)
		glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w,
		1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
	else
		glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h,
		1.5 * ( GLfloat ) w / ( GLfloat ) h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( ) ;
}

/* 定义对键盘的响应函数 */
void keyboard ( unsigned char key, int x, int y)
{
	/*按Esc键退出*/
	switch (key) 
	{
	case 27:
	exit ( 0 );
	break;
	}
}

void* main1(void *a)
{
	int i=0;
	char** p = NULL;
	glutInit (&i, p);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (300, 300);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ( "dpslam" );
	init ( );
//TODO
//use semaphore, controlled by slam process, to arise changes on graph.
//write demos to facilitate this mechanism.
	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutMainLoop( );
	return NULL;
}
int main(int argc, char** argv)
{
	pthread_t live_thread;
	pthread_create(&live_thread, (pthread_attr_t *)NULL, main1,(void*)NULL);
	pthread_join(live_thread, NULL);
}
