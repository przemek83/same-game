[![Build & test](https://github.com/przemek83/same-game/actions/workflows/buld-and-test.yml/badge.svg)](https://github.com/przemek83/same-game/actions/workflows/buld-and-test.yml)
[![CodeQL](https://github.com/przemek83/same-game/actions/workflows/github-code-scanning/codeql/badge.svg)](https://github.com/przemek83/same-game/actions/workflows/github-code-scanning/codeql)
[![Pylint](https://github.com/przemek83/same-game/actions/workflows/pylint.yml/badge.svg)](https://github.com/przemek83/same-game/actions/workflows/pylint.yml)
[![codecov](https://codecov.io/gh/przemek83/same-game/graph/badge.svg?token=EK3FL63RYS)](https://codecov.io/gh/przemek83/same-game)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_same-game&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_same-game)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_same-game&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_same-game)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_same-game&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_same-game)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_same-game&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_same-game)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_same-game&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_same-game)

## Same Game
Simple board game based on rules of Same Game (also known as JawBreaker and few other names). Played by basic algorithm.

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
- H >= 0 – board rows count
- W >= 0 – board columns count
- C >= 1 – color count

Program can remove cells if there are minimum 1 neighbors in same color
(above, below, left or right). Gravity fills the gaps if needed.
The game ends when, on the board, there are no cells with neighbor in the same color.

**Input**:  
Line containing H W C (space separated) followed by H lines with W numbers representing initial board.

**Output**:  
Coordinates of cells to pick in form of lines with 2 numbers separated by space each. Line format: `row column`.

## Getting Started
This section describes briefly how to setup the environment and build the project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.8+. Additionally, Git tool for downloading Google Test library as it is needed by test subproject.

### Building
Recommended:  
Use CMake directly or your favorite IDE supporting CMake projects.  

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
| CMake | 3.14 | 3.17 |
| Git | 2.20 | 2.25 |
| Qt Creator | 4.12.0 | 4.13.0 |

## Usage
Application expects in first line number of rows, columns and colors. In following lines, board.
Example input:
```
4 4 3
3 3 1 1
1 1 2 2
3 1 2 3
3 1 1 2
```
Example output:
```
3 1
2 0
2 2
```

Examples of usage:  
1. Pass example file named `testFile.txt` from project repo:
    ```shell
    $ ./same-game  < testFile.txt
    1 0
    2 0
    2 2
    ```
2. Use binary directly:
    ```shell
    $ ./same-game
    4 4 3
    3 3 1 1
    1 1 2 2
    3 1 2 3
    3 1 1 2
    2 2
    2 2
    1 0
    ```
3. Pipe input:
    ```shell
    $ cat testFile.txt | ./same-game
    3 2
    1 0
    2 2
    ```
## Testing
The project contains a test subproject based on Goggle Test framework. Compile testing subproject and launch tests via IDE or directly from console.  
Example run:
```
$ ./same-game-test
(...)
[==========] Running 28 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 10 tests from BoardTest
[ RUN      ] BoardTest.createBoard
[       OK ] BoardTest.createBoard (0 ms)
[ RUN      ] BoardTest.getRowCount
[       OK ] BoardTest.getRowCount (0 ms)
[ RUN      ] BoardTest.getColumnCount
[       OK ] BoardTest.getColumnCount (0 ms)
[ RUN      ] BoardTest.comparisonOperatorPositive
[       OK ] BoardTest.comparisonOperatorPositive (0 ms)
[ RUN      ] BoardTest.comparisonOperatorDifferentRowCount
[       OK ] BoardTest.comparisonOperatorDifferentRowCount (0 ms)
[ RUN      ] BoardTest.comparisonOperatorDifferentColumnCount
[       OK ] BoardTest.comparisonOperatorDifferentColumnCount (0 ms)
[ RUN      ] BoardTest.comparisonOperatorDifferentData
[       OK ] BoardTest.comparisonOperatorDifferentData (0 ms)
[ RUN      ] BoardTest.getColor
[       OK ] BoardTest.getColor (0 ms)
[ RUN      ] BoardTest.setColor
[       OK ] BoardTest.setColor (0 ms)
[ RUN      ] BoardTest.setEmpty
[       OK ] BoardTest.setEmpty (0 ms)
[----------] 10 tests from BoardTest (0 ms total)

[----------] 2 tests from SameGameTest
[ RUN      ] SameGameTest.makeMove6x5
[       OK ] SameGameTest.makeMove6x5 (0 ms)
[ RUN      ] SameGameTest.makeMove4x4
[       OK ] SameGameTest.makeMove4x4 (0 ms)
[----------] 2 tests from SameGameTest (0 ms total)

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
[----------] 7 tests from SameGameTest/GetClusterTests (1 ms total)

[----------] 4 tests from SameGameTest/PerformanceTests
[ RUN      ] SameGameTest/PerformanceTests.playGame/0
[       OK ] SameGameTest/PerformanceTests.playGame/0 (9 ms)
[ RUN      ] SameGameTest/PerformanceTests.playGame/1
[       OK ] SameGameTest/PerformanceTests.playGame/1 (25 ms)
[ RUN      ] SameGameTest/PerformanceTests.playGame/2
[       OK ] SameGameTest/PerformanceTests.playGame/2 (1283 ms)
[ RUN      ] SameGameTest/PerformanceTests.playGame/3
[       OK ] SameGameTest/PerformanceTests.playGame/3 (4448 ms)
[----------] 4 tests from SameGameTest/PerformanceTests (5765 ms total)

[----------] Global test environment tear-down
[==========] 28 tests from 5 test suites ran. (5766 ms total)
[  PASSED  ] 28 tests.
```

## License
The project is distributed under the MIT License. See `LICENSE` for more information.
