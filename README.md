# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Design Coice

## Ghost State

The ghost behavior system uses the State design pattern. 
Each ghost's behavior (Chase, Scatter, Frightened, Eaten) is encapsulated in a separate state class, 
and a state machine manages transitions between these states. 
This approach allows each state to define its own logic and makes it easy to add or modify behaviors without affecting other parts of the code. 

## Pathfinding

The ghost ai uses a simple breath-first search (BFS) algorithm for pathfinding.
It gets a target form the Behaviour.cpp and then searches for the shortest path to that target.
If the the target cell is not walkable, it will search for the next closest target.

## Game State

The game uses the **State design pattern** to manage high-level game states such as Menu, 
SinglePlayer, MultiPlayer, Paused, and GameOver. 
Each state is implemented as a class derived from a common FSMBase interface, and the GameStateManager handles transitions and delegates update and render calls to the active state. 
This approach cleanly separates the logic for each game mode and makes it easy to add or modify states without affecting the rest of the codebase. 

## Level loading

The level loading system uses a simple text-based format to define the layout of the game world and cet cell properties.

## Interaction between Ghost and Player

This is handled using the Observer design pattern.
The Player class acts as the subject, and the Ghost classes are observers that react to changes in the player's state 
(e.g., when the player is eaten or when the ghost is frightened).

## Input Handling Command Pattern

The input handling system uses the Command design pattern.
Each input action (e.g., move up, down, left, right) is encapsulated in a command class,
and the InputManager maps these commands to specific input events (keyboard, mouse, etc.).

