#ifndef AGARPLUSPLUS_REPLICATIONACTIONTYPE_H
#define AGARPLUSPLUS_REPLICATIONACTIONTYPE_H

#include <string>

enum class ReplicationActionType
{
    INST,
    UPDT,
    DSTR,
    MAX_VALUE
};

std::string replicationActionTypeAsString(ReplicationActionType type);

#endif
