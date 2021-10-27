#include "chatserver.h"
#include <QDebug>
#include <QWebSocket>

ChatServer::ChatServer(QObject *parent) : QObject(parent), server("Chat", QWebSocketServer::NonSecureMode) {
	this->connect(&server, &QWebSocketServer::newConnection, this, &ChatServer::newConnection);
}

void ChatServer::start() {
	if(!server.listen(QHostAddress::LocalHost, 2020)) {
		qWarning()<<server.errorString();
		return;
	}
	qInfo()<<"Server listening on port 2020";
}

void ChatServer::quit() {
	server.close();
}

void ChatServer::disconnected() {
	QWebSocket* socket = qobject_cast<QWebSocket*>(this->sender());
	connections.removeAll(socket);
	usernames.remove(socket);
	qInfo() << "Disconnected"<<socket->localAddress();
	socket->deleteLater();
}

void ChatServer::newConnection() {
	QWebSocket* connection=server.nextPendingConnection();
	this->connect(connection, &QWebSocket::disconnected, this, &ChatServer::disconnected);
	this->connect(connection,&QWebSocket::textMessageReceived, this, &ChatServer::receiveTextMessage);
	connections.append(connection);
	qInfo()<<"New connection:"<<connection->localAddress();
}

void ChatServer::receiveTextMessage(QString message) {
	qInfo() << "Received:"<<message;
	if(message.startsWith("JOIN ")) {
		usernames[qobject_cast<QWebSocket*>(this->sender())] = message.mid(5);
		for(auto client : connections) {
			client->sendTextMessage("SERVER " + message.mid(5) + " joined!");
		}
	}
	else if(message.startsWith("NICK ")) {
		QWebSocket* socket = qobject_cast<QWebSocket*>(this->sender());
		QString old_name = usernames[socket];
		usernames[socket] = message.mid(5);
		for(auto client : connections) {
			client->sendTextMessage("SERVER " + old_name + " changed his name to " + usernames[socket]);
		}
	}
	else {
		QString username = usernames[qobject_cast<QWebSocket*>(this->sender())];
		for(auto client : connections) {
			client->sendTextMessage(username + ':' + message);
		}
	}
}
