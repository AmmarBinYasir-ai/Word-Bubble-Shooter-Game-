//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================


//Ammar Yasir 20I-0501 Project Section E








#ifndef WORD_SHOOTER_CPP







#define WORD_SHOOTER_CPP















//#include <GL/gl.h>







//#include <GL/glut.h>







#include <iostream>

#include<string>

#include<cmath>

#include<fstream>

#include "util.h"

#include <cstdlib> 

#include <time.h>

#include <ctime>

using namespace std;







#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....







#define MIN(A,B) ((A) < (B) ? (A):(B))







#define ABS(A) ((A) < (0) ? -(A):(A))







#define FPS 10















string * dictionary;







int dictionarysize = 370099;







#define KEY_ESC 27 // A















// 20,30,30







const int bradius = 30; // ball radius in pixels...















int width = 930, height = 660;







int byoffset = bradius;















int nxcells = (width - bradius) / (2 * bradius);







int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);







int nfrows = 2; // initially number of full rows //







float score = 0;







int **board; // 2D-arrays for holding the data...







int bwidth = 130;







int bheight = 10;







int bsx, bsy;







const int nalphabets = 26;



//my variables declaration 
const int ball_of_x=width/2, ball_of_y=bheight;

int counter=0; //counter as int and is set to zero

int time_remains=120; //time remains as int and 120 seconds which is 2 minutes

bool started_game = false; //Checks the game wheter it has started or not
bool ball_movement = true; //We check wheter the shooter ball is being shot or not
float gradient; //For checking the ball movement we use gradient to find the initial clicks by the user
int temporary_of_y = ball_of_y; //pixel coordinates of the ball while moving
int temporary_of_x = ball_of_x; //pixel coordinates of ball while moving
bool swapping = false; //Checks the swapping when the user clicks the right button on the mouse
int next_ball_shooter=0; //The first ball that must be launched 
int second_ball_shooter=0; //The second ball which will replace that must be shooted after first ball is launched
int coordinates_x = ball_of_x; //Displays the coordinates of x by storing the ball of x
int coordinates_y = ball_of_y; //Displays the coordinates of y by storing the ball of y

















enum alphabets {







	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z







};







GLuint texture[nalphabets];







GLuint tid[nalphabets];







string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",







"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",







"x.bmp", "y.bmp", "z.bmp" };







GLuint mtid[nalphabets];







int awidth = 60, aheight = 60; // 60x60 pixels cookies...























//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE







void RegisterTextures_Write()







//Function is used to load the textures from the







// files and display







{







	// allocate a texture name







	glGenTextures(nalphabets, tid);







	vector<unsigned char> data;







	ofstream ofile("image-data.bin", ios::binary | ios::out);







	// now load each cookies data...















	for (int i = 0; i < nalphabets; ++i) {















		// Read current cookie















		ReadImage(tnames[i], data);







		if (i == 0) {







			int length = data.size();







			ofile.write((char*)&length, sizeof(int));







		}







		ofile.write((char*)&data[0], sizeof(char) * data.size());















		mtid[i] = tid[i];







		// select our current texture







		glBindTexture(GL_TEXTURE_2D, tid[i]);















		// select modulate to mix texture with color for shading







		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);















		// when texture area is small, bilinear filter the closest MIP map







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,







			GL_LINEAR_MIPMAP_NEAREST);







		// when texture area is large, bilinear filter the first MIP map







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);















		// if wrap is true, the texture wraps over at the edges (repeat)







		//       ... false, the texture ends at the edges (clamp)







		bool wrap = true;







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,







			wrap ? GL_REPEAT : GL_CLAMP);







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,







			wrap ? GL_REPEAT : GL_CLAMP);















		// build our texture MIP maps







		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,







			GL_UNSIGNED_BYTE, &data[0]);







	}







	ofile.close();















}







void RegisterTextures()







/*Function is used to load the textures from the







* files and display*/







