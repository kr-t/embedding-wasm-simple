#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "bh_platform.h"
#include "bh_read_file.h"
#include "wasm_export.h"

#define WASM_APP_PATH "../sample_app/build/sockets.wasm"
//#define FORKING_SOLUTION

typedef struct {
    wasm_module_inst_t module_inst;
} thread_args_t;

void *wasm_main_thread(void *arg) {
    // Initialize WASM runtime thread environment
    wasm_runtime_init_thread_env();
    thread_args_t *args = (thread_args_t *)arg;
    wasm_application_execute_main(args->module_inst, 0, NULL);
    wasm_runtime_destroy_thread_env();
    return NULL;
}

void run_wamr_app(const char *wasm_path)
{
    char *buffer, error_buf[128];
    wasm_module_t module;
    wasm_module_inst_t module_inst;
    uint32 size, stack_size = 8092 * 32, heap_size = 8092;

    /* read WASM file into a memory buffer */
    buffer = bh_read_file_to_buffer(wasm_path, &size);
    if (!buffer) {
        fprintf(stderr, "Failed to read WASM file: %s\n", wasm_path);
        return;
    }

    /* parse the WASM file from buffer and create a WASM module */
    module = wasm_runtime_load(buffer, size, error_buf, sizeof(error_buf));
    if (!module) {
        fprintf(stderr, "Failed to load WASM module: %s\n", error_buf);
        free(buffer);
        return;
    }

    const char *addr_pool[1] = {"0.0.0.0/0"};
    wasm_runtime_set_wasi_addr_pool(module, addr_pool, 1);

    /* create an instance of the WASM module (WASM linear memory is ready) */
    module_inst = wasm_runtime_instantiate(module, stack_size, heap_size,
                                         error_buf, sizeof(error_buf));
    if (!module_inst) {
        fprintf(stderr, "Failed to instantiate WASM module: %s\n", error_buf);
        wasm_runtime_unload(module);
        free(buffer);
        return;
    }

    pthread_t tid;
    thread_args_t args = { .module_inst = module_inst };

    if (pthread_create(&tid, NULL, wasm_main_thread, &args) != 0) {
        fprintf(stderr, "Failed to create thread\n");
        wasm_runtime_deinstantiate(module_inst);
        wasm_runtime_unload(module);
        wasm_runtime_destroy();
        free(buffer);
        return;
    }
    sleep(1);
#ifndef FORKING_SOLUTION
    wasm_runtime_terminate(module_inst);
    pthread_cancel(tid);
#else
    pthread_join(tid, NULL);
#endif
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    free(buffer);
}

#ifndef FORKING_SOLUTION
int main()
{
    wasm_runtime_init();    
    run_wamr_app(WASM_APP_PATH);
    wasm_runtime_destroy();
    sleep(1);
    wasm_runtime_init();   
    run_wamr_app(WASM_APP_PATH);
    wasm_runtime_destroy();
    
    return 0;
}
#else
int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Forked process: run WASM app
        wasm_runtime_init();
        run_wamr_app(WASM_APP_PATH);
        wasm_runtime_destroy();
        exit(0);
    } else {
        // Wait, then kill the process
        sleep(5); // Let it run for 5 seconds
        printf("Killing the forked process %d\n", pid);
        kill(pid, SIGKILL); // Forcefully kill the process
        int status;
        waitpid(pid, &status, 0); // Clean up the process
        printf("Forked process exited with status %d\n", status);
    }

    wasm_runtime_init();
    run_wamr_app(WASM_APP_PATH);
    wasm_runtime_destroy();

    return 0;
}
#endif
