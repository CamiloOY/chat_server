#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QWebSocketServer>

class ChatServer : public QObject {
	Q_OBJECT
public:
	explicit ChatServer(QObject *parent = nullptr);

private:
	QWebSocketServer server;
	QList<QWebSocket*> connections;

public slots:
	void start();
	void quit();
	void disconnected();
	void newConnection();
	void receiveTextMessage(QString message);
};

#endif // CHATSERVER_H
