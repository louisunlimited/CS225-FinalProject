# Dev Log

## Week 1

This week our main goal was to finish all the documentation and figure out a feasible way of implementing our leading questions. We accomplished all of our goals for this week.

Hezi Jiang: I participated in discussing functions and algorithms of our final project, as well as finished the leading questions 2 & 3.

Tianyue Cao: I searched for the graph algorithms for functions one and four. I also calculated the time and space complexities of the graph algorithms.

Louis Qian: This week, I set up the project structure as well as all the files needed for this project. I researched the first and fourth leading questions and made edits. I also tried to incorporate CI into our workflow to make everything go smoothly.

Xiping Sun: I updated data acquisition and finished the team contract. I also joined in the discussion of functions and algorithms.

Everything went smoothly, and we were able to meet all of our goals. One thing that was challenging was figuring out what exactly do we want our proposal question 4 to be like, but thankfully we were able to come up with a good idea and agree on it.

Next week we will officially start our development process and implement Police Training Simulator with DFS. We also want to set up all the function signatures and some of the test cases for this algorithm.

## Week 2

For this week, our main goal was to finish Police Training Simulator with DFS, along side with data handling & validation. We accomplished all of our goals for this week.

Louis Qian: I tried to implement `readNormalizedNode` and `convertNode` in filereader. I tried to use fstream to parse the input data and model them into a vector. I tried to figure out a way to translate all the data to real coordinates.

Tianyue Cao: I finished the constructor of the graph, including populating the adjacency list, building the kdtree, and removing outliers from the data.

Xiping Sun: I participated in discussing data handling, and finished DFS in the first algorithm.

Hezi Jiang: I finished reading the data from the two data file and the police station position. I convert it to the vector for future use.

Everything went fairly well. Our frist goal for next week is to modify our project proposal accoring to the feedback, which, require us to rethink question 4 again with a different algorithm. Then, as our regular work flow goes, we will be trying to finish the follwing: Test case for DFS; Optimum route for chasing criminals; Identify the importance of places in the city; Implement PNG output.

## Week 3

This week, we started to build upon what we had last week, implementing more complex and challenging functions. We tried to finish PNG and calculate the betweeness centrality for our first objective.

Louis Qian: I worked a bit more on `readNode` since we believe the original version was not properly coded. I also started working on writing testcases for some of the functions.

Tianyue Cao: I started working on drawing the map on a PNG. This funtion will use a colorpicker for the node and edge colors such that it can be used by other functions.

Hezi Jiang: I finished the function "importance" and its helper  function "importanceAsVec" in the file sf_map.cpp. This function use the  Dijkstra's algorithm to identify the importance of places in the city and color the graph using different color accordingly.

Xiping Sun: I wrote test cases for DFS and importance and participated in the discussion of choosing a new algorithm.

For next week, during thanksgiving, our goal will be: Implementing test cases for the importance of places; Finding the next best position for a new police station; Implement GIF output; and Integrate image output into existing features.

## Week 4

This week during thanks giving, we tried to finish all the functions and wrap things up. We aimed to fininsh function 2 and 4 and we made a considerable amount of progress towards this goal. Apart from these, we also started building testcases for all functions. We ran into some issues when trying to build custom datasets for testcases but were able to figure out a way around it.

Louis Qian: I fininshed the first version of minimum spanning tree and started to work on testcases for this. I also did code review and helped merge a few branches.

Tianyue Cao: I finished the PNG helper function and colorpicker. I also fixed bugs in the constructor and in the testcases.

Xiping Sun: I set up six data bases for testing, and finished testing escapeRouteAsVect and getParents.

Hezi Jiang: I finished the goal 4, find the next best position for a new police station. I also finish the getParents helper function.

For next week, we hope to do a overview of what we have done and try to finish any leftovers due. Then we will, hopefully, start preparing for the final presentaion.

## Mid project checkin

• What have you accomplished so far?

Almost everything.

• What tasks do you still need to accomplish?

1. Identify the importance of places in the city

    Improve run time and draw png

2. Emergency Contact Access Point

    Testcase for MST, SFMap data parsing. PNG for this.

3. Police Training Simulator

    GIF

4. Finding the next best position for a new police station

    Correct? Testcase, KDTree?

• Do you have a clear understanding of how to do the remaining tasks?

1. Identify the importance of places in the city

    Try to limit the search distance for Dijkstra.

2. Emergency Contact Access Point

    Build testcases based on nodes,

    SFMap data parsing: deg != 2: adj\[u\].size() != 2

    PNG for this.

3. Police Training Simulator

    one frame per node in vec.

4. Finding the next best position for a new police station

    Find bug in KDTree, testcase...

• Are there any major problems you have encountered while working on the project?

N/A

• Do you have any questions?

Can we still change function inputs.

• Functional code demo

fileReader

Betweeness centrality graph

•  Run of a test suite and a brief explanation of those tests.

Filereader testcase

dijkstra testcase
