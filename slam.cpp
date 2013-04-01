#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <GL/glut.h>
#include "high.h"
#include "mt-rand.h"

// The initial seed used for the random number generated can be set here.
#define SEED 1

// Default names for printing map files.
// File types are automatically appended.
#define MAP_PATH_NAME "map"
#define PARTICLES_PATH_NAME "particles"


//
// Globals
//

// The means by which the slam thread can be told to halt, either by user command or by the end of a playback file.
int continueSlam;



//
// This calls the procedures in the other files which do all the real work. 
// If you wanted to not use hierarchical SLAM, you could remove all references here to High*, and make
// certain to set LOW_DURATION in low.h to some incredibly high number.
//
void *Slam(void *a)
{
  TPath *path, *trashPath;
  TSenseLog *obs, *trashObs;

  InitHighSlam();
  InitLowSlam();

  while (continueSlam) {
	clock_t t0 = clock();
    LowSlam(continueSlam, &path, &obs);
    clock_t t1 = clock();
	printf("LowSlam time: %ld\n", t1-t0);
	HighSlam(path, obs);

    // Get rid of the path and log of observations
    while (path != NULL) {
      trashPath = path;
      path = path->next;
      free(trashPath);
    }
    while (obs != NULL) {
      trashObs = obs;
      obs = obs->next;
      free(trashObs);
    }
  }

  CloseLowSlam();
  return NULL;
}

//samples to verify slam with gl drawing

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

float len = 0.5;

void display(void)
{
	while(1)
	{ 
		sem_wait(&semLive);
		
		//draw the lowMap
		glClear(GL_COLOR_BUFFER_BIT);
		glPointSize(1.0);
		glBegin(GL_POINTS);
		for(int x = 0; x < H_MAP_WIDTH; x++)
			for(int y = 0; y < H_MAP_HEIGHT; y++)
			{  
//				float fc = (float)map[x][y] / 255;
				float fc = (float)h_map[x][y]/255;
				glColor3f( fc, fc, fc);
				glVertex2f( -3 * (float)x/H_MAP_WIDTH + 1.5, -3 * (float)y/H_MAP_HEIGHT + 1.6);
			}
		glEnd();

		//TODO
		//WK, draw the car
//		INFORMAL
		float cx = 35 * lastY/H_MAP_WIDTH;//10 times. because change of X is too small compared with map scale
		float cy = 35 * lastX/H_MAP_HEIGHT;
		float len = (float)15/H_MAP_WIDTH;
//#define THETA (savedParticle[pBest].theta)
#define THETA lastTheta
		//3 vertices of a triangle
		float vert[3][2] = 
		{  
			{cx + 6 * len * sin(THETA), cy - 6 * len * cos(THETA)},
			{cx + len * sin(THETA + 0.7), cy - len * cos(THETA + 0.7)},
			{cx + len * sin(THETA - 0.7), cy - len * cos(THETA - 0.7)}
		};

		glBegin(GL_TRIANGLES);
		glColor3f( 1, 0, 0);
		glVertex2f( 3 * vert[0][0], 3 * vert[0][1]);
		glVertex2f( 3 * vert[1][0], 3 * vert[1][1]);
		glVertex2f( 3 * vert[2][0], 3 * vert[2][1]);

		glEnd();

/*
		cx = savedParticle[pBest].x /H_MAP_WIDTH;//10 times. because change of X is too small compared with map scale
		cy = savedParticle[pBest].y /H_MAP_HEIGHT;
		len = (float)15/H_MAP_WIDTH;
#define THETA (savedParticle[pBest].theta)
		float vert1[3][2] = 
		{  
			{cx + 6 * len * cos(THETA), cy + 6 * len * sin(THETA)},
			{cx + len * cos(THETA + 0.7), cy + len * sin(THETA + 0.7)},
			{cx + len * cos(THETA - 0.7), cy + len * sin(THETA - 0.7)}
		};
printf("cx=%f cy=%f\n",cx,cy);
		glVertex2f( 0,0);
		glBegin(GL_TRIANGLES);
		glColor3f( 0, 1, 0);
		glVertex2f( 3 * vert1[0][0] - 0.75, 3 * vert1[0][1] - 0.75);
		glVertex2f( 3 * vert1[1][0] - 0.75, 3 * vert1[1][1] - 0.75);
		glVertex2f( 3 * vert1[2][0] - 0.75, 3 * vert1[2][1] - 0.75);
		glEnd();*/

		glutSwapBuffers();
		glFlush();
	}
}
void* mGlFunc(void* arg)
{ 
	sem_init(&semLive,0,1);
/* GLUT环境初始化*/
	int i=0;
	glutInit (&i, NULL);
/* 显示模式初始化 */
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
/* 定义窗口大小 */
	glutInitWindowSize (H_MAP_WIDTH/4, H_MAP_HEIGHT/4);
/* 定义窗口位置 */
	glutInitWindowPosition (100, 100);
/* 显示窗口，窗口标题为执行函数名 */
	glutCreateWindow ("dpslam live");
/* 调用OpenGL初始化函数 */
//	init ( );
/* 注册OpenGL绘图函数 */
	glutDisplayFunc ( display );
/* 注册窗口大小改变时的响应函数 */
	glutReshapeFunc ( reshape );
/* 注册键盘响应函数 */
//	glutKeyboardFunc ( keyboard );
/* 进入GLUT消息循环，开始执行程序 */
	glutMainLoop( );
	return 0;
}
//
// Start of main program.
// IF things are set to read from a robot's sensors and not a data log, then this would be the best place
// to actually put in controls for the robot's behaviors and actions. The main SLAM process is called as a
// seperate thread off of this function.
//
int main (int argc, char *argv[])
 {
  pthread_t slam_thread;
  pthread_t live_thread;
    
  RECORDING = "";
  PLAYBACK = "";
  int x;
  for (x = 1; x < argc; x++) {
    if (!strncmp(argv[x], "-R", 2))
      RECORDING = "current.log";
    if (!strncmp(argv[x], "-r", 2)) {
      x++;
      RECORDING = argv[x];
    }
    else if (!strncmp(argv[x], "-p", 2)) {
      x++;
      PLAYBACK = argv[x];
    }
    else if (!strncmp(argv[x], "-P", 2))
      PLAYBACK = "current.log";
  }

  fprintf(stderr, "********** Localization Example *************\n");
  fprintf(stderr, "********** World Initialization ***********\n");

  seedMT(SEED);
  // Spawn off a seperate thread to do SLAM
  //
  // Should use semaphores or similar to prevent reading of the map
  // during updates to the map.
  //
  continueSlam = 1;
  pthread_create(&slam_thread, (pthread_attr_t *) NULL, Slam, &x);
  sem_init(&semLive, 0, 1);
  sem_init(&semHigh, 0, 1);
  //pthread_create(&live_thread, (pthread_attr_t *) NULL, doLive, NULL);
  pthread_create(&live_thread, (pthread_attr_t *) NULL, mGlFunc, NULL);

  fprintf(stderr, "*********** Main Loop (Movement) **********\n");

  pthread_join(slam_thread, NULL);
  pthread_join(live_thread, NULL);
  sem_destroy(&semLive);
  return 0;
} 
