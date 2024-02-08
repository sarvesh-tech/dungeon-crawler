#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream file; 
    file.open(fileName); 

    if (!file.is_open()) {
        return nullptr;
    }

    int row;
    int col;
    
    file >> row >> col;
    if(file.fail() || row <= 0 || col <= 0 || row > 999999 || col > 999999){
        return nullptr;
    }
    maxRow = row;
    maxCol = col;

    int playerX;
    int playerY;
    
    file >> playerX >> playerY;
    if(file.fail() || playerX < 0 || playerY < 0 || playerX >= maxRow || playerY >= maxCol){
        return nullptr;
    }
    player.row = playerX;
    player.col = playerY;

    //create the map
    char** genMap = createMap(maxRow, maxCol);

    char point = ' ';
    bool doorPresent = false;
    bool exitPresent = false;
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            file >> point;
            
            if(file.fail()){
                return nullptr;
            }

            if(i == player.row && j == player.col && point == '-'){
                genMap[i][j] = TILE_PLAYER;
            }
            if(point == '$'){
                genMap[i][j] = TILE_TREASURE;
            }
            if(point == '@'){
                genMap[i][j] = TILE_AMULET;
            }
            if(point == 'M'){
                genMap[i][j] = TILE_MONSTER;
            }
            if(point == '+'){
                genMap[i][j] = TILE_PILLAR;
            }
            if(point == '?'){
                genMap[i][j] = TILE_DOOR;
                doorPresent = true;
            }
            if(point == '!'){
                genMap[i][j] = TILE_EXIT;
                exitPresent = true;
            }
        }
    }

    file >> point;

    if(!(file.fail())){
        return nullptr;
    }

    if((doorPresent == false && exitPresent == false)){
        return nullptr;
    }
    return genMap;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    switch (tolower(input)) {
     case MOVE_UP:
          --nextRow;
          break;
     case MOVE_DOWN:
          ++nextRow;
          break;
     case MOVE_LEFT:
          --nextCol;
          break;
     case MOVE_RIGHT:
          ++nextCol;
          break;
     default:
          return;
     }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if (maxRow < 0){
        return nullptr;
    }
    if (maxCol < 0){
        return nullptr;
    }
        
    char** map = new char* [maxRow];
    for (int i = 0; i < maxRow; ++i) {
        map[i] = new char[maxCol];
        for (int j = 0; j < maxCol; ++j){
            map[i][j] = TILE_OPEN;
        }
    }

    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if (map) {
        for (int i = 0; i < maxRow; ++i){
            delete[] map[i];
        }
        delete[] map;
    }

    map = nullptr;
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if (map == nullptr) {
        return nullptr;
    }

    if (maxRow < 1) {
        return nullptr;
    }

    if (maxRow > INT32_MAX / 2) {
        return nullptr;
    }

    if (maxCol < 1) {
        return nullptr;
    }

    if (maxCol > INT32_MAX / 2) {
        return nullptr;
    }

    int rowOriginal = maxRow, colOriginal = maxCol;

    char** newArr = new char* [maxRow * 2];

    for (int i = 0; i < maxRow * 2; ++i){
        newArr[i] = new char[maxCol * 2];
    }
        

    for (int row = 0; row < rowOriginal; ++row){
        for (int col = 0; col < colOriginal; ++col) {
            newArr[row][col] = map[row][col];
            newArr[row][col + colOriginal] = map[row][col] == TILE_PLAYER ? TILE_OPEN : map[row][col];
            newArr[row + rowOriginal][col + colOriginal] = map[row][col] == TILE_PLAYER ? TILE_OPEN : map[row][col];
            newArr[row + rowOriginal][col] = map[row][col] == TILE_PLAYER ? TILE_OPEN : map[row][col];
        }
    }

    deleteMap(map, rowOriginal);
    maxRow *= 2;
    maxCol *= 2;

    return newArr;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if (nextRow < 0 || nextRow >= maxRow || nextCol < 0 || nextCol >= maxCol) {
        return STATUS_STAY;
    }

    int currStatus;

    char nextTile = map[nextRow][nextCol];

    if (nextTile == TILE_PILLAR || nextTile == TILE_MONSTER) {
        return STATUS_STAY;
    }

    else if (nextTile == TILE_TREASURE) {
        player.treasure += 1; 
        currStatus = STATUS_TREASURE;
    }

    else if (nextTile == TILE_AMULET) {
        currStatus = STATUS_AMULET;
    }

    else if (nextTile == TILE_DOOR) {
        currStatus = STATUS_LEAVE;
    }

    else if (nextTile == TILE_EXIT) {
        if (player.treasure >= 1) {
            currStatus = STATUS_ESCAPE;
        } 
        else {
            return STATUS_STAY;
        }
    }

    else if (nextTile == TILE_OPEN){
        currStatus = STATUS_MOVE;
    }

    map[nextRow][nextCol] = TILE_PLAYER;
    map[player.row][player.col] = TILE_OPEN;
    player.row = nextRow;
    player.col = nextCol;

    return currStatus; 
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    if(player.row-1 >= 0){
        for(int i = player.row-1; i >= 0; i--){
            if(map[i][player.col] == TILE_PILLAR){
                break;
            }

            if(map[i][player.col] == TILE_MONSTER){
                map[i][player.col] = TILE_OPEN;
                map[i + 1][player.col] = TILE_MONSTER;
            }
        }
    }

    if(player.row+1 < maxRow){
        for(int i = player.row+1; i < maxRow; i++){
            if(map[i][player.col] == TILE_PILLAR){
                break;
            }
            if(map[i][player.col] == TILE_MONSTER){
                map[i][player.col] = TILE_OPEN;
                map[i - 1][player.col] = TILE_MONSTER;
            }
        }
    }
    
    if(player.col-1 >= 0){
        for(int i = player.col-1; i >= 0; i--){
            if(map[player.row][i] == TILE_PILLAR){
                break;
            }
            if(map[player.row][i] == TILE_MONSTER){
                map[player.row][i] = TILE_OPEN;
                map[player.row][i + 1] = TILE_MONSTER;
            }
        }
    }

    if(player.col+1 < maxCol){
        for(int i = player.col+1; i < maxCol; i++){
            if(map[player.row][i] == TILE_PILLAR){
                break;
            }
            if(map[player.row][i] == TILE_MONSTER){
                map[player.row][i] = TILE_OPEN;
                map[player.row][i - 1] = TILE_MONSTER;
            }
        }
    }

    if(map[player.row][player.col] == TILE_MONSTER){
        return true;
    }

    return false;
}