//
//  main.cpp
//  p_1_281
//
//  Created by Helena Grobel on 9/1/22.
//
//IDENTIFIER  = B99292359FFD910ED13A7E6C7F9705B8742F0D79
//uniqname = hgrobel

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>
#include <stack>
#include <queue>
#include <string>
#include <sstream>


#include "getopt.h"
#include "xcode_redirect.hpp"
//#include "input-parse.hpp"
//#include "com-line.hpp"
//#include "algorithm.hpp"




using namespace std;


class Game {
    
private:
    
    bool isStackMode = false;
    char mode = 'M';
    string inputMode = "M";
    struct singleTile{

        singleTile() {} //default ctor
        bool wasDiscovered = false;
        bool isStart = false;
        char spaceType = '.';
    //    uint32_t startArray[3]; do this to speed up runtime!!
        uint32_t startArray[3] = {0, 0, 0};
        uint32_t ancestorTile[3] = {0, 0, 0};
    };
            
    std::deque<singleTile> searchContainer;
    std::vector<std::vector<std::vector<singleTile>>> mapVector;
    uint32_t startRoom, startRow, startCol, total_row,
    total_col, total_room = 0;
    bool queenFound = false;
    int discoveredTiles = 1;
    singleTile cherry;
    
    
public:
    
    void updateStruct(struct singleTile &tile, char c,
                      uint32_t room, uint32_t row, uint32_t col) {
        //creates a new tile for every space and adds it to the map vector
        tile.spaceType = c;
        tile.startArray[0] = room;
        tile.startArray[1] = row;
        tile.startArray[2] = col;
        if(c == 'S') {
            tile.isStart = true;
            tile.wasDiscovered = true;
            searchContainer.push_back(tile);
        }
        mapVector[room][row][col] = tile;
    }



    std::vector<std::vector<std::vector<singleTile>>> fillVector() {
     //parses the txt file input and fills 3D vector
        int linecount = 0;
        std::string line;
        

        while(linecount <=2) {
            getline(std::cin, line);
            linecount++;
            if (linecount == 1) {
                inputMode = line;
            }
            if (linecount == 2) {
                total_room = static_cast<uint32_t >(stoi(line));
                mapVector.resize(total_room); //reserve
            }
            if (linecount == 3) {
                total_col = static_cast<uint32_t >(stoi(line));
                total_row = static_cast<uint32_t >(stoi(line));
                for(uint32_t  i = 0; i < total_room; i++) {
                    mapVector[i].resize(total_row); //reserve
                    for(uint32_t  j = 0; j < total_row; j++) {
                        mapVector[i][j].resize(total_col);
                    }
                }
            }
            for(uint32_t  i = 0; i < total_room; i++) {
                for(uint32_t  j = 0; j < total_row; j++) {
                    for(uint32_t  x = 0; x < total_col; x++) {
                        mapVector[i][j][x].startArray[0] = i;
                        mapVector[i][j][x].startArray[1] = j;
                        mapVector[i][j][x].startArray[2] = x;
                    }
                }
            }
            
        }
        //handling everything after the first 3 lines
        //going into map or list mode
        char newVal;
        std::string new_line;
        uint32_t curr_room = 0;
        uint32_t curr_row = 0;
        uint32_t curr_col = 0;
        while (std::cin >> newVal) {
            struct singleTile tile;
            
            if(inputMode == "M") {
                //map mode
                
                if(newVal == '/') {
                    curr_row = 0;
                    getline(std::cin, new_line);
                    continue;
                    
                }
                if(newVal != '.' && newVal != '#' && newVal != '!'
                   && newVal != '0' && newVal != '1' && newVal != '2'
                   && newVal != '3' && newVal != '4' && newVal != '5'
                   && newVal != '6' && newVal != '7' && newVal != '8'
                   && newVal != '9' && newVal != 'S' && newVal != 'C') {
                    cerr << "Unknown map character" << endl;
                    exit(1);
                        //accounts for incorrect characters
                    
                }
                
                
                
                curr_col++;
                if((curr_col - 1) < total_col && curr_row < total_row) {
                    updateStruct(tile, newVal, curr_room,
                                 curr_row, (curr_col - 1));
                }
                if(curr_row == (total_row - 1) &&
                   curr_room < (total_room - 1) &&
                   curr_col == total_col) {
                    curr_room++;
                }
                if(curr_col == total_col) {
                    curr_col = 0;
                    curr_row++;
                }
 
                
            }
            else {
                //if List mode is on
                if(newVal == '/') {
                    getline(std::cin, new_line);
                }
                if(newVal == '(') {
                    string values [4];
                    string value;
                    uint32_t j = 0;
                    getline(std::cin, new_line);
                    for(uint32_t i = 0; i < new_line.size(); i++) {
                        if(new_line[i] != ',' && new_line[i] != ')') {
                            value += string(1, new_line[i]);
                        }
                        else if(new_line[i] == ',' || new_line[i] == ')') {
                            values[j] = value;
                            value = "";
                            j++;
                        }
                        
                        
                    }

                    
                    uint32_t room = static_cast<uint32_t>(stoi(values[0]));
                    uint32_t row = static_cast<uint32_t>(stoi(values[1]));
                    uint32_t col = static_cast<uint32_t>(stoi(values[2]));
                    char type = new_line[new_line.size() - 2];
                    if(room > (total_room - 1)) {
                        cerr << "Invalid room number" << endl;
                        exit(1);
                    }
                    if(row > (total_row - 1)) {
                        cerr << "Invalid row number" << endl;
                        exit(1);
                    }
                    if(col > (total_col - 1)) {
                        cerr << "Invalid col number" << endl;
                        exit(1);
                    }
                    if(type != '.' && type != '#' && type != '!'
                       && type != '0' && type != '1' && type != '2'
                       && type != '3' && type != '4' && type != '5'
                       && type != '6' && type != '7' && type != '8'
                       && type != '9' && type != 'S' && type != 'C') {
                        cerr << "Unknown map character" << endl;
                        exit(1);
                            //accounts for incorrect characters
                        
                    }
                    
                    updateStruct(tile, type, room, row, col);
                    
                }
                if(newVal == ')') {
                    continue;
                }

            }

        }
        
        return mapVector;
    }
    
