/*
 *  ofxInstagram
 *
 *  David Haylock 2015
 */

#include "ofVec2f.h"
#include "ofxJSON.h"
#include "ofxInstagramTypes.h"

class ofxInstagram
{
public:
    //User Endpoint Callbacks
    std::function<void(ofxInstagramTypes::UserInfo)> onUserInfoReceived;
    std::function<void(ofxInstagramTypes::Posts)> onUserFeedReceived,
        onUserRecentMediaReceived,
        onUserLikedMediaReceived;
    std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> onUserSearchReceived;

    //Relationship Endpoint Callbacks
    std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> onUserFollowingReceived,
        onUserFollowersReceived,
        onUserFollowRequestsReceived;
    std::function<void(ofxInstagramTypes::Relationship)> onUserRelationshipReceived;

    //Media Endpoint Callbacks
    std::function<void(ofxInstagramTypes::PostData)> onMediaInformationReceived;
    std::function<void(ofxInstagramTypes::Posts)> onMediaSearchReceived, onMediaPopularReceived;

    //Media Endpoint Callbacks
    std::function<void(std::vector<ofxInstagramTypes::Comment>)> onCommentsForMediaReceived;

    //Like Endpoint Callbacks
    std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> onLikeListReceived;

    //Tag Endpoint Callbacks
    std::function<void(ofxInstagramTypes::TagInfo)> onTagInfoReceived;
    std::function<void(ofxInstagramTypes::Posts)> onPostsForTagReceived;
    std::function<void(std::vector<ofxInstagramTypes::TagInfo>)> onTagSearchReceived;

    //Location Endpoint Callbacks
    std::function<void(ofxInstagramTypes::Location)> onLocationInfoReceived;
    std::function<void(ofxInstagramTypes::Posts)> onPostsFromLocationReceived;
    std::function<void(std::vector<ofxInstagramTypes::Location>)> onLocationSearchReceived;

public:
    ofxInstagram();

    // Setup Tokens Etc...
    void setup(std::string auth_token, std::string clientID);
    void setCertFileLocation(std::string path);

    void draw();
    void drawJSON(int x);

    void resetScroll();
    void mouseScroll(int scrollY);

    std::string getParsedJSONString() const;

    //------------- USER ENDPOINTS -------------

    // GET User Info
    void getUserInformation(std::string who = "self", std::function<void(ofxInstagramTypes::UserInfo)> callback = nullptr);

    // GET User Feed use count to limit number of returns
    void getUserFeed(int count = 20, std::string username = "self", std::function<void(ofxInstagramTypes::Posts)> callback = nullptr, std::string minID = "",
                     std::string maxID = "");

    // GET User recent images from user pass the who as the user ID number
    void getUserRecentMedia(std::string who = "self", int count = 20, std::function<void(ofxInstagramTypes::Posts)> callback = nullptr,
                            std::string maxTimestamp = "",
                            std::string minTimestamp = "", std::string minID = "", std::string maxID = "");

    // GET User Liked Media
    void getUserLikedMedia(int count = 20, string username = "self", std::function<void(ofxInstagramTypes::Posts)> callback = nullptr, std::string maxLikeID = "");

    // GET User Search for users
    void getSearchUsers(std::string query = "", int count = 20, std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> callback = nullptr);

    //------------- RELATIONSHIP ENDPOINTS -------------

    // GET User Follows
    void getWhoUserFollows(std::string who = "self", std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> callback = nullptr);

    // GET User Followed By
    void getUserFollowers(std::string who = "self", std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> callback = nullptr);

    // GET User Requested-by
    void getWhoHasRequestedToFollow(std::string who = "self", std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> callback = nullptr);

    // GET User Relationship
    void getRelationshipToUser(std::string who = "self", std::function<void(ofxInstagramTypes::Relationship)> callback = nullptr);

    // POST User Modify Relationship
    void changeRelationshipToUser(std::string who = "self", std::string action = "", std::function<void(ofxInstagramTypes::UserInfo)> callback = nullptr);

    //------------- MEDIA ENDPOINTS -------------

    // GET Info about Media Object
    void getMediaInformation(std::string mediaID, std::function<void(ofxInstagramTypes::PostData)> callback = nullptr);

    // GET Info about Media using Shortcode
    void getMediaInfoUsingShortcode(std::string shortcode = "", std::function<void(ofxInstagramTypes::PostData)> callback = nullptr);

    // GET Media Search
    void searchMedia(std::string lat = "", std::string lng = "", std::string min_timestamp = "", std::string max_timestamp = "", int distance = 1000,
                     std::function<void(ofxInstagramTypes::Posts)> callback = nullptr);
    void searchMedia(const std::string &tag, std::function<void(ofxInstagramTypes::Posts)> callback = nullptr);

    // GET Popular Media
    void getPopularMedia(std::function<void(ofxInstagramTypes::Posts)> callback = nullptr);

