#include "grouplist.h"



void GroupList::addNewGroup(Chat &chat)
{

}

GroupList::GroupList(QWidget *parent)
    :QListWidget(parent)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText("kek");
    this->setStyleSheet(
                        "QListWidget{ outline: 0;}"
                        "QListWidget::item:hover{   background-color:  rgb(240, 240, 243);selection-color: transperent;}"
                        "QListWidget::item:selected{ background-color:  rgb(235, 235, 248);selection-color: transperent;}"
                       );
    this->addItem(item);

    getChatRoomCallback = [](const ChatRoom chatroom) {};
    getChatCallback = [](std::vector<chatinfo> &items){};
}

std::list<QListWidgetItem *> GroupList::getItems()
{
    std::list<QListWidgetItem*> listItems;
    for(int i = 0; i < this->count(); ++i){
        listItems.push_back(this->item(i));
    }
    return listItems;
}

void GroupList::getChatById()
{

}

void GroupList::getItemsFromNet()
{

}
