#include "ofxInstagram.h"
#include "ofMain.h"
#include <io.h>

using UserInfo = ofxInstagram::UserInfo;
using Caption = ofxInstagram::Caption;
using Comment = ofxInstagram::Comment;
using PostImage = ofxInstagram::PostMedia;
using PostData = ofxInstagram::PostData;
using Posts = ofxInstagram::Posts;
using Relationship = ofxInstagram::Relationship;
using TagInfo = ofxInstagram::TagInfo;
using Pagination = ofxInstagram::Pagination;
using Location = ofxInstagram::Location;
using Meta = ofxInstagram::Meta;

ofxInstagram::ofxInstagram()
    : m_UsersURL("https://api.instagram.com/v1/users/")
    , m_MediaURL("https://api.instagram.com/v1/media/")
    , m_TagsURL("https://api.instagram.com/v1/tags/")
    , m_LocationsURL("https://api.instagram.com/v1/locations/")
    , m_Response()
    , m_AuthToken("")
    , m_ClientID("")
    , m_ResponseData("")
    , m_CertPath("")
    , m_ScrollValue(0)
    , m_ScrollAmount(0, 0)
    , m_ClickOrigin(0, 0)
    , m_ReleasePos(0, 0)
{

}

void ofxInstagram::setup(std::string auth_token, std::string clientID)
{
    m_ScrollValue = 0;
    // Set the Tokens
    m_AuthToken = auth_token;
    m_ClientID = clientID;
}

void ofxInstagram::setCertFileLocation(std::string path)
{
    m_CertPath = path;
}

void ofxInstagram::drawJSON(int x)
{
    ofPushMatrix();
    {
        ofTranslate(x, m_ScrollValue);
        ofDrawBitmapString(getParsedJSONString(), 0, 0);
    }
    ofPopMatrix();
}

void ofxInstagram::mouseScroll(int scrollY)
{
    m_ScrollValue += scrollY;
}

void ofxInstagram::resetScroll()
{
    m_ScrollValue = 0;
}

// *                        USER ENDPOINTS
// *  GET Info
// *  GET User Feed
// *  GET User Recent Media
// *  GET User Like Media
// *  GET User Search Users

UserInfo ofxInstagram::getUserInformation(std::string who)
{
    std::stringstream url;
    url << m_UsersURL << who << "/?access_token=" << m_AuthToken;

    std::cout << __FUNCTION__ << ": " << "Getting Info about User: This is your request: " << url.str()  << "\n";
    m_Response = ofLoadURL(url.str());
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return UserInfo();
    }

    return constructUserInfo(json["data"]);
}

Posts ofxInstagram::getUserFeed(int count, std::string username, std::string minID, std::string maxID)
{
    std::stringstream url;
    url << m_UsersURL << username << "/feed?access_token=" << m_AuthToken << "&count=" << std::to_string(count);

    if (minID.length() != 0) {
        url << "&minID=" << minID;
    }

    if (maxID.length() != 0) {
        url << "&maxID=" << maxID;
    }

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "Getting Users Feed: This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

Posts ofxInstagram::getUserRecentMedia(std::string who, int count, std::string maxTimestamp, std::string minTimestamp, std::string minID, std::string maxID)
{
    std::stringstream url;
    url << m_UsersURL << who << "/media/recent?access_token=" << m_AuthToken << "&count=" << std::to_string(count);

    if (minID.length() != 0) {
        url << "&minID=" << minID;
    }

    if (maxID.length() != 0) {
        url << "&maxID=" << maxID;
    }

    if (minTimestamp.length() != 0) {
        url << "&min_timestamp=" << minTimestamp;
    }

    if (maxTimestamp.length() != 0) {
        url << "&max_timestamp=" << maxTimestamp;
    }

    m_Response = ofLoadURL(url.str());
    std::cout << __FUNCTION__ << ": " << "Getting " << who << "'s Feed: This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));

}

Posts ofxInstagram::getUserLikedMedia(int count, std::string username, std::string maxLikeID)
{
    std::stringstream url;
    url << m_UsersURL << username << "/media/liked?access_token=" << m_AuthToken << "&count=" << std::to_string(count);

    if (maxLikeID.length() != 0) {
        url << "&max_like_ID=" << maxLikeID;
    }

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

std::vector<UserInfo> ofxInstagram::getSearchUsers(std::string query, int count)
{
    std::stringstream url;
    url << m_UsersURL << "search?access_token=" << m_AuthToken << "&count=" << std::to_string(count);

    if (query.length() != 0) {
        url << "&q=" << query;
    }

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<UserInfo>();
    }

    const ofxJSONElement usersJson = json["data"];
    const unsigned int returnCount = usersJson.size();
    std::vector<UserInfo> users;

    for (unsigned int userIndex = 0; userIndex < returnCount; userIndex++) {
        users.push_back(constructUserInfo(usersJson[userIndex]));
    }

    return users;
}

// *                RELATIONSHIP ENDPOINTS
// *  GET who User Follows
// *  GET who User is Followed By
// *  GET get who User has Requested to Follow
// *  GET relationship to User
// *  POST change Relationship to User

std::vector<UserInfo> ofxInstagram::getWhoUserFollows(std::string who)
{
    std::stringstream url;
    url << m_UsersURL << who << "/follows?access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<UserInfo>();
    }

    const ofxJSONElement usersJson = json["data"];
    const unsigned int returnCount = usersJson.size();
    std::vector<UserInfo> users;

    for (unsigned int userIndex = 0; userIndex < returnCount; userIndex++) {
        users.push_back(constructUserInfo(usersJson[userIndex]));
    }

    return users;
}

