#include "gui.h"

/* ---------------------------------------------------------------------------
 * Callbacks
 * ------------------------------------------------------------------------- */
void gui_default_setup(Gui* gui, void* arg) {}

void gui_default_handle(Gui* gui, void* arg)
{
    while (SDL_PollEvent(&gui->ev))
    { // Store in renderer struct
        switch (gui->ev.type)
        {
        case SDL_QUIT: gui->quit = true; break;
        case SDL_KEYDOWN:
            if (gui_key(gui->ev) == SDLK_ESCAPE) gui->quit = true;
            break;
        }
    }
}

void gui_default_render(Gui* gui, void* arg) { gui->Clear(); }

/* ---------------------------------------------------------------------------
 * Init, Destroy Run
 * ------------------------------------------------------------------------- */
GuiError Gui::Init(char* title, i32 width, i32 height, GuiCallbacks callbacks)
{
    bounds   = SDL_Rect{0, 0, width, height};
    interval = 1000 / FPS;
    setup    = callbacks.setup;
    handle   = callbacks.handle;
    render   = callbacks.render;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) return GUI_FAILED_INIT;
    SDL_ShowCursor(0);

    win = SDL_CreateWindow(title, 0, 0, bounds.w, bounds.h, SDL_WINDOW_SHOWN);
    if (win == NULL) return GUI_FAILED_WINDOW;

    rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL) return GUI_FAILED_RENDERER;

    // Load font
    {
        // TODO: Use SDL fontcache
    }

    Fill(SDL_Color{0});
    SDL_RenderPresent(rnd);

    return GUI_SUCCESS;
}

GuiError Gui::Destroy()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rnd);
    SDL_Quit();
    return GUI_SUCCESS;
}

GuiError Gui::Run(void* arg)
{
    quit = false;
    while (!quit)
    {
        tstart = SDL_GetTicks();

        handle(this, arg);
        render(this, arg);
        SDL_RenderPresent(rnd);

        telapsed = SDL_GetTicks() - tstart;
        if (telapsed < interval) SDL_Delay(interval - telapsed);
    }
    return GUI_SUCCESS;
}

GuiError Gui::Show(char* title, i32 width, i32 height, GuiCallbacks callbacks, void* arg)
{
    GuiError err;

    err = Init(title, width, height, callbacks);
    if (err) return err;

    setup(this, arg);

    err = Run(arg);
    if (err) return err;

    err = Destroy();
    if (err) return err;

    return GUI_SUCCESS;
}

void Gui::Clear()
{
    Fill(SDL_Color{0});
    SDL_RenderClear(rnd);
}

void Gui::Fill(SDL_Color color)
{
    SDL_SetRenderDrawColor(rnd, color.r, color.g, color.g, color.a);
    SDL_RenderClear(rnd);
}

void Gui::Rect(SDL_Rect bounds, SDL_Color color)
{
    SDL_SetRenderDrawColor(rnd, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(rnd, &bounds);
}
