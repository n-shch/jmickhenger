#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>
#include "app-qt/src/custommbutton/custombutton.h"
#include <netlib/info/Info.h>
#include <memory>
using MessageItem = inf::Message;

class Msg : public MessageItem
{
public:
    QString nickname;
    Msg(MessageItem &item)
        :MessageItem(std::move(item)){
    }
    Msg(){};
};

Q_DECLARE_METATYPE(Msg)
class ChatModel : public QAbstractListModel,public enable_shared_from_this<ChatModel>
{
    Q_OBJECT
public:
    ChatModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void createMessage(Msg &_message);
    void addCallbacks();

    void setData(std::vector<MessageItem>& msgs);
    std::function<void(std::vector<MessageItem>&,std::optional<string>&)> &getChatCallback() ;
    std::function<void(std::optional<string>&)> &getSendMsgCallback() ;
    std::function<void(std::optional<string>&)> &getChangeMsgCallback() ;
    std::function<void(std::optional<string>&)> &getDelMsgCallback() ;

    void slotEditMessage();
    void DeleteMessage(int pos);
private:
    std::vector<Msg> items;
    bool newMessageOnBottom;
    std::optional<string> errString;
    std::function<void(std::vector<MessageItem>&,std::optional<string>&)> chatCallback;
    std::function<void(std::optional<string>&)> sendMsgCallback;
    std::function<void(std::optional<string>&)> changeMsgCallback;
    std::function<void(std::optional<string>&)> delMsgCallback;
signals:
};

#endif // CHATMODEL_H