    void solve() {
        
        mapVector = fillVector();
        
        //find starting location
//        singleTile starter = searchContainer.front();
        startRoom = searchContainer.front().startArray[0];
        startRow = searchContainer.front().startArray[1];
        startCol = searchContainer.front().startArray[2];
        singleTile placeHolder, north, south, east, west, warpLocation;
        while(searchContainer.empty() == false && queenFound == false) {
            
            if(isStackMode == true) { //if stack mode (LIFO)
                placeHolder = searchContainer.back();
                searchContainer.pop_back();
            }
            else { //if queue mode (FIFO)
                placeHolder = searchContainer.front();
                searchContainer.pop_front();
            }
            uint32_t currRoom = placeHolder.startArray[0];
            uint32_t currRow = placeHolder.startArray[1];
            uint32_t currCol = placeHolder.startArray[2];

            
            //warp pipe
            if((placeHolder.spaceType == '0' || placeHolder.spaceType == '1' ||
               placeHolder.spaceType == '2' || placeHolder.spaceType == '3' ||
               placeHolder.spaceType == '4' || placeHolder.spaceType == '5' ||
               placeHolder.spaceType == '6' || placeHolder.spaceType == '7' ||
               placeHolder.spaceType == '8' || placeHolder.spaceType == '9') &&
               (((static_cast<uint32_t>(placeHolder.spaceType - '0')) < total_room) &&
               (mapVector[static_cast<uint32_t>(placeHolder.spaceType - '0')][currRow][currCol].wasDiscovered == false))) {
                //add the corresponding position that the pipe leads to, i.e., the same row/col position but with the specified room number.

                int warp = placeHolder.spaceType - '0';
                warpLocation = mapVector[static_cast<uint32_t>(warp)][currRow][currCol];

                if(warpLocation.spaceType == 'C') {
                    
                    cherry = warpLocation;
                    cherry.spaceType = 'C';
                    cherry.ancestorTile[0] = placeHolder.startArray[0];
                    cherry.ancestorTile[1] = placeHolder.startArray[1];
                    cherry.ancestorTile[2] = placeHolder.startArray[2];
                    cherry.wasDiscovered = true;
                    queenFound = true;
                    return;
                }
                if(warpLocation.wasDiscovered == false && warpLocation.spaceType != '#'
                   && warpLocation.spaceType != '!' && static_cast<uint32_t>(warp) < total_room) {
//                    mapVector[static_cast<uint32_t>(warp)][currRow][currCol].wasDiscovered = true;
                    mapVector[static_cast<uint32_t>(warp)][currRow][currCol].wasDiscovered = true;
                    discoveredTiles++;
                    mapVector[static_cast<uint32_t>(warp)][currRow][currCol].ancestorTile[0] = placeHolder.startArray[0];
                    mapVector[static_cast<uint32_t>(warp)][currRow][currCol].ancestorTile[1] = placeHolder.startArray[1];
                    mapVector[static_cast<uint32_t>(warp)][currRow][currCol].ancestorTile[2] = placeHolder.startArray[2];
                    searchContainer.push_back(mapVector[static_cast<uint32_t>(warp)][currRow][currCol]);
                    
                }
                

            }
            else if ((placeHolder.spaceType == '0' || placeHolder.spaceType == '1' ||
                      placeHolder.spaceType == '2' || placeHolder.spaceType == '3' ||
                      placeHolder.spaceType == '4' || placeHolder.spaceType == '5' ||
                      placeHolder.spaceType == '6' || placeHolder.spaceType == '7' ||
                      placeHolder.spaceType == '8' || placeHolder.spaceType == '9') &&
                     ((((static_cast<uint32_t>(placeHolder.spaceType - '0')) >= total_room) ||
                      (mapVector[static_cast<uint32_t>(placeHolder.spaceType - '0')][currRow][currCol].wasDiscovered == true) ||
                      (mapVector[static_cast<uint32_t>(placeHolder.spaceType - '0')][currRow][currCol].spaceType == '!') ||
                      (mapVector[static_cast<uint32_t>(placeHolder.spaceType - '0')][currRow][currCol].spaceType == '#')))){
                continue;
                
            }
            else {
                //check north
                bool north_ex = false;
                if((static_cast<int>(currRow) - 1) >= 0) {
                    north_ex = true;
                    north = mapVector[currRoom][currRow - 1][currCol];
                }
                else {
                    north = placeHolder;
                }

                if(north.spaceType != '#' && north.wasDiscovered == false
                   && north_ex == true && north.spaceType != '!') { // change this so it's "if it is a walkable space type'
                    mapVector[currRoom][currRow - 1][currCol].wasDiscovered = true;
                    discoveredTiles++;
                    
                    if(north.spaceType == 'C') {
                        cherry = mapVector[currRoom][currRow - 1][currCol];
                        cherry.spaceType = 'C';
                        cherry.ancestorTile[0] = placeHolder.startArray[0];
                        cherry.ancestorTile[1] = placeHolder.startArray[1];
                        cherry.ancestorTile[2] = placeHolder.startArray[2];
                        cherry.wasDiscovered = true;
                        queenFound = true;
                        return;
                    }
                    mapVector[currRoom][currRow - 1][currCol].ancestorTile[0] = placeHolder.startArray[0];
                    mapVector[currRoom][currRow - 1][currCol].ancestorTile[1] = placeHolder.startArray[1];
                    mapVector[currRoom][currRow - 1][currCol].ancestorTile[2] = placeHolder.startArray[2];
                    searchContainer.push_back(mapVector[currRoom][currRow - 1][currCol]);
                    
                }
                //check east
                bool east_ex = false;
                if(((static_cast<int>(currCol)) + 1) < (static_cast<int>(total_col))) {
                    east_ex = true;
                    east = mapVector[currRoom][currRow][currCol + 1];
                }
                else {
                    east = placeHolder;

                }

                if(east.spaceType != '#' && east.wasDiscovered == false
                   && east_ex == true && east.spaceType != '!') { // change this so it's "if it is a walkable space type'
                    mapVector[currRoom][currRow][currCol + 1].wasDiscovered = true;
                    discoveredTiles++;
                    
                    if(east.spaceType == 'C') {
                        cherry = mapVector[currRoom][currRow][currCol + 1];
                        cherry.spaceType = 'C';
                        cherry.ancestorTile[0] = placeHolder.startArray[0];
                        cherry.ancestorTile[1] = placeHolder.startArray[1];
                        cherry.ancestorTile[2] = placeHolder.startArray[2];
                        cherry.wasDiscovered = true;
                        queenFound = true;
                        return;
                    }
                    mapVector[currRoom][currRow][currCol + 1].ancestorTile[0] = placeHolder.startArray[0];
                    mapVector[currRoom][currRow][currCol + 1].ancestorTile[1] = placeHolder.startArray[1];
                    mapVector[currRoom][currRow][currCol + 1].ancestorTile[2] = placeHolder.startArray[2];
                    searchContainer.push_back(mapVector[currRoom][currRow][currCol + 1]);
                    
                }
                //check south

                bool south_ex = false;
                if(((static_cast<int>(currRow)) + 1) < ((static_cast<int>(total_row)))) {
                    south_ex = true;
                    south = mapVector[currRoom][currRow + 1][currCol];
                }
                else {
                    south = placeHolder;

                }

                if(south.spaceType != '#' && south.wasDiscovered == false
                   && south_ex == true && south.spaceType != '!') { // change this so it's "if it is a walkable space type'
                    mapVector[currRoom][currRow + 1][currCol].wasDiscovered = true;
                    discoveredTiles++;
                    
                    if(south.spaceType == 'C') {
                        
                        cherry = mapVector[currRoom][currRow + 1][currCol];
                        cherry.spaceType = 'C';
                        cherry.ancestorTile[0] = placeHolder.startArray[0];
                        cherry.ancestorTile[1] = placeHolder.startArray[1];
                        cherry.ancestorTile[2] = placeHolder.startArray[2];
                        cherry.wasDiscovered = true;
                        queenFound = true;
                        return;
                    }
                    mapVector[currRoom][currRow + 1][currCol].ancestorTile[0] = placeHolder.startArray[0];
                    mapVector[currRoom][currRow + 1][currCol].ancestorTile[1] = placeHolder.startArray[1];
                    mapVector[currRoom][currRow + 1][currCol].ancestorTile[2] = placeHolder.startArray[2];
                    searchContainer.push_back(mapVector[currRoom][currRow + 1][currCol]);
                    
                }
                //check west

                bool west_ex = false;
                if(((static_cast<int>(currCol)) - 1) >= 0) {
                    west_ex = true;
                    west = mapVector[currRoom][currRow][currCol - 1];
                }
                else {
                    west = placeHolder;

                }

                if(west.spaceType != '#' && west.wasDiscovered == false
                   && west_ex == true && west.spaceType != '!') { // change this so it's "if it is a walkable space type'
                    mapVector[currRoom][currRow][currCol - 1].wasDiscovered = true;
                    discoveredTiles++;
                    
                    if(west.spaceType == 'C') {
                        
                        cherry = mapVector[currRoom][currRow][currCol - 1];
                        cherry.spaceType = 'C';
                        cherry.ancestorTile[0] = placeHolder.startArray[0];
                        cherry.ancestorTile[1] = placeHolder.startArray[1];
                        cherry.ancestorTile[2] = placeHolder.startArray[2];
                        cherry.wasDiscovered = true;
                        queenFound = true;
                        return;
                    }
                    mapVector[currRoom][currRow][currCol - 1].ancestorTile[0] = placeHolder.startArray[0];
                    mapVector[currRoom][currRow][currCol - 1].ancestorTile[1] = placeHolder.startArray[1];
                    mapVector[currRoom][currRow][currCol - 1].ancestorTile[2] = placeHolder.startArray[2];
                    searchContainer.push_back(mapVector[currRoom][currRow][currCol - 1]);
                    
                }
            }
                    
        }
        
    }
    
