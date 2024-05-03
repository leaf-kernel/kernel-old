#include <drivers/tty/tty.h>

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

    cdlog("done.");
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
    cdlog("tty%04d destroyed", id);
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
    struct nighterm_ctx *context = kmalloc(sizeof(struct nighterm_ctx));
    currentTTY->ctx = context;
    currentTTY->mapped_com = mapped_com;

    int s;
    if(font != NULL) {
        char *out;
        vfs_op_status status;
        status = drive_read((VFS_t *)__LEAF_GET_VFS__(), 0, font, &out);

        if(status == STATUS_OK) {
            s = nighterm_initialize(currentTTY->ctx, out, framebuffer->address,
                                    framebuffer->width, framebuffer->height,
                                    framebuffer->pitch, framebuffer->bpp,
                                    kmalloc, kfree);
        } else {
            s = nighterm_initialize(currentTTY->ctx, NULL, framebuffer->address,
                                    framebuffer->width, framebuffer->height,
                                    framebuffer->pitch, framebuffer->bpp,
                                    kmalloc, kfree);
        }
    } else {
        s = nighterm_initialize(currentTTY->ctx, NULL, framebuffer->address,
                                framebuffer->width, framebuffer->height,
                                framebuffer->pitch, framebuffer->bpp, kmalloc,
                                kfree);
    }

    if(s != NIGHTERM_SUCCESS) {
        debug_log(__FILE__, __LINE__, __func__,
                  "Failed to initialize nighterm for tty%03d!", id);
        return;
    }

    tty_switch(id);
    if(mapped_com > 0 && mapped_com <= 8)
        cdlog("tty%03d mapped to COM%d", id, mapped_com);
    else
        cdlog("tty%03d mapped to nothing", id);
}

void tty_switch(uint8_t id) {
    if(ttys[id] != NULL) {
        ttys[id]->id = id;
        currentTTYid = id;
        currentTTY = ttys[id];
        switch_serial(currentTTY->mapped_com, 0);
        tty_flush();
    }
}

void tty_flush() {
    if(currentTTY != NULL && currentTTY->ctx != NULL && currentTTY != NULL &&
       ttys[currentTTYid] != NULL) {
        flush_serial();
        nighterm_flush(currentTTY->ctx, 0, 0, 0);
        nighterm_set_bg_color(currentTTY->ctx, 0, 0, 0);
        nighterm_set_fg_color(currentTTY->ctx, 255, 255, 255);
        nighterm_set_cursor_position(currentTTY->ctx, 0, 0);
    }
}

void tty_write(char ch) {
    if(currentTTY != NULL && currentTTY->ctx != NULL) {
        nighterm_write(currentTTY->ctx, ch);
    }

    if(currentTTY->mapped_com <= 8 && currentTTY->mapped_com > 0) {
        switch_serial(currentTTY->mapped_com, 0);
        write_serial(ch);
    }
}
