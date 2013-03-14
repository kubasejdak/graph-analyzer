#ifndef RPCTAGRESOLVER_H
#define RPCTAGRESOLVER_H

#include <QMap>
#include <QList>
#include <QString>

enum RPCTag {
	NULL_TAG = 0,
	ANALYZE_TAG,
	EXPERIMENT_TAG,
	EXPORT_VERSION_TAG
};

class RPCTagResolver {
public:
	RPCTagResolver();
	virtual ~RPCTagResolver() {}

	RPCTag tag(QString funcName);
	QList<QString> availableCalls();

private:
	QMap<QString, RPCTag> m_tagMap;
};

#endif /* RPCTAGRESOLVER_H */
