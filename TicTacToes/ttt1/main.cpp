//  ********************
//  ********************
// TIC TAC TOE: A Game
//  ********************
// Keyboard input
// 'v' = view ortho/perspective
//  ********************
// Library need to include in order to run opengl application on replit
// glut (gl utility toolkit) basic window, keyboard, mouse etc functions.
#include <GL/glut.h>
// Essential Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

//  ********************
// Global Game Variables

// to change screen if certain menu option is chosen
int screen_option = 0;
// mouse variables: Win = windows size, mouse = cursor position, object_select = selected box Number,
int mouse_x, mouse_y, Win_x, Win_y, object_select;
// state variables to switch between Ortho or Perspective view
static int view_state = 0;
// Use to spin X's and O's
// spinboxes will have the box no of all three boxes of winning sequence
int spin, spinboxes;
// a flag set which is set to
//1 if Player wins, -1 if Computer wins, 2 if its a Tie.
int win;
// player's move is indicated by variable 'player' which = -1
// computer's move is indicated by variable 'computer' which = 1
int player, computer;
// start_game indicates that game is in play.
int start_game;
// quadric pointer for build our X
GLUquadricObj * Cylinder;

//  ********************
// Grid related information and Variables

// All the 9 boxes are assigned a number, which are displayed below
// 0 | 1 | 2
// 3 | 4 | 5
// 6 | 7 | 8
// it will act as the main grid/game board where each cell is a box or square
int box_map[9];
// Each cell/box in the main grid can store 3 different values {-1,0,1}
// -1 = Player made a move in that box so, will be displayed as 'O' on the display screen
// 1 = Computer made a move in that box so, will be displayed as 'X' on the display screen
// 0 = Vacant Square

// x,y location coordinates for each of the 9 squares
// This will help in identifying which square is selected from cursor position
int object_map[9][2] = {{-6,6},{0,6},{6,6},{-6,0},{0,0},{6,0},{-6,-6},{0,-6},{6,-6}};

// We have 8 possible winning sequence, 3 across, 3 down and 2 diagonally
// All 8 possible winning positions in the matrix where ith row determines which the ith winning sequence and jth position determines the box no.
static int box[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

//  ********************
// Initialising our main grid
void init_game(void) {
  int i;
  // Clear grid for new game
  for (i = 0; i < 9; i++) {
    box_map[i] = 0;
  }
  // Initially set win = 0 as there is no winner, start_game = 1
  win = 0;
  start_game = 1;
}

// Check for three in a row/column/diagonal
// returns 1 if there is a winner, 2 if tie has been reached and 0 if game is not over yet
int check_move(void) {

  int i, t = 0;
  //Check for three in a row
  for (i = 0; i < 8; i++) {
    // take sum of all the three box positions of all the eight winning positions stored in box
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    if ((t == 3) || (t == -3)) {
      // Store the box no of the winning box sequence to rotate
      spinboxes = i;
      // return 1 means there is a winner
      return (1);
    }
  }

  t = 0;
  // check for tie
  for (i = 0; i < 8; i++) {
    t = t + abs(box_map[box[i][0]]) + abs(box_map[box[i][1]]) + abs(box_map[box[i][2]]);
  }
  // if tie return 2
  if (t == 24) return (2);

  // if nothing, that means game is not over return 0
  return (0);
}

// A simple algo to detect if player can win in the next move, then block it
// Also, if computer can win in the next move then it will make that move to win
int blocking_win() {
  int i, t;
  for (i = 0; i < 8; i++) {
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    // we are looking for combinations from all 8 possible winning sequences, in which two positions are filled and one is vacant
    if ((t == 2) || (t == -2)) {
      // if found empty, we assign an X to that position in the matrix
      // that means computer has played this move in that particular position
      if (box_map[box[i][0]] == 0) box_map[box[i][0]] = computer;
      if (box_map[box[i][1]] == 0) box_map[box[i][1]] = computer;
      if (box_map[box[i][2]] == 0) box_map[box[i][2]] = computer;
      // if computer successfully played its turn return 1
      return (1);
    }
  }
  // if computer didnt play its turn return 0
  return (0);
}

// check for a free space in corner
int check_corner(void) {
  int i;
  if (box_map[0] == 0) {
    box_map[0] = computer;
    i = 1;
    return (1);
  }
  if (box_map[2] == 0) {
    box_map[2] = computer;
    i = 1;
    return (1);
  }
  if (box_map[6] == 0) {
    box_map[6] = computer;
    i = 1;
    return (1);
  }
  if (box_map[8] == 0) {
    box_map[8] = computer;
    i = 1;
    return (1);
  }
  return (0);
}

// Check for free space in row
int check_row(void) {
  // first we check for center
  if (box_map[4] == 0) {
    box_map[4] = computer;
    return (1);
  }
  // then we check for all other
  if (box_map[1] == 0) {
    box_map[1] = computer;
    return (1);
  }
  if (box_map[3] == 0) {
    box_map[3] = computer;
    return (1);
  }
  if (box_map[5] == 0) {
    box_map[5] = computer;
    return (1);
  }
  if (box_map[7] == 0) {
    box_map[7] = computer;
    return (1);
  }
  // if no vacant position found
  return (0);
}

// logic for computer's turn
int computer_move() {
  if (blocking_win() == 1) return (1);
  if (check_corner() == 1) return (1);
  if (check_row() == 1) return (1);
  return (0);
}

// Use this to put text on the screen
void Sprint(int x, int y, char * st) {
  int l, i;
  // see how many characters are in text string.
  l = strlen(st);
  // location to start printing text
  glRasterPos2i(x, y);
   // loop until i is greater then l
  for (i = 0; i < l; i++){
    // Print a character on the screen
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
  }
}

// This creates the spinning of the cube.
static void TimeEvent(int te) {
  // increase cube rotation by 1
  spin++;
  // if over 360 degress, start back at zero.
  if (spin > 360) spin = 180;
  // Update screen with new rotation matrix data
  glutPostRedisplay();
  // Reset our timer.
  glutTimerFunc(8, TimeEvent, 1);
}

// Setup our Opengl world, called once at startup.
void init(void) {

  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 0.0);
  // Setting the game variables
  start_game = 0;
  win = 0;

  // Create a new quadric
  // used for making "X"
  Cylinder = gluNewQuadric();
  gluQuadricDrawStyle(Cylinder, GLU_FILL);
  gluQuadricNormals(Cylinder, GLU_SMOOTH);
  gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
}

