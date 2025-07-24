# Gui

## API

### Callbacks

```cpp
typedef struct Gui Gui; // Container for each platform
typedef void (*gui_fn_setup)(Gui* gui, void* arg);
typedef void (*gui_fn_render)(Gui* gui, void* arg);
typedef void (*gui_fn_handle)(Gui* gui, void* arg);
```

### Structs

```cpp
typedef enum {
    GUI_SUCCESS = 0,
    GUI_FAILED_INIT,
    GUI_FAILED_WINDOW,
    GUI_FAILED_INPUTS,
    GUI_FAILED_RENDERER,
    GUI_FAILED_IMAGE,
} GuiError;

typedef struct {
    const char*  path;
    SDL_Texture* tex;
    int          h, w;
} GuiFont;

typedef struct Gui {
    // SDL references
    SDL_Window*   win;
    SDL_Renderer* rnd;
    SDL_Event     ev;

    // State
    SDL_Rect bounds;
    bool     quit;

    // TODO: Use lambdas to handle keys?
    gui_fn_setup  setup  = gui_default_setup;
    gui_fn_handle handle = gui_default_handle;
    gui_fn_render render = gui_default_render;

    // Timing
    u32 interval;
    u32 tstart;
    u32 telapsed;

    // Font
    GuiFont font;

    // Lifetime
    GuiError Init(char* title, i32 width, i32 height, GuiCallbacks callbacks);
    GuiError Run(void* arg);
    GuiError Destroy();

    // Oneshot, share arg with callbacks
    GuiError Show(char* title, i32 width, i32 height, GuiCallbacks callbacks, void* arg);

    // Methods
    void Clear();
    void Fill(SDL_Color color);
    void Rect(SDL_Rect bounds, SDL_Color color);

    // TODO: Use SDL FontCache
    // void Text(char* text, int len, int x, int y);

} Gui;
```
