#include "raylib.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
#define PLAYABLE_BOARD_SEGMENT_COUNT 16

typedef enum GameScreen {LOGO = 0, TITLE, GAMEPLAY, ENDING} GameScreen;
typedef enum GuessColor {RED_GUESS = 0, ORANGE_GUESS, YELLOW_GUESS, GREEN_GUESS, BLUE_GUESS, PURPLE_GUESS, COUNT_GUESS, EMPTY_GUESS} GuessColor;
typedef enum ButtonShape {RECTANGLE, CIRCLE} ButtonShape;
typedef enum ButtonType {CONFIRM, COLORSELECT, GUESS_INPUT} ButtonType;
typedef struct Circle
{
    float radius;
    Vector2 center;
} Circle;

typedef struct Button 
{
    ButtonShape shape;
    ButtonType type;
    Rectangle buttonRect;
    Rectangle innerRect;
    int borderThicknessValue;
    bool isRectBordered;
    Circle buttonCircle;
    Color activeColor;
    Color inactiveColor;
    bool isActive;
    char *text;
    int fontsize;
    Color fontColor;
    int textX;
    int textY;
    GuessColor guessColor;
} Button;

typedef struct Peg
{
    Rectangle rect;
    Rectangle border;
    Color pegColor;
    Color borderColor;
} Peg;

typedef struct Board
{
    Button rectangularGuessButtons[4];
    Rectangle background;
    Peg pegs[4];
    bool isActive;
} Board;

Color GuessColorToRaylibColor (GuessColor input)
{
    switch(input)
    {
        case RED_GUESS:
        {
            return RED;          
        } break;
        case ORANGE_GUESS:
        {
            return ORANGE;
        } break;
        case YELLOW_GUESS:
        {
            return YELLOW;
        } break;
        case GREEN_GUESS:
        {
            return GREEN;
        } break;
        case BLUE_GUESS:
        {
            return BLUE;
        } break;
        case PURPLE_GUESS:
        {
            return PURPLE;
        } break;
        case COUNT_GUESS:
        {
            return PINK;
        } break;
        case EMPTY_GUESS:
        {
            return WHITE;
        }break;

        default: break;
    }
    return PINK;
}

bool isInRectangle(Vector2 position, Rectangle rectangle)
{
    if((position.x >= rectangle.x && position.x <= rectangle.x + rectangle.width) &&
       (position.y >= rectangle.y && position.y <= rectangle.y + rectangle.height))
    {
        return true;
    } 
    else
    {
        return false;
    }
}

Vector2 CreateVector2(float x, float y)
{
    Vector2 result;
    result.x = x;
    result.y = y;
    return result;
}

Rectangle CreateRectangle(int x, int y, int width, int height)
{
    Rectangle result;
    result.x = x;
    result.y = y;
    result.width = width;
    result.height = height;
    return result;
}

Circle CreateCircle(float radius, Vector2 center)
{
    Circle result;
    result.radius = radius;
    result.center = center;
    return result;
}

