#include "RPCTagResolver.h"

RPCTagResolver::RPCTagResolver()
{
	m_tagMap["analyze"] = ANALYZE_TAG;
	m_tagMap["experiment"] = EXPERIMENT_TAG;
	m_tagMap["export-version"] = EXPORT_VERSION_TAG;
}

RPCTag RPCTagResolver::tag(QString funcName)
{
	if(m_tagMap.find(funcName) == m_tagMap.end())
		return NULL_TAG;

	return m_tagMap[funcName];
}

QList<QString> RPCTagResolver::availableCalls()
{
	return m_tagMap.keys();
}