    //------------- COMMENTS ENDPOINTS -------------

    // GET Comments on Media Object
    void getCommentsForMedia(std::string mediaID, std::function<void(std::vector<ofxInstagramTypes::Comment>)> callback = nullptr);

    //------------- LIKE ENDPOINTS -------------

    // GET List of Users who have Liked a Media Object
    void getListOfUsersWhoLikedMedia(std::string mediaID, std::function<void(std::vector<ofxInstagramTypes::UserInfo>)> callback = nullptr);

    //------------- TAG ENDPOINTS -------------

    // GET Info about tagged object
    void getInfoForTag(std::string tagname, std::function<void(ofxInstagramTypes::TagInfo)> callback = nullptr);

    // GET List of recently tagged objects
    void getListOfTaggedObjectsNormal(std::string tagname, int count = 20, std::function<void(ofxInstagramTypes::Posts)> callback = nullptr,
                                      std::string min_tagID = "",
                                      std::string max_tagID = "");
    // GET List of recently tagged objects
    void getListOfTaggedObjectsPagination(std::string tagname, int count = 20, std::function<void(ofxInstagramTypes::Posts)> callback = nullptr,
                                          std::string max_tagID = "");
    // GET Search Tags
    void searchForTags(std::string query, std::function<void(std::vector<ofxInstagramTypes::TagInfo>)> callback = nullptr);

    //------------- LOCATIONS ENDPOINTS -------------

    // GET Info about a Location
    void getInfoAboutLocation(std::string locationID, std::function<void(ofxInstagramTypes::Location)> callback = nullptr);

    // GET Recent Media from location
    void getRecentMediaFromLocation(std::string locationID, std::function<void(ofxInstagramTypes::Posts)> callback = nullptr, std::string minTimestamp = "",
                                    std::string maxTimestamp = "", std::string minID = "", std::string maxID = "");

    // GET Find Location ID
    void searchForLocations(std::string distance, std::string lat, std::string lng,
                            std::function<void(std::vector<ofxInstagramTypes::Location>)> callback = nullptr,
                            std::string facebook_PlacesID = "", std::string foursquareID = "");

    ofxInstagramTypes::Meta getLastError() const;

    void urlResponse(ofHttpResponse &response);

private:
    const std::string m_UsersURL,
          m_MediaURL,
          m_TagsURL,
          m_LocationsURL;

    //Request Names - User
    const std::string m_RequestUserInfo,
          m_RequestUserFeed,
          m_RequestUserRecentMedia,
          m_RequestUserLikedMedia,
          m_RequestUserSearch;

    //Request Names - Relationship
    const std::string m_RequestRelationshipFollowing,
          m_RequestRelationshipFollowers,
          m_RequestRelationshipFollowRequests,
          m_RequestRelationshipUserRel;

    //Request Names - Media
    const std::string m_RequestMediaInformation,
          m_RequestMediaSearch,
          m_RequestMediaPopular;

    //Request Names - Comment
    const std::string m_RequestCommentForMedia;

    //Request Names - Like
    const std::string m_RequestLikesUserListForMedia;

    //Request Names - Tag
    const std::string m_RequestTagInfo,
          m_RequestTagPostList,
          m_RequestTagSearch;

    //Request Names - Location
    const std::string m_RequestLocationInfo,
          m_RequestLocationRecentMedia,
          m_RequestLocationSearch;

    //Holds the response data for the latest request
    ofHttpResponse m_Response;

    std::string m_AuthToken;
    std::string m_ClientID;
    std::string m_ResponseData;
    std::string m_CertPath;

    int m_ScrollValue;
    ofVec2f m_ScrollAmount;
    ofVec2f m_ClickOrigin;
    ofVec2f m_ReleasePos;

    std::vector<unsigned int> m_RequestIDs;

private:
    std::vector<ofxInstagramTypes::PostData> constructPostDatas(const ofxJSONElement &json) const;
    ofxInstagramTypes::PostData constructPostData(const ofxJSONElement &postJson) const;

    std::vector<ofxInstagramTypes::UserInfo> constructUserInfos(const ofxJSONElement &json) const;
    ofxInstagramTypes::UserInfo constructUserInfo(const ofxJSONElement &userJson) const;

    std::vector<ofxInstagramTypes::Comment> constructComments(const ofxJSONElement &commentsJson) const;
    ofxInstagramTypes::Pagination constructPagination(const ofxJSONElement &paginationJson) const;

    ofxInstagramTypes::Location constructLocation(const ofxJSONElement &locationJson) const;

    ofxInstagramTypes::Meta constructMeta(const ofxJSONElement &metaJson) const;

    void handleUserEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleRelationshipEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleMediaEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);

    void handleCommentEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleLikeEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleTagEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);

    void handleLocationEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
};
