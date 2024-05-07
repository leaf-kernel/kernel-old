#include <sys/logger.h>
#include <sys/run/runner.h>

int _runner(service_t *self, void *in) {
    plog_ok("%s", in);
    return 1;
}

char *service_err(int status) {
    char *err = NULL;
    switch(status) {
    case SERVICE_ERROR_NO_VMM:
        err = "VMM not present, cannot continue";
        break;
    default:
        err = "Unknown error";
        break;
    }

    return err;
}

int register_service(service_config_t *conf, void *in) {

    if(conf == NULL) {
        plog_fail("Invalid service config passed (ERROR: NULL_PASSED)!");
        return -1;
    }

    service_t service;
    service.config = conf;

    if(service.config->verbose) {
        service.flags |= SERVICE_FLAG_VERBOSE;
    }

    if(service.config->run_once) {
        service.flags |= SERVICE_FLAG_RUN_ONCE;
    }

    if(service.config->auto_start) {
        service.flags |= SERVICE_FLAG_AUTO_START;
    }

    if(service.config->stop_when_done) {
        service.flags |= SERVICE_FLAG_STOP_WHEN_DONE;
    }

    plog_ok("Reached target \033[1m%s\033[0m", service.config->name);
    service.runner = service.config->runner;

    if(service.flags & SERVICE_FLAG_AUTO_START) {

        if(service.flags & SERVICE_FLAG_STOP_WHEN_DONE) {
            int status = service.runner(&service, in);
            if(status != 0) {
                plog_fail(
                    "Service \"%s\" crashed (ERROR: \"%s\", ERRNO: 0x%02x)",
                    service.config->name, service_err(status), status);
                return 1;
            } else {
                service.has_been_run = true;
            }
        } else {
            while(service.has_been_run) {
                int status = service.runner(&service, in);
                if(status != 0) {
                    plog_fail(
                        "Service \"%s\" crashed (ERROR: \"%s\", ERRNO: 0x%02x)",
                        service.config->name, service_err(status), status);
                    return 1;
                } else {
                    service.has_been_run = true;
                }
            }
        }
    }

    return 0;
}
void test_service() {
    service_config_t conf = {
        .name = "test service",
        .verbose = true,
        .run_once = true,
        .auto_start = true,
        .stop_when_done = true,
        .runner = &_runner,
    };

    register_service(&conf, "Hello, World!");
}