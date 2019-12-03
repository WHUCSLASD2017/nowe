#include <NoweGlobal.h>
#include <QXmppVCardManager.h>

namespace Nowe {

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

}
