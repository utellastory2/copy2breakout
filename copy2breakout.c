//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 401

// paddle dimensions in pixels
#define PAD_WDTH 60
#define PAD_THCK 6

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void freeEvent(GEvent event);
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);

GLabel initScoreboard(GWindow window);

GLabel initBricksLabel(GWindow window);
GLabel initRemainingBricks(GWindow window);

GLabel initScoreLabel(GWindow window);
GLabel initScore(GWindow window);

GLabel initLivesLabel(GWindow window);
GLabel initRemainingLives(GWindow window);

GLabel initThatsAll(GWindow window);
GLabel initGameOver(GWindow window);
GLabel initWinner(GWindow window);

void updateScoreboard2(GWindow window, GLabel label, int bricks);
void updateScoreboard1(GWindow window, GLabel label, int points);
void updateScoreboard3(GWindow window, GLabel label, int lives);
GObject detectCollision(GWindow window, GOval ball);
void removeGWindow(GWindow window, GObject gobj);
// void step1(GObject brick, double y_velocity, double x_velocity, int points);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered at top of window. 
    GLabel label = initScoreboard(window);
    
    GLabel bricks_label = initBricksLabel(window);
    GLabel brck_remain = initRemainingBricks(window);
    
    GLabel score_label = initScoreLabel(window);
    GLabel score = initScore(window);
    
    GLabel lives_label = initLivesLabel(window);
    GLabel livs_remain = initRemainingLives(window);
    
    // GLabel t_all_label = initThatsAll(window);
    // GLabel winner_label = initThatsAll(window);

    // number of bricks initially
    int bricks = COLS * ROWS;
    printf("\n\nline 100 intialize brick count");
    printf("                 line 101 bricks = %i\n\n\n", bricks);

    // number of lives initially
    int lives = LIVES;
    int lives_left = 3;

    // number of points initially
    int points = 0;
    
    // ball speed
    double y_velocity = 3.0;
    double x_velocity = 1.5 * drand48();
    
    updateScoreboard2(window, brck_remain, bricks);
                    
    updateScoreboard1(window, score, points);
                    
    updateScoreboard3(window, livs_remain, lives_left);
    
    // wait for click before starting
    waitForClick();
    
    int cnt_while = 0;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
        // TODO while loop
    {
        cnt_while = cnt_while + 1;
        // printf("cnt_while = %i", cnt_while);
        
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows cursor
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, 555);
                // printf("paddle X = %f", x);
                freeEvent(event);
            }
        }
        printf("\ncnt_while = %i\n", cnt_while);
        // printf("\nbegin step 2\n");
        move(ball, x_velocity, y_velocity);
        pause(10);
        // printf("\nbegin step 3\n");    
        // GObject detectCollision(GWindow window, GOval ball);
        GObject object = detectCollision(window, ball);
         
        
        
        
        
        
        
        
        
        
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            x_velocity = -x_velocity;
            printf("line 246 check for R edge collisions");
        }
        else if (getX(ball) <= 0)
        {
            x_velocity = -x_velocity;
            printf("line 251 check for L edge collisions "); 
        }
        // we've past the paddle - lose a turn OR GAME OVER
        else if (getY(ball) + getHeight(ball) >= 590)
        {
            lives = lives - 1;
            printf("line 257 check for B edge collisions");
            
            lives_left = lives;
            updateScoreboard3(window, livs_remain, lives_left);
            
            // check 4 unsuccessful game over
            if (lives == 0)
            {
                // remove ball
                removeGWindow(window, ball);
                
                // new ball in the center and game over messages
                ball = initBall(window);
                GLabel t_all_label = initThatsAll(window);
                GLabel g_over_label = initGameOver(window);
                
                freeGObject(t_all_label);
                freeGObject(g_over_label);

                y_velocity = 0;
                x_velocity = 0;
            }
            // lose a turn
            else
            {
                // we've hit bottom w/ lives left so update scoreboard
                // pause remove ball
                
                pause(1250);
                removeGWindow(window, ball);
                
                // get new ball 
                ball = initBall(window);
                
                waitForClick();
                
                y_velocity = 3.0;
                x_velocity = 1.5 * drand48();
                        
                move(ball, x_velocity, y_velocity);
            }
        }
        
        
        
        else if (getY(ball) <= 0)
        { 
            // we've hit the top 
            y_velocity = -y_velocity;
            printf("line 228 check for TOP edge collisions");
            
            
            object = detectCollision(window, ball);
            if (object != NULL)
            {
                if (strcmp(getType(object), "GRect") == 0) 
                {
                // we've hit a brick so remove it
                printf("we've hit a brick coming down so remove it & bounce");
                removeGWindow(window, object);
            
                double y = getY(object);
                printf("\ny = getY(object) = %.0f\n", y);
                freeGObject(object);
                    if (y > 60)
                    printf("y = %.0f\n\n", y);
                    {
                        pause(150);
                        y_velocity = -3.00;
                        x_velocity = 1.75 * drand48();
                        
                        // y_velocity = -y_velocity;
                        points = points + 5;
                        printf("line 172 y_velocity = %f\n", y_velocity);
                    }
                // we've hit a brick so remove it
                // printf("we've hit a brick coming down so remove it & bounce");
                // removeGWindow(window, object);
                
                
                }
            }
            
            
        }   
        
        
        
        
        
        
        
        
        // object = detectCollision(window, ball);
        
        if (object == paddle)
        {
            y_velocity = -y_velocity; 
        }
        else if (object != NULL)
        {
            if (strcmp(getType(object), "GRect") == 0) 
            {
                // we've hit a brick so remove it
                removeGWindow(window, object);
                    
                double y = getY(object);
                freeGObject(object);
                // printf("line 162 y_velocity = %f", y_velocity);
                // printf("line 161 just removed a brick at y = %f\n", y); 
                // printf("line 162 y_velocity = %f\n", y_velocity);
// void step1(GObject brick, double y_velocity, double x_velocity, int points);
                
                printf("\ny = getY(object) = %.0f\n", y);
                    
                if (y < 60)
                {
                    printf("y = %.0f\n\n", y);
                    pause(150);
                    y_velocity = 6.00;
                    x_velocity = 2.00 * drand48();
                    
                    // y_velocity = -y_velocity;
                    points = points + 5;
                    printf("line 172 y_velocity = %f\n", y_velocity);
                }
                else if (y == 64)
                {
                    pause(150);
                    y_velocity = 5.25;
                    x_velocity = 1.75 * drand48();
                    
                    // y_velocity = -y_velocity;
                    points = points + 4;
                    printf("line 181 y_velocity = %f\n", y_velocity);
                } 
                else if (y == 78)
                {
                    y_velocity = 4.5;
                    x_velocity = 1.5 * drand48();
                    
                    // y_velocity = -y_velocity;
                    points = points + 3;
                    printf("line 190 y_velocity = %f\n", y_velocity);
                } 
                else if (y == 92)
                {
                    y_velocity = 3.75;
                    x_velocity = 1.25 * drand48();

                    // y_velocity = -y_velocity;
                    points = points + 2;
                    printf("line 199 y_velocity = %f\n", y_velocity);
                }
                else if (y == 106)
                {
                    y_velocity = -y_velocity;
                    points = points + 1;
                    // printf("line 205 y_velocity = %f\n", y_velocity);
                    // printf("line 325 y = getY(object) = %f\n", y);
                }
                
                
                // printf("line 199 y = %f\n", y);
                
                
                
                 
                if (bricks == 1)
                {
                    // remove ball
                    removeGWindow(window, ball);
                        
                    pause(1200);
                        
                    // new ball in the center
                    ball = initBall(window);
                        
                    // game over
                    GLabel winner_label = initWinner(window);
                    GLabel g_over_label = initGameOver(window);
                    freeGObject(g_over_label);
                    freeGObject(winner_label);
                }
                
                bricks = bricks - 1;
                lives_left = lives;
                
                printf("\n\nmaking updates bricks = %i lives = %i\n", bricks, lives);
                // printf("line num 331  y_velocity = %f\n", y_velocity);    

                updateScoreboard2(window, brck_remain, bricks);
                    
                updateScoreboard1(window, score, points);
                    
                updateScoreboard3(window, livs_remain, lives_left);
                // printf("before leaving bracket 342 on line 341\n"); 
                // printf("line num 341  y_velocity = %f\n", y_velocity);
            
            
            }
            
        } 
        
        
    }
    // Leaving while loop
    // wait for click before exiting
    waitForClick();

    // game over so free memory
    freeGObject(ball);
    freeGObject(paddle);
    freeGObject(label);
    freeGObject(score);
    freeGObject(bricks_label);
    freeGObject(score_label);
    freeGObject(lives_label);
    // freeGObject(t_all_label);
    // freeGObject(g_over_label);
    // freeGObject(winner_label);
    freeGObject(brck_remain);
    freeGObject(livs_remain);
    freeGObject(window);
    // freeGObject(object);
    freeGRect(brick);
    
    // game over
    closeGWindow(window);
    // freeGObject(window);

    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO •newGRect(x, y, width, height)
    // printf("This function draws a grid of 50 bricks."); 
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(2 + j * 40 , 50 + i * 14, 36, 10);
            setFilled(brick, true);
            
            if (i == 0)
            {
                setColor(brick, "RED");
            }
            else if (i == 1)
            {
                setColor(brick, "GREEN");
            }
            else if (i == 2)
            {
                setColor(brick, "BLUE");
            }
            else if (i == 3)
            {
                setColor(brick, "YELLOW");
            }
            else if (i == 4)
            {
                setColor(brick, "CYAN");
            } 
               
            add(window, brick);
            
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    // printf("This function draws a green ball at (190, 290).\n");
    GOval ball = newGOval(190, 290, 20, 20);
    setFilled(ball, true);
    setColor(ball, "GREEN");
    add(window, ball);
      
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // printf("This function draws a blue paddle at (170, 547).\n");
    GRect paddle = newGRect(170, 547, PAD_WDTH, PAD_THCK);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");

    add(window, paddle);
     
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    // printf("This function prints SCOREBOARD at the top of the window");
    GLabel label = newGLabel("SCOREBOARD");
    setFont(label, "SansSerif-15");
    double x = (getWidth(window) - getWidth(label)) / 2;
    setLocation(label, x, 20);
    add(window, label);

    return label;
 }
 
