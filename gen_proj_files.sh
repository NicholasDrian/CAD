#!/bin/bash

echo GENERATING PROJECT FILES...

./dependencies/premake/premake5.exe vs2022

echo PRESS ENTER TO CLOSE...
read