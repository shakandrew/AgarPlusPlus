#ifndef AGARPLUSPLUS_REPLICATIONCOMMAND_H
#define AGARPLUSPLUS_REPLICATIONCOMMAND_H

#include "ReplicationActionType.h"

class ReplicationCommand
{
    bool isDirty;

    ReplicationActionType type;

public:

    ReplicationCommand(ReplicationActionType type) : isDirty(true), type(type) { }

    void convertToDestroy() { type = ReplicationActionType::DSTR; setAsDirty(); }

    void convertToUpdate() { type = ReplicationActionType ::UPDT; setAsDirty(); }

    ReplicationActionType getAction() { return type; }

    bool getIsDirty() { return isDirty; };

    void setAsClean() { isDirty = false; }

    void setAsDirty() { isDirty = true; }

};

#endif
