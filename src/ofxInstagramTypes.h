#ifndef OFXINSTAGRAMTYPES_H
#define OFXINSTAGRAMTYPES_H
#include <string>
#include <vector>
#include <ostream>
#include "ofVec2f.h"

namespace ofxInstagramTypes
{
struct UserInfo {
    std::string bio = "",
                fullName = "",
                id = "",
                profilePicture = "",
                username = "",
                website = "";

    unsigned int followerCount = 0,
                 followingCount = 0,
                 mediaCount = 0;
};

struct Caption {
    std::string createdTime = "", id = "", text = "";
    UserInfo from;
};

struct Comment {
    std::string createdTime = "",
                id = "",
                text = "";
    UserInfo from;
};

struct PostMedia {
    std::string url = "";
    unsigned int width = 0, height = 0;
};

struct Location {
    std::string id = "", name = "";
    float latitude = 0.f, longitude = 0.f;
};

struct PostData {
    Caption caption;
    std::string attribution = "",
                createdTime = "",
                filter = "",
                link = "",
                type = "",
                id = "";

    Location location;

    PostMedia imageLowResolution,
              imageStandarResolution,
              imageThumbnail;

    PostMedia videoLowBandwidth,
              videoLowResolution,
              videoStandartResolution;

    UserInfo user;
    bool userHasLiked = false;
    unsigned int likeCount = 0, commentCount = 0;

    std::vector<Comment> comments;
    std::vector<std::string> tags;
    std::vector<std::pair<ofVec2f, UserInfo>> usersInPhoto;
    std::vector<UserInfo> likes;
};

struct Relationship {
    std::string outgoingStatus = "", incomngStatus = "";
};

struct TagInfo {
    unsigned int mediaCount = 0;
    std::string name = "";
};

struct Pagination {
    std::string minTagID = "",
                nextMaxID = "",
                nextMaxTagID = "",
                nextMinID = "",
                nextURL = "";
};

struct Meta {
    std::string errorType = "",
                code = "",
                errorMessage = "";

    friend std::ostream &operator<<(std::ostream &output, const Meta &m)
    {
        output << "Meta: {\n";
        output << "    errorType: " << m.errorType << "\n    code:" << m.code << "\n    errorMessage: " << m.errorMessage << "\n}";
        return output;
    }
};

using Posts = std::pair<std::vector<PostData>, Pagination>;
}

#endif // OFXINSTAGRAMTYPES_H