GLabel initBricksLabel(GWindow window)
{
    // TODO
    // This function prints BRICKS LEFT at the top of 
    // the window in the 1st 3rd of pixcel line 40.

    GLabel bricks_label = newGLabel("Bricks Left = ");
    setFont(bricks_label, "SansSerif-15");
    setLocation(bricks_label, 15, 40);
    add(window, bricks_label);

    return bricks_label;
 }
 
GLabel initScoreLabel(GWindow window)
{
    // TODO
    // This function prints SCORE at the top of 
    // the window in the 2nd 3rd of pixcel line 40.

    GLabel score_label = newGLabel("Score = ");
    setFont(score_label, "SansSerif-15");
    setLocation(score_label, 160, 40);
    add(window, score_label);

    return score_label;
 }
   
GLabel initLivesLabel(GWindow window)
{
    // TODO
    // This function prints Lives LEFT at the top of 
    // the window in the 1st 3rd of pixcel line 40.

    GLabel lives_label = newGLabel("Lives Left = ");
    setFont(lives_label, "SansSerif-15");
    setLocation(lives_label, 285, 40);
    add(window, lives_label);

    return lives_label;
 }
 
GLabel initThatsAll(GWindow window)
{
    // TODO
    // This function prints "That's All Folks" at the top of 
    // the window in the 1st 3rd of pixcel line 40.

    GLabel t_all_label = newGLabel("'That's All Folks'");
    setFont(t_all_label, "SansSerif-25");
    setLocation(t_all_label, 100, 270);
    add(window, t_all_label);

    return t_all_label;
 }
 
