#ifndef __RUNNER_H__
#define __RUNNER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SERVICE_FLAG_RUN_ONCE 0x0001
#define SERVICE_FLAG_AUTO_START 0x0010
#define SERVICE_FLAG_STOP_WHEN_DONE 0x0100
#define SERVICE_FLAG_VERBOSE 0x1000

typedef struct service_t service_t;

typedef struct {
    char name[32];
    bool verbose;
    bool run_once;
    bool auto_start;
    bool stop_when_done;
    int (*runner)(service_t *, void *);
} service_config_t;

struct service_t {
    service_config_t *config;
    bool has_been_run;
    uint16_t flags;
    int (*runner)(service_t *, void *);
};

int register_service(service_config_t *conf, void *in);
void test_service();

#endif  // __RUNNER_H__
