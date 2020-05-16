#include "gtest/gtest.h"

#include <iostream>
#include <ctime>

#include "AppNetwork.h"

using namespace inf;
using namespace std;

TEST(testAnnounser, test1) {
    Announcer an;
    int c = 0;
    auto testCallback = [&c](inf::ChatChange & change){
        ++c;
    };
    int idChat = 781;
    inf::Message msg(781, 903, "i am 903 msg in 781 chat from user id 67", 67, time(nullptr), false);
    inf::ChatChange testChange(idChat, "newMsg", {msg});
    an.addChatCallback(idChat, testCallback);
    an.notifyChat(testChange);
    ASSERT_EQ(1, c);
}

TEST(testAnnounser, test2) {
    Announcer an;
    int c = 0;
    auto testCallback = [&c](inf::ChatChange & change){
        ++c;
    };
    int idChat = 781;
    inf::Message msg(781, 903, "i am 903 msg in 781 chat from user id 67", 67, time(nullptr), false);
    inf::ChatChange testChange(idChat, "newMsg", {msg});
    an.addChatCallback(idChat, testCallback);
    int n = 78;
    for(size_t i = 0; i < n; ++i) {
        an.notifyChat(testChange);
    }
    ASSERT_EQ(c, n);
}

TEST(testAnnounser, test3) {
    Announcer an;
    int c = 0;
    auto testCallback = [&c](inf::ChatChange & change){
        ++c;
    };
    int idChat = 781;
    inf::Message msg(781, 903, "i am 903 msg in 781 chat from user id 67", 67, time(nullptr), false);
    int idOtherChat = 98;
    inf::ChatChange testChange(idOtherChat, "newMsg", {msg});
    an.addChatCallback(idChat, testCallback);
    int n = 78;
    for(size_t i = 0; i < n; ++i) {
        an.notifyChat(testChange);
    }
    ASSERT_EQ(c, 0);
}

TEST(testAnnounser, test4) {
    Announcer an;
    int c = 0;
    string context = "test4";
    int n = 89;
    int tmp = 0;
    function<void(int &)> testCallback = [&n](int & number) {
        number = n;
    };
    int cmd = 8;
    an.addCallback<string, int &>(cmd, context, testCallback);
    auto f = an.getCallback<string, int &>(cmd, context);
    ASSERT_EQ((bool)f, true);
    f.value()(tmp);
    ASSERT_EQ(tmp, n);
    f = an.getCallback<string, int &>(cmd, context);
    ASSERT_EQ((bool)f, false);
}

TEST(testAnnounser, test5) {
    Announcer an;
    int c = 0;
    string context = "test5";
    string otherContext = "blablabla";
    int n = 89;
    int tmp = 0;
    function<void(int &)> testCallback = [&n](int & number) {
        number = n;
    };
    int cmd = 8;
    an.addCallback<string, int &>(cmd, context, testCallback);
    auto f = an.getCallback<string, int &>(cmd, otherContext);
    ASSERT_EQ((bool)f, false);
}

TEST(testCodeble, test1) {
    string json = "{\n"
                  "    \"idChat\": \"5\",\n"
                  "    \"name\": \"testChat\"\n"
                  "}";
    ChatInfo info; info.decode(json);
    ASSERT_EQ(info.idChat,5);
    ASSERT_EQ(info.name, "testChat");
}

TEST(testCodeble, test2) {
    string json = "{\n"
                  "    \"myId\": \"78\",\n"
                  "    \"login\": \"testUser\",\n"
                  "    \"password\": \"12345\",\n"
                  "    \"pathImage\": \"3\",\n"
                  "    \"chats\": [\n"
                  "        \"1\",\n"
                  "        \"189\",\n"
                  "        \"90\"\n"
                  "    ],\n"
                  "    \"friends\": [\n"
                  "        \"78\",\n"
                  "        \"90\",\n"
                  "        \"23\"\n"
                  "    ]\n"
                  "}";
    MyAccount acc; acc.decode(json);
    ASSERT_EQ(acc.id, 78);
    ASSERT_EQ(acc.login, "testUser");
    ASSERT_EQ(acc.password, "12345");
    vector<int> chatsTest = {1, 189, 90};
    for(size_t i = 0; i < acc.chats.size(); ++i) {
        ASSERT_EQ(acc.chats[i], chatsTest[i]);
    }
    vector<int> frndsTest = {78, 90, 23};
    for(size_t i = 0; i < acc.friends.size(); ++i) {
        ASSERT_EQ(acc.friends[i], frndsTest[i]);
    }
}