// Will draw "O" and also rotate if the player wins
void Draw_O(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(a, 0, 1, 0);
  //glutSolidTorus(0.5, 2.0, 20, 16);
   glutSolidTorus(0.3, 1.0, 10, 300);
  glPopMatrix();
}

// Will draw "X" and also rotate if the computer wins
void Draw_X(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glPushMatrix();
  glRotatef(a, 0, 1, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(45, 1, 0, 0);
  //glTranslatef(0, 0, -3);
  glTranslatef(0, 0, -2);
  //gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  gluCylinder(Cylinder, 0.3, 0.3, 4, 10, 16);
  glPopMatrix();
  glPushMatrix();
  glRotatef(a, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(315, 1, 0, 0);
  //glTranslatef(0, 0, -3);
   glTranslatef(0, 0, -2);
  //gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  gluCylinder(Cylinder, 0.3, 0.3, 4, 10, 16);
  glPopMatrix();
  glPopMatrix();
}

// Main function to draw our world on the display screen
void display(void) {

  // if help option is chosen
  if (screen_option == 3) {
    //Clear the screen and set the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    // Switching to Project Matrix Work to display the changes in display screen
    glMatrixMode(GL_PROJECTION);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();
    // Setup an Ortho view
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    // Switching to Model Matrix Work to draw
    glMatrixMode(GL_MODELVIEW);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();
    // Disabling to avoid material color to track the current color (background color)
    glDisable(GL_COLOR_MATERIAL);
    // Set the font color
    glColor3f(1.0, 1.0, 1.0);
    // Display the text on the screen
    Sprint(-3, -2, "To Start the game, press right button and select start game");
    // To swap the contents of the front buffer with back buffer
    glutSwapBuffers();
  }

  // start screen
  else if (screen_option == 0) {

    //Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Switching to Project Matrix Work to display the changes in display screen
    glMatrixMode(GL_PROJECTION);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();
    // Setting up the Ortho view
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    // Switching to Project Matrix Work to display the changes in display screen
    glMatrixMode(GL_MODELVIEW);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();
    // Disabling to avoid material color to track the current color (background color)
    glDisable(GL_COLOR_MATERIAL);
    // Set the font color
    glColor3f(1.0, 1.0, 1.0);
    // Printing the following text on the display screen
    Sprint(-2, 3,  "TIC - TAC - TOE");
    Sprint(-2, 1,  "A Game created by");
    Sprint(-2, -1, "Your Name in here");
    Sprint(-3, -5, "Right Click to Start the Game");

    // To swap the contents of the front buffer with back buffer
    glutSwapBuffers();
  }

  // else game ongoing
  else {
    int ix, iy;
    int i; // for iterating over a loop
    int j; // to set the spin

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Switching to Project Matrix Work to display the changes in display screen
    glMatrixMode(GL_PROJECTION);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();
    // Setting up the Ortho view
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    // Switching to Project Matrix Work to draw
    glMatrixMode(GL_MODELVIEW);
    // Clear the matrix to flush out previous changes
    glLoadIdentity();

    // Disabling to avoid material color to track the current color (background color)
    glDisable(GL_COLOR_MATERIAL);
    // Setting up the font color
    glColor3f(1.0, 0.0, 0.0);

    // If Win, display
    if (win == 1) Sprint(-2, 1, "Congratulations!! You Won :)");
    if (win == -1) Sprint(-2, 1, "Computer Won :(");
    if (win == 2) Sprint(-2, 1, "It's a Tie");

    // Setup view, and print view state on screen
    // for perspective view
    if (view_state == 1) {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-3, 8, "Perspective View");
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(60, 1, 1, 30);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
    // for ortho view
    else {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-2, 8, "Ortho View");
    }

    // to get a reference to the centre of the display screen
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

    // Draw Grid
    // 4 lines vertically
    for (ix = 0; ix < 4; ix++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9, -9 + ix * 6);
        glVertex2i(9, -9 + ix * 6);
      glEnd();
      glPopMatrix();
    }
    // 4 lines horizontally
    for (iy = 0; iy < 4; iy++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9 + iy * 6, 9);
        glVertex2i(-9 + iy * 6, -9);
      glEnd();
      glPopMatrix();
    }

    // Draw object in box's
    for (i = 0; i < 9; i++) {
      j = 0;
      // first we look if game is over or not
      if (abs(win) == 1) {
        // we check if i is a winning position in either of the 8 possible winning sequences
        if ((i == box[spinboxes][0]) || (i == box[spinboxes][1]) || (i == box[spinboxes][2])) {
          j = spin;
        } else j = 0;
      }
      // else we draw O or X on the screen
      if (box_map[i] == 1) Draw_X(object_map[i][0], object_map[i][1], -1, j);
      if (box_map[i] == -1) Draw_O(object_map[i][0], object_map[i][1], -1, j);
    }

    // To swap the contents of the front buffer with back buffer
    glutSwapBuffers();
  }
}

