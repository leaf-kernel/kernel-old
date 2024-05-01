#include <drivers/tty/tty.h>

TTY_t **ttys;
uint8_t currentTTYid;
TTY_t *currentTTY;

void init_tty()
{
    if (ttys == NULL)
    {
        ttys = kmalloc(MAX_TTYS * sizeof(TTY_t *));
        if (ttys == NULL)
        {
            debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for TTY array!");
            return;
        }

        for (size_t i = 0; i < MAX_TTYS; i++)
        {
            ttys[i] = NULL;
        }
    }
    else
    {
        for (size_t i = 0; i < MAX_TTYS; i++)
        {
            if (ttys[i] != NULL)
            {
                kfree(ttys[i]);
                ttys[i] = NULL;
            }
        }
    }

    cdlog("done.");
}

void tty_spawn(uint8_t id, char *font, uint8_t mapped_com)
{
    if (id >= MAX_TTYS || ttys[id] != NULL)
    {
        return;
    }

    ttys[id] = kmalloc(sizeof(TTY_t));
    if (ttys[id] == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for tty%04d!", id);
        return;
    }

    currentTTYid = 0;
    currentTTY = ttys[currentTTYid];
    struct nighterm_ctx *context = kmalloc(sizeof(struct nighterm_ctx));
    currentTTY->ctx = context;
    currentTTY->mapped_com = mapped_com;

    int s;
    if (font != NULL)
    {
        char *out;
        vfs_op_status status;
        status = drive_read((VFS_t *)__LEAF_GET_VFS__(), 0, font, &out);

        if (status == STATUS_OK)
        {
            s = nighterm_initialize(currentTTY->ctx,
                                    out,
                                    framebuffer->address,
                                    framebuffer->width,
                                    framebuffer->height,
                                    framebuffer->pitch,
                                    framebuffer->bpp,
                                    kmalloc, kfree);
        }
        else
        {
            s = nighterm_initialize(currentTTY->ctx,
                                    NULL,
                                    framebuffer->address,
                                    framebuffer->width,
                                    framebuffer->height,
                                    framebuffer->pitch,
                                    framebuffer->bpp,
                                    kmalloc, kfree);
        }
    }
    else
    {
        s = nighterm_initialize(currentTTY->ctx,
                                NULL,
                                framebuffer->address,
                                framebuffer->width,
                                framebuffer->height,
                                framebuffer->pitch,
                                framebuffer->bpp,
                                kmalloc, kfree);
    }

    if (s != NIGHTERM_SUCCESS)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to initialize nighterm for tty%04d!", id);
        return;
    }

    tty_switch(id);
    if (mapped_com > 0 && mapped_com <= 8)
        cdlog("tty%04d mapped to COM%d", id, mapped_com);
    else
        cdlog("tty%04d mapped to nothing", id);
}

void tty_switch(uint8_t id)
{
    ttys[id]->id = id;
    currentTTYid = id;
    currentTTY = ttys[id];
    tty_flush();
}

void tty_flush()
{
    if (currentTTY != NULL && currentTTY->ctx != NULL)
    {
        nighterm_flush(currentTTY->ctx, 27, 27, 27);
        nighterm_set_bg_color(currentTTY->ctx, 27, 27, 27);
        nighterm_set_fg_color(currentTTY->ctx, 255, 255, 255);
        nighterm_set_cursor_position(currentTTY->ctx, 0, 0);
    }
}

void tty_write(char ch)
{
    if (currentTTY != NULL && currentTTY->ctx != NULL)
    {
        nighterm_write(currentTTY->ctx, ch);
        vvcdlog("Wrote '%c' to tty%03d", ch, currentTTY->id);
    }

    if (currentTTY->mapped_com <= 8 && currentTTY->mapped_com > 0)
    {
        switch_serial(currentTTY->mapped_com, 0);
        write_serial(ch);
    }
}