std::vector<UserInfo> ofxInstagram::getWhoUserIsFollowedBy(std::string who)
{
    std::stringstream url;
    url << m_UsersURL << who << "/followed-by?access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<UserInfo>();
    }

    const ofxJSONElement usersJson = json["data"];
    const unsigned int returnCount = usersJson.size();
    std::vector<UserInfo> users;

    for (unsigned int userIndex = 0; userIndex < returnCount; userIndex++) {
        users.push_back(constructUserInfo(usersJson[userIndex]));
    }

    return users;
}

std::vector<UserInfo> ofxInstagram::getWhoHasRequestedToFollow(std::string who)
{
    std::stringstream url;
    url << m_UsersURL << who << "/requested-by?access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<UserInfo>();
    }

    const ofxJSONElement usersJson = json["data"];
    const unsigned int returnCount = usersJson.size();
    std::vector<UserInfo> users;

    for (unsigned int userIndex = 0; userIndex < returnCount; userIndex++) {
        users.push_back(constructUserInfo(usersJson[userIndex]));
    }

    return users;
}

Relationship ofxInstagram::getRelationshipToUser(std::string who)
{
    std::stringstream url;
    url << m_UsersURL << who << "/relationship?access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Relationship();
    }

    Relationship rel;
    rel.outgoingStatus = json["data"]["outgoing_status"].asString();
    rel.incomngStatus = json["data"]["incoming_status"].asString();
    return rel;
}

void ofxInstagram::changeRelationshipToUser(std::string who, std::string action)
{
    //follow/unfollow/block/unblock/approve/ignore.
    // TO DO
}


// *                        MEDIA ENDPOINTS
// *  GET Info about Media
// *  GET Info using Shortcode
// *  GET Search Media
// *  GET Popular Media
// *

PostData ofxInstagram::getMediaInformation(std::string mediaID)
{
    std::stringstream url;
    url << m_MediaURL << mediaID << "?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    json.parse(m_Response.data);
    return constructPostData(json["data"]);
}

PostData ofxInstagram::getMediaInfoUsingShortcode(std::string shortcode)
{
    std::stringstream url;
    url << m_MediaURL << "shortcode/" << shortcode << "?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    json.parse(m_Response.data);
    return constructPostData(json["data"]);
}

Posts ofxInstagram::searchMedia(std::string lat, std::string lng, std::string min_timestamp, std::string max_timestamp, int distance)
{
    std::stringstream url;
    url << m_MediaURL << "search?access_token=" << m_AuthToken;

    if (lat.length() != 0) {
        url << "&lat=" << lat;
    }

    if (lng.length() != 0) {
        url << "&lng=" << lng;
    }

    if (min_timestamp.length() != 0) {
        url << "&min_timestamp=" << min_timestamp;
    }

    if (max_timestamp.length() != 0) {
        url << "&max_timestamp=" << max_timestamp;
    }
    url << "&distance=" << distance;

    m_Response = ofLoadURL(url.str());
    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

Posts ofxInstagram::getPopularMedia()
{
    std::stringstream url;
    url << m_MediaURL << "popular?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

// *
// *                        COMMENT ENDPOINTS
// *  GET Comments on Media Object
// *  POST Comment on Media Object
// *  DELETE Comment on Media Object
// *

std::vector<Comment> ofxInstagram::getCommentsForMedia(std::string mediaID)
{
    std::stringstream url;
    url << m_MediaURL << mediaID << "/comments?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<Comment>();
    }

    return constructComments(json["data"]);
}

// *                        LIKE ENDPOINTS
// *  GET List of Likes on Media Object
// *  POST Like Media
// *  POST unlike Media
// *

std::vector<UserInfo> ofxInstagram::getListOfUsersWhoLikedMedia(std::string mediaID)
{
    std::stringstream url;
    url << m_MediaURL << mediaID << "/likes?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<UserInfo>();
    }

    const ofxJSONElement usersJson = json["data"];
    const unsigned int returnCount = usersJson.size();
    std::vector<UserInfo> users;

    for (unsigned int userIndex = 0; userIndex < returnCount; userIndex++) {
        users.push_back(constructUserInfo(usersJson[userIndex]));
    }

    return users;
}

