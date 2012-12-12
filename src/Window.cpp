#include "Window.hpp"
#include <limits>

const double PI180=3.141592653589793/180;

Window::Window(std::string title, Vector _L, const unsigned int subdivisions)
{
    q_value = 0;
    w_value = 0;

    position_camera = Vector(0.f, 0.f, -25.f);
    position_mouse = Vector(0.f, 0.f);

    L=_L;
    // Create the main window
    sf::WindowSettings settings;
    settings.DepthBits=24;
    settings.StencilBits=8;
    settings.AntialiasingLevel=2;
    windowRatio=800./600;
    App=new sf::Window(sf::VideoMode(L[0], L[0]/windowRatio, 32), title, sf::Style::Close, settings);

    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.2f, 1.f, 500.f);
    App->SetFramerateLimit(50);
    (this)->m_segments = subdivisions;

}

void Window::ProcessEvents()
{
    const sf::Input &Input = App->GetInput();
    App->SetActive();
    if (!App)
        throw std::logic_error("Window::isOpen(): there is no window!");
    // Process events
    sf::Event Event;
    while (App->GetEvent(Event))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Close window : exit
        if (Event.Type == sf::Event::Closed)
            App->Close();

        // Escape key : exit
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
            App->Close();

        // left, right, up, down arrows : move frame
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Left))
            position_camera[0] += 1.0f;

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Right))
            position_camera[0] -= 1.0f;

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down))
            position_camera[1] += 1.0f;

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up))
            position_camera[1] -= 1.0f;

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Add))
        {
            position_camera[2] += 10.0f;
        }

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Subtract))
        {
            position_camera[2] -= 10.0f;
        }

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Q))
        {
            (this)->q_value++;
        }

        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::W))
        {
            (this)->w_value++;
        }
        // Resize event : adjust viewport
        if (Event.Type == sf::Event::Resized)
            glViewport(0, 0, Event.Size.Width, Event.Size.Height);

        // Minus half of the screen width and height
        // because the OpenGL origin is in the middle of the screen
        position_mouse[0] = Input.GetMouseX()-App->GetWidth()/2.f;
        position_mouse[1] = Input.GetMouseY()-App->GetHeight()/2.f;

        // I don't know any better way to flip the Y axis so this is what I did
        if(position_mouse[1] >= 0) {
            position_mouse[1] = -(position_mouse[1]);
        }
        else {
            position_mouse[1] = abs(position_mouse[1]);
        }

        // Calculate the angle which the polygon needs to rotate at
    }
}

void Window::Clear()
{
    App->SetActive();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::DrawCircle(const Vector &pos, const Color &color, const double r) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);

    //Draw single particle
    glColor3f(color.mR, color.mG, color.mB);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    const double frequency = 2.0*M_PI/(this)->m_segments;
    for(unsigned int i = 0; i < (this)->m_segments; i++) {
        const double angle = i*frequency;
        glVertex3f(pos[0] + (cos(angle) * r), pos[1] + (sin(angle) * r), 0.0f);
    }
    glEnd();
}

void Window::DrawRectangle(const Vector &pos, const double width, const double height, const Color &color) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);
    glLineWidth(1.0f);
    glColor3f(color.mR, color.mG, color.mB);
    glBegin(GL_LINE_LOOP);
        glVertex3f(pos[0] -0.5*width, pos[1]+0.5*height, 0.0);
        glVertex3f(pos[0]+0.5*width,  pos[1]+0.5*height, 0.0);
        glVertex3f(pos[0]+0.5*width,  pos[1]-0.5*height, 0.0);
        glVertex3f(pos[0] -0.5*width, pos[1]-0.5*height, 0.0);
    glEnd();
}

void Window::DrawArrow(const Vector &x, const Vector &y, const Color &color) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);
    glLineWidth(1.0f);
    glColor3f(color.mR, color.mG, color.mB);
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0], x[1], 0.0);
        glVertex3f(y[0], y[1], 0.0);
    glEnd();
}

void Window::DrawHline(const double x, const double y, const Color &color) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);
    glLineWidth(1.0f);
    glColor3f(color.mR, color.mG, color.mB);

    glBegin(GL_LINE_STRIP);
        glVertex3f(x, -y/2.0, 0.0);
        glVertex3f(x,  y/2.0, 0.0);
    glEnd();
}

void Window::DrawVline(const double x, const double y, const Color &color) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);
    glLineWidth(1.0f);
    glColor3f(color.mR, color.mG, color.mB);

    glBegin(GL_LINE_STRIP);
        glVertex3f(-x/2.0, y, 0.0);
        glVertex3f( x/2.0, y, 0.0);
    glEnd();
}

void Window::DrawErrorbar(const Vector &x, const double size, const double error, const Color &color) const
{
    glColor3f(color.mR, color.mG, color.mB);
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0] - size, x[1] - error, x[2]);
        glVertex3f(x[0] + size, x[1] - error, x[2]);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0] - size, x[1] + error, x[2]);
        glVertex3f(x[0] + size, x[1] + error, x[2]);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0], x[1] - error, x[2]);
        glVertex3f(x[0], x[1] + error, x[2]);
    glEnd();
}

void Window::DrawCross(const Vector &x, const double size, const Color &color) const
{
    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(1.0f);

    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);

    glColor3f(color.mR, color.mG, color.mB);
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0] - size, x[1], x[2]);
        glVertex3f(x[0] + size, x[1], x[2]);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex3f(x[0], x[1] - size, x[2]);
        glVertex3f(x[0], x[1] + size, x[2]);
    glEnd();
}

void Window::DrawVector(const Vector &vec, const Color &color, const double zoom) const
{
    Vector v = vec;
    v = v / norm(v) / (-log(norm(v)));
    v *= 15.0 * zoom;

    App->SetActive();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(position_camera[0], position_camera[1], position_camera[2]);

    //Draw single particle
    glLineWidth(1.0f);
    glColor3f(color.mR, color.mG, color.mB);
    glBegin(GL_LINE_STRIP);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(v[0], v[1],v[2]);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(0.0, -20.0, 0.0);
        glVertex3f(0.0,  20.0, 0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex3f(-25.0, 0.0, 0.0);
        glVertex3f( 25.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++) {
        double angle = i*2*M_PI/100;
        glVertex3f(cos(angle) * 15.0, sin(angle) * 15.0, 0.0);
    }
    glEnd();
}

void Window::SetCameraPosition(const Vector& pos){
	position_camera = pos;
}
