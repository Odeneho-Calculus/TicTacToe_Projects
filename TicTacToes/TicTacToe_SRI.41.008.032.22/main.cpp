/*
ANTWI THOMAS
SRI.41.008.038.22
CE200B
*/
#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h> // glut (gl utility toolkit) basic windows functions, keyboard, mouse.
#include <stdio.h>   // standard (I/O library)
#include <stdlib.h>  // standard library (set of standard C functions
#include <math.h>    // Math library (Higher math functions )
#include <string.h>

// to change screen if certain menu option is chosen
int screen_option = 0;

// state variables to switch between Ortho or Perspective view
static int view_state = 0;

int abc = 0;
int mouse_x, mouse_y, Win_x, Win_y, object_select;
int spin, spinboxes;// Use to spin X's and O's
int player, computer, win, start_game;

// row, column, diagonal information
static int box[8][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}

    };

int box_map[9];
// center x,y location for each box
int object_map[9][2] = {
    {-6, 6},
    {0, 6},
    {6, 6},
    {-6, 0},
    {0, 0},
    {6, 0},
    {-6, -6},
    {0, -6},
    {6, -6}};
    // quadric pointer for build our X
GLUquadricObj *Cylinder;
// Begin game routine
void init_game(void)
{
    int i;
    // Clear map for new game
    for (i = 0; i < 9; i++)
    {
        box_map[i] = 0;
    }
    // Set 0 for no winner
    win = 0;
    start_game = 1;
}
// Check for three in a row/column/diagonal
// returns 1 if there is a winner
int check_move(void)
{
    int i, t = 0;
    //Check for three in a row
    for (i = 0; i < 8; i++)
    {
        t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
        if ((t == 3) || (t == -3))
        {
            spinboxes = i;
            return (1);
        }
    }
    t = 0;
    // check for tie
    for (i = 0; i < 8; i++)
    {
        t = t + abs(box_map[box[i][0]]) + abs(box_map[box[i][1]]) + abs(box_map[box[i][2]]);
    }
    if (t == 24)
        return (2);
    return (0);
}
// Do we need to block other player?
int blocking_win(void)
{
    int i, t;
    for (i = 0; i < 8; i++)
    {
        t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
        if ((t == 2) || (t == -2))
        {
            // Find empty
            if (box_map[box[i][0]] == 0)
                box_map[box[i][0]] = computer;
            if (box_map[box[i][1]] == 0)
                box_map[box[i][1]] = computer;
            if (box_map[box[i][2]] == 0)
                box_map[box[i][2]] = computer;
            return (1);
        }
    }
    return (0);
}
// check for a free space in corner
int check_corner(void)
{
    int i;
    if (box_map[0] == 0)
    {
        box_map[0] = computer;
        i = 1;
        return (1);
    }
    if (box_map[2] == 0)
    {
        box_map[2] = computer;
        i = 1;
        return (1);
    }
    if (box_map[6] == 0)
    {
        box_map[6] = computer;
        i = 1;
        return (1);
    }
    if (box_map[8] == 0)
    {
        box_map[8] = computer;
        i = 1;
        return (1);
    }
    return (0);
} // Check for free space in row
int check_row(void)
{
    if (box_map[4] == 0)
    {
        box_map[4] = computer;
        return (1);
    }
    if (box_map[1] == 0)
    {
        box_map[1] = computer;
        return (1);
    }
    if (box_map[3] == 0)
    {
        box_map[3] = computer;
        return (1);
    }
    if (box_map[5] == 0)
    {
        box_map[5] = computer;
        return (1);
    }
    if (box_map[7] == 0)
    {
        box_map[7] = computer;
        return (1);
    }
    return (0);
}
// logic for computer's turn
int computer_move()
{
    if (blocking_win() == 1)
        return (1);
    if (check_row() == 1)
        return (1);
    if (check_corner() == 1)
        return (1);
    return (0);
}
// I use this to put text on the screen
void Sprint(int x, int y, char *st)
{
    int l, i;
    l = strlen(st);         // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for (i = 0; i < l; i++) // loop until i is greater then l
    {

        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }
}
void SPrint(int x, int y, char *st)
{
    int l, i;
    l = strlen(st);         // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for (i = 0; i < l; i++) // loop until i is greater then l
    {

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
}
// This creates the spinning of the cube.
static void TimeEvent(int te)
{
    spin++; // increase cube rotation by 1
    if (spin > 360)
        spin = 180;                 // if over 360 degress, start back at zero.
    glutPostRedisplay();            // Update screen with new rotation data
    glutTimerFunc(8, TimeEvent, 1); // Reset our timmer.
}

// Setup our Opengl world, called once at startup.
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // When screen cleared, use black.
    glShadeModel(GL_SMOOTH);          // How the object color will be rendered smooth or flat
    glEnable(GL_DEPTH_TEST);          // Check depth when rendering
    start_game = 0;
    win = 0;
    // Create a new quadric
    Cylinder = gluNewQuadric();
    gluQuadricDrawStyle(Cylinder, GLU_FILL);
    gluQuadricNormals(Cylinder, GLU_SMOOTH);
    gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
}
void Draw_O(int x, int y, int z, int a)
{
    glPushMatrix();
    glColor3f(1.0, 0.7, 0.0);
    glTranslatef(x, y, z);
    glRotatef(a, 1, 0, 0);
    glutSolidTorus(0.3, 1.0, 10, 300);
    //glutWireTorus(0.3, 1.0, 10, 20);
    glPopMatrix();
}
void Draw_X(int x, int y, int z, int a)
{
    glPushMatrix();
    glColor3f(1.0, .0, .7);
    glTranslatef(x, y, z);
    glPushMatrix();
    glRotatef(a, 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glRotatef(45, 1, 0, 0);
    glTranslatef(0, 0, -2);
    gluCylinder(Cylinder, 0.3, 0.3, 4, 10, 16);
    glPopMatrix();
    glPushMatrix();
    glRotatef(a, 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glRotatef(315, 1, 0, 0);
    glTranslatef(0, 0, -2);
    //gluCylinder(Cylinder, 0.3, 0.3, 4, 10, 16);
    gluCylinder(Cylinder, 0.3, 0.3, 4, 10, 16);
    glPopMatrix();
    glPopMatrix();
}

// Draw our world
void display(void)
{
    if (abc == 3)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen
        glColor3f(0.0, 0.0, 0.0);
        glMatrixMode(GL_PROJECTION);              // Tell opengl that we are doing project matrix work
        glLoadIdentity();                         // Clear the matrix
        glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0); // Setup an Ortho view
        glMatrixMode(GL_MODELVIEW);               // Tell opengl that we are doing model matrix work. (drawing)
        glLoadIdentity();                         // Clear the model matrix
        glDisable(GL_COLOR_MATERIAL);
        //glDisable(GL_LIGHTING);
        glColor3f(0.9, 0.8, 0.7);
        Sprint(-7, 5, "TicTacToe_Beta 1.3.0");
        glColor3f(0.1, 0.2, 0.3);
        Sprint(-7, 4, "By Thomas");
        glColor3f(0.9, 0.8, 0.7);
        Sprint(-7, 2, "Overview");
        glColor3f(0.1, 0.2, 0.3);
        SPrint(-7, 1, "Tic Tac Toe is a two-player game where the first player to");
        SPrint(-7, 0,  "connect a line of pieces from one side or corner of the board to the other wins.");
        glColor3f(0.9, 0.8, 0.7);
        Sprint(-7, -2, "Rules");
        glColor3f(0.1, 0.2, 0.3);
        SPrint(-7, -3, "1.X moves first.");
        SPrint(-7, -4, "2.A piece may be placed on any empty space.");
        SPrint(-7, -5, "3.A player wins by being the first to connect a line of friendly");
        SPrint(-7, -6, "  pieces from one side or corner of the board to the other.");
        SPrint(-7, -7, "4.The game ends when either one player wins or it is no longer possible");
        SPrint(-7, -8, "  for a player to win (in which case the result is a draw).");
        glutSwapBuffers();
    }
    else if (abc == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen
        glMatrixMode(GL_PROJECTION);                        // Tell opengl that we are doing project matrix work
        glLoadIdentity();                                   // Clear the matrix
        glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);           // Setup an Ortho view
        glMatrixMode(GL_MODELVIEW);                         // Tell opengl that we are doing model matrix work. (drawing)
        glLoadIdentity();                                   // Clear the model matrix
        glDisable(GL_COLOR_MATERIAL);
        //glDisable(GL_LIGHTING);
        glColor3f(0.9, 0.8, 0.7);
        Sprint(-3, 0, "TicTacToe_Beta 1.3.0");
        glColor3f(0.7, 0.5, 0.0);
        Sprint(-3, -1, "By Thomas");
        glColor3f(0.8, 0.2, 0.3);
        Sprint(-3, -6, "Right Click for Menu");
        glutSwapBuffers();
    }
    else
    {
        //char txt[30];
        int ix, iy;
        int i;
        int j;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen
        glMatrixMode(GL_PROJECTION);                        // Tell opengl that we are doing project matrix work
        glLoadIdentity();                                   // Clear the matrix
        glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);           // Setup an Ortho view
        glMatrixMode(GL_MODELVIEW);                         // Tell opengl that we are doing model matrix work. (drawing)
        glLoadIdentity();                                   // Clear the model matrix
        glDisable(GL_COLOR_MATERIAL);
        //glDisable(GL_LIGHTING);
        //glColor3i(0,0,1);




        if (win == 1){
            glColor3f(0.0, 0.0, 1.0);
            Sprint(-2, 8, "Congratulations! You Won");
            PlaySound(TEXT("applause1.wav"),NULL,SND_SYNC);
        }else if (win == -1){
            glColor3f(1.0, 0.0, 0.0);
            Sprint(-2, 8, "Computer Won");
            PlaySound(TEXT("loss1.wav"),NULL,SND_SYNC);
        }else if (win == 2){
            glColor3f(0.0, 1.0, 0.0);
            Sprint(-1, 8, "Draw");
            PlaySound(TEXT("draw1.wav"),NULL,SND_SYNC);
        }

                 // Setup view, and print view state on screen
    // for perspective view
    if (view_state == 1) {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-8, 8, "Perspective View");
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(60, 1, 1, 30);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
    // for ortho view
    else {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-8, 8, "Ortho View");
    }
        // Setup view, and print view state on screen
        //glColor3f(0.0, 0.0, 1.0);
        /*
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1, 1, 30);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        */





        //glEnable(GL_LIGHTING);

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


        glutSwapBuffers();

    }

}
// This is called when the window has been resized.
void reshape(int w, int h)
{
    Win_x = w;
    Win_y = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
} // Read the keyboard
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // to switch between views
    case 'v':
    case 'V':
      view_state = abs(view_state - 1);
      break;
    case 27:
        exit(0); // exit program when [ESC] key presseed
        break;
    default:
        break;
    }
}
void mouse(int button, int state, int x, int y)
{
    // We convert windows mouse coords to out openGL coords
    mouse_x = (18 * (float)((float)x / (float)Win_x)) / 6;
    mouse_y = (18 * (float)((float)y / (float)Win_y)) / 6;
    // What square have they clicked in?
    object_select = mouse_x + mouse_y * 3;
    if (start_game == 0)
    {
        if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
        {
            player = 1;
            computer = -1;
            init_game();
            computer_move();
            return;
        }
        if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
        {
            player = -1;
            computer = 1;
            init_game();
            return;
        }
    }
    if (start_game == 1)
    {
        if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
        {
            if (win == 0)
            {
                if (box_map[object_select] == 0)
                {
                    box_map[object_select] = player;
                    win = check_move();
                    if (win == 1)
                    {
                        start_game = 0;
                        return;
                    }

                    computer_move();
                    win = check_move();
                    if (win == 1)
                    {
                        win = -1;
                        start_game = 0;
                    }
                }
            }
        }
    }
    if (win == 2)
        start_game = 0;
}
void menu(int choice)
{
    switch (choice)
    {
    case 1:
        abc = 1;
        glutMouseFunc(mouse);
        break;
        // To change the viewport
    case 2:
        view_state = abs(view_state - 1);
        break;

    case 3:
        abc = 3;
        glutMouseFunc(mouse);
        break;
        // Exit the game
    case 4:
        exit(0);
        break;
    }
}
// Main program
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(850, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow(argv[0]);
    glutSetWindowTitle("Tic Tac Toe");
    init();

     // setting the menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Start game", 1);
    glutAddMenuEntry("Prespective View", 2);
    glutAddMenuEntry("Help", 3);
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    glutTimerFunc(50, TimeEvent, 1);
    glutMainLoop();
    return 0;
}
