#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "main/fsqueue.h"

const RECT D_80028A20 = { SCREEN_WIDTH, 256, 192, SCREEN_HEIGHT };

// ========================================
// MAP
// ========================================

void func_80066D90(void) // 0x80066D90
{
    s32       i;
    s32       frameCount;
    DR_TPAGE* var1; // Guessed type.
    TILE*     var2; // Guessed type.

#ifdef SH_PC_PORT
    /* PSX spends 63 VBlanks hiding disc access here. PC waits for the queue
     * explicitly below, so retaining that fixed delay only makes returning
     * from the map to inventory take over a second. Two draws preserve the
     * double-buffer refresh without the artificial optical-disc wait. */
    frameCount = 2;
#else
    frameCount = 63;
#endif

    for (i = 0; i < frameCount; i++)
    {
        var1 = PSX_SCRATCH;
        setDrawTPage(var1, 0, 1, getTPageN(0, 2, 0, 0));
        DrawPrim(var1);

        var2 = PSX_SCRATCH;
        setlen(var2, 3);

        setRGBC0(var2, 8, 8, 8, PRIM_RECT | RECT_BLEND); // `setTile(); setSemiTrans();`
        setXY0Fast(var2, -(SCREEN_WIDTH / 2), -FRAMEBUFFER_HEIGHT_PROGRESSIVE);
        setWHFast(var2, SCREEN_WIDTH, FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2);
        DrawPrim(var2);

        Fs_QueueUpdate();
        VSync(SyncMode_Wait);
    }

    Fs_QueueWaitForEmpty();
}

void func_80066E40(void) // 0x80066E40
{
    DrawSync(SyncMode_Wait);
    StoreImage(&D_80028A20, FS_BUFFER_3);
    DrawSync(SyncMode_Wait);
}

void func_80066E7C(void) // 0x80066E7C
{
    LoadImage(&D_80028A20, FS_BUFFER_3);
    DrawSync(SyncMode_Wait);
}