// This is called when the window has been resized.
void reshape(int w, int h) {
  Win_x = w;
  Win_y = h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

// to Read the keyboard input
void keyboard(unsigned char key, int x, int y) {

  switch (key) {
    // to switch between views
    case 'v':
    case 'V':
      view_state = abs(view_state - 1);
      break;
    // exit program when [ESC] key pressed
    case 27:
      exit(0);
      break;
    default:
      break;
  }
}

// to deal with all the mouse input
void mouse(int button, int state, int x, int y) {

  // We convert windows mouse coordinates to out openGL coords
  mouse_x = (18 * (float)((float) x / (float) Win_x)) / 6;
  mouse_y = (18 * (float)((float) y / (float) Win_y)) / 6;

  // this variable will have the box number of selected box
  object_select = mouse_x + mouse_y * 3;

  // if the game has not started
  if (start_game == 0) {
    // if left mouse button pressed
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      player = -1;
      computer = 1;
      init_game();
      return;
    }
  }
  // if game is currently ongoing
  if (start_game == 1) {
    // if left mouse button is pressed
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      // if no one has won the game
      if (win == 0) {
        // checking if the box selected vacant or not
        if (box_map[object_select] == 0) {
          // we assign an "O" to that box in the main game board
          box_map[object_select] = player;
          // check if the player has won or not
          win = check_move();
          // check if player has won
          if (win == 1) {
            start_game = 0;
            return;
          }
          // computer's turn
          computer_move();
          // check if it won or not
          win = check_move();
          if (win == 1) {
            win = -1;
            start_game = 0;
          }
        }
      }
    }
    }
  // check if game has tied
  if (win == 2) start_game = 0;
}

// setting up the menu
void menu(int choice) {
  switch (choice) {
  // To start game
  case 1:
    screen_option = 1;
    glutMouseFunc(mouse);
    break;
  // To change the viewport
  case 2:
    view_state = abs(view_state - 1);
    break;
  // To help screen
  case 3:
    screen_option = 3;
    glutMouseFunc(mouse);
    break;
  // Exit the game
  case 4:
    exit(0);
    break;
  }
}

//  ********************
// Main program
int main(int argc, char ** argv) {

  // Basic
  glutInit( & argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(850, 600);
  glutInitWindowPosition(10, 10);
  glutCreateWindow(argv[0]);
  glutSetWindowTitle("Tic Tac Toe 3D");
  init();

  // setting the menu
  glutCreateMenu(menu);
  glutAddMenuEntry("Start Game", 1);
  glutAddMenuEntry("Prespective View", 2);
  glutAddMenuEntry("Help", 3);
  glutAddMenuEntry("Quit", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // Calling the essential functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Timer function to properly spin
  glutTimerFunc(50, TimeEvent, 1);

  glutMainLoop();
  return 0;
}

//  ********************
//  ********************