TEST(testCodeble, test3) {
    string json = "{\n"
                  "    \"idChat\": \"78\",\n"
                  "    \"number\": \"6\",\n"
                  "    \"text\": \"msg number 6 for 78 chat from 99 user\",\n"
                  "    \"owner\": \"99\",\n"
                  "    \"time\": \"1589639870\",\n"
                  "    \"checked\": \"false\"\n"
                  "}";
    Message msg; msg.decode(json);
    ASSERT_EQ(msg.chatId, 78);
    ASSERT_EQ(msg.number, 6);
    ASSERT_EQ(msg.text, "msg number 6 for 78 chat from 99 user");
    ASSERT_EQ(msg.idOwner, 99);
    ASSERT_EQ(msg.timesend, 1589639870);
    ASSERT_EQ(msg.checked, false);
}

TEST(testCodeble, test4) {
    string json = "{\n"
                  "    \"idUser\": \"4\",\n"
                  "    \"login\": \"testUser\",\n"
                  "    \"pathImage\": \"3avatar.jpg\"\n"
                  "}";
    UserInfo info; info.decode(json);
    ASSERT_EQ(info.id, 4);
    ASSERT_EQ(info.login, "testUser");
    ASSERT_EQ(info.pathToAvatar, "3avatar.jpg");
}

TEST(testCodeble, test5) {
    string json = "{\n"
                  "    \"idChat\": \"6\",\n"
                  "    \"name\": \"testName\",\n"
                  "    \"users\": [\n"
                  "        \"5\",\n"
                  "        \"19\",\n"
                  "        \"90\"\n"
                  "    ],\n"
                  "    \"admins\": [\n"
                  "        \"5\"\n"
                  "    ]\n"
                  "}";
    ChatRoom room; room.decode(json);
    ASSERT_EQ(room.idChat, 6);
    ASSERT_EQ(room.name, "testName");
    vector<int> testUsers = {5, 19, 90};
    for(size_t i = 0; i < room.idUsers.size(); ++i) {
        ASSERT_EQ(testUsers.at(i), room.idUsers.at(i));
    }
    vector<int> testAdmins = {5};
    for(size_t i = 0; i < room.idAdmins.size(); ++i) {
        ASSERT_EQ(testAdmins.at(i), room.idAdmins.at(i));
    }
}

TEST(testCodeble, test6) {
    string json = "{\n"
                  "    \"idChat\": \"78\",\n"
                  "    \"cmd\": \"newMsg\",\n"
                  "    \"messages\": [\n"
                  "        {\n"
                  "            \"idChat\": \"6\",\n"
                  "            \"number\": \"2\",\n"
                  "            \"text\": \"randomText\",\n"
                  "            \"owner\": \"999\",\n"
                  "            \"time\": \"1589657915\",\n"
                  "            \"checked\": \"false\"\n"
                  "        }\n"
                  "    ]\n"
                  "}";

    ChatChange change; change.decode(json);
    ASSERT_EQ(change.idChat, 78);
    ASSERT_EQ(change.action, "newMsg");
    Message testMsg(6, 2, "randomText", 999, time(nullptr), false);
    vector<Message> testMsgs = {testMsg};
    for(size_t i = 0; i < change.messages.size(); ++i) {
        ASSERT_EQ(testMsgs.at(i).text, change.messages.at(i).text);
        // дальше нет смысла проверять, и так inf::Message уже проверен
    }
}

TEST(testCodeble, test7) {
    string json = "{\n"
                  "    \"error\": \"\",\n"
                  "    \"status\": \"0\",\n"
                  "    \"cmd\": \"5\",\n"
                  "    \"body\": {\n"
                  "        \"idChat\": \"6\",\n"
                  "        \"number\": \"2\",\n"
                  "        \"text\": \"randomText\",\n"
                  "        \"owner\": \"999\",\n"
                  "        \"time\": \"1589658525\",\n"
                  "        \"checked\": \"false\"\n"
                  "    }\n"
                  "}";
    Message testMsg(6, 2, "randomText", 999, time(nullptr), false);
    Reply reply; reply.decode(json);
    ASSERT_EQ(reply.cmd, 5);
    ASSERT_EQ(reply.err, "");
    ASSERT_EQ(reply.status, 0);
    Message msg; msg.decode(reply.body);
    ASSERT_EQ(msg.text, testMsg.text);
    // дальше нет смысла проверять, и так inf::Message уже проверен
}

TEST(testCodeble, test8) {
    string json = "{\n"
                  "    \"cmd\": \"5\",\n"
                  "    \"body\": {\n"
                  "        \"idChat\": \"6\",\n"
                  "        \"number\": \"2\",\n"
                  "        \"text\": \"randomText\",\n"
                  "        \"owner\": \"999\",\n"
                  "        \"time\": \"1589659581\",\n"
                  "        \"checked\": \"false\"\n"
                  "    }\n"
                  "}";

    Query query; query.decode(json);
    ASSERT_EQ(query.cmd, 5);
    Message testMsg; testMsg.decode(query.body);
    ASSERT_EQ(testMsg.text, "randomText");
}
