# CS225-FinalProject

<p align="center">
    <img src="entry/map-colorpicker.png" width="300"/>
</p>

## Data

We are using [San Francisco Road Network Data](https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm) and working with the following two datasets:

- [San Francisco Road Network's Nodes](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cnode)

- [San Francisco Road Network's Edges](https://www.cs.utah.edu/~lifeifei/research/tpq/SF.cedge)

Additionally, we added [San Francisco Police Station data](https://data.sfgov.org/Public-Safety/Map-of-Police-Stations-2011-/a9xm-jdem) to our existing dataset for reference.

## File Structure

### Documents

All relevant docs can be found inside the `/docs` directory. These include dev logs, project proposal, team contract, etc.

Datasets can be found inside `/data` directory. These include all the relevant datas used in this project.

## Running Instructions

Make sure you have [docker](www.docker.com) installed on your machine and you are running this in your docker environment. If you have trouble setting up, please refer to [this](https://courses.engr.illinois.edu/cs225/fa2022/resources/own-machine/) link for reference.

Once you are in your docker environment, navigate to the root directory of this project and run the following commands to properly initalize the build directory with [cmake](https://cmake.org/):

```bash
mkdir build
cd build
cmake ..
```

### Executable

#### Main

To run our wonderful tool, make sure you first run `make` then run `./main` to run our **wonderful** CLI! Follow all the instructions there are you will be all set!

#### Tests

To run all of our testcases, make sure you run `make` first! Then execute `./test_all [optinal tag]` to run all the tests we have! 

We also provide extra tags for you so you don't have to run'em all in one go! Avaliable tags are:

`[FileReader]`: All data parsing tests

`[SFMap]`: SFMap constructor related tests

`[DFS]`: DFS testcases

`[Dijkstra]`: Dijkstra testcases

`[prim]`: Prim's algorithm testcase

Apart from the above ones, you can use `[png]` or `[gif]` to run all the png/gif-outputting cases.
