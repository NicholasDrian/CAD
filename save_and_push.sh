#!/bin/bash

cd C:/Users/15303/Desktop/cs/CAD
timestamp=$(date +%c)

read -p "Enter commit message:" message

git add .
git commit -m "$message" 
git push

git status

echo PRESS ENTER TO CLOSE...
read