// *
// *                        TAG ENDPOINTS
// *  GET Info about Tags
// *  GET List of Tagged Objects
// *  GET Search for Tag Objects
// *

TagInfo ofxInstagram::getInfoForTags(std::string tagname)
{
    std::stringstream url;
    url << m_TagsURL << tagname << "?access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return TagInfo();
    }

    TagInfo tagInfo;
    tagInfo.mediaCount = json["data"]["media_count"].asInt();
    tagInfo.name = json["data"]["name"].asString();

    return tagInfo;
}

Posts ofxInstagram::getListOfTaggedObjectsNormal(std::string tagname, int count, std::string min_tagID, std::string max_tagID)
{
    std::stringstream url;
    url << m_TagsURL << tagname << "/media/recent?access_token=" << m_AuthToken;

    if (min_tagID.length() != 0) {
        url << "&min_tag_id=" << min_tagID;
    }

    if (max_tagID.length() != 0) {
        url << "&max_tag_id=" << max_tagID;
    }

    url << "&count=" << count;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

Posts ofxInstagram::getListOfTaggedObjectsPagination(std::string tagname, int count, std::string max_tagID)
{
    std::stringstream url;
    url << m_TagsURL << tagname << "/media/recent?access_token=" << m_AuthToken;

    if (max_tagID.length() != 0) {
        url << "&max_tag_id=" << max_tagID;
    }

    url << "&count=" << count;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

std::vector<TagInfo> ofxInstagram::searchForTags(std::string query)
{
    std::stringstream url;
    url << m_TagsURL << "search?q=" << query << "&access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<TagInfo>();
    }

    std::vector<TagInfo> tags;
    const ofxJSONElement tagsJson = json["data"];
    const unsigned int tagCount = tagsJson.size();
    for (unsigned int tagIndex = 0; tagIndex < tagCount; tagIndex++) {
        TagInfo tagInfo;
        tagInfo.mediaCount = tagsJson[tagIndex]["media_count"].asInt();
        tagInfo.name = tagsJson[tagIndex]["name"].asString();
        tags.push_back(tagInfo);
    }

    return tags;
}

// *                   LOCATIONS ENDPOINTS
// *  GET Info about Locations
// *  GET Recent Media from Location
// *  GET Search for Locations by LAT,LNG

Location ofxInstagram::getInfoAboutLocation(std::string locationID)
{
    std::stringstream url;
    url << m_LocationsURL << locationID << "?access_token=" << m_AuthToken;

    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Location();
    }

    return constructLocation(json["data"]);
}

Posts ofxInstagram::getRecentMediaFromLocation(std::string locationID, std::string minTimestamp, std::string maxTimestamp, std::string minID, std::string maxID)
{
    std::stringstream url;
    url << m_LocationsURL << locationID << "/media/recent?access_token=" << m_AuthToken;

    if (minID.length() != 0) {
        url << "&min_id=" << minID;
    }

    if (maxID.length() != 0) {
        url << "&max_id=" << maxID;
    }

    if (minTimestamp.length() != 0) {
        url << "&min_timestamp=" << minTimestamp;
    }

    if (maxTimestamp.length() != 0) {
        url << "&max_timestamp=" << maxTimestamp;
    }
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return Posts();
    }

    return std::make_pair(constructPostsData(json), constructPagination(json["pagination"]));
}

