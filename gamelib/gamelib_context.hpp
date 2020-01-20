#ifndef GAMELIB_CONTEXT_HPP
#define GAMELIB_CONTEXT_HPP

#include <gamelib_base.hpp>

namespace GameLib {
    constexpr int WindowDefault = 0;
    constexpr int WindowFullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP;
    constexpr int WindowBorderless = SDL_WINDOW_BORDERLESS;
    constexpr int WindowResizeable = SDL_WINDOW_RESIZABLE;
    constexpr int WindowOpenGL = SDL_WINDOW_OPENGL;

    class Context {
    public:
        Context(int width, int height, int flags = WindowResizeable);
        ~Context();

        operator bool() const { return initialized_; }
        bool hadError() const;
        const std::string errorString() const { return errorString_; }

        // handle all SDL events
        int getEvents();

        // clear the screen to a color
        void clearScreen(glm::u8vec4 color);

        // swap the back buffer to the front
        void swapBuffers();

        // add a search path for loading files
        void addSearchPath(const std::string& path);

        // clear all search paths for loading files
        void clearSearchPaths();

        // find a valid path using the configured search paths
        // if file is not a regular file, returns an empty string
        std::string findSearchPath(const std::string& filename) const;

        // load the filename from the current directory, or the search paths
        SDL_Texture* loadImage(const std::string& filename);

        // frees all currently loaded images
        void freeImages();

        // returns true if the resource name is loaded
        bool imageLoaded(const std::string& resourceName) const;

        // returns a pointer to the SDL Surface, or nullptr if it does not exist
        SDL_Texture* getImage(const std::string& resourceName) const;

        // draws a rectangle to the screen. returns 0 if success, -1 if error
        int drawTexture(glm::vec2 position, glm::vec2 size, SDL_Texture* texture);

        // set quitRequested to a nonzero value to indicate the game loop should end
        int quitRequested{ 0 };

        // keyboard is used to represent the current keys pressed
        struct KEYBOARDSTATE {
            std::vector<int> scancodes;
            std::map<int, int> keys;
            int mod{ 0 };
        } keyboard;

        // MaxJoysticks reflects the XInput library max of four controllers
        static constexpr int MaxJoysticks{ 4 };

        // This is the number of joysticks available
        unsigned joystickCount{ 0 };

        // This is an array of game pad information. If controller pointer is not null, it is available
        struct JOYSTICKSTATE {
            bool enabled{ false };
            SDL_GameController* controller{ nullptr };
            std::string name;
        } joysticks[MaxJoysticks];

    private:
        bool _init();
        bool _initSubsystems();
        bool _initScreen(int width, int height, int windowFlags);
        void _openGameControllers();
        void _closeGameControllers();
        void _updateGameControllers();
        void _kill();
        void _setError(std::string&& errorString);
        bool initialized_{ false };
        mutable bool hadError_{ false };
        std::string errorString_;

        std::vector<std::string> searchPaths_;
        std::map<std::string, SDL_Texture*> images_;

        SDL_Window* window_{ nullptr };
        SDL_Renderer* renderer_{ nullptr };
    };
}

#endif