{







	// allocate a texture name







	glGenTextures(nalphabets, tid);















	vector<unsigned char> data;







	ifstream ifile("image-data.bin", ios::binary | ios::in);















	if (!ifile) {







		cout << " Couldn't Read the Image Data file ";







		//exit(-1);







	}







	// now load each cookies data...







	int length;







	ifile.read((char*)&length, sizeof(int));







	data.resize(length, 0);







	for (int i = 0; i < nalphabets; ++i) {







		// Read current cookie







		//ReadImage(tnames[i], data);







		/*if (i == 0) {







		int length = data.size();







		ofile.write((char*) &length, sizeof(int));







		}*/







		ifile.read((char*)&data[0], sizeof(char)* length);















		mtid[i] = tid[i];







		// select our current texture







		glBindTexture(GL_TEXTURE_2D, tid[i]);















		// select modulate to mix texture with color for shading







		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);















		// when texture area is small, bilinear filter the closest MIP map







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,







			GL_LINEAR_MIPMAP_NEAREST);







		// when texture area is large, bilinear filter the first MIP map







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);















		// if wrap is true, the texture wraps over at the edges (repeat)







		//       ... false, the texture ends at the edges (clamp)







		bool wrap = true;







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,







			wrap ? GL_REPEAT : GL_CLAMP);







		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,







			wrap ? GL_REPEAT : GL_CLAMP);















		// build our texture MIP maps







		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,







			GL_UNSIGNED_BYTE, &data[0]);







	}







	ifile.close();







}







void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,







	int cheight = 60)







	/*Draws a specfic cookie at given position coordinate







	* sx = position of x-axis from left-bottom







	* sy = position of y-axis from left-bottom







	* cwidth= width of displayed cookie in pixels







	* cheight= height of displayed cookiei pixels.







	* */







{







	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight







		/ height * 2;







	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;















	glPushMatrix();







	glEnable(GL_TEXTURE_2D);







	glBindTexture(GL_TEXTURE_2D, mtid[cname]);







	glBegin(GL_QUADS);







	glTexCoord2d(0.0, 0.0);







	glVertex2d(fx, fy);







	glTexCoord2d(1.0, 0.0);







	glVertex2d(fx + fwidth, fy);







	glTexCoord2d(1.0, 1.0);







	glVertex2d(fx + fwidth, fy + fheight);







	glTexCoord2d(0.0, 1.0);







	glVertex2d(fx, fy + fheight);







	glEnd();















	glColor4f(1, 1, 1, 1);















	//	glBindTexture(GL_TEXTURE_2D, 0);















	glDisable(GL_TEXTURE_2D);







	glPopMatrix();















	//glutSwapBuffers();







}







int GetAlphabet() {







	return GetRandInRange(1, 26);







}















void Pixels2Cell(int px, int py, int & cx, int &cy) {







}







void Cell2Pixels(int cx, int cy, int & px, int &py)







// converts the cell coordinates to pixel coordinates...







{







}







void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)















{







	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight







		/ height * 2;







	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;















	glPushMatrix();







	glEnable(GL_TEXTURE_2D);







	glBindTexture(GL_TEXTURE_2D, -1);







	glBegin(GL_QUADS);







	glTexCoord2d(0.0, 0.0);







	glVertex2d(fx, fy);







	glTexCoord2d(1.0, 0.0);







	glVertex2d(fx + fwidth, fy);







	glTexCoord2d(1.0, 1.0);







	glVertex2d(fx + fwidth, fy + fheight);







	glTexCoord2d(0.0, 1.0);







	glVertex2d(fx, fy + fheight);







	glEnd();















	glColor4f(1, 1, 1, 1);















	//	glBindTexture(GL_TEXTURE_2D, 0);















	glDisable(GL_TEXTURE_2D);







	glPopMatrix();















	//glutSwapBuffers();







}







/*







* Main Canvas drawing function.







* */