GLabel initGameOver(GWindow window)
{
    // TODO
    // This function prints "GAME OVER" in the middle of 
    // the window just below the ball.

    GLabel g_over_label = newGLabel("'GAME OVER'");
    setFont(g_over_label, "SansSerif-30");
    setLocation(g_over_label, 100, 350);
    add(window, g_over_label);

    return g_over_label;
 }
 
GLabel initWinner(GWindow window)
{
    // TODO
    // This function prints "YOU ARE A WINNER" in the middle of 
    // the window just ABOVE the ball.

    GLabel winner_label = newGLabel("'YOU ARE A WINNER'");
    setFont(winner_label, "SansSerif-30");
    setLocation(winner_label, 50, 270);
    add(window, winner_label);

    return winner_label;
 }
 
/**
* Instantiates, configures, and returns label for remaining bricks.
*/     
GLabel initRemainingBricks(GWindow window)
{  
    GLabel brck_remain = newGLabel("");
    setFont(brck_remain, "SansSerif-15");
    add(window, brck_remain);

    return brck_remain;
} 
   
/**
* Instantiates, configures, and returns label for score.
*/     
GLabel initScore(GWindow window)
{  
    // double score_x;
    GLabel score = newGLabel("");
    setFont(score, "SansSerif-15");
    add(window, score);

    return score;
    }

