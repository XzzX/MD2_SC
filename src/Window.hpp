#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Window.hpp>
#include <stdexcept>
#include <GL/glut.h>

#include <iostream>
#include <list>
#include	"Color.h"
#include "Vector.hpp"
#include "Particle.hpp"

#include <SFML/Graphics.hpp>

class Window
{
  public:
    Window();
    Window(std::string title, Vector _L, const unsigned int subdivisions);
    ~Window();
    bool IsOpen();
    void ProcessEvents();

    void DrawCircle(const Vector &pos, const Color &color, const double r) const;
    void DrawRectangle(const Vector &pos, const double width, const double height, const Color& color) const;
    void DrawHline(const double x, const double y, const Color &color) const;
    void DrawVline(const double x, const double y, const Color &color) const;
    void DrawCross(const Vector &x, const double size, const Color &color) const;
    void DrawErrorbar(const Vector &x, const double size, const double error, const Color &color) const;
    void DrawVector(const Vector &vec, const Color &color, const double zoom) const;
    void DrawArrow(const Vector &x, const Vector &y, const Color &color) const;
    void Display();
    void Close();
    void Clear();

    int q_value;
    int w_value;
    int a_value;
    int s_value;
    int y_value;
    int x_value;

	void SetCameraPosition(const Vector& pos);

    sf::RenderWindow* App;

  private:
    GLuint circleList;
    Vector L;
    Vector position_camera;
    Vector position_mouse;

    double windowRatio;
    unsigned int referenceCounter;
    unsigned int m_segments;
};

inline Window::Window()
{
  App=NULL;
}


inline double dmod(double a, double b)
{
  return a-static_cast<int>(a/b)*b;
}

inline void Window::Display()
{
    if (!App)
        throw std::logic_error("Window::isOpen(): there is no window!");
        // Finally, display rendered frame on screen
    App->Display();
}

inline void Window::Close()
{
    App->Close();
}

inline Window::~Window()
{
    if (App)
    {
        // FIXME: copy-constructor not dealing right with this:
        delete App;
    }
}

inline bool Window::IsOpen()
{
    if (!App)
        throw std::logic_error("Window::isOpen(): there is no window!");
    return App->IsOpened();
}


#endif
