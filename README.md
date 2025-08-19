# embedding-wasm-simple
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
bind: Address in use
```
