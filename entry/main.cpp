#include <iostream>
#include "../src/filereader.h"
#include "../src/sf_map.h"

using namespace std;

SFMap loadMap() {
    cout << "Please wait while we load the map..." << endl;
    vector<Coord> nodes = FileReader::readRawNode("../data/SF.cnode.txt");
    Coord anchor1(37.5108, -122.1117);
    Coord normalizedAnchor1(5037.15, 4518.17);
    Coord anchor2(37.9956, -123.0232);
    Coord normalizedAnchor2(2261.93, 0);
    cout << "Converting coordinates..." << endl;
    FileReader::convertNode(nodes, anchor1, normalizedAnchor1, anchor2, normalizedAnchor2);
    vector<pair<int, int>> edges = FileReader::readEdge("../data/SF.cedge.txt");
    cout << "Loading police stations..." << endl;
    vector<Coord> police = FileReader::readPoliceStation("../data/Map_of_Police_Stations__2011_.csv");
    return SFMap(nodes, edges, police);
}

bool confirm() {
    cout << "Are you sure you want to choose this function? (Y/N)" << endl;
    char confirm;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y') {
        return true;
    } 
    cout << "Please restart the program to try another function to run X_X" << endl;
    return false;
}

void checkFileExistence(string filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File \033[1;31m" << filename << "\033[m failed to generate" << endl;
        cout << "Please restart the program and try again." << endl;
        exit(1);
    } else {
        cout << "Your image is saved as \033[1;32m" + filename + "\033[m in your \033[1m/build\033[m directory" << endl;
    }
}

int main(int argc, char* argv[]) {
    cout << "\033[1mWelcome! Thanks for using \033[1;32mTHE BEST\033[m \033[1mtool for police training and planning!\033[m" << endl;
    cout << "============================================================" << endl;
    SFMap map = loadMap();
    cout << "Map successfully loaded!" << endl;
    cout << "============================================================" << endl;
    cout << "We have 4 main functions: " << endl;
    cout << "1. Identify the importance of places in the city" << endl;
    cout << "2. Emergency Contact Access Point" << endl;
    cout << "3. Police Training Simulator" << endl;
    cout << "4. Finding the next best position for a new police station" << endl;
    cout << "============================================================" << endl;
    cout << "Please enter the number of the function you want to use: " << endl;

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
        {
            cout << "You have chosen to identify the importance of places in the city" << endl;
            if (!confirm()) return 1;
            break;
        }
        case 2:
        {

            cout << "You have chosen to find the Emergency Contact Access Point" << endl;
            if (!confirm()) return 1;
            cout << "Please wait while we calculate all the possible point and cabel plans!" << endl;
            PNG image = map.accessPoint();
            cout << "Successfully calculated all the possible point and cabel plans!" << endl;
            cout << "Saving your image..." << endl;
            image.writeToFile("access-point.png");
            checkFileExistence("access-point.png");
            break;
        }
        case 3:
        {
            cout << "You have chosen to use the Police Training Simulator" << endl;
            if (!confirm()) return 1;
            cout << "Please wait while we randomly choose an excape route for our virtual thief!" << endl;
            cout << "Route found! Drawing GIF output!" << endl;
            Animation animation = map.escapeRoute(Coord(37.5108, -122.1117), 10, 18); // TODO: enable user to input the start point
            cout << "----------------------------------------------------------------" << endl;
            cout << "Writing to GIF file..." << endl;
            animation.write("escapeRoute.gif");
            checkFileExistence("escapeRoute.gif");
            break;
        }
        case 4:
        {
            cout << "You have chosen to find the next best position for a new police station" << endl;
            if (!confirm()) return 1;
            break;
        }
        default:
            cout << "Invalid choice, please restart the program X_X" << endl;
            return 1;
    }

    cout << "============================================================" << endl;
    cout << "\033[1mThank you for using our tool! We hope you have a great day :)\033[m" << endl;

    return 0;
}