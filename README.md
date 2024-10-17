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
- [Example run](#example-run)
- [Usage:  ](#usage)
- [Testing](#testing)
- [License](#license)

## Problem description
Write application playing Same Game. Program gets H x W board on input with C different numbers as cells. Assumption are:
- H is board rows count and H > 0,
- W is board columns count and W > 0,
- C is color count and C >= 1.

Program can in each step remove cells if there are minimum 1 neighbors in same color (up, down, left or right). Gravity fills the gaps if needed.
The game ends when, on the board, there are no cells with neighbor in the same color.

**Input**:  
Line containing H W C (space separated) followed by H lines with W numbers representing initial board.

**Output**:  
Coordinates of cells to pick in form of lines with 2 numbers separated by space each. Line format: `row column`.

## Getting Started
This section describes briefly how to setup the environment and build the project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.14+. Additionally, Git tool for downloading Google Test library as it is needed by test subproject.

### Building
Clone and use CMake directly or via any IDE supporting it. CMake should:

- configure everything automatically,
- compile and create binaries.

As a result of compilation, binary for simulations and binary for testing should be created.

## Built with
| Tool |  Windows 10 | Ubuntu 24.04 |
| --- | --- | --- |
| GCC | 13.1.0 | 13.2.0 |
| CMake | 3.30.2 | 3.28.3 |
| Git | 2.46.0 | 2.43.0 |
| GoogleTest | 1.15.2 | 1.15.2 |
| Python | - | 3.12.3 |

## Example run
Application expects in the first line the number of rows, columns and colors. In the following lines, expect a board.
Example input:
```
4 4 3
3 3 1 1
1 1 2 2
3 1 2 3
3 1 1 2
```

Steps:
|   |   |   |   |
|---|---|---|---|
| 3 | 3 | 1 | 1 |   
| 1 | 1 | 2 | 2 |  
| 3 | 1 | 2 | 3 |  
| 3 | 1 | 1 | 2 |  

Pick row 3, column 1 having color 1. Remove all neighbors of the same color.
|   |   |   |   |
|---|---|---|---|
| 3 | 3 | 1 | 1 |   
| x | x | 2 | 2 |  
| 3 | x | 2 | 3 |  
| 3 | x | x | 2 |  

Use gravity:
|   |   |   |   |
|---|---|---|---|
|   |   |   | 1 |   
| 3 |   | 1 | 2 |  
| 3 |   | 2 | 3 |  
| 3 | 3 | 2 | 2 |  

Pick row 2, column 0 having color 3. Remove all neighbors of the same color.
|   |   |   |   |
|---|---|---|---|
|   |   |   | 1 |   
| x |   | 1 | 2 |  
| x |   | 2 | 3 |  
| x | x | 2 | 2 |  

Use gravity:
|   |   |   |   |
|---|---|---|---|
|   |   |   | 1 |   
|   |   | 1 | 2 |  
|   |   | 2 | 3 |  
|   |   | 2 | 2 |  

Pick row 2, column 2 having color 2. Remove all neighbors of the same color.
|   |   |   |   |
|---|---|---|---|
|   |   |   | 1 |   
|   |   | 1 | 2 |  
|   |   | x | 3 |  
|   |   | x | 2 |  

Use gravity:
|   |   |   |   |
|---|---|---|---|
|   |   |   | 1 |   
|   |   |   | 2 |  
|   |   |   | 3 |  
|   |   | 1 | 2 |  

No more moves. Algorithm ends.

Example output:
```
3 1
2 0
2 2
```

## Usage:  
1. Pass example file named `testFile.txt` from project repo:
    ```shell
    $ ./same-game  < testFile.txt
    1 0
    2 0
    2 2
    ```
2. Use binary directly and type data:
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
For testing purposes, the gtest framework is used. Build the project first. Make sure that the `same-game-test` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build/test` directory, where the⁣ binary `same-game-test` should be available. Launching it should produce the following output on Linux:
Example run:
```
$ ./same-game-test
(...)
[==========] Running 30 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 4 tests from SameGameTest
[ RUN      ] SameGameTest.makeMove6x5
[       OK ] SameGameTest.makeMove6x5 (0 ms)
[ RUN      ] SameGameTest.makeMove4x4
[       OK ] SameGameTest.makeMove4x4 (0 ms)
[ RUN      ] SameGameTest.playWith4x4SymetricalBoard
[       OK ] SameGameTest.playWith4x4SymetricalBoard (0 ms)
[ RUN      ] SameGameTest.playWith3x1AsymmetricalBoard
[       OK ] SameGameTest.playWith3x1AsymmetricalBoard (0 ms)
[----------] 4 tests from SameGameTest (0 ms total)

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

[----------] 4 tests from SameGameTest/Benchmark
[ RUN      ] SameGameTest/Benchmark.playGame/0
[       OK ] SameGameTest/Benchmark.playGame/0 (3 ms)
[ RUN      ] SameGameTest/Benchmark.playGame/1
[       OK ] SameGameTest/Benchmark.playGame/1 (10 ms)
[ RUN      ] SameGameTest/Benchmark.playGame/2
[       OK ] SameGameTest/Benchmark.playGame/2 (427 ms)
[ RUN      ] SameGameTest/Benchmark.playGame/3
[       OK ] SameGameTest/Benchmark.playGame/3 (1467 ms)
[----------] 4 tests from SameGameTest/Benchmark (1908 ms total)

[----------] Global test environment tear-down
[==========] 30 tests from 5 test suites ran. (1908 ms total)
[  PASSED  ] 30 tests.
```
As an alternative, CTest can be used to run tests from the `build/test` directory:

    $ ctest
    Test project <some path>/same-game/build/test
        Start  1: SameGameTest.makeMove6x5
    1/30 Test  #1: SameGameTest.makeMove6x5 ......................................................................................................................................................................................................................................................................................   Passed    0.01 sec

    (...)

    30/30 Test #30: SameGameTest/Benchmark.playGame/(200	200	20	5	19	3	9	4	16	15	16	13	7	4	16	1	13	14	20	1	15	9	8	19	411	1	1	1	18	1	13	7	14	1	17	8	15	16	18	8	12	8	8	15	10	1	14	18	4	6	10	4	11	17	14	17	7	10	10	19	16	17	13	19	216	8	13	14	6	12	18	12	3	15	17	4	6	17	13	12	16	1	16	2	10	20	19	19	13	6	6	17	8... .................   Passed    1.47 sec

    100% tests passed, 0 tests failed out of 30

    Total Test time (real) =   2.10 sec

## License
This project is licensed under the MIT License. See the LICENSE file for details.

The project uses the following open-source software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| GoogleTest | BSD-3-Clause | https://github.com/google/googletest | testing framework |