#include <NoweGlobal.h>
#include <QXmppVCardManager.h>
#include <QXmppBookmarkManager.h>
#include <QXmppBookmarkSet.h>

namespace Nowe {

class nowe_on_exit {
public:
    ~nowe_on_exit()
    {
        myClient()->disconnectFromServer();
    }
} static _;

QXmppClient* myClient() {
    static QXmppClient client;
    return &client;
}

const QXmppVCardIq& myVCard() {
    return myClient()->findExtension<QXmppVCardManager>()->clientVCard();
}

QString myJid() {
    return myClient()->configuration().jid();
}

QString myJidBare() {
    return myClient()->configuration().jidBare();
}

QXmppMucManager * myMucManager() {
    return myClient()->findExtension<QXmppMucManager>();
}

void sendBookMarkRequest()
{
    QXmppPubSubIq pbiq;
    pbiq.setQueryType(QXmppPubSubIq::ItemsQuery);
    pbiq.setQueryNode("storage:bookmarks");
    pbiq.setType(QXmppIq::Get);
    pbiq.setFrom(Nowe::myJid());
    myClient()->sendPacket(pbiq);

}

//创建聊天室书签
void createBookMark( QString markName)
{
    qDebug() << "createBookMark " << "markName" << markName << endl;

    //加载已存在的书签
    auto markMsg = myClient()->findExtension<QXmppBookmarkManager>();
    QXmppBookmarkSet markset = markMsg->bookmarks();

    //服务器书签列表
    QList<QXmppBookmarkConference> markList= markset.conferences();
    //服务器名
    QString serverName = "chirsz.cc";
    //聊天室JID
    QString jid=markName+"@conference."+serverName;


    //若该书签已经存在则返回
    foreach(QXmppBookmarkConference mark, markList)
    {
        if(mark.jid() == jid)
            return ;
    }

    //增加书签
    QXmppBookmarkConference * bm = new QXmppBookmarkConference;
    bm->setJid(jid);
    bm->setName(markName);
    bm->setAutoJoin(true);      //设置登陆时自动加入
    bm->setNickName(Nowe::myJidBare()); //必须设置
    markList.append(*bm);
    markset.setConferences(markList);

    markMsg->setBookmarks(markset);

}

void registerWithRoom(QString roomJid)
{
    QXmppElementList membersEls;
    QXmppElement queryEl;

    queryEl.setTagName("query");
    queryEl.setAttribute("xlmns", "jabber:iq:register");

    membersEls.append(queryEl);

    QXmppIq membersIq(QXmppIq::Get);
    membersIq.setTo(roomJid);
    membersIq.setFrom(Nowe::myJid());
    membersIq.setExtensions(membersEls);

    Nowe::myClient()->sendPacket(membersIq);
}

}
