# maddy benchmarks

## How to run

The benchmarks have to be run in release mode to give proper results.

```shell
# in main folder
mkdir tmp
cd tmp
cmake -DMADDY_BUILD_WITH_BENCH=ON -DCMAKE_BUILD_TYPE=Release ..
make BUILD_TYPE=Release -j4
```
