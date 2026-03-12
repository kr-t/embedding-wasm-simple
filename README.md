> [!CAUTION]
> This is an archived repo, which was used to test abruption of wasm applications in wasm-micro-runtime. The better and improved solution is now part of [ocre-runtime](https://github.com/project-ocre/ocre-runtime)

# embedding-wasm-simple

> [!TIP]
> This solution works by using `wasm_runtime_terminate` function to terminate abruptly threads which use different resources, like sockets.

Simple application using wasm-micro-runtime via threads and killing those abruptly. 

Build and execute steps (run the last command without `-r` just to build)
```
cd sample_app
./build.sh
cd ..
./build.sh -r
```

Expected output: 
```
**********************************************
Server is starting...
Server listening on port 8000

**********************************************
Server is starting...
Server listening on port 8000
```
