#How to build

##MacOS

1. Follow [this][v8_build] guide to pull and build the V8 locally on your machine
2. Run the
```
g++ -I/path/to/v8/include main.cc -o ejsr -lv8_monolith -L/path/to/v8/out/x64.release/obj/ -pthread -std=c++0x
```
3. Run the binary with 
```
./ejsr js.js
```


[v8_build]: https://v8.dev/docs/build "V8 build link"