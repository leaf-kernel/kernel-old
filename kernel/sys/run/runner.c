#include <sys/logger.h>
#include <sys/run/runner.h>

int _runner(service_t *self, void *in) {
    ok("%s", in);
    return 1;
}

char *service_err(int status, void *in) {
    char *err = (char *)kmalloc(MAX_SERVICE_ERR_LEN * sizeof(char));
    switch(status) {
    case SERVICE_ERROR_NO_VMM:
        err = "VMM not present";
        break;
    case SERVICE_ERROR_INVALID_SIGNATURE:
        sprintf(err, "Invalid signature: %s", (char *)in);
        break;
    case SERVICE_ERROR_INVALID_MAGIC:
        sprintf(err, "Invalid magic: 0x%08X", (char *)in);
        break;
    case SERVICE_ERROR_FILE_NOT_FOUND:
        sprintf(err, "File \"%s\" not found!", (char *)in);
        break;
    default:
        err = "Unknown error";
        break;
    }

    return err;
}

int register_service(service_config_t *conf, void *in) {

    if(conf == NULL) {
        fail("Invalid service config passed (ERROR: NULL_PASSED)!");
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

    vok("Registered service \"%s\". TYPE: 0x%02X", service.config->name);
    service.runner = service.config->runner;

    if(service.flags & SERVICE_FLAG_AUTO_START) {
        ok("Reached target \033[1m%s\033[0m", service.config->name);
        if(service.flags & SERVICE_FLAG_STOP_WHEN_DONE) {
            int status = service.runner(&service, in);
            if(status != 0) {
                if(status == SERVICE_WARN_MEMORY) {
                    warn("\033[1m%s\033[0m exited with a warning (ERROR: Leaf "
                         "recommends 64MB of RAM, ERRNO: 0x%02x)",
                         service.config->name, status);
                    return 2;
                }
                fail("\033[1m%s\033[0m failed (ERROR: \"%s\", ERRNO: 0x%02x)",
                     service.config->name, service_err(status, in), status);
                switch(service.config->type) {
                case SERVICE_TYPE_CHECK:
                    fail("A check service (\033[1m%s\033[0m) failed!",
                         service.config->name);
                    break;
                case SERVICE_TYPE_CONF:
                    fail("A config service (\033[1m%s\033[0m) failed! Some "
                         "services or programms might not be working properly.",
                         service.config->name);
                    break;
                case SERVICE_TYPE_DAEMON:
                    fail("A daemon (\033[1m%s\033[0m) failed! Please restart "
                         "the deamon, if that dont work reboot.",
                         service.config->name);
                    break;
                case SERVICE_TYPE_DRIVER:
                    fail("A driver (\033[1m%s\033[0m) failed! Please restart "
                         "the driver, if that dont work reinstall the driver.",
                         service.config->name);
                    break;
                case SERVICE_TYPE_KINIT:
                    fatal("A kernel module (\033[1m%s\033[0m) failed! Please "
                          "restart your computer! If that dont work, then your "
                          "fucked",
                          service.config->name);
                    break;
                default:
                    fail("The service %s failed. Unknown type 0x%02X!",
                         service.config->name, service.config->type);
                    break;
                }
                return 1;
            } else {
                service.has_been_run = true;
            }
        } else {
            while(service.has_been_run) {
                int status = service.runner(&service, in);
                if(status != 0) {
                    if(status == SERVICE_WARN_MEMORY) {
                        warn("\033[1m%s\033[0m exited with a warning (ERROR: "
                             "Leaf "
                             "recommends 64MB of RAM, ERRNO: 0x%02x)",
                             service.config->name, status);
                        return 2;
                    }
                    fail("\033[1m%s\033[0m failed (ERROR: \"%s\", ERRNO: "
                         "0x%02x)",
                         service.config->name, service_err(status, in), status);
                    switch(service.config->type) {
                    case SERVICE_TYPE_CHECK:
                        fail("A check service (\033[1m%s\033[0m) failed!",
                             service.config->name);
                        break;
                    case SERVICE_TYPE_CONF:
                        fail("A config service (\033[1m%s\033[0m) failed! Some "
                             "services or programms might not be working "
                             "properly.",
                             service.config->name);
                        break;
                    case SERVICE_TYPE_DAEMON:
                        fail("A daemon (\033[1m%s\033[0m) failed! Please "
                             "restart "
                             "the deamon, if that dont work reboot.",
                             service.config->name);
                        break;
                    case SERVICE_TYPE_DRIVER:
                        fail("A driver (\033[1m%s\033[0m) failed! Please "
                             "restart "
                             "the driver, if that dont work reinstall the "
                             "driver.",
                             service.config->name);
                        break;
                    case SERVICE_TYPE_KINIT:
                        fatal(
                            "A kernel module (\033[1m%s\033[0m) failed! Please "
                            "restart your computer! If that dont work, then "
                            "your "
                            "fucked",
                            service.config->name);
                        break;
                    default:
                        fail("The service %s failed. Unknown type 0x%02X!",
                             service.config->name, service.config->type);
                        break;
                    }
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
