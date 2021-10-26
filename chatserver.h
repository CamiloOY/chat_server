#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QHash>

class ChatServer : public QObject {
	Q_OBJECT
public:
	explicit ChatServer(QObject *parent = nullptr);

private:
	QWebSocketServer server;
	QList<QWebSocket*> connections;
	QHash<QWebSocket*, QString> usernames;

public slots:
	void start();
	void quit();
	void disconnected();
	void newConnection();
	void receiveTextMessage(QString message);
};

#endif // CHATSERVER_H
