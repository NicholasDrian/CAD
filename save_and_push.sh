#!/bin/bash

cd C:\Users\15303\Desktop\cs\CAD
timestamp=$(date +%c)

git add .
git commit -m "Backup at: `%timestamp%`" 
git push

echo PRESS ENTER TO CLOSE...
read