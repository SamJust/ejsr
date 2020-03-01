g++ -I/Users/apple/v8/v8/include main.cc src/*.cc -o out/ejsr -L/Users/apple/v8/v8/out.gn/x64.release.sample/obj/ -I/Users/apple/ejsr/src -lv8_monolith -pthread -std=c++0x
echo "Compiled"