##About
A raytracer in C. It has shadows, reflections, lightning ([phong model](http://en.wikipedia.org/wiki/Phong_reflection_model)). It needs [CSFML 2.0](http://www.sfml-dev.org/download.php) to run.

##Usage
You could use included xcode project (remember to have required csfml files installed in /usr/local/lib and /usr/local/include). You should also be able to compile it using a makefile (in raytracer directory) with ```make mac``` or ```make linux``` command.
It has 3 hardcoded demos, you can edit scene.c to change them.

For instance:
```
./raytracer teapot
```  

Have fun.

##Screenshots
![frist](screens/1.png)  
![second](screens/2.png)  

##License
[The MIT License (MIT)](http://opensource.org/licenses/mit-license.php)