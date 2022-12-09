# CS225-FinalProject

<p align="center">
    <a href="#data">Data</a> - 
    <a href="#file-structure">File Structure</a> - 
    <a href="#running-instructions">Running Instructions</a> - 
    <a href="#executable">Executable</a> - 
    <a href="#ending">Ending</a>
</p>

<p align="center">
    <img src="entry/map-logo.png" width="500"/>
</p>

<div align="center">
    <img src="https://forthebadge.com/images/badges/made-with-c-plus-plus.svg" width="135"/> 
    <br/>
    <a href="https://louisunlimited.com">
    <img src="entry/presentation-video-link.svg" width="200"/>
    </a>
</div>

## Data

We are using [San Francisco Road Network Data](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) and working with the following two datasets:

- [San Francisco Road Network's Nodes](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cnode)

- [San Francisco Road Network's Edges](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cedge)

Additionally, we added [San Francisco Police Station data](https://data.sfgov.org/Public-Safety/Map-of-Police-Stations-2011-/a9xm-jdem) to our existing dataset for reference.

## File Structure

### Documents

All relevant docs can be found inside the `/docs` directory. These include dev logs, project proposal, team contract, etc.

Datasets can be found inside `/data` directory. These include all the relevant datas used in this project.

### Code base

```text
.
├── data
├── docs
├── entry
├── lib
│   ├── cs225
│   └── lodepng
├── src
└── tests
```

The main entry point to our program lies within `main.cpp` in the `/entry` directory. We call all the functions in this file for you!

The `/lib` directory contains all files for producing PNG, creating pixels, and more.

```text
.
├── CMakeLists.txt
├── cs225                   /* CS225 lib */
│   ├── HSLAPixel.cpp       /* Pixel Class */
│   ├── HSLAPixel.h
│   ├── PNG.cpp             /* PNG Class */
│   ├── PNG.h
│   └── RGB_HSL.h
└── lodepng
    ├── lodepng.cpp
    └── lodepng.h
```

In the `/src` directory lies the heart of our project.

```text
.
├── CMakeLists.txt
├── animation.cpp           /* GIF output */
├── animation.h             
├── coord.cpp               /* Class for Coordinates */
├── coord.h
├── filereader.cpp          /* Data Processing */
├── filereader.h
├── kdtree.cpp              /* K-d Tree */
├── kdtree.h
├── mst.cpp                 /* MST w/ Prim's */
├── mst.h
├── sf_map.cpp              /* Main class for this program */
├── sf_map.h
└── sf_map_helpers.cpp      /* Extra helpers */
```

The `/test` directory contains all of our tests as well as our own dataset created for testing.

```text
.
├── large.edge.txt          /* Custom Large Dataset */
├── large.node.txt
├── medium.edge.txt         /* Custom Medium Dataset */
├── medium.node.txt
├── small.edge.txt          /* Custom Small Dataset */
├── small.node.txt
└── tests.cpp               /* Main test file */
```

Furthermore, thanks to [Github Actions](https://docs.github.com/en/actions), we are utilizing CI/CD in our project to automate the developing process. We are surprised by how our CI/CD pipelines had helped us finding potential errors that are not reported on our own dev machines. It has been a great help and a great taste of DevOps in our project. As always, the workflow file can befound in the `cmake.yml` file inside `/.github/workflows` directory.

## Running Instructions

Make sure you have [docker](www.docker.com) installed on your machine and you are running this in your docker environment. If you have trouble setting up, please refer to [this](https://courses.engr.illinois.edu/cs225/fa2022/resources/own-machine/) link for reference.

Once you are in your docker environment, navigate to the root directory of this project and run the following commands to properly initialize the build directory with [cmake](https://cmake.org/):

```bash
mkdir build
cd build
cmake ..
```

## Executable

If you followed the initialization steps above, you should already be in the `/build` directory!

Now run `make` to compile all the executables mentioned below! As an alternative, you can make individual executables by running `make <exec name>`.

After finishing this step, you may proceed to acctually run our program!

### Main

Run `./main` in the `/build` directory to run our **wonderful** CLI! Follow all the instructions there and you will be all set! By default, all the outputs will be exported to the `/build` directory in this project, but please consult the CLI for further references.

We have four main goals in this project, and the required inputs for each of the functionalities are as follows:

1. Identify the importance of places in the city
    - Input:

        N/A, but you can choose to run this yourself or see the result of a previous run as promoted by the CLI.
    - Output:

        `importance.png` showing the importance of all nodes in this map by it's color lightness. More important nodes tends to be darker.
2. Emergency Contact Access Point
    - Input:

        N/A
    - Output:

        `access-point.png` showing all the possible access points listed as nodes colored in red, along with black routes showing the optimum cable plannings for powering those access points.
3. Police Training Simulator
    - Input:

        A pair of earth coordinates (latitude before longitude, to 4 decimal) as the starting point. We are using k-d tree to find the best match for your inputs, so technically we do not have hard restrictions on your input. But for the best result please try to be as accurate as possible and stay within the San Francisco Area.

        A zoom factor (double) for zooming the map. We recommend choosing between 1 and 18.

        A minimum distance (double) in kilometers for the thief to run. We recommend keeping this between 1 and 100 for the best result.

    - Output: `escapeRoute.gif` showing an animated escape route for our virtual thief to run.
4. Finding the next best position for a new police station
    - Input:

        A zoom factor (double) for zooming the map. We recommend choosing between 1 and 18.

    - Output:

        `police-station.png` showing the next-best position for the next police station to be.

We do not support custom datasets just yet, but we might consider adding it in the future.

### Tests

Use `./test_all [optinal tag]` to run all the testcases we have!

We also provide extra tags for you so you don't have to run'em all in one go! Avaliable tags are:

`[FileReader]`: All data parsing tests

`[SFMap]`: SFMap constructor related tests

`[DFS]`: DFS testcases

`[Dijkstra]`: Dijkstra testcases

`[prim]`: Prim's algorithm testcase

Apart from the above ones, you can use `[png]` or `[gif]` to run all the testcases with PNG/GIF output.

We've also created several custom datasets for testing our algorithms to make sure they are working as expected. As mentioned above, they are all listed in the same directory as all the tests themselves. Feel free to take a look at them. Apart from testing DFS, Dijkstra, and Prim's, we also tested general functions like data parsing and PNGs. There are two testcases being commented out because those may take days to run, but feel free to un-comment them and try them yourself.

---

If you have questions, please consult the GIF below for reference.

<p align="center">
    <img src="entry/runninginstruction.gif" width="350"/>
</p>

---

## Ending

Thank you for choosing our wonderful tool. We hope you have a great day :)

#### Made with ♡ by Tianyue, Hezi, Louis, and Xiping
