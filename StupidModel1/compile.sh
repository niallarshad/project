#!/bin/bash

g++ -c $1.cpp
g++ $1.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app