    void output() {
        
        std::vector<singleTile> output_vec;
        
        if(queenFound == false) {
            cout << "No solution, " << discoveredTiles << " tiles discovered.\n";
            return;
        }
        
        //overwrite tiles
        
        singleTile currPos = cherry;
        singleTile prevPos, copy;
        uint32_t currRoom, currRow, currCol, newRoom, newRow, newCol = 0;
        output_vec.resize(1);
        output_vec.push_back(cherry);
        
        for(int i = 0; i < discoveredTiles; i++) {
            
            currRoom = currPos.startArray[0]; //this handles warp pipes
            currRow = currPos.startArray[1];
            currCol = currPos.startArray[2];
            newRoom = currPos.ancestorTile[0];
            newRow = currPos.ancestorTile[1];
            newCol = currPos.ancestorTile[2];
            

            prevPos = mapVector[newRoom][newRow][newCol];
            
            if(newRow < currRow) {
                mapVector[newRoom][newRow][newCol].spaceType = 's';
                copy = prevPos;
                copy.spaceType = 's';
                output_vec.reserve(1);
                output_vec.push_back(copy);
                currPos = prevPos;
            }
            if(newRow > currRow) {
                mapVector[newRoom][newRow][newCol].spaceType = 'n';
                copy = prevPos;
                copy.spaceType = 'n';
                output_vec.reserve(1);
                output_vec.push_back(copy);
                currPos = prevPos;
            }
            if(newCol < currCol) {
                mapVector[newRoom][newRow][newCol].spaceType = 'e';
                copy = prevPos;
                copy.spaceType = 'e';
                output_vec.reserve(1);
                output_vec.push_back(copy);
                currPos = prevPos;
            }
            if(newCol > currCol) {
                mapVector[newRoom][newRow][newCol].spaceType = 'w';
                copy = prevPos;
                copy.spaceType = 'w';
                output_vec.reserve(1);
                output_vec.push_back(copy);
                currPos = prevPos;
            }
            if(newRoom != currRoom) {
                mapVector[newRoom][newRow][newCol].spaceType = 'p';
                copy = prevPos;
                copy.spaceType = 'p';
                output_vec.reserve(1);
                output_vec.push_back(copy);
                currPos = prevPos;
            }
            if(currPos.spaceType == 'S') {
                break;
            }
//            else {
//                continue;
//            }
            
            
        }
        
        
        //cout solution
        
        if(mode == 'M') { //map mode
            cout << "Start in room " << startRoom << ", row " << startRow <<
            ", column " << startCol << '\n';
            for(uint32_t i = 0; i < total_room; i++) {
                cout << "//castle room " << i << '\n';
                for(uint32_t j = 0; j < total_row; j++) {
                    for(uint32_t x = 0; x < total_col; x++) {
                        cout << mapVector[i][j][x].spaceType;
                    }
                    cout << '\n';
                    
                }
            }
            
        }
        else { //list mode
            cout << "Path taken:" << '\n';
            uint32_t size = static_cast<uint32_t>(output_vec.size());
            for(uint32_t i = size; i --> 0;) {
                //int i = myArray.Length; i --> 0;
                singleTile curr = output_vec[i];
                if(curr.spaceType != 'C') {
                    cout << "(" << curr.startArray[0] << "," <<
                    curr.startArray[1] << "," << curr.startArray[2] <<
                    "," << curr.spaceType << ")" << '\n';
                }
                else {
                    return;
                }
                
            }
            
        }
        
        
        
        
    }
    
    
    
    
    // Print help for the user when requested.
    // argv[0] is the name of the currently executing program
    void printHelp() {

        cout << "Usage: [--queue | --q] or [--stack | -s] to choose queue or stack mode" << '\n';
        cout << "Choose output mode with --output (M|L) to choose map or list output mode" << '\n';
        cout << "Example: --queue --output M < infile > outfile" << '\n';
        exit(0);
    } // printHelp()


