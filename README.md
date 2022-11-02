# CS225-FinalProject

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

Then, run

```bash
# run all (main + tests)
make 

# run main
make main
./main

# run tests
make test_all
./test_all
```
