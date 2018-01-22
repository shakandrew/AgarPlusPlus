#include "replication/ReplicationActionType.h"
#include <stdexcept>
std::string replicationActionTypeAsString(ReplicationActionType type)
{
    switch (type) {
        case ReplicationActionType::INST:
            return "INST";
        case ReplicationActionType::DSTR:
            return "DSTR";
        case ReplicationActionType::UPDT:
            return "UPDT";
        default:
            throw std::invalid_argument("Unsupported replication action type.");
            break;
    }
}
