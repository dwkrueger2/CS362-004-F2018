This file contains references to GIT tasks I had to look up and found an online resource to solve the problem

Problem:  File that I want ignored keeps showing up cannot be removed from version control:
Solution:    A previous committ and push caused the file to be in the repository.  Now the file is changed and so it must show up even if it matches the .gitignore.
			2 solutions - 1) Find the previous commit and remove the file or 2) set the index to consider the file unchanged.
			Command:  git update-index --assume-unchanged [untrackme.file]
Reference: https://stackoverflow.com/questions/44160735/trying-to-undo-ignore-by-adding-slnx-sqlite-file-in-git-ignore-file-in-visual-s

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Problem:
Solution:
Reference:


