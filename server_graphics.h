#ifndef SERVER_GRAPHICS_H
#define SERVER_GRAPHICS_H
#include"structs_ch.h"
#include<QStringListModel>


class MainWindow;
class MainWindowDesigner
{
public:
    MainWindowDesigner(std::shared_ptr<MainWindow> mainwin);
    void EditStatusLabel( str_type text, QColor color);
    void StatusLabelOn();
    void StatusLabelOff();
protected:
    std::shared_ptr<MainWindow> _mainwin;
    QPalette _palette_status;
    std::shared_ptr<QStringListModel> _room_list_model;
};

class GraphicWidgets{
protected:
    friend class GraphicsServer;
    friend class ServerSession;
    std::shared_ptr<MainWindow> _maiwin;
    std::shared_ptr<MainWindowDesigner> _maiwindes;
};











#endif // SERVER_GRAPHICS_H
