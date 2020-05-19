#include "app-qt/src/mainwidget/mainwidget.h"
#include "app-qt/src/models/chatmodel.h"
#include "app-qt/src/chatinput/chatinput.h"
#include "netlib/AppNetwork.h"
#include "netlib/info/Info.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    qRegisterMetaType<inf::ChatRoom>();
    ui->setupUi(this);

    menuWidget = new MenuWidget(this);
    msgMenu = new QMenu(this);
    chatModel = std::make_shared<ChatModel>();
    groupModel = std::make_shared<GroupModel>();
    groupModel->addCallbacks();
    chatModel->addCallbacks();

    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(groupModel.get());
    ui->groupList->setModel(proxyModel);
    ui->groupList->setItemDelegate(new GroupDelegate);
    ui->groupList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->searchInput->setWindowFlag(Qt::FramelessWindowHint);
    ui->messageInput->setWindowFlag(Qt::FramelessWindowHint);

    QPalette linesPallet(palette());
    linesPallet.setColor(QPalette::Light,QColor(230,230,230));
    ui->line->setPalette(linesPallet);
    ui->line_4->setPalette(linesPallet);
    ui->line_2->setPalette(linesPallet);
    ui->line_3->setPalette(linesPallet);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    auto font =  ui->label->font();
    font.setBold(true);
    ui->label->setFont(font);
    ui->chatList->setModel(chatModel.get());
    ui->chatList->setItemDelegate(new ChatDelegate);
    ui->chatList->setSelectionMode(QAbstractItemView::NoSelection);
    ui->chatList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QAction * editDevice = new QAction("Редактировать", this);
    QAction * deleteDevice = new QAction("Удалить", this);

    connect(editDevice, &QAction::triggered, this, &MainWidget::editMessageInChat);
    connect(deleteDevice, &QAction::triggered, this, &MainWidget::removeMessageFromChat);

    msgMenu->addAction(editDevice);
    msgMenu->addAction(deleteDevice);
    ui->chatList->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->chatList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->chatList->setSelectionMode(QAbstractItemView::NoSelection);
    ui->chatList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(menuWidget->getCreateWidget(),&CreateGroupWidget::groupCreated,groupModel.get(),&GroupModel::createChatByUser);
    connect(ui->chatList,&ChatView::customContextMenuRequested,this,&MainWidget::showContextMenu);
    connect(ui->menuButton,&CustomButton::clicked,menuWidget,&MenuWidget::show);
    connect(ui->menuButton,&CustomButton::clicked,this,&MainWidget::menuClicked);
    connect(ui->sendButton,&CustomButton::clicked,this,&MainWidget::sendMessageFromInput);
    connect(ui->searchInput,&QLineEdit::textChanged,proxyModel,&ProxyModel::search_String_Changed);
    connect(ui->messageInput,&ChatInput::sendMessageOnEnter,this,&MainWidget::sendMessageFromInput);
    connect(chatModel.get(),&ChatModel::messageCreateByUser,groupModel.get(),&GroupModel::messageCreateByUser);
    connect(groupModel.get(),&GroupModel::sendChatRoom,this,&MainWidget::setGroupInfoSlot);
    connect(this,&MainWidget::sendAvatar,menuWidget,&MenuWidget::on_image_change);
    connect(groupModel.get(),&GroupModel::sendNewMessages,chatModel.get(),&ChatModel::newMessages);
    connect(groupModel.get(),&GroupModel::updateItems,ui->groupList,&GroupListView::doItemsLayout);
    connect(chatModel.get(),&ChatModel::updateItems,ui->chatList,&ChatView::doItemsLayout);
    this->setLayout(ui->MainLayout);
}

MainWidget::~MainWidget()
{
    delete ui;
    delete menuWidget;

}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    menuWidget->resize(0,event->size().height());
}

void MainWidget::moveEvent(QMoveEvent *event)
{
    menuWidget->move(event->pos());
}

void MainWidget::wheelEvent(QWheelEvent *event)
{
    ui->chatList->doItemsLayout();
}

void MainWidget::menuClicked()
{
    menuWidget->move(QPoint(geometry().x(),geometry().y()));
    emit menuWidget->setPosition();
}

void MainWidget::sendMessageFromInput()
{

    Msg message;
    QString text = ui->messageInput->toPlainText().trimmed();
    if(text.size() > 1024 || text == "")
        return;
    removeDoubleEnter(text);
    message.chatId = ui->groupList->selectionModel()->currentIndex().data().value<Chat>().idChat;
    message.text = text.toStdString();
    message.nickname = QString::fromStdString(UserModel::instance()->getAcc().login);
    message.idOwner = UserModel::instance()->getId();
    long int ttime;
    ttime = time(NULL);
    message.timesend  = ttime;

    chatModel->createMessage(message);
    emit chatModel->messageCreateByUser(message);
    ui->messageInput->clear();
    auto net = AppNet::shared();
    net->sendMsg(message,chatModel->getSendMsgCallback());
    emit ui->chatList->doItemsLayout();
    emit ui->groupList->doItemsLayout();
}

void MainWidget::on_groupList_clicked(const QModelIndex &index)
{
    ui->label->setText(QString::fromStdString(index.model()->data(index).value<Chat>().name));
    auto net = AppNet::shared();
    Chat chat = index.model()->data(index).value<Chat>();
    chatModel->getMessagesInChat(chat.lastMessage);
    groupModel->chatInfoSet(chat.idChat);
    ui->chatList->doItemsLayout();
}


void MainWidget::after_Login_slot()
{
    this->show();
    Account ac = UserModel::instance()->getAcc();
    auto net = AppNet::shared();
    net->getListChat(ac.id,groupModel->getChatCallBack());
    menuWidget->setName(QString::fromStdString(ac.login));
    emit sendAvatar(QString::fromStdString(ac.pathToAvatar));
    connect(UserModel::instance(),&UserModel::nickNameChanged,menuWidget,&MenuWidget::on_nickname_rename);
}

void MainWidget::removeMessageFromChat()
{
    Msg msg = ui->chatList->selectionModel()->currentIndex().data().value<Msg>();
    if(msg.idOwner == UserModel::instance()->getId()){
        chatModel->DeleteMessage(ui->chatList->selectionModel()->currentIndex().row());
    }
    ui->chatList->doItemsLayout();
}

void MainWidget::editMessageInChat()
{

}

void MainWidget::showContextMenu(const QPoint &pos)
{
    msgMenu->popup(ui->chatList->viewport()->mapToGlobal(pos));
}

void MainWidget::setGroupInfoSlot(const inf::ChatRoom &room)
{
    QString info = QString::number(room.idUsers.size());
    if(room.idUsers.size() <= 2)
        info += "  участника";
    else
        info += "  участников";

    ui->label_2->setText(info);
}

void MainWidget::removeDoubleEnter(QString &str){

    int counter = 0;
    int pos = 0;
    for(int i = 0; i < str.size(); ++i){
        if(str[i] == '\n'){
            pos = i;
            counter++;
            ++i;
            while(str[i] == '\n'){
                counter++;
                ++i;
            }
        }
        else
            counter = 0;
        if(counter > 1){
            str.remove(pos + 1,counter - 1);
        }
    }
}

