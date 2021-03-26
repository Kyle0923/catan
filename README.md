# Catan in C++
## Description
This is a C++ implementation of the board game "Catan"  
Author: Zonghao Huang <kyle0923@qq.com>  

## Platform and Build Environment
This project is intended to build for Windows.  
The tool chain is MinGW GCC and G++ compilers.  
Makefiles are provided and tested using git-bash.  
This project uses recursive make. Dependencies are configured such that third party libraries only rebuild at link time (if required).  
Makefiles use bash commands `mkdir` and `rm`, so they are not expected to work under powershell; though it should be relatively easy to refactor to work under powershell  
### Compile command and flags
To compile, simply run `make`, this will build the debug version of the program.  
To build release, set RELEASE to 1, i.e, `RELEASE=1 make`  
Use `-jN` to use *make*'s parallelization to speed up the build.  
To clean up artifacts, run `make clean`, this will only clean Catan's artifacts.  
To clean all including third party libraries, run `make clean_all`

## User Interface
This project uses command line and mouse to accept user's input and print out ASCII graph as output.  
An example of the ASCII graph of the game map:
```
                                          3:1                                           
                                         xx  xx                                         
                                       xx      xx                                       
                                      +----------+                                      
                                     /            \                                     
                                    /              \                                    
                    xxxx+----------+     SHEEP      +----------+xxxx                    
               WHEAT   /            \      5       /            \   3:1                 
                    x /              \            /              \ x                    
          +----------+     WHEAT      +----------+     DESERT     +----------+          
         /            \      6       /            \              /            \         
        /              \            /              \            /              \        
       +     SHEEP      +----------+      ORE       +----------+      WOOD      +       
        \      4       /            \      3       /            \      9       /        
         \            /              \            /              \            /         
      xxxx+----------+     WHEAT      +----------+      ORE       +----------+xxxx      
   3:1   /            \      6       /            \      8       /            \   SHEEP 
      x /              \            /              \            /              \ x      
       +      CLAY      +----------+      CLAY      +----------+     SHEEP      +       
        \      4       /            \      10      /            \      11      /        
         \            /              \            /              \            /         
          +----------+      ORE       +----------+     WHEAT      +----------+          
         /            \      11      /            \      12      /            \         
        /              \            /              \            /              \        
       +      WOOD      +----------+      CLAY      +----------+      WOOD      +       
      x \      10      /            \      2       /            \      8       / x      
   3:1   \            /              \            /              \            /   3:1   
      xxxx+----------+     WHEAT      +----------+     SHEEP      +----------+xxxx      
                      \      9       /            \      5       /                      
                       \            /              \            /                       
                        +----------+      WOOD      +----------+                        
                         xx      xx \      3       / xx      xx                         
                           xx  xx    \            /    xx  xx                           
                            3:1       +----------+      3:1                             
```

## Acknowledgment
This project uses third party library pdcurses.  
Read more about PDCurses on their Repo https://github.com/wmcbrine/PDCurses  
A trimmed copy of the source code of pdcurses version 3.9 is checked in to the repo under third party.
