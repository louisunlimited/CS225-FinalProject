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
    cout << "\033[1mYour choice: \033[m";
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
        cout << "We are sorry for the inconvenience :( hope you have a great day XD" << endl;
        exit(1);
    } else {
        cout << "Your image is saved as \033[1;32m" + filename + "\033[m in your \033[1m/build\033[m directory" << endl;
    }
}

int main(int argc, char* argv[]) {
    cout << "\033[1mWelcome! Thanks for using \033[1;32mTHE BEST\033[m \033[1mtool for police training and planning!\033[m" << endl;
    cout << R"( 
            ███████╗███████╗███╗   ███╗ █████╗ ██████╗ 
            ██╔════╝██╔════╝████╗ ████║██╔══██╗██╔══██╗
            ███████╗█████╗  ██╔████╔██║███████║██████╔╝
            ╚════██║██╔══╝  ██║╚██╔╝██║██╔══██║██╔═══╝ 
            ███████║██║     ██║ ╚═╝ ██║██║  ██║██║     
            ╚══════╝╚═╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     
            )" << endl;
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
    cout << "\033[1mYour choice: \033[m";
    cin >> choice;

    switch (choice) {
        case 1:
        {
            cout << "You have chosen to identify the importance of places in the city" << endl;
            cout << "\033[1;33mWarning:\033[m this function may take a long time to run (potentially up to days)" << endl;
            cout << "Do you want to run from the start or see the result of a previous run?" << endl;
            cout << "1. Run from the start" << endl;
            cout << "2. See the result of a previous run" << endl;
            int choice;
            cout << "\033[1mYour choice: \033[m";
            cin >> choice;
            if (choice == 1) {
                if (!confirm()) return 1;
                cout << "Please wait while we calculate the importance of places in the city!" << endl;
                PNG image = map.importance();
                cout << "Successfully calculated the importance of places in the city!" << endl;
                cout << "Saving your image..." << endl;
                image.writeToFile("importance.png");
            } else if (choice == 2) {
                (void) !system("cp ../entry/importance.png .");
            } else {
                cout << "Invalid choice, please restart the program X_X" << endl;
                return 1;
            }
            checkFileExistence("importance.png");
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
            cout << "Do you want to use the default setup {Coord: (37.5108, -122.1117), minDist: 10, zoom: 18} or enter your own?" << endl;
            cout << "1. Use the default start point" << endl;
            cout << "2. Enter my own start point" << endl;
            int choice;
            cout << "\033[1mYour choice: \033[m";
            cin >> choice;
            switch (choice) {
                case 1: 
                {
                    cout << "Using the default default setup {Coord: (37.5108, -122.1117), minDist: 10, zoom: 18}" << endl;
                    cout << "Please wait while we randomly choose an excape route for our virtual thief!" << endl;
                    cout << "Route found! Drawing GIF output!" << endl;
                    Animation animation = map.escapeRoute(Coord(37.5108, -122.1117), 10, 18);
                    cout << "----------------------------------------------------------------" << endl;
                    cout << "Writing to GIF file..." << endl;
                    animation.write("escape-route.gif");
                    checkFileExistence("escape-route.gif");
                    break;
                } 
                case 2: 
                {
                    cout << "Please enter a valid start point coordinate (to 4 decimal):" << endl;
                    double lat;
                    cout << "\033[1mLatitude: \033[m";
                    cin >> lat;
                    double lon;
                    cout << "\033[1mLongitude: \033[m";
                    cin >> lon;
                    cout << "Using the start point (" << lat << ", " << lon << ")" << endl;
                    cout << "Please enter the minimum distance (in km) for the virtual thief to run: " << endl;
                    double minDist;
                    cout << "\033[1mMinimun Distant: \033[m";
                    cin >> minDist;
                    cout << "Using the minimum distance of " << minDist << " km" << endl;
                    cout << "Please enter the zoom level for the map (we recommend 1 <= zoom <= 18): " << endl;
                    double zoom;
                    cout << "\033[1mZoom Level: \033[m";
                    cin >> zoom;
                    cout << "Using the zoom level of " << zoom << endl;
                    cout << "Please wait while we randomly choose an excape route for our virtual thief!" << endl;
                    cout << "Route found! Drawing GIF output!" << endl;
                    Animation animation = map.escapeRoute(Coord(lat, lon), minDist, zoom);
                    cout << "----------------------------------------------------------------" << endl;
                    cout << "Writing to GIF file..." << endl;
                    animation.write("escape-route.gif");
                    checkFileExistence("escape-route.gif");
                    break;
                } 
                default: 
                {
                    cout << "Invalid choice, please restart the program X_X" << endl;
                    return 1;
                }
            }
            break;
        }
        case 4:
        {
            cout << "You have chosen to find the next best position for a new police station" << endl;
            if (!confirm()) return 1;
            cout << "This function accepts an optional zoom level (we recommend 1 <= zoom <= 18)" << endl;
            cout << "Do you want to use the default zoom level (1.0) or enter your own?" << endl;
            cout << "1. Use the default zoom level" << endl;
            cout << "2. Enter my own zoom level" << endl;
            int choice;
            cout << "\033[1mYour choice: \033[m";
            cin >> choice;
            switch (choice) {
                case 1: 
                {
                    cout << "Using the default zoom level (1.0)" << endl;
                    cout << "Please wait while we calculate the next best position for a new police station!" << endl;
                    PNG image = map.nextPoliceStation(1.0);
                    cout << "Successfully calculated the next best position for a new police station!" << endl;
                    cout << "Saving your image..." << endl;
                    image.writeToFile("police-station.png");
                    checkFileExistence("police-station.png");
                    break;
                } 
                case 2: 
                {
                    cout << "Please enter the zoom level for the map (we recommend 1 <= zoom <= 18): " << endl;
                    double zoom;
                    cout << "\033[1mZoom Level: \033[m";
                    cin >> zoom;
                    cout << "Using the zoom level of " << zoom << endl;
                    cout << "Please wait while we calculate the next best position for a new police station!" << endl;
                    PNG image = map.nextPoliceStation(zoom);
                    cout << "Successfully calculated the next best position for a new police station!" << endl;
                    cout << "Saving your image..." << endl;
                    image.writeToFile("police-station.png");
                    cout << "The next best position is highlighted in \033[1;32mGREEN\033[m on the graph" << endl;
                    checkFileExistence("police-station.png");
                    break;
                } 
                default: 
                {
                    cout << "Invalid choice, please restart the program X_X" << endl;
                    return 1;
                }
            }
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