std::vector<Location> ofxInstagram::searchForLocations(std::string distance, std::string lat, std::string lng, std::string facebook_PlacesID,
        std::string foursquareID)
{
    std::stringstream url;
    url << m_LocationsURL << "search?";

    if (lat.length() != 0) {
        url << "&lat=" << lat;
    }

    if (lng.length() != 0) {
        url << "&lng=" << lng;
    }

    if (facebook_PlacesID.length() != 0) {
        url << "&facebook_places_id=" << facebook_PlacesID;
    }

    if (foursquareID.length() != 0) {
        url << "&foursquare_id=" << foursquareID;
    }

    url << "&distance=" << distance;
    url << "&access_token=" << m_AuthToken;
    m_Response = ofLoadURL(url.str());

    std::cout << __FUNCTION__ << ": " << "This is your request: " << url.str()  << "\n";
    ofxJSONElement json;
    const bool isParseSuccesful = json.parse(m_Response.data);
    if (isParseSuccesful == false) {
        ofLogError("ofxInstagram") << __FUNCTION__ << ": Parse error!";
        return std::vector<Location>();
    }

    std::vector<Location> locations;
    const ofxJSONElement locationsJson = json["data"];
    const unsigned int locationCount = locationsJson.size();
    for (unsigned int locationIndex = 0; locationIndex < locationCount; locationIndex++) {
        locations.push_back(constructLocation(locationsJson[locationIndex]));
    }

    return locations;
}

Meta ofxInstagram::getLastError() const
{
    ofxJSONElement json;
    json.parse(m_Response.data);
    return constructMeta(json["meta"]);
}

std::string ofxInstagram::getParsedJSONString() const
{
    if (m_Response.data.size() == 0) {
        return "";
    }
    else {
        return ofxJSONElement(m_Response.data).toStyledString();
    }
}

std::vector<ofxInstagram::PostData> ofxInstagram::constructPostsData(const ofxJSONElement &json) const
{
    std::vector<ofxInstagram::PostData> posts;
    const unsigned int responseCount = json["data"].size();
    const ofxJSONElement postsJson = json["data"];
    for (unsigned int postIndex = 0; postIndex < responseCount; ++postIndex) {
        const ofxJSONElement postJson = postsJson[postIndex];
        posts.push_back(constructPostData(postJson));
    }

    return posts;
}

PostData ofxInstagram::constructPostData(const ofxJSONElement &postJson) const
{
    PostData post;
    post.attribution = postJson["attribution"].asString();
    post.createdTime = postJson["created_time"].asString();
    post.filter = postJson["filter"].asString();
    post.id = postJson["id"].asString();
    post.link = postJson["link"].asString();
    post.type = postJson["type"].asString();
    post.userHasLiked = postJson["user_has_liked"].asBool();
    post.user = constructUserInfo(postJson["user"]);

    //Location
    const ofxJSONElement locationJson = postJson["location"];
    post.location = constructLocation(locationJson);

    //Caption
    post.caption.createdTime = postJson["caption"]["created_time"].asString();
    post.caption.id = postJson["caption"]["id"].asString();
    post.caption.text = postJson["caption"]["text"].asString();
    post.caption.from.fullName = postJson["caption"]["from"]["full_name"].asString();
    post.caption.from.id = postJson["caption"]["from"]["id"].asString();
    post.caption.from.profilePicture = postJson["caption"]["from"]["profile_picture"].asString();
    post.caption.from.username = postJson["caption"]["from"]["username"].asString();

    // Comments
    const ofxJSONElement commentsJson = postJson["comments"]["data"];
    post.commentCount = postJson["comments"]["count"].asInt();
    post.comments = constructComments(commentsJson);

    //Images
    if (post.type == "image") {
        post.imageLowResolution.height = postJson["images"]["low_resolution"]["height"].asInt();
        post.imageLowResolution.width = postJson["images"]["low_resolution"]["width"].asInt();
        post.imageLowResolution.url = postJson["images"]["low_resolution"]["url"].asString();

        post.imageStandarResolution.height = postJson["images"]["standard_resolution"]["height"].asInt();
        post.imageStandarResolution.width = postJson["images"]["standard_resolution"]["width"].asInt();
        post.imageStandarResolution.url = postJson["images"]["standard_resolution"]["url"].asString();

        post.imageThumbnail.height = postJson["images"]["thumbnail"]["height"].asInt();
        post.imageThumbnail.width = postJson["images"]["thumbnail"]["width"].asInt();
        post.imageThumbnail.url = postJson["images"]["thumbnail"]["url"].asString();
    }

    //Videos
    if (post.type == "video") {
        post.videoLowBandwidth.width = postJson["videos"]["low_bandwidth"]["width"].asInt();
        post.videoLowBandwidth.height = postJson["videos"]["low_bandwidth"]["height"].asInt();
        post.videoLowBandwidth.url = postJson["videos"]["low_bandwidth"]["url"].asString();

        post.videoLowResolution.height = postJson["videos"]["low_resolution"]["height"].asInt();
        post.videoLowResolution.width = postJson["videos"]["low_resolution"]["width"].asInt();
        post.videoLowResolution.url = postJson["videos"]["low_resolution"]["url"].asString();

        post.videoStandartResolution.height = postJson["videos"]["standard_resolution"]["height"].asInt();
        post.videoStandartResolution.width = postJson["videos"]["standard_resolution"]["width"].asInt();
        post.videoStandartResolution.url = postJson["videos"]["standard_resolution"]["url"].asString();
    }

    //Likes
    const ofxJSONElement likesJson = postJson["likes"]["data"];
    post.likeCount = postJson["likes"]["count"].asInt();
    const unsigned int likeCount = likesJson.size();

    for (unsigned int likeIndex = 0; likeIndex < likeCount; likeIndex++) {
        UserInfo user;
        const ofxJSONElement likeJson = likesJson[likeIndex];
        user.fullName = likeJson["full_name"].asString();
        user.id = likeJson["id"].asString();
        user.profilePicture = likeJson["profile_picture"].asString();
        user.username = likeJson["username"].asString();
        post.likes.push_back(user);
    }

    //Tags
    const ofxJSONElement tagsJson = postJson["tags"];
    const unsigned int tagCount = tagsJson.size();
    for (unsigned int tagIndex = 0; tagIndex < tagCount; tagIndex++) {
        post.tags.push_back(tagsJson[tagIndex].asString());
    }

    //Users In Photo
    const ofxJSONElement userInPhotoJson = postJson["users_in_photo"];
    const unsigned int userInPhotoCount = userInPhotoJson.size();
    for (unsigned int userInPhotoIndex = 0; userInPhotoIndex < userInPhotoCount; userInPhotoIndex++) {
        const ofxJSONElement userInPhoto = userInPhotoJson[userInPhotoIndex];
        const ofVec2f pos(userInPhoto["position"]["x"].asFloat(), userInPhoto["position"]["y"].asFloat());
        post.usersInPhoto.push_back(std::make_pair(pos, constructUserInfo(userInPhoto["user"])));
    }

    return post;
}

