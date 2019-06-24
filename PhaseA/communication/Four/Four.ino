// Send string in the format
// H111V1011 (2 by 3 maze)
// that will print
//  --- --- ---
// |   |       |
//  ---     ---
// |   |   |   |
//  --- --- ---


#define ROWS 2 // Number of rows in maze
#define COLS 3 // Number of cols in maze

#define H_WALL_ROWS ROWS - 1
#define H_WALL_COLS COLS
#define V_WALL_ROWS ROWS
#define V_WALL_COLS COLS - 1

#define VERT_EXIST_VISUAL '|'
#define VERT_NO_EXIST_VISUAL ' '
#define HORI_EXIST_VISUAL "---"
#define HORI_NO_EXIST_VISUAL "   "


bool vWall[V_WALL_ROWS][V_WALL_COLS];
bool hWall[H_WALL_ROWS][H_WALL_COLS];


void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop()
{

    if (Serial1.available())
    {
        String s = Serial1.readString();
        Serial.println(s);

        int i = 0;

        // Read H array
        if (s[i] == 'H')
        {
            ++i;
            for (int row = 0; row < H_WALL_ROWS; ++row)
            {
                for (int col = 0; col < H_WALL_COLS; ++col)
                {
                    hWall[row][col] = (s[i] == '1');
                    ++i;
                }
            }
        }

        // Read V array
        if (s[i] == 'V')
        {
            ++i;
            for (int row = 0; row < V_WALL_ROWS; ++row)
            {
                for (int col = 0; col < V_WALL_COLS; ++col)
                {
                    vWall[row][col] = (s[i] == '1');
                    ++i;
                }
            }
        }

        printMaze();
    }
}

void printMaze()
{
    // print top wall
    printHorizontalWallLine();


    // print vert, hori, vert, hori, ...
    for (int row = 0; row < ROWS * 2 - 1; ++row)
    {
        if (row % 2 == 0)
            printVert(row / 2);
        else
            printHori(row / 2);
    }

    // print bot wall
    printHorizontalWallLine();
}

void printHorizontalWallLine()
{
    // print top wall
    for (int col = 0; col < H_WALL_COLS; ++col)
    {
        Serial.print(" ");
        Serial.print(HORI_EXIST_VISUAL);
    }
    Serial.println();
}

void printVert(int row)
{
    Serial.print("|");
    for (int col = 0; col < V_WALL_COLS; ++col)
    {
        Serial.print("   ");
        Serial.print(vWall[row][col] ? VERT_EXIST_VISUAL : VERT_NO_EXIST_VISUAL);
    }
    Serial.print("   |");
    Serial.println("");
}

void printHori(int row)
{
    for (int col = 0; col < H_WALL_COLS; ++col)
    {
        Serial.print(" ");
        Serial.print((hWall[row][col] ? HORI_EXIST_VISUAL : HORI_NO_EXIST_VISUAL));
    }
    Serial.println("");
}
