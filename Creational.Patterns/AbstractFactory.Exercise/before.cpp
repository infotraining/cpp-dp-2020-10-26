#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

#define MOTIF

enum class IconType
{
    none,
    ok,
    cancel,
    warning,
    error
};

class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class Button : public Widget
{
    std::string caption_;
    IconType icon_type_;

public:
    Button(const std::string& caption, IconType icon_type)
        : caption_ {caption}
        , icon_type_ {icon_type}
    {
    }

    std::string caption() const
    {
        return caption_;
    }

    IconType icon() const
    {
        return icon_type_;
    }
};

class Menu : public Widget
{
    std::string text_;

public:
    Menu(const std::string& text)
        : text_ {text}
    {
    }

    std::string text() const
    {
        return text_;
    }
};

class MotifButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "MotifButton [ " << caption() << " ]\n";
    }
};

class MotifMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "MotifMenu { " << text() << " }\n";
    }
};

class WindowsButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "WindowsButton [ " << caption() << " ]\n";
    }
};

class WindowsMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "WindowsMenu { " << text() << " }\n";
    }
};

class WidgetAbstractFactory
{
public:
    virtual ~WidgetAbstractFactory() = default;
    virtual std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) = 0;
    virtual std::unique_ptr<Menu> CreateMenu(const std::string& text) = 0;
};

class MotifWidgetFactory : public WidgetAbstractFactory
{
public:
    std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) override
    {
        return std::make_unique<MotifButton>(caption, icon_type);
    }
    std::unique_ptr<Menu> CreateMenu(const std::string& text) override
    {
        return std::make_unique<MotifMenu>(text);
    }
};

class WindowsWidgetFactory : public WidgetAbstractFactory
{
public:
    std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) override
    {
        return std::make_unique<WindowsButton>(caption, icon_type);
    }
    std::unique_ptr<Menu> CreateMenu(const std::string& text) override
    {
        return std::make_unique<WindowsMenu>(text);
    }
};

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;
protected:
    WidgetAbstractFactory& widgetFactory_;

public:
    Window(WidgetAbstractFactory& widgetFactory)
        : widgetFactory_(widgetFactory)
    {
    }

    void display() const
    {
        std::cout << "######################\n";
        for (const auto& w : widgets)
            w->draw();
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(move(widget));
    }
};

class WindowOne : public Window
{

public:
    WindowOne(WidgetAbstractFactory& widgetFactory)
        : Window(widgetFactory)
    {
        add_widget(widgetFactory_.CreateButton("OK", IconType::ok));
        add_widget(widgetFactory_.CreateMenu("File"));
    }
};

class WindowTwo : public Window
{

public:
    WindowTwo(WidgetAbstractFactory& widgetFactory)
        : Window(widgetFactory)
    {
        add_widget(widgetFactory_.CreateMenu("Edit"));
        add_widget(widgetFactory_.CreateButton("OK", IconType::ok));
        add_widget(widgetFactory_.CreateButton("Cancel", IconType::cancel));
    }
};

int main(void)
{
#ifdef MOTIF
    MotifWidgetFactory widgetFactor;
#else
    WindowsWidgetFactory widgetFactor;
#endif

    WindowOne w1(widgetFactor);
    w1.display();

    WindowTwo w2(widgetFactor);
    w2.display();
}