bool isInCircle(Vector2 position, Circle circle)
{
    int deltaX = position.x - circle.center.x;
    int deltaY = position.y - circle.center.y;
    if(deltaX * deltaX + deltaY * deltaY <= circle.radius * circle.radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
 
bool IsInButton(Vector2 position, Button button)
{
    if(button.shape == RECTANGLE)
    {
        return isInRectangle(position, button.buttonRect);
    }
    if(button.shape == CIRCLE)
    {
        return isInCircle(position, button.buttonCircle);
    }

    return false;
}

void DrawPeg(Peg peg)
{
    DrawRectangleRec(peg.border, peg.borderColor);
    DrawRectangleRec(peg.rect, peg.pegColor);
}

void DrawButton (Button button)
// Change the button logic from looking at the button shape to looking at the type of the button. Reason: This will make the gameplay and the drawing code a little bit easier. 
{
    switch(button.type)
    {
        case GUESS_INPUT:
        {
            if(button.isActive)
            {
                DrawRectangleRec(button.buttonRect, button.activeColor);
            }
            else
            {
                DrawRectangleRec(button.buttonRect, button.inactiveColor);
            }
            DrawRectangleRec(button.innerRect, GuessColorToRaylibColor(button.guessColor));
        }break;
        case COLORSELECT:
        {
        if(button.isActive)
            {
                DrawCircle(button.buttonCircle.center.x, button.buttonCircle.center.y, button.buttonCircle.radius, button.inactiveColor);
                DrawCircle(button.buttonCircle.center.x, button.buttonCircle.center.y, button.buttonCircle.radius*0.9, button.activeColor);
            }
            else
            {
                DrawCircle(button.buttonCircle.center.x, button.buttonCircle.center.y, button.buttonCircle.radius, button.activeColor);
            } 
        }break;
        case CONFIRM:
        {
            if(button.isActive)
            {
                DrawRectangleRec(button.buttonRect, button.activeColor);
            }
            else
            {
                DrawRectangleRec(button.buttonRect, button.inactiveColor);
            }
            DrawText(button.text, button.textX, button.textY, button.fontsize, button.fontColor);
        }break;
        default:{}
    }

}

Board InitializeSolutionBoard (int boardX, int boardWidth, int boardHeight)
{
    Board solutionBoard;
    for(int i = 0; i < 4; i++)
    {
        solutionBoard.rectangularGuessButtons[i].shape = RECTANGLE;
        solutionBoard.rectangularGuessButtons[i].type = GUESS_INPUT;
        solutionBoard.rectangularGuessButtons[i].buttonRect = CreateRectangle(boardX +boardWidth/5*i, 0, boardWidth/5, boardHeight);
        solutionBoard.rectangularGuessButtons[i].borderThicknessValue = 5;
        solutionBoard.rectangularGuessButtons[i].innerRect = CreateRectangle(solutionBoard.rectangularGuessButtons[i].buttonRect.x + solutionBoard.rectangularGuessButtons[i].borderThicknessValue, solutionBoard.rectangularGuessButtons[i].buttonRect.y + solutionBoard.rectangularGuessButtons[i].borderThicknessValue, solutionBoard.rectangularGuessButtons[i].buttonRect.width - 2*solutionBoard.rectangularGuessButtons[i].borderThicknessValue, solutionBoard.rectangularGuessButtons[i].buttonRect.height - 2*solutionBoard.rectangularGuessButtons[i].borderThicknessValue);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        solutionBoard.rectangularGuessButtons[i].isRectBordered = true;
        solutionBoard.rectangularGuessButtons[i].buttonCircle = CreateCircle(0,CreateVector2(0,0)); // Not A Circle
        solutionBoard.rectangularGuessButtons[i].activeColor = WHITE;
        solutionBoard.rectangularGuessButtons[i].inactiveColor = GRAY;
        solutionBoard.rectangularGuessButtons[i].isActive = false;
        solutionBoard.rectangularGuessButtons[i].text = 0;
        solutionBoard.rectangularGuessButtons[i].fontsize = 0;
        solutionBoard.rectangularGuessButtons[i].fontColor = BLACK;
        solutionBoard.rectangularGuessButtons[i].textX = 0;
        solutionBoard.rectangularGuessButtons[i].textY = 0;
        solutionBoard.rectangularGuessButtons[i].guessColor = GetRandomValue(RED_GUESS, COUNT_GUESS-1); //352
    }  
    return solutionBoard;
}

Board InitializePlayableBoardSegment (int boardX, int boardWidth, int boardHeight, int i, int screenHeight)
{
    Board boardSegment;
    for(int j = 0; j < 4; j++)
    {
        boardSegment.rectangularGuessButtons[j].shape = RECTANGLE;
        boardSegment.rectangularGuessButtons[j].type = GUESS_INPUT;
        boardSegment.rectangularGuessButtons[j].buttonRect = CreateRectangle(boardX + boardWidth/5*j, screenHeight - (i+1)*boardHeight, boardWidth/5, boardHeight);
        boardSegment.rectangularGuessButtons[j].borderThicknessValue = 5;
        boardSegment.rectangularGuessButtons[j].innerRect = CreateRectangle(boardSegment.rectangularGuessButtons[j].buttonRect.x + boardSegment.rectangularGuessButtons[j].borderThicknessValue, boardSegment.rectangularGuessButtons[j].buttonRect.y + boardSegment.rectangularGuessButtons[j].borderThicknessValue, boardSegment.rectangularGuessButtons[j].buttonRect.width - 2*boardSegment.rectangularGuessButtons[j].borderThicknessValue, boardSegment.rectangularGuessButtons[j].buttonRect.height - 2*boardSegment.rectangularGuessButtons[j].borderThicknessValue);
        boardSegment.rectangularGuessButtons[j].isRectBordered = true;
        boardSegment.rectangularGuessButtons[j].buttonCircle = CreateCircle(0,CreateVector2(0,0));
        boardSegment.rectangularGuessButtons[j].activeColor = BLACK;
        boardSegment.rectangularGuessButtons[j].inactiveColor = GRAY;
        boardSegment.rectangularGuessButtons[j].isActive = false;
        boardSegment.rectangularGuessButtons[j].text = 0;
        boardSegment.rectangularGuessButtons[j].fontsize = 0;
        boardSegment.rectangularGuessButtons[j].fontColor = BLACK;
        boardSegment.rectangularGuessButtons[j].textX = 0;
        boardSegment.rectangularGuessButtons[j].textY = 0;
        boardSegment.rectangularGuessButtons[j].guessColor = EMPTY_GUESS;

        int pegWidth = (boardWidth/5)/2;
        int pegHeight = boardHeight/2;
        for(int k = 0; k < 4; k++)
        {
            int pegX = boardX + boardWidth/5*4;
            if(k % 2 == 1)
            {
                pegX += pegWidth;
            }
            int pegY = screenHeight - (i+1)*boardHeight;
            if(k > 1)
            {
                pegY += pegHeight;
            }
            boardSegment.pegs[k].border = CreateRectangle(pegX, pegY, pegWidth, pegHeight);
            boardSegment.pegs[k].borderColor = GRAY;
            boardSegment.pegs[k].rect = CreateRectangle(pegX + pegWidth*5/100, pegY + pegHeight*5/100, pegWidth*9/10, pegHeight*9/10);
            boardSegment.pegs[k].pegColor = PINK;

        }

    }
    return boardSegment;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1400;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic screen manager");

    // Color selection box variables
    Rectangle colorSelectionBox = { 0.8*screenWidth,     // top left corner x
                                    0,                   // top left corner y
                                    0.2*screenWidth,     // width
                                    0.75*screenHeight }; // height


    // Button variables
    Button screenButtons[16];
    int screenButtonCount = 0;
    Button confirmButton = {0}; // Sets all the memory to 0 in the struct
    confirmButton.shape = RECTANGLE;
    confirmButton.type = CONFIRM;
    Rectangle confirmButtonRect = { 0.8*screenWidth,     // top left corner x
                                    0.75*screenHeight,   // top left corner y
                                    0.2*screenWidth,     // width
                                    0.25*screenHeight    // height
                                  };
    confirmButton.buttonRect =  confirmButtonRect; 
    confirmButton.activeColor = GREEN;
    confirmButton.inactiveColor = RED;
    confirmButton.isActive = false;
    confirmButton.text = "CONFIRM";
    confirmButton.fontsize = 48,
    confirmButton.fontColor = BLACK,
    confirmButton.textX = 0.82*screenWidth,
    confirmButton.textY = 0.82*screenHeight,
    screenButtons[screenButtonCount] = confirmButton;
    screenButtonCount++;

    // Create color buttons
    float colorSelectionButtonRadius = 1.0/16.0 * screenHeight;
    for( GuessColor i = 0; i < COUNT_GUESS; i++)
    {
        Button colorSelectButton = {0};
        colorSelectButton.shape = CIRCLE;
        colorSelectButton.type = COLORSELECT;
        Vector2 colorSelectButtonCenter = {0.9*screenWidth, colorSelectionButtonRadius + 2*colorSelectionButtonRadius*i};
        Circle colorSelectButtonCircle = {colorSelectionButtonRadius, colorSelectButtonCenter};
        colorSelectButton.buttonCircle = colorSelectButtonCircle;
        colorSelectButton.activeColor = GuessColorToRaylibColor(i);
        colorSelectButton.inactiveColor = BLACK;
        colorSelectButton.isActive = false;
        colorSelectButton.guessColor = i;
        
        screenButtons[screenButtonCount] = colorSelectButton;
        screenButtonCount++;
    }

    int colorButtonSelectionCount = 0;

    //The Board
    int boardX = 0.2*screenWidth;
    int boardWidth = 0.6*screenWidth;
    int boardHeight = 1.0/(PLAYABLE_BOARD_SEGMENT_COUNT + 1)*screenHeight;

    Board solutionBoard = InitializeSolutionBoard(boardX, boardWidth, boardHeight);
    
    Board playableBoard[PLAYABLE_BOARD_SEGMENT_COUNT];
    for (int i = 0; i < PLAYABLE_BOARD_SEGMENT_COUNT; i++)
    {
        
        playableBoard[i] = InitializePlayableBoardSegment(boardX, boardWidth, boardHeight, i, screenHeight);
    }

    int activeBoardSegmentSelectionCount = 0;
    int turnCount = 0;
    bool gameOver = false;
    bool moveConfirmed = false;
    bool playerWon = false;

    GameScreen currentScreen = LOGO;

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!

                framesCounter++;    // Count frames

                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 0)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!
                Board *activeBoardSegment = &playableBoard[turnCount];

                // this code is process if you press the enter key or tap the screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    Vector2 mousePosition = GetMousePosition();
                    //Checking if screenbutton was pressed
                    for(int i = 0; i < screenButtonCount; i++)
                    {
                        Button *currentButton = &screenButtons[i];
                        if(IsInButton(mousePosition, *currentButton))
                        {
                            switch(currentButton->type)
                            {
                                case CONFIRM:
                                {
                                    if(currentButton->isActive)
                                    {
                                        moveConfirmed = true;
                                    }

                                }break;
                                case COLORSELECT:
                                {
                                    if(colorButtonSelectionCount == 0 && activeBoardSegmentSelectionCount == 0)
                                    {
                                        currentButton->isActive = true;
                                        colorButtonSelectionCount = 1;
                                    }
                                    else if(colorButtonSelectionCount == 0 && activeBoardSegmentSelectionCount == 1)
                                    {
                                        Button *activeBoardButton;
                                        for(int j = 0; j < 4; j++)
                                        {
                                            if(activeBoardSegment->rectangularGuessButtons[j].isActive)
                                            {
                                                activeBoardButton = &activeBoardSegment->rectangularGuessButtons[j];
                                                break;
                                            }
                                        }
                                        activeBoardButton->guessColor = currentButton->guessColor;
                                        activeBoardButton->isActive = false;
                                        activeBoardSegmentSelectionCount = 0;
                                    }
                                    else if(colorButtonSelectionCount == 1 && currentButton->isActive)
                                    {
                                        currentButton->isActive = false;
                                        colorButtonSelectionCount = 0;
                                    }
                                }break;
                                default:{};
                            }
                        }
                    }
                    // Loop through the playable board and check if button was pressed there. 
                    for(int i = 0; i < 4; i++)
                    {
                        if(IsInButton(mousePosition, activeBoardSegment->rectangularGuessButtons[i]))
                        {
                            Button *activeBoardButton = &activeBoardSegment->rectangularGuessButtons[i];
                            if(colorButtonSelectionCount > 0 && activeBoardSegmentSelectionCount == 0)  
                            {
                                //active colorselection button non-active boardselection button
                                Button *activeColorSelectionButton = &activeBoardSegment->rectangularGuessButtons[i];
                                for(int j = 0; j < screenButtonCount; j++)
                                {
                                    if(screenButtons[j].type && screenButtons[j].isActive == COLORSELECT)
                                    {
                                        activeColorSelectionButton = &screenButtons[j];
                                        break;
                                    }
                                }
                                activeBoardButton->guessColor = activeColorSelectionButton->guessColor;
                                activeBoardButton->isActive = false;
                                activeColorSelectionButton->isActive = false;
                                colorButtonSelectionCount = 0;
                            }
                            else if(colorButtonSelectionCount == 0 && activeBoardSegmentSelectionCount == 0)
                            {
                                //non-active colorselection button non-active boardselection button
                                activeBoardButton->isActive = true;
                                activeBoardSegmentSelectionCount++;
                            }

                            else if(colorButtonSelectionCount == 0 && activeBoardSegmentSelectionCount == 1)
                            {
                                //non-active colorselection button active boardselection button
                                if(activeBoardButton->isActive)
                                {
                                    activeBoardButton->isActive = false;
                                    activeBoardSegmentSelectionCount = 0;
                                }
                            }
                        }

                    }
                }
                // Check if the confirm button should become active. 
                Button *confirmButton;
                for(int i = 0; i < screenButtonCount; i++)
                {
                    if(screenButtons[i].type == CONFIRM)
                    {
                        confirmButton = &screenButtons[i];
                        break;
                    }
                }
                int currentFilledGuesses = 0;
                for (int i = 0; i < 4; i++)
                {
                    if(activeBoardSegment->rectangularGuessButtons[i].guessColor != EMPTY_GUESS)
                    {
                        currentFilledGuesses++;
                    }
                }
                if(currentFilledGuesses == 4)
                {
                    confirmButton->isActive = true;
                }
                else
                {
                    confirmButton->isActive = false;
                }

                // Update the pegs
                // Process a confirmed move
                if(moveConfirmed)
                {
                    GuessColor solutionColors[4];
                    GuessColor guessedColors[4];
                    int solutionColorsLength = 4;
                    int guessedColorsLength = 4;
                    int redPegCount = 0;
                    int whitePegCount = 0;
                    for(int i=0; i < 4; i++)
                    {
                        solutionColors[i] = solutionBoard.rectangularGuessButtons[i].guessColor;
                        guessedColors[i] = activeBoardSegment->rectangularGuessButtons[i].guessColor;

                    }

                    // Check red pegs
                    int i = 0;
                    while(i<guessedColorsLength && i<solutionColorsLength)
                    {
                        if(guessedColors[i] == solutionColors[i])
                        {
                            for(int j = i+1; j < solutionColorsLength; j++)
                            {
                                solutionColors[j-1] = solutionColors[j];
                                guessedColors[j-1] = guessedColors[j];
                            }
                            solutionColorsLength--;
                            guessedColorsLength--;
                            redPegCount++;
                        }
                        else
                        {
                            i++;
                        }
                    }
                    // Then check white pegs
                    i = 0;
                    int j = 0;
                    while(i<guessedColorsLength)
                    {
                        while(j<solutionColorsLength)
                        {
                            if(guessedColors[i] == solutionColors[j])
                            {
                                for(int k = i+1; k < guessedColorsLength; k++)
                                {
                                    guessedColors[k-1] = guessedColors[k];
                                }
                                for(int k = j+1; k < solutionColorsLength; k++)
                                {
                                    solutionColors[k-1] = solutionColors[k];
                                }
                                guessedColorsLength--;
                                solutionColorsLength--;
                                j = 0;
                                i = 0;
                                whitePegCount++;
                            }
                            else
                            {
                                j++;
                            }
                        }
                        i++;
                        j = 0;
                    }
                    if(redPegCount == 4)
                    {
                        gameOver = true;
                        playerWon = true;
                        
                    }
                    
                    // fill in the pegs on the active board segment
                    for(int i = 0; i < 4; i++)
                    {
                        if(redPegCount > 0)
                        {
                            activeBoardSegment->pegs[i].pegColor = RED;
                            redPegCount--;
                        }
                        else if(whitePegCount > 0)
                        {
                            activeBoardSegment->pegs[i].pegColor = WHITE;
                            whitePegCount--;
                        }
                    }
                    turnCount++;
                    moveConfirmed = false;
                    if(turnCount >= PLAYABLE_BOARD_SEGMENT_COUNT)
                    {
                        gameOver = true;
                    }
                }

                if(gameOver)
                {
                    currentScreen = ENDING;
                }

            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    solutionBoard = InitializeSolutionBoard(boardX, boardWidth, boardHeight);
                    for (int i = 0; i < PLAYABLE_BOARD_SEGMENT_COUNT; i++)
                    {
                        playableBoard[i] = InitializePlayableBoardSegment(boardX, boardWidth, boardHeight, i, screenHeight);
                    }
                    currentScreen = TITLE;
                    gameOver = false;
                    turnCount = 0;
                    playerWon = false;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                    DrawText("WELCOME TO MASTERMIND", 120, 220, 40, BLACK);

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    playableBoard[turnCount].isActive = true;

                    DrawRectangle(0, 0, screenWidth, screenHeight, BEIGE);
                    DrawText("", 20, 20, 40, RED);
                    DrawText("", 130, 220, 20, RED);

                    DrawRectangleRec(colorSelectionBox, WHITE);

                    // Draw all the buttons
                    for(int i = 0; i < screenButtonCount; i++)
                    {
                        DrawButton(screenButtons[i]);
                    }

                    for(int i = 0; i < PLAYABLE_BOARD_SEGMENT_COUNT; i++)
                    {
                        for(int j = 0; j < 4; j++)
                        {
                            DrawButton(playableBoard[i].rectangularGuessButtons[j]);
                            DrawPeg(playableBoard[i].pegs[j]);
                        }
                    }

                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                    if(playerWon)
                    {
                        DrawText("CONGRADULATIONS! YOU WON! YOU'RE A SKIBIDI SIGMA! ", 120, 220, 40, BLACK);
                    }
                    else
                    {
                        DrawText("You Lost. Not Skibidi. ", 120, 220, 40, BLACK);
                    }
                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
