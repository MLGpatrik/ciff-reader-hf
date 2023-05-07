
    ➜  ciff-reader-hf git:(main) ✗ ./parser --help
    
     ______     ______     ______   ______   ______     ______
    /\  ___\   /\  __ \   /\  ___\ /\  ___\ /\  ___\   /\  == \
    \ \ \____  \ \  __ \  \ \  __\ \ \  __\ \ \  __\   \ \  __<
     \ \_____\  \ \_\ \_\  \ \_\    \ \_\    \ \_____\  \ \_\ \_\
      \/_____/   \/_/\/_/   \/_/     \/_/     \/_____/   \/_/ /_/
    
    CAFFER: Crysys CAFF and CIFF converter
    usage: reader [--ciff | --caff] [target_file]
    
        Command Summary:
    
            --ciff target_file 	|	Read CIFF target_file and exports it to a JPEG file
            --caff target_file 	|	Read CAFF target_file and exports the first image to a JPEG file
            --help 			|	This help message



Requirements:
 - Ubuntu 22.04 LTS
 - libjpeg-dev
 - make
 - g++
 - c++20

## Install

First install libjpeg-dev:
```
sudo apt-get install libjpeg-dev
```

Then run make:
```
make
```

Done!

## Informations

Runs very slowly on macOS for some reason, but works great on linux and windows.

Tested with:
- Valgrind: No memory leaks found (log can be found [here](https://github.com/MLGpatrik/ciff-reader-hf/tree/main/docs/valgrind.log))
```
Used command:

valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind.log -- /home/kali/ciff-reader-hf/parser --caff testfiles/1.caff 
```
- Memtrace: No memory leaks found