void DisplayFunction() {







	// set the background color using function glClearColor.







	// to change the background play with the red, green and blue values below.







	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.







	//#if 0







	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,







		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour







	glClear(GL_COLOR_BUFFER_BIT); //Update the colors















	//write your drawing commands here or call your drawing functions...





	srand (NULL);	//declared the srand for the random alphabets

	int x_axis=15; //declared x_axis as int and set to 15 value

	int y_axis=530; //declared the y_axis as int and set to 530 value

	int random=GetRandInRange(1,26); //declared the random as int and gets the random alphabets 







  if (started_game==true) //checks wheter the game has started or not because to display the ending and starting screen

  { 

	if(time_remains>0) //if the time is more than 0 seconds

	{



	 for(int q=0;q<30;q++) //use for the first colums of alphabets which is less than 30

	 {

	  random = GetRandInRange(1,26); //shows 25 alphabets randomly



	  if(q<15) //if the columns or alphabtes is less than 25 

	  {



	   DrawAlphabet((alphabets)random,x_axis,y_axis, awidth, aheight); //shows the random alphabtes

	   x_axis=x_axis+60; //the x-axis is added by 60 spacing



	   if(q==14) //if the alphaets is equals to 14 then

	   {

	    x_axis=15; //sets the x_axis exactly to 15

	    y_axis=y_axis-60; //Reduces the spacing by 60

	   } 



	 }
	 

	 else if(q<30) //if the alphabets is less than 30 

	 {

	  DrawAlphabet((alphabets)random,x_axis,y_axis, awidth, aheight); //shows the alphabets of randomly

	  x_axis=x_axis+60; //adds the x-axis by 60

	 }
	 

       }

	//I tried making the shooter ball to move but it is moving but not displaying as its just displaying some alphabets 
	//attached to the another alphabets but its linked with the mouse click function which it will display only on that 	 //direction where you want to throw.

	DrawAlphabet((alphabets)second_ball_shooter,460,10, awidth, aheight); //Ball on the word shooter
	//In order to move the ball we have to put an IF condition to check wheter the ball is moving or not
	if (ball_movement== false)
	{
	 temporary_of_y = ball_of_y; //The ball of y at the shooter ball is stored in temporary value of y
	 temporary_of_y = ball_of_x; //Then the ball of x is stored in temporary value of y
	 coordinates_x = ball_of_x; //Then the ball of x is stored in coordinates value of x
	 coordinates_y = ball_of_y; //The ball of y is stored in the coordinates of y
	 DrawAlphabet((alphabets)next_ball_shooter,temporary_of_x, temporary_of_y, awidth, aheight); //Shooter ball
	}
       else //If not and the condition fails
       {
        if (temporary_of_y<=height) //The temporary value of y is less than and equals to the height of the screen
        {
	  temporary_of_y=temporary_of_y+10; //The temporary of y is added by 10 according to the screen
	 }
	 else  //If it doesn't added up
	 {
	  ball_movement=false; //The ball won't move
	 }
	  //The gradient is multiple the coordinates of y difference of temporary of y and added to the coordinates of x 
	  //is more than width and difference of 30 and condtion with the same thing but subtracted this time 			
	  if (gradient*(temporary_of_y-coordinates_y)+coordinates_x>=width-30 || gradient*(temporary_of_y-coordinates_y)+coordinates_x<=-30)
	  {
	     //To get the x coordinates we multiple the gradient to the temporary of y minus the coordinates of y and
	     //Added to the coordinates of x
	     coordinates_x=gradient*(temporary_of_y-coordinates_y)+coordinates_x; 
	     coordinates_y=temporary_of_y-30; //The coordinates of y is subtracted by 30
	  }
				
       }

       }	 

	counter=counter+5; //timer that runs by 5 intervals

	if(time_remains==0) //if the time is exactly zero then show the ending game screen and thank the user

	{

	   DrawString(350,300,width,height,"You have Ran out of time!",colors[BLUE_VIOLET]); //Displays the message
	   DrawString(267,274,width,height,"Thank you so much for playing the game!",colors[BLUE_VIOLET]); //Displays message

	}

	else if (counter==60) //if the count time interval is equals to 60

	{

	  counter=0; //counter is exactly zero

	  time_remains=time_remains-1; //the time remains is subtracted in negative numbers

	}



	

	

DrawString(40, height - 20, width, height , "Score " + Num2Str(score), colors[BLUE_VIOLET]); //Shows the score message

 

DrawString(width / 2 - 30, height - 25, width, height,

"Time is Left:" + Num2Str(time_remains) + " Seconds", colors[RED]); //shows the time left message



 }



else //The welcome screen of the game for extra bonus

{

 //DrawAlphabet((alphabets)25, 157, 78, awidth, aheight);
 //Displays the message to start the game on the welcome page

 DrawString(196, 383, width, height ,"Please enter or click anywhere on the screen to start the game", colors[BLUE_VIOLET]);
 DrawString(353, 310,width,height,"Welcome to the shooter game!!",colors[BLUE_VIOLET]);
 
 
 



}



























	// #----------------- Write your code till here ----------------------------#







	//DO NOT MODIFY THESE LINES







	DrawShooter((width / 2) - 35, 0, bwidth, bheight);







	glutSwapBuffers();







	//DO NOT MODIFY THESE LINES..







}















/* Function sets canvas size (drawing area) in pixels...







*  that is what dimensions (x and y) your game will have







*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)







* */







