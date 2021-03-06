#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "app-qt/src/custommbutton/custombutton.h"
#include "app-qt/src/menuwidget/menuwidget.h"
#include <app-qt/src/models/groupmodel.h>
#include <app-qt/src/views/grouplistview.h>
#include <app-qt/src/delegates/groupdelegate.h>
#include <app-qt/src/models/chatmodel.h>
#include <app-qt/src/views/chatview.h>
#include <app-qt/src/delegates/chatdelegate.h>
#include <app-qt/src/proxy/proxymodel.h>
#include <app-qt/src/models/usermodel.h>
#include <QFocusEvent>
#include <QAction>
#include <QMenu>
#include "ui_mainwidget.h"
#include <QGraphicsProxyWidget>

enum class Buttons{
    SEND_MESSAGE_BUTTON,
    MENU_BUTTON,
    GROUP_BUTTON,
};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    MenuWidget* getMenu();

public slots:
    void menuClicked();
    void sendMessageFromInput();
    void after_Login_slot();
    void removeMessageFromChat();
    void editMessageInChat();
    void messageEdited();
    void showContextMenu(const QPoint &pos);
    void setGroupInfoSlot(const inf::ChatRoom &room);
    void resizeSpacer(int height);
    void closeAll();
    void copyMessage();

signals:
    void sendMessageFromInput_s(const Message &message);
    void sendAvatar(const QString &path);
    void setTextForChange(const QString &text);
private:
    void removeDoubleEnter(QString &str);

protected:
    Ui::MainWidget *ui;
    std::shared_ptr<GroupModel> groupModel;
    std::shared_ptr<ChatModel> chatModel;
    MenuWidget *menuWidget;
    QMenu *msgMenu;
    ProxyModel *proxyModel;
    ChatDelegate *chatDelegate;
    int spacerHeight;

private slots:
    void on_groupList_clicked(const QModelIndex &index);
};

#endif // MAINWIDGET_H
