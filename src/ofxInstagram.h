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
    std::function<void(UserInfo)> onUserInfoReceived;
    std::function<void(Posts)> onUserFeedReceived,
        onUserRecentMediaReceived,
        onUserLikedMediaReceived;
    std::function<void(std::vector<UserInfo>)> onUserSearchReceived;

    //Relationship Endpoint Callbacks
    std::function<void(std::vector<UserInfo>)> onUserFollowingReceived,
        onUserFollowersReceived,
        onUserFollowRequestsReceived;
    std::function<void(Relationship)> onUserRelationshipReceived;

    //Media Endpoint Callbacks
    std::function<void(PostData)> onMediaInformationReceived;
    std::function<void(Posts)> onMediaSearchReceived, onMediaPopularReceived;

    //Media Endpoint Callbacks
    std::function<void(std::vector<Comment>)> onCommentsForMediaReceived;

    //Like Endpoint Callbacks
    std::function<void(std::vector<UserInfo>)> onLikeListReceived;

    //Tag Endpoint Callbacks
    std::function<void(TagInfo)> onTagInfoReceived;
    std::function<void(Posts)> onPostsForTagReceived;
    std::function<void(std::vector<TagInfo>)> onTagSearchReceived;

    //Location Endpoint Callbacks
    std::function<void(Location)> onLocationInfoReceived;
    std::function<void(Posts)> onPostsFromLocationReceived;
    std::function<void(std::vector<Location>)> onLocationSearchReceived;

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
    void getUserInformation(std::string who = "self", std::function<void(UserInfo)> callback = nullptr);

    // GET User Feed use count to limit number of returns
    void getUserFeed(int count = 20, std::string username = "self", std::function<void(Posts)> callback = nullptr, std::string minID = "",
                     std::string maxID = "");

    // GET User recent images from user pass the who as the user ID number
    void getUserRecentMedia(std::string who = "self", int count = 20, std::function<void(Posts)> callback = nullptr, std::string maxTimestamp = "",
                            std::string minTimestamp = "", std::string minID = "", std::string maxID = "");

    // GET User Liked Media
    void getUserLikedMedia(int count = 20, string username = "self", std::function<void(Posts)> callback = nullptr, std::string maxLikeID = "");

    // GET User Search for users
    void getSearchUsers(std::string query = "", int count = 20, std::function<void(std::vector<UserInfo>)> callback = nullptr);

    //------------- RELATIONSHIP ENDPOINTS -------------

    // GET User Follows
    void getWhoUserFollows(std::string who = "self", std::function<void(std::vector<UserInfo>)> callback = nullptr);

    // GET User Followed By
    void getUserFollowers(std::string who = "self", std::function<void(std::vector<UserInfo>)> callback = nullptr);

    // GET User Requested-by
    void getWhoHasRequestedToFollow(std::string who = "self", std::function<void(std::vector<UserInfo>)> callback = nullptr);

    // GET User Relationship
    void getRelationshipToUser(std::string who = "self", std::function<void(Relationship)> callback = nullptr);

    // POST User Modify Relationship
    void changeRelationshipToUser(std::string who = "self", std::string action = "", std::function<void(UserInfo)> callback = nullptr);

    //------------- MEDIA ENDPOINTS -------------

    // GET Info about Media Object
    void getMediaInformation(std::string mediaID, std::function<void(PostData)> callback = nullptr);

    // GET Info about Media using Shortcode
    void getMediaInfoUsingShortcode(std::string shortcode = "", std::function<void(PostData)> callback = nullptr);

    // GET Media Search
    void searchMedia(std::string lat = "", std::string lng = "", std::string min_timestamp = "", std::string max_timestamp = "", int distance = 1000,
                     std::function<void(Posts)> callback = nullptr);

    // GET Popular Media
    void getPopularMedia(std::function<void(Posts)> callback = nullptr);

    //------------- COMMENTS ENDPOINTS -------------

    // GET Comments on Media Object
    void getCommentsForMedia(std::string mediaID, std::function<void(std::vector<Comment>)> callback = nullptr);

    //------------- LIKE ENDPOINTS -------------

    // GET List of Users who have Liked a Media Object
    void getListOfUsersWhoLikedMedia(std::string mediaID, std::function<void(std::vector<UserInfo>)> callback = nullptr);

    //------------- TAG ENDPOINTS -------------

    // GET Info about tagged object
    void getInfoForTag(std::string tagname, std::function<void(TagInfo)> callback = nullptr);

    // GET List of recently tagged objects
    void getListOfTaggedObjectsNormal(std::string tagname, int count = 20, std::function<void(Posts)> callback = nullptr, std::string min_tagID = "",
                                      std::string max_tagID = "");
    // GET List of recently tagged objects
    void getListOfTaggedObjectsPagination(std::string tagname, int count = 20, std::function<void(Posts)> callback = nullptr, std::string max_tagID = "");
    // GET Search Tags
    void searchForTags(std::string query, std::function<void(std::vector<TagInfo>)> callback = nullptr);

    //------------- LOCATIONS ENDPOINTS -------------

    // GET Info about a Location
    void getInfoAboutLocation(std::string locationID, std::function<void(Location)> callback = nullptr);

    // GET Recent Media from location
    void getRecentMediaFromLocation(std::string locationID, std::function<void(Posts)> callback = nullptr, std::string minTimestamp = "",
                                    std::string maxTimestamp = "", std::string minID = "", std::string maxID = "");

    // GET Find Location ID
    void searchForLocations(std::string distance, std::string lat, std::string lng, std::function<void(std::vector<Location>)> callback = nullptr,
                            std::string facebook_PlacesID = "", std::string foursquareID = "");

    Meta getLastError() const;

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
    std::vector<PostData> constructPostDatas(const ofxJSONElement &json) const;
    PostData constructPostData(const ofxJSONElement &postJson) const;

    std::vector<UserInfo> constructUserInfos(const ofxJSONElement &json) const;
    UserInfo constructUserInfo(const ofxJSONElement &userJson) const;

    std::vector<Comment> constructComments(const ofxJSONElement &commentsJson) const;
    Pagination constructPagination(const ofxJSONElement &paginationJson) const;

    Location constructLocation(const ofxJSONElement &locationJson) const;

    Meta constructMeta(const ofxJSONElement &metaJson) const;

    void handleUserEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleRelationshipEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleMediaEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);

    void handleCommentEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleLikeEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
    void handleTagEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);

    void handleLocationEndpointResponse(const ofHttpResponse &response, const ofxJSONElement &json);
};
