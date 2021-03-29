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
                                          WOOD                                         
                                         xx  xx                                        
                                       xx      xx                                      
                                      +----------+                                     
                                     /            \                                    
                                    /              \                                   
                    xxxx+----------+     SHEEP      +----------+xxxx                   
                 ORE   /            \      5       /            \   3:1                
                    x /              \            /              \ x                   
          +----------+      WOOD      +----------+      WOOD      +----------+         
         /            \      9       /            \      12      /            \        
        /              \            /              \            /              \       
       +     WHEAT      +----------+     SHEEP      +----------+      CLAY      +      
        \      8       /            \      5       /            \      6       /       
         \            /              \            /              \            /        
      xxxx+----------+      ORE       +----------+     SHEEP      +----------+xxxx     
   3:1   /            \      11      /            \      9       /            \   CLAY 
      x /              \            /              \            /              \ x     
       +     WHEAT      +----------+      ORE       +----------+      CLAY      +      
        \      10      /            \      3       /            \      4       /       
         \            /              \            /              \            /        
          +----------+      WOOD      +----------+      WOOD      +----------+         
         /            \      11      /            \      2       /            \        
        /              \            /              \            /              \       
       +      CLAY      +----------+     WHEAT      +----------+      ORE       +      
      x \      4       /            \      6       /            \      8       / x     
 WHEAT   \            /              \            /              \            /   3:1  
      xxxx+----------+     DESERT     +----------+     WHEAT      +----------+xxxx     
                      \              /            \      3       /                     
                       \            /              \            /                      
                        +----------+     SHEEP      +----------+                       
                         xx      xx \      10      / xx      xx                        
                           xx  xx    \            /    xx  xx                          
                            3:1       +----------+      SHEEP                          
```

## Acknowledgment
This project uses third party library pdcurses.  
Read more about PDCurses on their Repo https://github.com/wmcbrine/PDCurses  
A trimmed copy of the source code of pdcurses version 3.9 is checked in to the repo under third party.