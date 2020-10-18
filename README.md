## Same Game
Simple board game based on rules of Same Game (known also as JawBreaker and few other names). Played by basic algorithm.

## Table of content
- [Problem description](#problem-description)
- [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Building](#building)
- [Built with](#built-with)
- [Usage](#usage)
- [Testing](#testing)
- [License](#license)

## Problem description
Write application playing Same Game. Program gets H x W board on input with C different numbers as cells.  
Assumption are:
- 4 <= H <= 50 – board rows count
- 4 <= W <= 50 – board columns count
- 3 <= C <= 20 – colour count

Program can remove cells if there are minimum 1 neighbor in same colour
(above, below, left or right). Gravity fills the gaps if needed.
Game ends when on board there is no cells with neighbor in same colour.

**Input**:  
Line containing H W C (space separated) followed by H lines with W numbers representing initial board.

**Output**:  
Coordinates of cells to pick. At end "-1 -1" meaning no more moves.

## Getting Started
This section describes briefly how to setup environment and build project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.8+. Additionally Git tool for downloading Google Test library as it is needed by test subproject.

### Building
Recommended:  
Use Cmake directly or your favorite IDE supporting CMake projects.  

Alternative:  
Use bare compiler for building project binary:
```shell
$ g++ -Wall -std=c++17 -O3 -c *.cpp
$ g++ -Wall -std=c++17 -O3 -o same-game *.o
```

## Built with
| Tool |  Windows 10 | Lubuntu 20.04 |
| --- | --- | --- |
| GCC | 7.3.0 | 9.3.0 |
| Cmake | 3.14 | 3.17 |
| Git | 2.20 | 2.25 |
| QtCreator | 4.12.0 | 4.13.0 |

## Usage
Application expects in first line number of rows, columns and colors. In following lines board.
Example input:
```
4 4 3
0 0 1 1
1 1 2 2
0 1 2 0
0 1 1 2
```
Example output:
```
3 1
2 0
2 2
-1 -1
```

Examples of usage:  
1. Pass example file named `testFile.txt` from project repo:
    ```shell
    $ ./same-game  < testFile.txt
    1 0
    2 0
    2 2
    -1 -1
    ```
2. Use binary directly:
    ```shell
    $ ./same-game
    4 4 3
    0 0 1 1
    1 1 2 2
    0 1 2 0
    0 1 1 2
    2 2
    2 2
    1 0
    -1 -1
    ```
3. Pipe input:
    ```shell
    $ cat testFile.txt | ./same-game
    3 2
    1 0
    2 2
    -1 -1
    ```
## Testing
Project contains test subproject based on Goggle Test framework. Compile testing subproject and launch tests via IDE or directly from console.  
Example run:
```
$ ./same-game-test.exe
(...)
[==========] Running 18 tests from 4 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from SameGameTest
[ RUN      ] SameGameTest.makeMove6x5
[       OK ] SameGameTest.makeMove6x5 (0 ms)
[ RUN      ] SameGameTest.makeMove4x4
[       OK ] SameGameTest.makeMove4x4 (0 ms)
[ RUN      ] SameGameTest.loadBoard
[       OK ] SameGameTest.loadBoard (0 ms)
[----------] 3 tests from SameGameTest (0 ms total)

[----------] 5 tests from SameGameTest/ImpactGravityTests
[ RUN      ] SameGameTest/ImpactGravityTests.impactGravity/0
[       OK ] SameGameTest/ImpactGravityTests.impactGravity/0 (0 ms)
[ RUN      ] SameGameTest/ImpactGravityTests.impactGravity/1
[       OK ] SameGameTest/ImpactGravityTests.impactGravity/1 (0 ms)
[ RUN      ] SameGameTest/ImpactGravityTests.impactGravity/2
[       OK ] SameGameTest/ImpactGravityTests.impactGravity/2 (0 ms)
[ RUN      ] SameGameTest/ImpactGravityTests.impactGravity/3
[       OK ] SameGameTest/ImpactGravityTests.impactGravity/3 (0 ms)
[ RUN      ] SameGameTest/ImpactGravityTests.impactGravity/4
[       OK ] SameGameTest/ImpactGravityTests.impactGravity/4 (0 ms)
[----------] 5 tests from SameGameTest/ImpactGravityTests (0 ms total)

[----------] 7 tests from SameGameTest/GetClusterTests
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/0
[       OK ] SameGameTest/GetClusterTests.GetCluster/0 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/1
[       OK ] SameGameTest/GetClusterTests.GetCluster/1 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/2
[       OK ] SameGameTest/GetClusterTests.GetCluster/2 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/3
[       OK ] SameGameTest/GetClusterTests.GetCluster/3 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/4
[       OK ] SameGameTest/GetClusterTests.GetCluster/4 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/5
[       OK ] SameGameTest/GetClusterTests.GetCluster/5 (0 ms)
[ RUN      ] SameGameTest/GetClusterTests.GetCluster/6
[       OK ] SameGameTest/GetClusterTests.GetCluster/6 (0 ms)
[----------] 7 tests from SameGameTest/GetClusterTests (0 ms total)

[----------] 3 tests from SameGameTest/PerformanceTests
[ RUN      ] SameGameTest/PerformanceTests.playGame/0
[       OK ] SameGameTest/PerformanceTests.playGame/0 (23 ms)
[ RUN      ] SameGameTest/PerformanceTests.playGame/1
[       OK ] SameGameTest/PerformanceTests.playGame/1 (1223 ms)
[ RUN      ] SameGameTest/PerformanceTests.playGame/2
[       OK ] SameGameTest/PerformanceTests.playGame/2 (4744 ms)
[----------] 3 tests from SameGameTest/PerformanceTests (5990 ms total)

[----------] Global test environment tear-down
[==========] 18 tests from 4 test suites ran. (5990 ms total)
[  PASSED  ] 18 tests.
```

## License
Project is distributed under the MIT License. See `LICENSE` for more information.