void SetCanvasSize(int width, int height) {







	/*glMatrixMode(GL_PROJECTION);







	glLoadIdentity();







	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/







}















/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)







* is pressed from the keyboard







*







* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...







*







* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the







* program coordinates of mouse pointer when key was pressed.







*







* */















void NonPrintableKeys(int key, int x, int y) {







	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {







		// what to do when left key is pressed...















	}







	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {















	}







	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {







	}







	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {







	}















	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call







	* this function*/







	/*







	glutPostRedisplay();







	*/







}







/*This function is called (automatically) whenever your mouse moves witin inside the game window







*







* You will have to add the necessary code here for finding the direction of shooting







*







* This function has two arguments: x & y that tells the coordinate of current position of move mouse







*







* */















void MouseMoved(int x, int y) {

//I was checking the coordinates of the screen with these commented out variables 

	//cout<<x<<endl;



	//cout<<y<<endl;







	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls















}















/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window







*







* You will have to add the necessary code here for shooting, etc.







*







* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),







* x & y that tells the coordinate of current position of move mouse







*







* */















void MouseClicked(int button, int state, int x, int y) {



	if (started_game==false) //Checks when clicked by the user to start the game

	{

	   started_game=true; //The game will start once clicked

	}







	//This condition deal when the user click the left button using the gradient in the screen

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button

	{

	   if (state == GLUT_UP&&ball_movement==false&&y<=height-bheight-10) //for ball movement for left we do these condtions

  	   {
  	  //We find the gradient by taking the float values of x ball and divide it by the height minus the ball of y 
  	    gradient = float(x-ball_of_x)/(height-y-ball_of_y); //Gradient equation
  	    ball_movement = true; //If the ball movement then goes true if the equation is applied

	   }

	}




	//This condition checked for the right button when the user clicked the right button and its stored in the 
	//temporary value of the first ball and then the second ball is stored in the first ball	
	
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button

	{
	  //We have to put a condtion so that we can check the ball launched  for the right button
	  if(swapping=false) //If the value is switched then
	  {
	   int temporary; //Declares an temporary value as int
	   
	   temporary = next_ball_shooter; //Temporary is equal to the ball launched
	   next_ball_shooter = second_ball_shooter; //The ball launched is equals to the second ball
	   swapping = true; //So therefore the value will be true then

	  }
	  else //if not
	  {
	   swapping = false; //Then the condtion would be false and the alphabets won't be swapped 
	  }







	glutPostRedisplay();







	  }
	  
}







/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)







* is pressed from the keyboard







* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the







* program coordinates of mouse pointer when key was pressed.







* */







void PrintableKeys(unsigned char key, int x, int y) {







	if (key == KEY_ESC/* Escape key ASCII*/) {







		exit(1); // exit the program when escape key is pressed.







	}







}















/*







* This function is called after every 1000.0/FPS milliseconds







* (FPS is defined on in the beginning).







* You can use this function to animate objects and control the







* speed of different moving objects by varying the constant FPS.







*







* */







void Timer(int m) 
{















	glutPostRedisplay();







	glutTimerFunc(1000.0/FPS, Timer, 0);







}























/*







* our gateway main function







* */







int main(int argc, char*argv[]) 
{







	InitRandomizer(); // seed the random number generator...















	//Dictionary for matching the words. It contains the 370099 words.







	dictionary = new string[dictionarysize]; 







	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings







	//print first 5 words from the dictionary







	for(int i=0; i < 5; ++i)







		cout<< " word "<< i << " =" << dictionary[i] <<endl;



		



		



	



	















	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets



	second_ball_shooter = GetAlphabet(); //Displays the random alphabets for a second ball
	next_ball_shooter =   GetAlphabet(); //Displays the random alphabets for the first ball



























	glutInit(&argc, argv); // initialize the graphics library...







	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode







	glutInitWindowPosition(50, 50); // set the initial position of our window







	glutInitWindowSize(width, height); // set the size of our window







	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window







	//SetCanvasSize(width, height); // set the number of pixels...















	// Register your functions to the library,







	// you are telling the library names of function to call for different tasks.







	RegisterTextures();







	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.







	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters







	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters







	glutMouseFunc(MouseClicked);







	glutPassiveMotionFunc(MouseMoved); // Mouse















	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...







	glutTimerFunc(1000.0/FPS, Timer, 0);















	//// now handle the control to library and it will call our registered functions when







	//// it deems necessary...















	glutMainLoop();















	return 1;

}







#endif /* */
