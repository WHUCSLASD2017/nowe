#include "group.h"

Group::Group(QString jid)
{
    this->room = nullptr;

    QXmppMucManager * roomMsg = Nowe::myClient()->findExtension<QXmppMucManager>();
    QList<QXmppMucRoom *> rooms = roomMsg->rooms();

    //查询聊天室JID是否冲突
    for (QXmppMucRoom * room : rooms) {
        qDebug() << "room->jid " << room->jid() << endl;

        if(room->jid() == jid) {
            this->room = room;
            this->roomJid = jid;

            this->isRefreshList = true;

            connect(Nowe::myClient(), &QXmppClient::iqReceived, this, &Group::on_iqReceived);
            connect(this->room, &QXmppMucRoom::joined, this, &Group::registerWithRoom);
            connect(this->room, &QXmppMucRoom::left, this, [=]() {
                removeMember(Nowe::myJidBare());
            });
            connect(this->room, &QXmppMucRoom::participantAdded, this, &Group::addMember);
            connect(this->room, &QXmppMucRoom::participantRemoved, this, &Group::removeMember);

            this->room->join();
        }
    }
}

Group::~Group()
{

}

bool Group::obtainMemberList(QString affiliation)
{
    if (this->room != nullptr && this->isRefreshList) {
        this->affiliation = affiliation;

        QXmppElementList membersEls;
        QXmppElement queryEl;
        QXmppElement itemEl;

        itemEl.setTagName("item");
        itemEl.setAttribute("affiliation", "member");

        queryEl.setTagName("query");
        queryEl.setAttribute("xlmns", "http://jabber.org/protocol/muc#" + affiliation);
        queryEl.appendChild(itemEl);

        membersEls.append(queryEl);

        QXmppIq membersIq(QXmppIq::Get);
        membersIq.setTo(this->roomJid);
        membersIq.setFrom(Nowe::myJidBare());
        membersIq.setExtensions(membersEls);

        Nowe::myClient()->sendPacket(membersIq);

        activeMembers = room->participants();

        this->isRefreshList = false;

        emit timeToRenderMemberList();
        return true;
    }

    qDebug() << "JID ERROR!" << endl;
    return false;
}

bool Group::obtainInactiveMemberList()
{
    for (QString member : this->members) {
        for (QString activeMember : this->activeMembers) {
            if (member == activeMember) {
                break;
            }
        }

        this->inactiveMembers.append(member);
    }

    emit timeToRenderMemberList();
    return true;
}

bool Group::inviteNewMember(QString jid)
{
    if (this->room == nullptr || this->members.contains(jid)) {
        qDebug() << "invalid invitation!" << endl;
        return false;
    }

    this->room->sendInvitation(jid, Nowe::myJidBare() + "向你发来了加群邀请，一起来玩吧～");
    return true;
}

void Group::registerWithRoom()
{
    QXmppElementList membersEls;
    QXmppElement queryEl;

    queryEl.setTagName("query");
    queryEl.setAttribute("xlmns", "jabber:iq:register");

    membersEls.append(queryEl);

    QXmppIq membersIq(QXmppIq::Get);
    membersIq.setTo(this->roomJid);
    membersIq.setFrom(Nowe::myJidBare());
    membersIq.setExtensions(membersEls);

    Nowe::myClient()->sendPacket(membersIq);
}

bool Group::on_iqReceived(const QXmppIq &iq)
{
    qDebug() << "Group::on_iqReceived iq.from() " << iq.from() << endl;
    qDebug() << "Group::on_iqReceived iq.to() " << iq.from() << endl;

    if (iq.from() != this->roomJid) {
        return false;
    }

    if (iq.type() == QXmppIq::Result) {
        QXmppElementList responseEls = iq.extensions();
        QXmppElement firstEl = responseEls.first();

        if (firstEl.attribute("xmlns") == "http://jabber.org/protocol/muc#" + this->affiliation) {
            return receiveMemberList(firstEl);

        } else if (firstEl.attribute("xmlns") == "jabber:iq:register") {
            QXmppElement xEl = firstEl.firstChildElement().nextSiblingElement();
            if (!xEl.isNull()) {
                if (xEl.attribute("xmlns") == "jabber:x:data") {
                    return receiveRegistrationForm();
                }
            }

        } else if (firstEl.attribute("xmlns") == "http://jabber.org/protocol/muc#user") {
            return updateMembership(firstEl);
        }

    }

    qDebug() << "QXmppIq Type ERROR" << endl;
    return false;
}

bool Group::receiveMemberList(QXmppElement queryEl)
{
    QXmppElement itemEl;

    itemEl = queryEl.firstChildElement();

    while (!itemEl.isNull()) {
        this->members.append(itemEl.attribute("jid"));
        itemEl = itemEl.nextSiblingElement();
    }

    obtainInactiveMemberList();

    return true;
}

bool Group::receiveRegistrationForm()
{
    QXmppElementList registrationEls;
    QXmppElement queryEl;
    QXmppElement xEl;

    xEl.setTagName("x");
    xEl.setAttribute("xmlns", "jabber:x:data");
    xEl.setAttribute("type", "submit");

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue("http://jabber.org/protocol/muc#register");

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "FORM_TYPE");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue("");

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_first");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue("");

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_last");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue(this->room->nickName());

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_roomnick");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue(this->room->nickName());

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_url");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue(this->room->nickName());

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_email");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    {
        QXmppElement valEl;
        QXmppElement fieldEl;

        valEl.setTagName("value");
        valEl.setValue(this->room->nickName());

        fieldEl.setTagName("field");
        fieldEl.setAttribute("var", "muc#register_faqentry");
        fieldEl.appendChild(valEl);

        xEl.appendChild(fieldEl);
    }

    queryEl.setTagName("query");
    queryEl.setAttribute("xlmns", "jabber:x:data");
    queryEl.appendChild(xEl);

    registrationEls.append(queryEl);

    QXmppIq registrationIq(QXmppIq::Set);
    registrationIq.setTo(this->roomJid);
    registrationIq.setFrom(Nowe::myJidBare());
    registrationIq.setExtensions(registrationEls);

    Nowe::myClient()->sendPacket(registrationIq);

    return true;
}

bool Group::updateMembership(QXmppElement xEl)
{
    QXmppElement itemEl;

    itemEl = xEl.firstChildElement();

    while (!itemEl.isNull() && itemEl.attribute("affiliation") == "member" && itemEl.attribute("jid") != Nowe::myJidBare()) {
        this->members.append(itemEl.attribute("jid"));
        itemEl = itemEl.nextSiblingElement();
    }

    return true;
}

void Group::addMember(QString jid)
{
    this->activeMembers.append(jid);
    emit memberJoin(jid);
}

void Group::removeMember(QString jid)
{
    int i;
    for (i = 0; i < this->activeMembers.size(); i++) {
        if (this->activeMembers[i] == jid) {
            break;
        }
    }

    this->activeMembers.removeAt(i);
    emit memberLeave(jid);
}
