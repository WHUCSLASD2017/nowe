#ifndef NOWEGLOBAL_H
#define NOWEGLOBAL_H

#include <QXmppClient.h>
#include <QXmppVCardIq.h>

namespace Nowe {
QXmppClient* myClient();
const QXmppVCardIq& myVCard();
QString myJid();
QString myJidBare();
}


#endif // NOWEGLOBAL_H
