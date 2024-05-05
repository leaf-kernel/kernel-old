#include <drivers/tty/tty.h>
#include <sys/_config.h>

TTY_t **ttys;
uint8_t currentTTYid;
TTY_t *currentTTY;

void init_tty() {
    if(ttys == NULL) {
        ttys = kmalloc(MAX_TTYS * sizeof(TTY_t *));
        if(ttys == NULL) {
            debug_log(__FILE__, __LINE__, __func__,
                      "Failed to allocate memory for TTY array!");
            return;
        }

        for(size_t i = 0; i < MAX_TTYS; i++) {
            ttys[i] = NULL;
        }
    } else {
        for(size_t i = 0; i < MAX_TTYS; i++) {
            if(ttys[i] != NULL) {
                kfree(ttys[i]);
                ttys[i] = NULL;
            }
        }
    }

    vvcplog("done.");
}

void tty_destroy(uint8_t id) {
    if(id >= MAX_TTYS || ttys[id] == NULL) {
        return;
    }

    if(ttys[id]->ctx != NULL) {
        kfree(ttys[id]->ctx);
        ttys[id]->ctx = NULL;
    }

    kfree(ttys[id]);
    ttys[id] = NULL;
    vcplog("tty%04d destroyed", id);
}

void tty_spawn(uint8_t id, char *font, uint8_t mapped_com) {
    if(id >= MAX_TTYS || ttys[id] != NULL) {
        return;
    }

    ttys[id] = kmalloc(sizeof(TTY_t));
    if(ttys[id] == NULL) {
        debug_log(__FILE__, __LINE__, __func__,
                  "Failed to allocate memory for tty%03d!", id);
        return;
    }

    currentTTYid = id;
    currentTTY = ttys[currentTTYid];
    currentTTY->mapped_com = mapped_com;

    currentTTY->ctx = flanterm_fb_init(
        (void *)kmalloc, (void *)kfree, framebuffer->address,
        framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 1, 0, 0, 0);

    if(currentTTY->ctx == NULL) {
        dlog("Failed to initialize Flanterm for tty%03d!", id);
        hcf();
    }

    tty_switch(id);
    if(mapped_com > 0 && mapped_com <= 8)
        vcplog("Spawned tty%03d (tty%03d -> COM%d)", id, id, mapped_com);
    else
        vcplog("Spawned tty%03d (tty%03d -> NULL)", id, id);
}

void tty_switch(uint8_t id) {
    if(ttys[id] != NULL) {
        ttys[id]->id = id;
        currentTTYid = id;
        currentTTY = ttys[id];

        if(currentTTY->mapped_com <= 8 && currentTTY->mapped_com > 0)
            switch_serial(currentTTY->mapped_com, 0);

        tty_flush();
    }
}

void tty_flush() {
    if(currentTTY != NULL && currentTTY->ctx != NULL && currentTTY != NULL &&
       ttys[currentTTYid] != NULL) {
        flush_serial();

        if(_leaf_should_flush_tty) {
            currentTTY->ctx->double_buffer_flush(currentTTY->ctx);
        }
    }
}

void tty_write(char ch) {
    if(currentTTY == NULL || currentTTY->ctx == NULL)
        return;
    else
        flanterm_write(currentTTY->ctx, &ch, sizeof(ch));

    if(currentTTY->mapped_com <= 8 && currentTTY->mapped_com > 0) {
        switch_serial(currentTTY->mapped_com, 0);
        write_serial(ch);
    } else {
        // Default to COM1.
        switch_serial(0, _SERIAL_COM1);
        write_serial(ch);
    }
}

void _tty_flag_set(bool *flag, bool value) {
    *flag = value;
    tty_flush();
}
