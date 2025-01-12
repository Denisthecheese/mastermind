#    - code file - -- raylib package configuration --   - output file   -- where to go to find raylib.h --
eval gcc mastermind.c $(pkg-config --libs --cflags raylib) -o mastermind.out -I/opt/homebrew/Cellar/raylib/5.0/include/