    void getMode(int argc, char * argv[]) {
        
        bool modeSpecified = false;
            opterr = false;
            int choice;
            int count = 0;
            int option_index = 0;
            option long_options[] = {
                {"stack", no_argument, nullptr, 's'},
                {"queue", no_argument, nullptr, 'q'},
                {"output", required_argument, nullptr, 'o'},
                // If the --output option is not specified, default to map output format (M), if --output is specified on the command line, the argument (either M or L) to it is required
                {"help", no_argument, nullptr, 'h'},
                { nullptr, 0,                 nullptr, '\0' }
            };
        
            while ((choice = getopt_long(argc, argv, "o:sqh", long_options, &option_index)) != -1) {
                switch (choice) {
                
                case 's':
                        isStackMode = true;
                        modeSpecified = true;
                        count++;
                        if(count > 1) {
                            cerr << "Stack or queue can only be specified once" << endl;
                            exit(1);
                        }
                        break;
                case 'q':
                        isStackMode = false;
                        modeSpecified = true;
                        count++;
                        if(count > 1) {
                            cerr << "Stack or queue can only be specified once" << endl;
                            exit(1);
                        }
                        break;
                case 'o':
                        mode = *optarg;
                        break;
                case 'h':
                    printHelp();
//                    modeSpecified = true;
                    exit(0);
                
        
                default:
                    cerr << "Unknown command line option" << endl;
                    exit(1);
                } // switch
            } // while
        
            if(count == 0) {
                cerr << "Stack or queue must be specified" << endl;
                exit(1);
            }
        
            if (modeSpecified == false) {
                cerr << "Stack or queue must be specified" << endl;
                exit(1);
            } // if

        
    }
    
    
    
};






int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    xcode_redirect(argc, argv);
    
    
    Game g;
    g.getMode(argc, argv);
    g.solve();
    g.output();
    

    return 0;
}