UserInfo ofxInstagram::constructUserInfo(const ofxJSONElement &userJson) const
{
    UserInfo user;
    user.bio = userJson["bio"].asString();
    user.fullName = userJson["full_name"].asString();
    user.id = userJson["id"].asString();
    user.profilePicture = userJson["profile_picture"].asString();
    user.username = userJson["username"].asString();
    user.website = userJson["website"].asString();

    user.followerCount = userJson["counts"]["followed_by"].asInt();
    user.followingCount = userJson["counts"]["follows"].asInt();
    user.mediaCount = userJson["counts"]["media"].asInt();

    return user;
}

std::vector<Comment> ofxInstagram::constructComments(const ofxJSONElement &commentsJson) const
{
    std::vector<Comment> comments;
    const unsigned int commentCount = commentsJson.size();

    for (unsigned int commentIndex = 0; commentIndex < commentCount; commentIndex++) {
        const ofxJSONElement commentJson = commentsJson[commentIndex];
        Comment comment;
        comment.createdTime = commentJson["created_time"].asString();
        comment.id = commentJson["id"].asString();
        comment.text = commentJson["text"].asString();

        comment.from.fullName = commentJson["from"]["full_name"].asString();
        comment.from.id = commentJson["from"]["id"].asString();
        comment.from.profilePicture = commentJson["from"]["profile_picture"].asString();
        comment.from.username = commentJson["from"]["username"].asString();

        comments.push_back(comment);
    }

    return comments;
}

Pagination ofxInstagram::constructPagination(const ofxJSONElement &paginationJson) const
{
    Pagination page;
    page.minTagID = paginationJson["min_tag_id"].asString();
    page.nextMaxID = paginationJson["next_max_id"].asString();
    page.nextMaxTagID = paginationJson["next_max_tag_id"].asString();
    page.nextMinID = paginationJson["next_min_id"].asString();
    page.nextURL = paginationJson["next_url"].asString();

    return page;
}

Location ofxInstagram::constructLocation(const ofxJSONElement &locationJson) const
{
    Location location;
    location.name = locationJson["name"].asString();
    location.id = locationJson["id"].asString();
    location.latitude = locationJson["latitude"].asFloat();
    location.longitude = locationJson["longitude"].asFloat();
    return location;
}

Meta ofxInstagram::constructMeta(const ofxJSONElement &metaJson) const
{
    Meta meta;
    meta.code = metaJson["code"].asString();
    meta.errorType = metaJson["error_type"].asString();
    meta.errorMessage = metaJson["error_message"].asString();
    return meta;
}
