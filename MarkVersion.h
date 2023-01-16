#ifndef _MARK_VERSION_H_
#define _MARK_VERSION_H_

#define MARK_VERSION_MAJOR 1
#define MARK_VERSION_MINOR 12
#define MARK_VERSION_PATCH 0
#define MARK_VERSION_STATUS BETA

typedef enum _version_statushola
{
    ALPHA,
    BETA,
    DEBUG,
    SPECIFIC_FEATURE_TESTING,
    PRODUCTION,
    UNFINISHED,
    RESERVED
} En_VersionStatus;


#endif // _MARK_VERSION_H_