/**
* Instantiates, configures, and returns label for remaining lives.
*/     
GLabel initRemainingLives(GWindow window)
{  
    GLabel livs_remain = newGLabel("");
    setFont(livs_remain, "SansSerif-15");
    add(window, livs_remain);
    
    return livs_remain;
}

/**
* Updates scoreboard2's label, keeping it centered 2nd 3rd of window.
*/
void updateScoreboard2(GWindow window, GLabel brck_remain, int bricks)
{
    // update score label
    
    char b[12];
    sprintf(b, "%i", bricks);
    setLabel(brck_remain, b);

    setLocation(brck_remain, 115, 40);
}

/**
* Updates scoreboard1's label, keeping it centered 1st 3rd of window.
*/
void updateScoreboard1(GWindow window, GLabel score, int points)
{
    // update score label
    
    char s[12];
    sprintf(s, "%i", points);
    setLabel(score, s);

    setLocation(score, 225, 40);
}

/**
* Updates scoreboard's label, keeping it centered 1st 3rd of window.
*/
void updateScoreboard3(GWindow window, GLabel livs_remain, int lives_left)
{
    // update score label
    
    char l[12];
    sprintf(l, "%i", lives_left);
    setLabel(livs_remain, l);

    setLocation(livs_remain, 375, 40);
}


/*


void step1(GObject brick, double y_velocity, double x_velocity, int points)
{
    double y = getY(brick);
    if (y == 50)
    {
        pause(500);
        y_velocity = -6.00;
        x_velocity = 2.00 * drand48();
        
        y_velocity = -y_velocity;
        points = points + 5;
       // printf("line 172 y_velocity = %f\n", y_velocity);
    }
    else if (y == 64)
    {
        pause(250);
        y_velocity = -5.25;
        x_velocity = 1.75 * drand48();
        
        y_velocity = -y_velocity;
        points = points + 4;
        // printf("line 181 y_velocity = %f\n", y_velocity);
    } 
    else if (y == 78)
    {
        y_velocity = -4.5;
        x_velocity = 1.5 * drand48();
        
        y_velocity = -y_velocity;
        points = points + 3;
        // printf("line 190 y_velocity = %f\n", y_velocity);
    } 
    else if (y == 92)
    {
        y_velocity = -3.75;
        x_velocity = 1.25 * drand48();

        points = points + 2;
        y_velocity = -y_velocity;
        // printf("line 199 y_velocity = %f\n", y_velocity);
    }
    else if (y == 106)
    {
        y_velocity = -y_velocity;
        points = points + 1;
        // printf("line 205 y_velocity = %f\n", y_velocity);
    }
    
    // printf("line 199 y = %f\n", y);
    
        if (bricks == 1)
        {
            // remove ball
            removeGWindow(window, ball);
                
            pause(1200);
                
            // new ball in the center
            ball = initBall(window);
                
            // game over
            GLabel winner_label = initWinner(window);
            GLabel g_over_label = initGameOver(window);
        }
        
        bricks = bricks - 1;
        lives_left = lives;
        
        printf("\n\nmaking updates bricks = %i lives = %i\n", bricks, lives);
        // printf("line num 331  y_velocity = %f\n", y_velocity);    

        updateScoreboard2(window, brck_remain, bricks);
            
        updateScoreboard1(window, score, points);
            
        updateScoreboard3(window, livs_remain, lives_left);
        // printf("before leaving bracket 342 on line 341\n"); 
        // printf("line num 341  y_velocity = %f\n", y_velocity);
    

}

*/

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);
    printf("y = getY(ball) = %f\n", y);

    // checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
