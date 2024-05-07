#include "Classes/Render.h"

void DebugPrintMatrixMode(const std::string& message) {
    GLint currentMatrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &currentMatrixMode);

    switch (currentMatrixMode) {
        case GL_MODELVIEW:
            std::cout << "DEBUG Current matrix mode: GL_MODELVIEW, " << message << std::endl;
            break;
        case GL_PROJECTION:
            std::cout << "DEBUG Current matrix mode: GL_PROJECTION, " << message << std::endl;
            break;
        case GL_TEXTURE:
            std::cout << "DEBUG Current matrix mode: GL_TEXTURE, " << message << std::endl;
            break;
        default:
            std::cout << "DEBUG Unknown matrix mode, " << message << std::endl;
            break;
    }
}

void initVideo()
{
	// SDL video device init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << SDL_GetError() << '\n';
		exit(1);
	}

	SDL_DisplayMode currentDisplayMode;
	for (size_t i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

		int displayMode = SDL_GetCurrentDisplayMode(i, &currentDisplayMode);
		if (displayMode != 0)
		{
			std::cout << SDL_GetError() << '\n';
			exit(1);
		}
	}

	width = currentDisplayMode.w;
	height = currentDisplayMode.h;

	// SDL TTF init
	TTF_Init();

	// SDL GL init
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	window = SDL_CreateWindow("Slot machine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		std::cout << SDL_GetError() << '\n';
		exit(1);
	}

	int result = SDL_SetWindowDisplayMode(window, &currentDisplayMode);
	if (result != 0)
	{
			std::cout << SDL_GetError() << '\n';
			exit(1);
	}

	// init GLContext
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if (glcontext == nullptr)
	{
		std::cout << SDL_GetError() << '\n';
		exit(1);
	}

	// init opengl
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

SDL_Surface* loadTexture(const std::string& fileName) {
  DebugPrintMatrixMode("ENTERING loadTexture");

	SDL_Surface* image = IMG_Load(fileName.c_str());
	if (!image)
	{
		std::cout << SDL_GetError() << '\n';
		exit(0);
	}
	image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);

	GLuint object;
	glGenTextures(1, &object);
	glBindTexture(GL_TEXTURE_2D, object);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  DebugPrintMatrixMode("BEFORE glTexImage2D IN loadTexture");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

  DebugPrintMatrixMode("AFTER glTexImage2D IN loadTexture");

  DebugPrintMatrixMode("LEAVING loadTexture");
	return image;
}

SDL_Surface* RenderText(const std::string& message, SDL_Color color, int x, int y, int size) {
  DebugPrintMatrixMode("ENTERING RenderText");

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, 0, height); // m_Width and m_Height is the resolution of window

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	TTF_Font* font = TTF_OpenFont("gamedata/fonts/arial.ttf", size);
	if (!font)
	{
		std::cout << SDL_GetError() << '\n';
		exit(1);
	}

	SDL_Surface* sFont = TTF_RenderText_Blended(font, message.c_str(), color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  DebugPrintMatrixMode("BEFORE glTexImage2D IN RenderText");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, sFont->pixels);

  DebugPrintMatrixMode("AFTER glTexImage2D IN RenderText");

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); glVertex2f(x, y);
		glTexCoord2f(1, 0); glVertex2f(x + sFont->w, y);
		glTexCoord2f(1, 1); glVertex2f(x + sFont->w, y + sFont->h);
		glTexCoord2f(0, 1); glVertex2f(x, y + sFont->h);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

  DebugPrintMatrixMode("READY TO POP IN RenderText");


	glPopMatrix(); // pop texture matrix
                 //
  glMatrixMode(GL_MODELVIEW); // return to modelview
  glPushMatrix();
  glLoadIdentity();

  DebugPrintMatrixMode("LEAVING RenderText");

	glDeleteTextures(1, &texture);

	return sFont;
}

void button_process_event(Button* btn, const SDL_Event* ev)
{
	if (ev->type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev->button.button == SDL_BUTTON_LEFT && ev->button.x >= btn->drawRect.x && ev->button.x <= (btn->drawRect.x + btn->drawRect.w) &&
			ev->button.y >= btn->drawRect.y && ev->button.y <= (btn->drawRect.y + btn->drawRect.h))
		{
			btn->pressed = true;
		}
	}
}

bool button(SDL_Surface* screen, Button* btn)
{
	SDL_BlitSurface(btn->surface, nullptr, screen, &btn->drawRect);

	if (btn->pressed)
	{
		SDL_BlitSurface(btn->surfaceHold, nullptr, screen, &btn->drawRect);
		btn->pressed = false;
		return true;
	}

	return false;
}
