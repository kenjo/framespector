#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <unistd.h>

void display_image(SDL_Window *window, SDL_Renderer *renderer, int width, int height, int text_size, int frame_rate, int vsync_enabled) {
    SDL_Event event;
    SDL_Texture *texture;
    SDL_Surface *image_surface;
    SDL_Color background_colors[2] = {{255, 0, 0, 255}, {0, 255, 0, 255}};
    int frame_num = 0;

    Uint32 frame_start;
    int frame_time;

    // Load the font outside the loop
    SDL_Color text_color = {0, 0, 0, 255};
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", text_size);
    if (!font) {
        fprintf(stderr, "Error: Could not load font. Make sure 'DejaVuSans.ttf' is present.\n");
        exit(1);
    }

    // Create a surface (image) to draw on outside the loop
    image_surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    while (1) {
        frame_start = SDL_GetTicks(); // Get the current time

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                TTF_CloseFont(font);
                SDL_FreeSurface(image_surface); // Free the surface
                SDL_Quit();
                exit(0);
            }
        }

        // Fill the surface with the alternating background color
        SDL_FillRect(image_surface, NULL, SDL_MapRGB(image_surface->format,
                                                     background_colors[frame_num % 2].r,
                                                     background_colors[frame_num % 2].g,
                                                     background_colors[frame_num % 2].b));

        // Draw the frame number on the surface
        char frame_text[16];
        snprintf(frame_text, sizeof(frame_text), "%d", frame_num + 1);
        SDL_Surface *text_surface = TTF_RenderText_Blended(font, frame_text, text_color);
        SDL_Rect text_rect = {(width - text_surface->w) / 2, (height - text_surface->h) / 2, 0, 0};
        SDL_BlitSurface(text_surface, NULL, image_surface, &text_rect);
        SDL_FreeSurface(text_surface);

        // Create texture from the surface
        texture = SDL_CreateTextureFromSurface(renderer, image_surface);

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Increase the frame number
        frame_num++;

        // Calculate the time taken for the frame
        frame_time = SDL_GetTicks() - frame_start;

        // Delay to control the frame rate
        if (!vsync_enabled) {
            if (frame_time < 1000 / frame_rate) {
                SDL_Delay((1000 / frame_rate) - frame_time);
            }
        }
        //}
    }

    // Clean up outside the loop
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image_surface);
    TTF_CloseFont(font);
}

int main(int argc, char *argv[]) {
    int width=1920, height=1080, text_size=200, frame_rate=60, vsync_enabled=0;
    SDL_Window *window;
    SDL_Renderer *renderer;

    int opt;
    while ((opt = getopt(argc, argv, "w:h:t:f:v")) != -1) {
        switch (opt) {
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 't':
            text_size = atoi(optarg);
            break;
        case 'f':
            frame_rate = atoi(optarg);
            break;
        case 'v':
            vsync_enabled = 1;
            break;

        default: /* '?' */
            fprintf(stderr, "Usage: %s -w <width> -h <height> -t <text_size> -f <frame_rate>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    window = SDL_CreateWindow("SDL Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    display_image(window, renderer, width, height, text_size, frame_rate, vsync_enabled);

    return 0;
}
