#!/bin/bash

cd C:\Users\15303\Desktop\cs\CAD
timestamp=$(date +%c)

git add .
git commit -m "Automated Backup at: $timestamp" 
git push

git status

echo PRESS ENTER TO CLOSE...
read