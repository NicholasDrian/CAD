#!/bin/bash
cd C:\Users\15303\Desktop\cs\CAD
set timestamp=$(date +"%D %T")

git add .
git commit -m "Backup at: `%timestamp%`" 
git push