/*Mason Hill Assignment 1
OCtober 8th*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#define M_PI 3.14159265358979323846 // Window doesn't seem to define this
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
using namespace std;


//Variable that sets the walls to have collision by default.
bool walltest =true;
//This holds the angle of rotation of the user
double angleofrotation = 0.0;


// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float dx=0.0f,dz=-1.0f;
// XZ position of the camera
float x=0.0f,z=0.0f;
//Storing the shooting blocks x and z
float blockx;
float blockz;

//Sets the default transparency to false
bool transparencyCheck=false;
//Holds the shooting blocks time to live
int timetolive = 400;

//The maze
//2 is a spinning cube, 1 is a wall, 0 is nothing
double maze[20][20] = {
{0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,0,2,1,1,0,1,0,1,0,1,1,1,1,1,1,0,1},
{1,0,1,0,1,0,1,0,1,0,1,0,1,1,2,1,0,1,0,1},
{1,0,1,0,1,0,2,0,2,0,1,0,0,0,0,1,2,1,0,1},
{1,0,1,0,1,0,1,0,0,0,1,0,1,1,0,0,0,1,0,1},
{0,0,2,0,0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1},
{1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1},
{1,0,1,0,1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,1},
{1,0,1,2,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,1},
{1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1},
{0,0,0,1,0,1,1,1,0,1,0,0,1,1,1,0,0,1,0,1},
{1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1},
{1,0,1,0,1,1,1,1,0,0,0,1,1,2,1,0,1,0,0,1},
{1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1},
{1,0,0,0,0,1,1,1,1,0,1,0,1,1,0,0,2,1,0,1},
{1,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
{1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};




//A regualr wall cube
void cube(void){
  glColor3f(1.0,0.6,0);
  //Builds the cube
  glutSolidCube(1.0);

}
//a spinning cube
void spinningCube(void){
  glColor3f(1.0,0.0,0.0);
  //Makes the cube smaller
  glutSolidCube(0.3);


}






//Setting the lighting of the game
void lighting(){
  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { -1.5f, 1.0f, 8.0f, 1.0f };
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

//Allowing the cube to spin
void movement (int i){
  //x is a global variable that is incremented to make a cube spin. We add 0.2 to the angle every time.
  angleofrotation=angleofrotation+0.2;
  if(angleofrotation>=360.0){
    angleofrotation=0;
  }

  glutPostRedisplay();
  //This timer function calls the movement function over and over again.

  glutTimerFunc(4,movement,8);
}


//Main display function
void display(void)
{
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // background colour
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the background
 //Adjusting the light matrix so we are moving the light.

  glColor3f(1.0, 1.0, 0.0);
  if(transparencyCheck==true){
    //Setting the maze to be transparent
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  //Setting the maze to have nontransparent cubes
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


  glLoadIdentity();

  //Starting the maze at an offset so the origin is at 0,0,0
  glTranslatef(-10.0,0,-10.0);
  //Building the maze
      for(int i=0;i<20;i++){
         for(int j=0;j<20;j++){





           glLoadIdentity();
           //allows the users to rotate
           glRotatef(angle,0,1.0,0);
           //Translating each cube when building
           glTranslatef(-10+j+x,0,-10+i+z);


           //If the maze =1 then it has a wall, =2 is a rotating block
           if(maze[i][j]==1){
             //inserting a wall.

             cube();

           }
           else if (maze[i][j]==2){

             //Rotating the cube
             glRotatef(angleofrotation,0.0,1.0,0);
             spinningCube();
           }



         }

       }



      glFlush(); // make sure the polygon is pushed to the graphics card

      glutSwapBuffers(); // flip the buffers so the polygon I just drew is now on the front buffer
}

//Checking collision
bool collision(double nextx, double nextz){

  cout << "future x "<< nextx << endl;
  cout << "future z "<< nextz << endl;
  //Seeing if the user wants to test the wall collision
  if(walltest==true){
    //Checking for collision at the array position by rounding the x and z into whole array coordinates
    if(maze[(int)round(-1*nextz+10)][(int)round(-1*nextx+10)]==1){
      return false;
    }
    else
    return true;
  }
  return true;
}

//Getting the user movement input
void processSpecialKeys(int key, int xx, int yy) {
  //speed throttle
	float fraction = 0.1f;

	switch (key) {
    //If the user inputs left or right, calculate what the angle and dx and dz are of the new looking position
		case GLUT_KEY_LEFT :
			angle -= 1.5f;
			dx = sin(angle*3.14159265358979323846/180);
			dz = -cos(angle*3.14159265358979323846/180);
			break;

		case GLUT_KEY_RIGHT :
			angle += 1.5f;
			dx = sin(angle*3.14159265358979323846/180);
			dz = -cos(angle*3.14159265358979323846/180);
			break;

      //If the user moves forward or backward, check for collision and move them
		case GLUT_KEY_UP :
    //Plugging in our current x,z,dx,dz, into the collision function to check if the collision is true/there is  collision
    if(collision((x-dx*(fraction*2)),(z-dz*(fraction*2)))==true){

      //Put collision check here, this is where i ended.
			x -= dx * fraction;
			z -= dz * fraction;
    }
    break;
		case GLUT_KEY_DOWN :
    if(collision((x+dx*(fraction*2)),(z+dz*(fraction*2)))==true){
			x += dx * fraction;
			z += dz * fraction;
    }
    break;

    case 32 :
      blockx = x;
      glTranslatef(blockx+dx,0,blockz+dz);
      spinningCube();

    break;

	}
  cout << "x" << x << endl;
  cout << "z" << z<< endl;
  cout << "dx" << dx<< endl;
  cout << "dz" << dz<<endl;
  cout << "angle" << angle<< endl<<endl;

  glutPostRedisplay();
}

//Getting user input for the wall collision test and the transparency test
void keyboardFunc(unsigned char key, int x, int y ){

switch(key){
  case 'o' :
    if(transparencyCheck==false){

      transparencyCheck = true;

    }
    else {transparencyCheck = false;}

    break;

  case 'w' :
    if(walltest==false){
      walltest=true;

    }
    else {walltest=false;}
    break;
  }
}


//when moving make dz = sin(x) and dx=cos(x)



//Setting up the program
void begin(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//Setting our view
  gluPerspective(45.5,1.0f,0.1f,1000);

  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);


  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


int main(int argc, char** argv)
{
    // First set up GLUT
    glutInit( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Make the window
    int windowHandle = glutCreateWindow("Program 1");
    glutSetWindow(windowHandle);
    lighting();
    begin();

    // Place and size the window
    glutPositionWindow ( 100, 100 );
    glutReshapeWindow( 500, 500 );

    //Calling our functions for drawing and user input.
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(processSpecialKeys);

    glutTimerFunc(400,movement,30);


    // Go into an infinite loop waiting for messages
    glutMainLoop();
    return 0;
}
